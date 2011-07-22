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
#include "FrameHandler.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
using namespace cv;

using namespace std;
FrameHandler *mFrameHandler = new FrameHandler();
int workerFunc(FrameHandler* fHandler);
int main(){
	namedWindow("buffer");
	boost::thread workerThread(workerFunc, mFrameHandler);;
	cvWaitKey(500);
	for(;;)
	{
		if(mFrameHandler->threadReady){
			if(mFrameHandler->getFrame().rows <= 0){
				//cout << "No Frame\n";
			}else{
				imshow("buffer", mFrameHandler->getFrame());
			}
		}else{
			std::cout<<"waiting....\n";
		}


		if(((cvWaitKey(1) & 255) == 27)){
			cvDestroyWindow("edges");
			return 0;
		}
	}
	return 0;
}


int workerFunc(FrameHandler* fHandler){


	VideoCapture cap(0);
	//Mat edges;
	namedWindow("edges",1); ; // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;


	for(;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		if(frame.size <= 0){
			cout << "NO FRAME!\n" ;
		}
		else{
			fHandler->saveFrame(frame);
			imshow("edges", frame);

		}
		if(((cvWaitKey(1) & 255) == 27)){
			cvDestroyWindow("edges");
			return 0;
		}
	}
	return 0;
}

//int main(){
//	cvNamedWindow("in", CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("out", CV_WINDOW_AUTOSIZE);
//
//	CvCapture *fc = cvCaptureFromCAM(0);
//
//	IplImage* frame = cvQueryFrame(fc);
//
//	if (!frame) {
//	    return 0;
//	}
//
//	IplImage* greyscale = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
//	IplImage* output = cvCreateImage(cvGetSize(frame),IPL_DEPTH_32F , 1);
//
//	while(1){
//
//	    frame= cvQueryFrame(fc);
//	    cvShowImage("in", frame);
//
//	    // manually convert to greyscale
//	    for (int y = 0; y < frame->height; y++) {
//	        uchar* p = (uchar*) frame->imageData + y* frame->widthStep; // pointer to row
//	        uchar* gp = (uchar*) greyscale->imageData + y*greyscale->widthStep;
//	        for(int x = 0; x < frame->width; x++){
//	            gp[x] = (p[3*x] + p[3*x+1] + p[3*x+2])/3;   // average RGB values
//	        }
//	    }
//
//	    cvShowImage("out", greyscale);
//
//	    char c = cvWaitKey(33);
//	    if (c == 27) {
//	        return 0;
//	    }
//	}
//}
