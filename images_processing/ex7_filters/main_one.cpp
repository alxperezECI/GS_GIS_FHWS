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


    if( _imgIn.channels() != 1 )            //  First, test if it's a gray image
        cv::cvtColor(_imgIn, _imgIn, cv::COLOR_RGBA2GRAY );

    if( _imgIn.type() != CV_8U )            //  This histogram program only works with
        _imgIn.convertTo( _imgIn, CV_8U );  //  0-255 images

    cv::namedWindow( "Gray", 1 );    cv::imshow( "Gray", _imgIn );

    // Initialize parameters
    int histSize = 256;    // bin size
    float range[] = { 0, 255 };
    const float *ranges[] = { range };

    // Calculate histogram
    cv::MatND hist;
    cv::calcHist( &_imgIn, 1, 0, cv::Mat(), hist, 1, &histSize, ranges, true, false );

 /*   // Show the calculated histogram in command window
    double total;
    total = _imgIn.rows * _imgIn.cols;
    for( int h = 0; h < histSize; h++ )
         {
            float binVal = hist.at<float>(h);
            std::cout<<" "<<binVal;
         }
*/
    // Plot the histogram
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    cv::Mat histImage( hist_h, hist_w, CV_8UC1, cv::Scalar( 0,0,0) );
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

    for( int i = 1; i < histSize; i++ ){
      cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                       cv::Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                       cv::Scalar( 255, 0, 0), 2, 8, 0  );
    }

    _imgOut = histImage.clone();
    cv::namedWindow("Example", FP_NORMAL);
    cv::imshow("Example",_imgOut);
    cv::waitKey(0);


    //  The histogram could be used to equalize the image
    cv::equalizeHist( _imgIn, _imgOut );
    cv::namedWindow("Equalized", FP_NORMAL);
    cv::imshow("Equalized",_imgOut);
    cv::waitKey(0);

    //  Now is possible to make some type of filtering like smoothing
    cv::Mat kernel = (cv::Mat_<float>(3,3) <<  1, 1, 1,
                      1, 1, 1,
                      1, 1, 1);
    kernel = kernel / 9;
    cv::filter2D( _imgIn,_imgOut,_imgIn.depth(), kernel );
    cv::namedWindow("Smoothing", FP_NORMAL);
    cv::imshow("Smoothing",_imgOut);
    cv::waitKey(0);

    kernel = cv::Mat(7, 7, CV_32FC1, 1);;
    kernel = kernel / 49;
    cv::filter2D( _imgIn,_imgOut,_imgIn.depth(), kernel );
    cv::namedWindow("Smoothing 2", FP_NORMAL);
    cv::imshow("Smoothing 2",_imgOut);
    cv::waitKey(0);

    // Sharpening
    kernel = (cv::Mat_<float>(3,3) <<  -1, -1, -1,
                          -1, 17, -1,
                          -1, -1, -1);
    kernel = kernel / 9;
    cv::filter2D( _imgIn,_imgOut,_imgIn.depth(), kernel );
    cv::namedWindow("Sharpening", FP_NORMAL);
    cv::imshow("Sharpening",_imgOut);
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
