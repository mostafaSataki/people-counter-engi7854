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
}

void FrameHandler::saveFrame(Mat newFrame){
	if(!mReadFromA){
		//if the buffer is full before a write, clear the buffer
		//		if((int)mBufferA.size() == MAX_QUEUE_SIZE ){
		//			clearQueue(mBufferA);
		//		}
		//add new frame to the queue
		mBufferA.push(newFrame);
		std::cout << "writing to A\n";

		// if the queue is full after adding. switch to the next buffer
		if((int)mBufferA.size() == MAX_QUEUE_SIZE){
			mReadFromA=true;
		}
	}
	else{ // if writing to buffer B
		//if the buffer is full before a write, clear the buffer
		//		if((int)mBufferB.size() == MAX_QUEUE_SIZE ){
		//			clearQueue(mBufferB);
		//		}
		//add the new frame to queue
		mBufferB.push(newFrame);
		std::cout << "writing to B\n";

		// if the queue is full after adding. switch to the next buffer
		if((int)mBufferB.size() == MAX_QUEUE_SIZE){
			mReadFromA =false;
		}
	}
}

Mat FrameHandler::getFrame(){
	Mat tmp;
	if(mReadFromA){
		if(mBufferA.size() != 0){ // if reading from bufferA
			tmp = (mBufferA.front());
			mBufferA.pop();
			std::cout << "reading from A\n";
		}
	}
	else{// if reading from bufferB
		if(mBufferB.size() != 0){
			tmp = (mBufferB.front());
			mBufferB.pop();
			std::cout << "reading from B\n";
		}
		else{
			std::cout << "nothing to read\n";
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
