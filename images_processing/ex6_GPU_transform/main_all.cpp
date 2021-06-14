#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <glob.h>
#include <string>
#include  <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/stitching/stitcher.hpp>
//#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/opencv.hpp>


int main (int argc, char** argv){
    /*  This program can read all images inside a path passed as a first
     *  parameter and then may apply a proccess to each one and it saves
     *  the results inside a folder created to that.
     *
     *  The second parameter passed as a parameter is used inside the
     *  process.
     */

    std::string _workingPath, _sourcePath, _outputPath, _parameter;

    try{                                        //  This block deals with possible errors and prevents the error screen.
        if(argc == 3){
            _workingPath = argv[0];             //  Linux always sends the program path as a first parameter.
            _sourcePath = argv[1];              //  This is the first parameter the user sends to app.
            _parameter = argv[2];               //  This is the second one.

            std::size_t foundPath = _workingPath.find_last_of("/\\");
            _workingPath = _workingPath.substr(0,foundPath);       // Find the name

            chdir(_sourcePath.c_str());         //  This is a system call to change to the folder where the images are.

            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            _sourcePath = cwd;

            _outputPath = _workingPath + "/Output";     //  Defining the path where the app will put the images.

            mkdir(_outputPath.c_str(), 0755);   //  This is a system call to create the folder with the permission described.
            std::cout << "Folder created to leave images inside it: " << _outputPath << std::endl;
        }else
            throw(2);                           // If the user forgot any parameter we need to tell him.
    }catch(...){                                //  Herer the errors are catched.
        std::cout << "The parameters are wrong. Please be sure the using:" << std::endl;
        std::cout << "Example# [imagePath] secParameter" << std::endl << std::endl;
        std::cout << "This process all images inside the pointed path." << std::endl;
        std::cout << "=================================================" << std::endl;
        std::cout << "Also remove all Output folder and its contents." << std::endl;
        return(1);
    }

    std::vector<std::string> files;
    std::cout <<"Looking for images in the folder: "<< _sourcePath << std::endl;

    cv::glob(_sourcePath, files);
    std::cout <<"There are " << files.size() << " images in the folder: "<< _sourcePath << std::endl;


    std::string _imgInPath, _imgOutPath, _imgName;
    cv::Mat _imgIn, _imgOut;

    // At this point, start the time measuremet

    unsigned long t_AAtime=0, t_BBtime=0;
    float t_pt;
    float t_fpt;
    t_AAtime =  cv::getTickCount();
    //   These lines could be uncommented for the debug process.
    std::cout << _workingPath << std::endl;
    std::cout << _imgInPath << std::endl;
    std::cout << _imgOutPath << std::endl;

    try{
        for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it) {
            _imgInPath= *it;

            std::size_t foundPath = _imgInPath.find_last_of("/\\");
            _imgName = _imgInPath.substr(foundPath);                   // Find the sequence code name
            _imgOutPath = _outputPath + _imgName;

             _imgIn = cv::imread(_imgInPath);

#ifdef  DEBUG
            std::cout << "Processing " << _imgInPath << std::endl;
            std::cout << "Saving " << _imgOutPath << std::endl;
#endif
            //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
            //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
            //  Here you should put all the code to make the process.
            //
            //  As in other CUDA program, you should to ensure the data are uploaded
            //  to the GPU unit in order to be processed.

            cv::gpu::GpuMat _gTempMatIn, _gTempMatOut;
            _gTempMatIn.upload(_imgIn);

            double angle = std::stod( _parameter );
            cv::Mat M = cv::getRotationMatrix2D( cv::Point(_imgIn.cols/2,_imgIn.rows/2), angle, 1.0 );

            cv::gpu::warpAffine( _gTempMatIn, _gTempMatOut, M, cv::Size( _imgIn.cols,_imgIn.rows ) );


            _gTempMatOut.download(_imgOut);             //  Once the GPU processing have finished, the data should be download
                                                        //  to the CPU unit to write they to the disk.

            //  Here end the process and show the results.
            //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
            //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

            cv::imwrite(_imgOutPath, _imgOut);          // Writing the image to disk

#ifdef  DEBUG
            std::cout << "Create the image: " << _imgOutPath << std::endl;
            cv::namedWindow("Example", CV_NORMAL);
            cv::imshow("Example",_imgOut);
            cv::waitKey(0);
#endif
        }

        // time measure end
        t_BBtime = cv::getTickCount();
        t_pt = (t_BBtime-t_AAtime) /  cv::getTickFrequency();
        printf("time: %.4lf sec\n", t_pt);

      }catch(...){
        std::cout << "There are a problem with the files in " << _sourcePath << " or with the folder "
                  << _outputPath << std::endl;

      }

    return 0;
}
