/*
 * FrameHandler.h
 *
 *  Created on: 2011-07-22
 *      Author: Justin
 */

#ifndef FRAMEHANDLER_H_
#define FRAMEHANDLER_H_

#include <queue>
#include <highgui.h>
#include <cv.h>

using namespace cv;
using namespace std;


class FrameHandler{

public:
	FrameHandler();
	~FrameHandler(){};

	//max queuesize
	static const int MAX_QUEUE_SIZE = 2;

	//Get first frame from the current Read List
	Mat getFrame();
	//Save Frame to the current WRITE list
	void saveFrame(Mat newFrame);

	//empties both buffers
	void clearBufferLists();

	//true cooresponds mBufferA is the Write List
	//flase cooresponds mBufferB is the Write List
	bool mReadFromA;

	bool threadReady;

private:

	queue<Mat> mBufferA;
	queue<Mat> mBufferB;

	// true means READ from A
	// false means READ from B
	bool getReadBuffer();

	// true means WRITE to A
	// false means WRITE to B
	bool getWriteBuffer();

	//clears the queue;
	void clearQueue(queue<Mat> &q);
	Mat tmp;


};

#endif /* FRAMEHANDLER_H_ */
