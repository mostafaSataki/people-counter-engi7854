/*
 * ProcessedVideo.h
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#ifndef PROCESSEDVIDEO_H_
#define PROCESSEDVIDEO_H_

#include <cv.h>
#include <highgui.h>
#include <stdio.h>

class ProcessedVideo{

public:
	ProcessedVideo();
	~ProcessedVideo();
	void displayVideo();

private:


	IplImage* mCurrentFrame;



};


#endif /* PROCESSEDVIDEO_H_ */
