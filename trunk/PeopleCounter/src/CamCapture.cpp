/*
 * CamCapture.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#include "CamCapture.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>

CamCapture::CamCapture(){
	cvNamedWindow("People Counter 'Camera Feed'");
	mCamCapture = cvCaptureFromCAM(CV_CAP_ANY);
}

CamCapture::~CamCapture(){
	cvReleaseCapture (&mCamCapture);
	cvReleaseImage(&mFrame);
	cvDestroyWindow("People Counter 'Camera Feed'");
}

void CamCapture::displayVideo(){
	while (1){
		mFrame = cvQueryFrame(mCamCapture);

		if(!mFrame){
			std::cout << "No Camera Frame Found!" ;
			break;
		}

		cvShowImage ("People Counter 'Camera Feed'", mFrame);
		//exit if esc is pressed
		if( (cvWaitKey(1) & 255 == 27) ) break;
	}
}

