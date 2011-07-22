/*
 * FrameHandler.cpp
 *
 *  Created on: 2011-07-22
 *      Author: Justin
 */

#include <queue>
#include <highgui.h>
#include <cv.h>
#include "FrameHandler.h"
#include <iostream>

using namespace cv;
using namespace std;

FrameHandler::FrameHandler(){
	mReadFromA = false;
	threadReady = false;
}

void FrameHandler::saveFrame(Mat newFrame){
	if(!mReadFromA){
		mBufferA.push(newFrame);
		// if the queue is full after adding. switch to the next buffer
		if((int)mBufferA.size() == MAX_QUEUE_SIZE){
			mReadFromA=true;
			threadReady=true;
		}
	}
	else{ // if writing to buffer B
		mBufferB.push(newFrame);
		// if the queue is full after adding. switch to the next buffer
		if((int)mBufferB.size() == MAX_QUEUE_SIZE){
			mReadFromA =false;
		}
		if(!threadReady)
			threadReady = true;
	}
}

Mat FrameHandler::getFrame(){
	Mat tmp;
	if(mReadFromA){
		if(mBufferA.size() != 0){ // if reading from bufferA
			tmp = (mBufferA.front());
			mBufferA.pop();
		}else{
			//std::cout << "A  is empty\n";
		}
	}
	else{// if reading from bufferB
		if(mBufferB.size() != 0){
			tmp = (mBufferB.front());
			mBufferB.pop();
		}
		else{
			//std::cout << "B is empty \n";
		}
	}
	return tmp;
}

void FrameHandler::clearQueue(queue<Mat> &q){
	queue<Mat> empty;
	swap(q,empty);
}

bool FrameHandler::getReadBuffer(){
	return true;
}

bool FrameHandler::getWriteBuffer(){
	return mReadFromA;
}
