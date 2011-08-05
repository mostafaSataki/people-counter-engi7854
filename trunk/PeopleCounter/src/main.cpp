/*
 * main.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <iostream>
#include "massCenter.h"

using namespace cv;

using namespace std;

CvHaarClassifierCascade *cascade;
CvMemStorage *ms;
void detectBody(IplImage *img);

int main(){
	// main program window
	cvNamedWindow("Modified", CV_WINDOW_AUTOSIZE);
	// vector to store object centers
	std::vector<massCenter> centersList;
	// create a blank string to hold filename
	std::string filename = "";
	// assign the file name, no path indicates same location as .exe, absolute path is also acceptable
	filename = "video6.avi";
	// create the frame capture tool for grabing frame from a file
	CvCapture *fc = cvCreateFileCapture(filename.c_str());

	// check to see the capture was created
	if(!fc)
		return 0;

	// Load a frame from the capture tool
	IplImage* frame = cvQueryFrame(fc);

	//check to see if the frame was loaded
	if (!frame) {
		return 0;
	}

	//holder images for image function below
	IplImage* greyImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* colourImg;
	IplImage* movingAverage = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 3);
	IplImage* difference;
	IplImage* temp;
	IplImage* prevFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* eig_image = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
	IplImage* tmp_image = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);

	// set a max corer for optical flow, 500 corners able to be tracked at one time
	int corner_count = 500;

	// corner points array for optical flow
	CvPoint2D32f *cornersA = new CvPoint2D32f[corner_count];
	int win_size = 10;

	//Initialise a bounding box to be used later
	CvRect bndRect = cvRect(0,0,0,0);

	// initialise two points for use later
	CvPoint pt1, pt2;

	// initialise a font for using text later
	CvFont font;
	char buf[65];

	//initiaise frame count
	int frame_count = 0;

	//variables used later in the bounding rects
	int avgY = 0;
	int avgX = 0;

	// current size of bounding rects
	int currentListSize;

	//bool to tell the program if a new blob has been hadded
	bool addNewCenter;

	// bool to tell the program it is thefirst run through
	bool first = true;

	//initialize the number of people to 0
	int numPeople = 0;

	//while there is a frame to capture
	while((frame = cvQueryFrame(fc)) != NULL){

		colourImg = cvCloneImage(frame);

		//ensure the colorImg was cloned properly
		if(!colourImg)
			return 0;

		//compute the initial image to use in runing average
		if(first){
			difference = cvCloneImage(colourImg);
			temp = cvCloneImage(colourImg);
			cvConvertScale(colourImg, movingAverage, 1.0, 0.0);
			first = false;
		}else{
			//consistantly adjust the running avg of the background
			cvRunningAvg(colourImg, movingAverage, 0.020, NULL);
		}


		cvConvertScale(movingAverage, temp, 1.0, 0.0);
		//take absolute difference between current frame and saved background
		cvAbsDiff(colourImg, temp, difference);
		//convert the frame to greylevels
		cvCvtColor(difference, greyImg, CV_RGB2GRAY);
		//threshold image
		cvThreshold(greyImg, greyImg, 38, 255, CV_THRESH_BINARY);
		//erode to remove small objects
		cvErode(greyImg, greyImg, 0, 6);//6
		//dilate re-expand original objects and merge near by objects
		cvDilate(greyImg, greyImg, 0, 33);//33
		//erode to make new objects smaller
		cvErode(greyImg, greyImg, 0, 6);//6

		//red line -50Pixels from center
		cvLine(colourImg, cvPoint(2, (colourImg->height/2)-50), cvPoint(colourImg->width, (colourImg->height/2)-50), CV_RGB(255,0,0),2,CV_AA);
		//blue line at frame center
		cvLine(colourImg, cvPoint(2, colourImg->height/2), cvPoint(colourImg->width, (colourImg->height/2)), CV_RGB(0,0,255),2,CV_AA);
		//red line +50Pixels from center
		cvLine(colourImg, cvPoint(2, (colourImg->height/2)+50), cvPoint(colourImg->width, (colourImg->height/2)+50), CV_RGB(255,0,0),2,CV_AA);


//Remove these comments to add green lines every +- 50 pixals from center of video
//		for(int j=frame->height/2; j<frame->height; j = j+50){
//			cvLine(colourImg, cvPoint(0,j), cvPoint(colourImg->width, j), CV_RGB(0,255,0),1);
//		}
//		for(int j = frame->height/2; j>=0; j = j-50){
//			cvLine(colourImg, cvPoint(0,j), cvPoint(colourImg->width, j), CV_RGB(0,255,0),1);
//		}

		//Don't contour track for the 100 frame setup period
		if(frame_count > 100){
			//Memory to use for finding/storing contours
			CvMemStorage* storage = cvCreateMemStorage(0);
			//Sequences are linked lists of structures. They will be used to store lists of points in each contour.
			CvSeq* contour = 0;
			//Find the contours and store them in a CCOMP structure, approximating to points from chain code
			cvFindContours(greyImg, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
			//For every contour
			for(;contour != 0; contour = contour->h_next){
				//Get the bounding rectangle
				bndRect = cvBoundingRect(contour, 0);
				pt1.x = bndRect.x;
				pt1.y = bndRect.y;
				pt2.x = bndRect.x + bndRect.width;
				pt2.y = bndRect.y + bndRect.height;
				//Calculate its center
				avgY = (pt1.y+pt2.y)/2;
				avgX = (pt1.x + pt2.x )/ 2;
				//Draw the rectangle and center
				cvRectangle(colourImg, pt1, pt2, CV_RGB(255,0,0), 1);
				cvCircle(colourImg, cvPoint((pt1.x+pt2.x)/2, avgY), 5, CV_RGB(0, 255, 0)), 2;

				//If its a new list
				if(centersList.size() == 0){
					centersList.push_back(*(new massCenter(avgX,avgY)));
				}
				else{
					//record the current size of the list, as new items may be added to the list
					currentListSize = centersList.size();
					addNewCenter = true;

					//update objects, objects within range are the same object, else it is a new object
					for(int i=0;i<currentListSize;i++){
						if ( (avgY > (centersList[i].lasty -70))  && (avgY < (centersList[i].lasty + 70)) ){
							if ( (avgX > (centersList[i].lastx -55)) && (avgX < (centersList[i].lastx + 55)) ) {
								centersList[i].prevX.push_back(centersList[i].lastx);
								centersList[i].prevY.push_back(centersList[i].lasty);
								centersList[i].lastx = avgX;
								centersList[i].lasty = avgY;
								addNewCenter = false;
								//std::cout<<"Updated! Centers List " << i << "\n";
							}
						}
					}
					if(addNewCenter == true){
						centersList.push_back(*(new massCenter(avgX,avgY)));
					}
				}
				//Finds good features to track, which are corners and areas of high contrast/color range
				cvGoodFeaturesToTrack(prevFrame, eig_image, tmp_image, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04);
				//Find corners to a subpixel accuracy
				cvFindCornerSubPix(prevFrame, cornersA, corner_count, cvSize(win_size, win_size), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));
				char features_found[500];
				float feature_errors[500];

				//O'Reilly recommends the pyrmidal method of optical flow, this algorithm was extrapolated from that.
				CvSize pyr_sz = cvSize(frame->width+8, frame->height/3);
				IplImage *pyrA = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
				IplImage *pyrB = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);

				CvPoint2D32f *cornersB = new CvPoint2D32f[500];

				cvCalcOpticalFlowPyrLK(prevFrame, greyImg, pyrA, pyrB, cornersA, cornersB, corner_count, cvSize(win_size, win_size), 5, features_found, feature_errors, cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.3), 0);

				//Here, all the flows are found and averaged, then a single line is drawn.

				/*** COMMENT FROM HERE TO NEXT COMMENT TO NOT DISPLAY OPTICAL FLOW SINCE ITS NOT ACTUALLY USED ***/
				for(int i = 0; i < corner_count; i++){
					CvPoint p0 = cvPoint(
							cvRound( cornersA[i].x ),
							cvRound( cornersA[i].y )
					);
					cvPutText(colourImg, _itoa(0, buf, 10),p0, &font, cvScalar(0,0,300));

					CvPoint p1 = cvPoint(
							cvRound( cornersB[i].x ),
							cvRound( cornersB[i].y )
					);
					cvPutText(colourImg, _itoa(1, buf, 10),p1, &font, cvScalar(0,0,300));

					cvLine( colourImg, p0, p1, CV_RGB(255,0,0),2 );
				}
				/*** STOP COMMENTING STUFF OUT NOW ***/
			}
		}

		//check for couting
		if(centersList.size() != 0){
			for(int i = 0; i<centersList.size(); i++){
				if( centersList[i].prevY.size() >= 2){
					if(centersList[i].checkNeg){
						if( (centersList[i].lasty > (frame->height/2)) && (centersList[i].lasty < ((frame->height/2)+30))){
							if( (centersList[i].prevY[centersList[i].prevY.size()-1] < (frame->height/2))  &&  ( centersList[i].prevY[centersList[i].prevY.size()-1] > ((frame->height/2)-30))){
								numPeople--;
								centersList[i].checkNeg = false;
							}
						}
					}
					if(centersList[i].checkPos){
						if( (centersList[i].lasty < (frame->height/2)) && (centersList[i].lasty > ((frame->height/2)-30))){
							if( (centersList[i].prevY[centersList[i].prevY.size()-1] > (frame->height/2))  &&  ( centersList[i].prevY[centersList[i].prevY.size()-1] < ((frame->height/2)+30))){
								numPeople++;
								centersList[i].checkPos = false;
							}
						}
					}
				}
			}
		}

		//Draw the count to the top left corner
		cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.2, 1.2, 1, 2);
		cvPutText(colourImg, _itoa(numPeople, buf, 10), cvPoint(20, 20), &font, cvScalar(0,0,300));

		//display the frame
		cvShowImage("Modified", colourImg);

		prevFrame = cvCloneImage(greyImg);
		frame_count++;
		//Uncomment below for stepping mode
		//cvWaitKey(0);
		char c = cvWaitKey(50);
		//if esc is pressed, clean up and exit
		if (c == 27) {
			cvReleaseImage(&frame);
			cvReleaseCapture(&fc);
			cvDestroyWindow("Modified");
			return 0;
		}
	}
}
