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
#include <string>
#include <sstream>
#include <cstring>


using namespace cv;

using namespace std;

int main(){
	cvNamedWindow("in", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("out", CV_WINDOW_AUTOSIZE);

	CvCapture *fc = cvCaptureFromCAM(0);

	IplImage* frame = cvQueryFrame(fc);

	if (!frame) {
		return 0;
	}

	IplImage* greyscale = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* canny = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);

	while(1){

		frame= cvQueryFrame(fc);
		cvShowImage("in", frame);

		// manually convert to greyscale
		cvCvtColor(frame, greyscale, CV_BGR2GRAY);
		cvCanny(greyscale, canny, 10, 100, 3);
		cvShowImage("out", canny);

		char c = cvWaitKey(33);
		if (c == 27) {
			cvReleaseCapture(&fc);
			cvReleaseImage(&greyscale);
			cvReleaseImage(&frame);
			cvReleaseImage(&canny);
			cvDestroyWindow("in");
			cvDestroyWindow("out");
			return 0;
		}
	}
}
