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
#include <opencv2/gpu/gpu.hpp>
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

    cv::Mat _imgGrey;
    if( _imgIn.channels() != 1 )            //  First, test if it's a gray image
        cv::cvtColor(_imgIn, _imgGrey, CV_BGRA2GRAY);

    cv::namedWindow( "Gray", 1 );
    cv::imshow( "Gray", _imgGrey );


    cv::threshold( _imgGrey, _imgGrey, 150, 255, cv::THRESH_BINARY_INV ); //Threshold the gray
    cv::namedWindow( "Threshold", 1 );
    cv::imshow( "Threshold", _imgGrey );
    cv::waitKey(0);

    int largest_area=0;
    int largest_contour_index=0;
    cv::Rect bounding_rect;
    std::vector<cv::vector<cv::Point>> contours; // Vector for storing contour
    std::vector<cv::Vec4i> hierarchy;
    findContours( _imgGrey, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    // iterate through each contour.
    for( int i = 0; i< contours.size(); i++ )
    {
        //  Find the area of contour
        double a = cv::contourArea( contours[i],false);
        if( a > largest_area ){
            largest_area = a;
            std::cout << i << " area  " << a << std::endl;
            // Store the index of largest contour
            largest_contour_index = i;
            // Find the bounding rectangle for biggest contour
            bounding_rect = boundingRect(contours[i]);
        }
    }
    cv::Scalar color(255,255,255);  // color of the contour in the
    //Draw the contour and rectangle
    _imgOut = _imgIn.clone();
    cv::drawContours( _imgOut, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy);
    cv::rectangle(_imgOut, bounding_rect, cv::Scalar(0,255,0), 2, 8,0);


    //  Here end the process and show the results.
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    cv::imwrite(_imgOutPath, _imgOut);          // Writing the image to disk
    std::cout << "The image was create in: " << _imgOutPath << std::endl;
    cv::namedWindow("Example", CV_NORMAL);
    cv::imshow("Example",_imgOut);
    cv::waitKey(0);



    return 0;
}
