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
#include <opencv2/cudafilters.hpp>
#include <opencv2/opencv.hpp>


int main (int argc, char** argv){
    /*  This program can read only one image that it path passed as a first
     *  parameter and then may apply a proccess. The file is save in the
     *  folder where the program is.
     *
     *  The second parameter passed as a parameter is used inside the
     *  process.
     */

    std::vector<double> _parameters;
    std::string _workingPath, _imgInPath, _imgOutPath, _imgName;

    try{                                    //  This block deals with possible errors and prevents the error screen.
        if(argc >= 3){
            _workingPath = argv[0];         //  OS always sends the programm path as a first parameter.
            _imgInPath = argv[1];           //  This is the first parameter the user sends to app.

            for(int i = 2; i < argc; i++)
                _parameters.push_back( std::stod( argv[i] ) );  //  These are the rest of the parameters as a numbers.

            std::size_t foundPath = _workingPath.find_last_of("/\\");
            _workingPath = _workingPath.substr(0,foundPath); // Find the name

             foundPath = _imgInPath.find_last_of("/\\");
            _imgName = _imgInPath.substr(foundPath);       // Find the name

            _imgOutPath = _workingPath + _imgName;         //  Defining the path where the app will put the images.


        }else
            throw(2);  // If the user forgot any parameter we need to tell him.
    }catch(...){       //  Herer the errors are catched.
        std::cout << "The parameters quantity are wrong. Please be sure the using:" << std::endl;
        std::cout << "portfolio [imagePath] list of parameters" << std::endl;
        std::cout << "This program process the image pointed by its path." << std::endl;
        return(1);
    }

    // At this point, start the time measuremet

    unsigned long t_AAtime=0, t_BBtime=0;
    float t_pt;
    float t_fpt;
    t_AAtime =  cv::getTickCount();

    cv::Mat _imgIn, _imgResized, _imgOut;


    std::cout << _imgIn << std::endl;
    std::cout << "Processing " << _imgInPath << std::endl;


    _imgIn = cv::imread( _imgInPath );

    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  Here you should put all the code to make the process.
    //


    if( _imgIn.channels() != 1 )            //  First, test if it's a gray image
        cv::cvtColor(_imgIn, _imgIn, cv::COLOR_RGBA2GRAY);

    if( _imgIn.type() != CV_8U )            //  This program only works with
        _imgIn.convertTo( _imgIn, CV_8U );  //  0-255 images

    cv::namedWindow( "Gray", 1 );
    cv::imshow( "Gray", _imgIn );
    //cv::waitKey(0);

    // If the image is bigger, then resize it.
    if(_imgIn.size().width > 800 ){
        cv::resize(_imgIn, _imgResized, cv::Size(800,600) );
        cv::namedWindow( "Resized", 1 );
        cv::imshow( "Resized", _imgResized );
        //cv::waitKey(0);
    }else
        _imgResized = _imgIn;


    // To reduce the number of borders, first blur the image.
    cv::blur( _imgResized, _imgResized, cv::Size(_parameters[0], \
              _parameters[0] ) );
    cv::namedWindow( "Blur", 1 );
    cv::imshow( "Blur", _imgResized );
    //cv::waitKey(0);

    /*
    // To make bluring on GPU
    cv::cuda::GpuMat _imgInGPU, _imgOutGPU;
    _imgInGPU.upload( _imgIn );
    cv::Ptr<cv::cuda::Filter>blurfilter = cv::cuda::createBoxFilter(_imgInGPU.type(), _imgOutGPU.type(), cv::Size(_parameters[0], \
                                                      _parameters[0] ) );

    blurfilter->apply(_imgInGPU, _imgOutGPU);
    _imgOutGPU.download( _imgOut );
*/

    //  Borders detection x derivative filter
    cv::Mat _diffX, _diffY, _absX, _absY;

    cv::Mat kernel = (cv::Mat_<float>(3,3) <<  -1, 0, 1,
                          -1, 0, 1,
                          -1, 0, 1);
    kernel = kernel;
    cv::filter2D( _imgResized, _diffX, _imgIn.depth(), kernel );
    cv::namedWindow("X derivative", FP_NORMAL);
    cv::imshow("X derivative", _diffX);
    cv::waitKey(0);

    //  Borders detection y derivative filter
    kernel = (cv::Mat_<float>(3,3) <<  1, 1, 1,
                          0, 0, 0,
                          -1, -1, -1);
    kernel = kernel;
    cv::filter2D( _imgResized, _diffY, _imgIn.depth(), kernel );
    cv::namedWindow("Y derivative", FP_NORMAL);
    cv::imshow("Y derivative",_diffY);
    cv::waitKey(0);

    cv::convertScaleAbs(_diffX,_absX);
    cv::convertScaleAbs(_diffY,_absY);

    cv::addWeighted(_absX, _parameters[1],\
                    _absY, _parameters[2], 0, _imgOut);
    cv::namedWindow("Borders", FP_NORMAL);
    cv::imshow("Borders", _imgOut);
    cv::waitKey(0);

    // Using Sobel filter

    cv::Sobel( _imgResized, _diffX, _imgIn.depth(), 1, 0, 3, 2, 0, \
               cv::BORDER_DEFAULT );
    cv::Sobel( _imgResized, _diffY, _imgIn.depth(), 0, 1, 3, 2, 0, \
               cv::BORDER_DEFAULT );

    cv::convertScaleAbs(_diffX,_absX);
    cv::convertScaleAbs(_diffY,_absY);

    cv::addWeighted(_absX, _parameters[3], \
                    _absY, _parameters[4], 0, _imgOut);
    cv::namedWindow("Sobel", FP_NORMAL);
    cv::imshow("Sobel", _imgOut);
    cv::waitKey(0);

    // Looking for a contours
    cv::Mat _intermediate;
    cv::Canny( _imgResized, _intermediate, _parameters[5], _parameters[6], 3);
    _intermediate.convertTo(_imgOut, CV_8U);
    cv::namedWindow("Canny", FP_NORMAL);
    cv::imshow("Canny", _imgOut);
    //cv::waitKey(0);

    // To reduce the number of borders, first blur the image.
    cv::blur( _imgOut, _imgOut, cv::Size(10, 10) );
    cv::threshold(_imgOut, _imgOut,50, 255, cv::THRESH_BINARY );
    cv::namedWindow( "Blur", 1 );
    cv::imshow( "Blur", _imgOut );
    //cv::waitKey(0);

    //  Here end the process and show the results.
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    cv::imwrite(_imgOutPath, _imgOut);          // Writing the image to disk
    std::cout << "The image was create in: " << _imgOutPath << std::endl;

    // time measure end
    t_BBtime = cv::getTickCount();
    t_pt = (t_BBtime-t_AAtime) /  cv::getTickFrequency();
    printf("time: %.4lf sec\n", t_pt);
    return 0;
}
