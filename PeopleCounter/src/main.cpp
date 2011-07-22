/*
 * main.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */
#define BOOST_THREAD_USE_LIB

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "CamCapture.h"
#include <iostream>
#include "ProcessedVideo.h"
#include "CamCapture.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>


using namespace std;

int workerFunc();
int main(){
	boost::thread workerThread(workerFunc);

	CamCapture *camera = new CamCapture();
	camera->displayVideo();
	return 0;
}

int workerFunc(){
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0, 1, CV_AA);

	using namespace cv;
    VideoCapture cap(0) ; // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);

        putText(edges, "Hello World!", cvPoint(10, 130), &font, cvScalar(255, 255, 255), 0 , 0 , true);
        imshow("edges", edges);
        if(((cvWaitKey(1) & 255) == 27)){
        	cvDestroyWindow("edges");
        	break;
        }
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


