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


    std::cout << _imgIn << std::endl;
    std::cout << "Processing " << _imgInPath << std::endl;


    _imgIn = cv::imread(_imgInPath);

    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  Here you should put all the code to make the process.
    //

    cv::Mat _imgIntermediate;
    cv::medianBlur( _imgIn, _imgIntermediate, 3);

    // Convert input image to HSV
    cv::Mat _imgHSV;
    cv::cvtColor(_imgIntermediate, _imgHSV, cv::COLOR_BGR2HSV);
    cv::namedWindow("HSV image", cv::WINDOW_AUTOSIZE);
    cv::imshow("HSV image", _imgHSV);
    cv::waitKey(0);

    // Threshold the HSV image, keep only the red pixels
    cv::Mat lower_hue_range;
    cv::Mat upper_hue_range;
    cv::inRange(_imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_hue_range);
    cv::inRange(_imgHSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_hue_range);

    cv::namedWindow("Threshold lower image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Threshold lower image", lower_hue_range);
    cv::namedWindow("Threshold upper image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Threshold upper image", upper_hue_range);
    cv::waitKey(0);

    // Combine the above two images
    cv::Mat _hue_image;
    cv::addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, _hue_image);

    cv::GaussianBlur(_hue_image, _hue_image, cv::Size(9, 9), 2, 2);

    cv::namedWindow("Combined threshold images", cv::WINDOW_AUTOSIZE);
    cv::imshow("Combined threshold images", _hue_image);
    cv::waitKey(0);

    // Use the Hough transform to detect circles in the combined threshold image
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(_hue_image, circles, cv::HOUGH_GRADIENT, 1, _hue_image.rows/8, 100, 20, 0, 0);

    // Loop over all detected circles and outline them on the original image
    _imgOut = _imgIn.clone();
    if(circles.size() == 0) std::exit(-1);
    for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
        cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
        int radius = std::round(circles[current_circle][2]);

        cv::circle(_imgOut, center, radius, cv::Scalar(0, 255, 0), 5);
    }

    cv::namedWindow("Detected red circles on the input image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Detected red circles on the input image", _imgOut);
    cv::waitKey(0);


    //  Here end the process and show the results.
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    cv::imwrite(_imgOutPath, _imgOut);          // Writing the image to disk
    std::cout << "The image was create in: " << _imgOutPath << std::endl;
    cv::namedWindow("Example", FP_NORMAL);
    cv::imshow("Example",_imgOut);
    cv::waitKey(0);



    return 0;
}
