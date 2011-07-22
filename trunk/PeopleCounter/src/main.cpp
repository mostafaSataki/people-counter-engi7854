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

using namespace std;
bool ready;
FrameHandler *mFrameHandler = new FrameHandler();
int workerFunc(FrameHandler* fHandler);
int main(){
	ready = false;

	boost::thread workerThread(workerFunc, mFrameHandler);;
	namedWindow("buffer");
	for(;;)
	{
		if(ready){
			if(mFrameHandler->getFrame().rows <= 0){
				cout << "CANNOT LOAD FROM BUFFER\n";
			}else{
				imshow("buffer", mFrameHandler->getFrame());
			}
		}else{
			//std::cout<<"not ready!\n";
		}
		if(((cvWaitKey(1) & 255) == 27)){
			cvDestroyWindow("edges");
			return 0;
		}
	}
	return 0;
}


int workerFunc(FrameHandler* fHandler){

	using namespace cv;
	VideoCapture cap(0) ; // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	//Mat edges;
	namedWindow("edges",1);
	for(;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		if(frame.size <= 0){
			cout << "NO FRAME!\n" ;
		}
		else{
			imshow("edges", frame);
			fHandler->saveFrame(frame);
		}

		if(!ready && fHandler->mReadFromA){
			ready = true;
		}
		if(((cvWaitKey(1) & 255) == 27)){
			cvDestroyWindow("edges");
			return 0;
		}
	}
	return 0;
}


