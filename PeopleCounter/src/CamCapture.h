/*
 * CamCapture.h
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#ifndef CAMCAPTURE_H_
#define CAMCAPTURE_H_

#include <cv.h>
#include <highgui.h>
#include <stdio.h>

class CamCapture{

public:

	CamCapture();
	~CamCapture();
	void displayVideo();
private:


	IplImage* mFrame;
	CvCapture* mCamCapture;

};


#endif /* CamCapture_H_ */
