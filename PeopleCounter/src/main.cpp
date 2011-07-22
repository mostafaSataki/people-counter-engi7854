/*
 * main.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "CamCapture.h"
#include <iostream>
#include "windows.h"
#include "ProcessedVideo.h"
#include "CamCapture.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

//class ProcessedVideo;
//class CamCapture;

using namespace std;

DWORD WINAPI ThreadFn(LPVOID vpParam);
bool done;
int main(){

	done = false;
	//create a thread
	unsigned int uiCounter = 0;
	DWORD qThreadID;
	// uiCounter is the paramter that can be passed into the thread upon execution if needed
	HANDLE hThread = CreateThread(0,0,ThreadFn,&uiCounter,0,&qThreadID);

	CamCapture *camera = new CamCapture();
	camera->displayVideo();
	CloseHandle(hThread);
	return 0;
}

DWORD WINAPI ThreadFn(LPVOID vpParam){
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

        imshow("edges", edges);
        if(((cvWaitKey(1) & 255) == 27)){
        	cvDestroyWindow("edges");
        	break;
        }
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


