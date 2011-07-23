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
	//cvNamedWindow("out", CV_WINDOW_AUTOSIZE);

	CvCapture *fc = cvCaptureFromCAM(-1);

	IplImage* frame = cvQueryFrame(fc);
	if (!frame) {
		return 0;
	}

	double fps = cvGetCaptureProperty(fc, CV_CAP_PROP_FPS);
	CvSize size = cvSize((int)cvGetCaptureProperty(fc, CV_CAP_PROP_FRAME_WIDTH), (int)cvGetCaptureProperty(fc, CV_CAP_PROP_FRAME_HEIGHT));

	CvVideoWriter *writer = cvCreateVideoWriter("video.avi", -1, fps, size);

//	IplImage* greyscale = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
//	IplImage* flip = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);

	while((frame = cvQueryFrame(fc)) != NULL){

//		frame= cvQueryFrame(fc);
		cvShowImage("in", frame);

		//cvCvtColor(frame, greyscale, CV_BGR2GRAY);
		//(greyscale, flip);
		//cvFlip(greyscale, flip, -1);
		//cvShowImage("out", flip);

		cvWriteFrame(writer, frame);
		char c = cvWaitKey(33);
		if (c == 27) {
			cvReleaseVideoWriter(&writer);
			//cvReleaseImage(&greyscale);
			cvReleaseImage(&frame);
			//cvReleaseImage(&flip);
			cvReleaseCapture(&fc);
			cvDestroyWindow("in");
			//cvDestroyWindow("out");
			return 0;
		}
	}
}
