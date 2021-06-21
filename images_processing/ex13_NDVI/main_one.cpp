#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <glob.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>




int main (int argc, char** argv){

    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  Here you should put all the code to make the process.
    //
    /*  This function finds the individual images for each two (2) bands, NIR and RED in order
     *  to create a composed image to show NDVI index. This function looking for all images
     *  inside folder pointed by path.
     */
    /*  Now, following the naming code, the program will find images taken at same time and want
     *  to process them in order to have the NDVI image. Now, following the naming code,
     *  the program will find images taken at same time and should to process them in order to
     *  have the NDVI image. This NDVI image is obtained (NIR - RED)/(NIR + RED)
     */

    bool _ndvi_process = false;

    try{
        if(argc>1){
            std::string argum = argv[1];
            std::size_t found = argum.find("--NDVI-COMPOSE");
            if (found != std::string::npos){
                _ndvi_process = true;
                chdir(argv[2]);
            }
            else
                chdir(argv[1]);
        }
        else
            chdir(argv[0]);
    }catch(...){
        std::cout << "The parameters are wrong. Please be sure the using:" << std::endl;
        std::cout << "Sequoia_processing [--NDVI-COMPOSE] [imagePath]" << std::endl;
        std::cout << "If you want to process raw images, please use the --NDVI-COMPOSE parameter and point imagePath" << std::endl;
        std::cout << "to folder where the sequoia images are. In other case, imagePath should point to folder where the " << std::endl;
        std::cout << "processed NDVI images are. " << std::endl;
        return(1);
    }

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string _WorkingPath = cwd;

    std::cout <<"Looking for images in the folder: "<< _WorkingPath << std::endl;
    // Defining variables

    cv::cuda::GpuMat _Nir, _Red, _Num, _Den, _NdviGpu;
    cv::Mat _Ndvi;
    std::vector<std::string> files;

    cv::glob(_WorkingPath, files);
    std::string _OutputPath = _WorkingPath + "/NDVI_out";
    //_WorkingPath = _OutputPath;

    try{
        mkdir(_OutputPath.c_str(), 0755);
    }catch(...){
      std::cout << "Unable to create directory Output inside " <<  _WorkingPath << std::endl;
      std::cout << "Delete the folder and try again." << std::endl;
      exit(1);
    }

#ifdef  DEBUG
    std::cout << "Creating NDVI images inside " << _OutputPath << std::endl;
    std::cout <<"There are " << files.size() << " images in the folder: "<< _WorkingPath << std::endl;
#endif
    std::string _namecode;
    std::string _ImgName;
    try{
        for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it) {
            _ImgName= *it;

#ifdef  DEBUG
            std::cout << _ImgName << std::endl;
#endif
            std::size_t found = _ImgName.find("_NIR");
            if (found != std::string::npos){

#ifdef  DEBUG
                std::cout << "Processing " << _ImgName << std::endl;
#endif

                _Ndvi = cv::imread(_ImgName, cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);      // Reading the NIR image
                _Nir.upload( _Ndvi);
                std::cout << "NIR size (" << _Ndvi.size() << ")" << std::endl;
                _Ndvi.release();

                std::size_t foundPath = _ImgName.find_last_of("/\\");
                _namecode = _ImgName.substr(0, found);                                // Find the sequence code name
                _namecode = _namecode.substr(foundPath);

                _ImgName = _WorkingPath + _namecode + "_RED.TIF";

#ifdef  DEBUG
                std::cout << "Processing " << _ImgName << std::endl;

#endif

                _Ndvi = cv::imread(_ImgName, cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);      // Reading the RED image
                _Red.upload(_Ndvi);
                std::cout << "RED size (" << _Ndvi.size() << ")" << std::endl;
                _Ndvi.release();


                _namecode = _OutputPath + _namecode + "_NDVI.TIF";                      // create the name of ndvi ouput image

                // Calculates the NDIV image from RED and NIR images.
                std::cout  << "adding\n";

                cv::cuda::add(_Nir,_Red, _Den);

                std::cout << "Substracting\n";
                // Computing the sum
                cv::cuda::subtract(_Nir, _Red, _Num);                                    // Computing the substract
                _Num.convertTo(_Num,CV_32FC1);
                _Den.convertTo(_Den,CV_32FC1);
                cv::cuda::divide(_Num, _Den, _NdviGpu,CV_32FC1);                         // Obtaining the NDVI image
                _NdviGpu.convertTo(_NdviGpu,CV_16UC1, 65536/3.f);                   // Convert NDVI image to integers. This method may be improved with equalization.
                _NdviGpu.download(_Ndvi);
#ifdef  DEBUG
                std::cout << "Saving " << _namecode << std::endl;
#endif
                imwrite(_namecode, _Ndvi);                                          // Writing the image to disk
                _Nir.release();
                _Red.release();
                _Num.release();                                                     // Freeing memory
                _Den.release();
                _NdviGpu.release();
#ifdef  DEBUG
//                std::cout << "NDVI = " << _Ndvi << std::endl;
                cv::namedWindow("Test", FP_NORMAL);
                cv::imshow("Test",_Ndvi);
                cv::waitKey(0);
#endif
                _Ndvi.release();
            }
        }
    }catch(...){
        std::cout << "Are you sure multispectral images are inside " << _WorkingPath << "?" << std::endl;

    }

    return 0;
}
