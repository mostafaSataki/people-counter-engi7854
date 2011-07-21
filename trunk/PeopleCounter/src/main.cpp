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

	CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
	cvNamedWindow( "People Counter 'Video Feed'");

	while ( 1 ) {
		// Get one frame
		IplImage* frame=cvQueryFrame( capture );

		if ( !frame ) {
			//stop is there is no frame captured
			fprintf( stderr, "ERROR: frame is null...\n" );
			getchar();
			break;
		}
		//display image/frame
		cvShowImage( "People Counter 'Video Feed'", frame );

		//If ESC key pressed exit
		if ( (cvWaitKey(10) & 255) == 27 ) break;
	}

	cvReleaseCapture( &capture );
	cvDestroyWindow( "People Counter 'Video Feed'" );

	CloseHandle(hThread);
	return 0;
}

DWORD WINAPI ThreadFn(LPVOID vpParam){
	//anything to be performed in the second thread goes in here

	CvCapture* capture2 = cvCaptureFromCAM( -1 );
	cvNamedWindow( "People Counter 'Processed Video'");

	while ( 1 ) {
		// Get one frame
		IplImage* frame2=cvQueryFrame( capture2 );

		if ( !frame2 ) {
			//stop is there is no frame captured
			fprintf( stderr, "ERROR: frame is null...\n" );
			getchar();
			break;
		}
		//display image/frame
		cvShowImage( "People Counter 'Processed Video'", frame2);

		//If ESC key pressed exit
		if ( (cvWaitKey(10) & 255) == 27 ) break;
	}

	done = false;
	cvReleaseCapture( &capture2 );
	cvDestroyWindow( "People Counter 'Processed Video'" );
	return 0;
}


