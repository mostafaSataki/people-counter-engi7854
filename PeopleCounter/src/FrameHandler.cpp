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

using namespace cv;
using namespace std;

FrameHandler::FrameHandler(){
	mWriteBuffer = true;
}

void FrameHandler::saveFrame(Mat newFrame){
	if(getWriteBuffer()){
		//if the buffer is full before a write, clear the buffer
		if((int)mBufferA.size() == MAX_QUEUE_SIZE ){
			clearQueue(mBufferA);
		}
		//add new frame to the queue
		mBufferA.push(newFrame);

		// if the queue is full after adding. switch to the next buffer
		if((int)mBufferA.size() == MAX_QUEUE_SIZE){
			mWriteBuffer=false;
		}
	}
	else{ // if writing to buffer B
		//if the buffer is full before a write, clear the buffer
		if((int)mBufferB.size() == MAX_QUEUE_SIZE ){
			clearQueue(mBufferB);
		}
		//add the new frame to queue
		mBufferB.push(newFrame);

		// if the queue is full after adding. switch to the next buffer
		if((int)mBufferB.size() == MAX_QUEUE_SIZE){
			mWriteBuffer =true;
		}
	}
}

Mat FrameHandler::getFrame(){

	if(getReadBuffer()){ // if reading from bufferA
		Mat tmp;
		tmp = (mBufferA.front());
		mBufferA.pop();
		return tmp;
	}
	else{// if reading from bufferB
		Mat tmp;
		tmp = (mBufferB.front());
		mBufferB.pop();
		return tmp;
	}
}

void FrameHandler::clearQueue(queue<Mat> &q){
	queue<Mat> empty;
	swap(q,empty);
}

bool FrameHandler::getReadBuffer(){
	return !mWriteBuffer;
}

bool FrameHandler::getWriteBuffer(){
	return mWriteBuffer;
}
