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

	cout << "done!" ;
	return 0;

}

DWORD WINAPI ThreadFn(LPVOID vpParam){
	//	unsigned int& uirCounter = *((unsigned int*)vpParam);
	//	while(uirCounter < 10000000 ){
	//		++uirCounter;
	//	}

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

	cout << "thread finished!";
	done = false;
	cvReleaseCapture( &capture2 );
	cvDestroyWindow( "People Counter 'Processed Video'" );
	return 0;
}

//int main() {
//	//seclect best available video capture device
//	CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
//	if ( !capture ) {
//		// stop if there is no appropriate device
//		fprintf( stderr, "ERROR: capture is NULL \n" );
//		getchar();
//		return -1;
//	}
//
//	// Create a window to display video
//	cvNamedWindow( "People Counter 'Video Feed'" );
//	cvNamedWindow( "People Counter 'Processed Video'");
//	// Show the image captured from the camera in the window and repeat
//	while ( 1 ) {
//		// Get one frame
//		IplImage* frame=cvQueryFrame( capture );
//
//		if ( !frame ) {
//			//stop is there is no frame captured
//			fprintf( stderr, "ERROR: frame is null...\n" );
//			getchar();
//			break;
//		}
//		//display image/frame
//		cvShowImage( "People Counter 'Video Feed'", frame );
//		cvShowImage( "People Counter 'Processed Video'", frame);
//
//		//If ESC key pressed exit
//		if ( (cvWaitKey(10) & 255) == 27 ) break;
//	}
//	// Release the capture device housekeeping
//	cvReleaseCapture( &capture );
//	cvDestroyWindow( "People Counter 'Video Feed'" );
//	cvDestroyWindow( "People Counter 'Processed Video'" );
//	return 0;
//}


