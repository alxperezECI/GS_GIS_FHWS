#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <glob.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/cudawarping.hpp>




int main (int argc, char** argv){
    /*  This program can read only one image that it path passed as a first
     *  parameter and then may apply a proccess. The file is save in the
     *  folder where the program is.
     *
     *  The second parameter passed as a parameter is used inside the
     *  process.
     */

    std::string _workingPath, _parameter;
    std::string _imgInPath, _imgOutPath, _imgName;

    try{                                       //  This block deals with possible errors and prevents the error screen.
        if(argc == 3){
            _workingPath = argv[0];            //  Linux always sends the programm path as a first parameter.
            _imgInPath = argv[1];              //  This is the first parameter the user sends to app.

            _parameter = argv[2];               //  This is the second one.

            std::size_t foundPath = _workingPath.find_last_of("/\\");
            _workingPath = _workingPath.substr(0,foundPath);       // Find the name

             foundPath = _imgInPath.find_last_of("/\\");
            _imgName = _imgInPath.substr(foundPath);       // Find the name

            _imgOutPath = _workingPath + _imgName;         //  Defining the path where the app will put the images.


        }else
            throw(2);                           // If the user forgot any parameter we need to tell him.
    }catch(...){                                //  Herer the errors are catched.
        std::cout << "The parameters are wrong. Please be sure the using:" << std::endl;
        std::cout << "Example1 [imagePath] secParameter" << std::endl;
        std::cout << "This program process one image pointed by the path." << std::endl;
        return(1);
    }

    cv::Mat _imgIn, _imgOut;

    // At this point, start the time measuremet

    unsigned long t_AAtime=0, t_BBtime=0;
    float t_pt;
    float t_fpt;
    t_AAtime =  cv::getTickCount();

    _imgIn = cv::imread(_imgInPath);
    //std::cout << _imgIn << std::endl;
    std::cout << "Processing " << _imgInPath << std::endl;

    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  Here you should put all the code to make the process.
    //
    //  As in other CUDA program, you should to ensure the data are uploaded
    //  to the GPU unit in order to be processed.

    cv::cuda::GpuMat _gTempMatIn, _gTempMatOut;
    _gTempMatIn.upload(_imgIn);

    double angle = std::stod( _parameter );
    cv::Mat M = cv::getRotationMatrix2D(cv::Point(_imgIn.cols/2,_imgIn.rows/2), angle, 1 );

    for(int i =0; i< 1000; i++)
        cv::cuda::warpAffine( _gTempMatIn, _gTempMatOut, M, cv::Size(_imgIn.cols,_imgIn.rows) );

    _gTempMatOut.download(_imgOut);             //  Once the GPU processing have finished, the data should be download
                                                //  to the CPU unit to write they to the disk.

    //  Here end the process and show the results.
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    // time measure end
    t_BBtime = cv::getTickCount();
    t_pt = (t_BBtime-t_AAtime) /  cv::getTickFrequency();
    printf("time: %.4lf sec\n", t_pt);

    cv::imwrite(_imgOutPath, _imgOut);          // Writing the image to disk
    std::cout << "The image was create in: " << _imgOutPath << std::endl;
    cv::namedWindow("Example",FP_NORMAL );
    cv::imshow("Example",_imgOut);
    cv::waitKey(0);




    return 0;
}
