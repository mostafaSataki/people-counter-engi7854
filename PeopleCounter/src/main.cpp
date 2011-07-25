/*
 * main.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

using namespace cv;

using namespace std;

CvHaarClassifierCascade *cascade;
CvMemStorage *ms;
void detectBody(IplImage *img);

int main(){
	//cvNamedWindow("Grey Image", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Threshold", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Erode 1", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Dialate ", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Modified", CV_WINDOW_AUTOSIZE);
	std::string filename = "";
	//cin >> filename;
	filename = "video2.avi";
	CvCapture *fc = cvCreateFileCapture(filename.c_str());

	if(!fc)
		return 0;

	IplImage* frame = cvQueryFrame(fc);

	if (!frame) {
		return 0;
	}

	IplImage* greyImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* colourImg;
	IplImage* movingAverage = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 3);
	IplImage* difference;
	IplImage* temp;

	//	std::string filename2 = "haarcascade_upperbody.xml";
	//	cascade = (CvHaarClassifierCascade*)cvLoad(filename2.c_str(), 0, 0, 0);
	//	ms = cvCreateMemStorage(0);


	IplImage* prevFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* eig_image = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
	IplImage* tmp_image = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
	int corner_count = 500;
	CvPoint2D32f *cornersA = new CvPoint2D32f[corner_count];
	int win_size = 10;

	CvRect bndRect = cvRect(0,0,0,0);

	CvPoint pt1, pt2;

	CvFont font;
	char buf[65];

	int frame_count = 0;
	int avgY = 0;
	bool first = true;

	int numPeople = 0;

	while((frame = cvQueryFrame(fc)) != NULL){

		//cvWaitKey(0);
		colourImg = cvCloneImage(frame);

		if(!colourImg)
			return 0;


		if(first){
			difference = cvCloneImage(colourImg);
			temp = cvCloneImage(colourImg);
			cvConvertScale(colourImg, movingAverage, 1.0, 0.0);
			first = false;
		}else{
			cvRunningAvg(colourImg, movingAverage, 0.020, NULL);
		}

		cvConvertScale(movingAverage, temp, 1.0, 0.0);

		cvAbsDiff(colourImg, temp, difference);

		cvCvtColor(difference, greyImg, CV_RGB2GRAY);

		cvThreshold(greyImg, greyImg, 38, 255, CV_THRESH_BINARY);


		//cvShowImage("Threshold", greyImg);
		cvErode(greyImg, greyImg, 0, 8);
		//cvShowImage("Erode 1", greyImg );
		cvDilate(greyImg, greyImg, 0, 35);
		//cvShowImage("Dialate ", greyImg );
		//		cvErode(greyImg, greyImg, 0, 37);
		//		cvShowImage("Grey Image", greyImg);
		//		cvDilate(greyImg, greyImg, 0, 2);

		cvRectangle(colourImg, cvPoint(2, colourImg->height/2), cvPoint(colourImg->width, colourImg->height/2), CV_RGB(0,0,255),1);

		if(frame_count > 100){
			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* contour = 0;
			cvFindContours(greyImg, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

			for(;contour != 0; contour = contour->h_next){
				bndRect = cvBoundingRect(contour, 0);
				pt1.x = bndRect.x;
				pt1.y = bndRect.y;
				pt2.x = bndRect.x + bndRect.width;
				pt2.y = bndRect.y + bndRect.height;

				avgY = (pt1.y+pt2.y)/2;
				cvRectangle(colourImg, pt1, pt2, CV_RGB(255,0,0), 1);
				cvCircle(colourImg, cvPoint((pt1.x+pt2.x)/2, avgY), 5, CV_RGB(0, 255, 0)), 2;


				cvGoodFeaturesToTrack(prevFrame, eig_image, tmp_image, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04);
				cvFindCornerSubPix(prevFrame, cornersA, corner_count, cvSize(win_size, win_size), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));
				char features_found[500];
				float feature_errors[500];

				CvSize pyr_sz = cvSize(frame->width+8, frame->height/3);
				IplImage *pyrA = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
				IplImage *pyrB = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);

				CvPoint2D32f *cornersB = new CvPoint2D32f[500];

				cvSetImageROI(prevFrame, cvRect(0, 50, frame->width, frame->height-50));
				cvSetImageROI(greyImg, cvRect(0, 50, frame->width, frame->height-50));
				cvCalcOpticalFlowPyrLK(prevFrame, greyImg, pyrA, pyrB, cornersA, cornersB, corner_count, cvSize(win_size, win_size), 5, features_found, feature_errors, cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.3), 0);
				cvResetImageROI(prevFrame);
				cvResetImageROI(greyImg);

				for(int i = 0; i < corner_count; i++){
					if(features_found[i] == 0 || feature_errors[i] > 1000)
						continue;
					CvPoint p0 = cvPoint(
							cvRound( cornersA[i].x ),
							cvRound( cornersA[i].y )
					);
					CvPoint p1 = cvPoint(
							cvRound( cornersB[i].x ),
							cvRound( cornersB[i].y )
					);
					cvLine( colourImg, p0, p1, CV_RGB(255,0,0),2 );

//				IplImage *velx = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
//				IplImage *vely = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
//
//				cvCalcOpticalFlowLK(prevFrame, greyImg, cvSize(10,10), velx, vely);
//				cvWaitKey(0);
//				cvShowImage("velx", velx);
//				cvShowImage("vely", vely);
				}
			}
		}

		cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.2, 1.2, 1, 2);
		cvPutText(colourImg, _itoa(numPeople, buf, 10), cvPoint(20, 20), &font, cvScalar(0,0,300));

		cvShowImage("Modified", colourImg);

		//		if(frame_count > 100)
		//			detectBody(frame);

		prevFrame = cvCloneImage(greyImg);
		frame_count++;
		char c = cvWaitKey(132);
		if (c == 27) {
			cvReleaseImage(&frame);
			//cvReleaseImage(&temp);
			//cvReleaseImage(&difference);
			//cvReleaseImage(&greyImg);
			//cvReleaseImage(&movingAverage);

			cvReleaseCapture(&fc);

			//cvDestroyWindow("Stock");
			cvDestroyWindow("Modified");
			return 0;
		}
	}
}

//void detectBody(IplImage* img)
//{
//	IplImage* greyImg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
//	int i;
//	cvClearMemStorage(ms);
//	CvSeq *bodies;
//	cvCvtColor(img, greyImg, CV_RGB2GRAY);
//	bodies = cvHaarDetectObjects(greyImg, cascade, ms, 1.3, 3, 0, cvSize(40, 100));
//	for(i = 0; i < (bodies ? bodies->total : 0); i++){
//		CvRect *r = (CvRect*)cvGetSeqElem(bodies, i);
//		cvRectangle(img, cvPoint(r->x, r->y),cvPoint( r->x + r->width, r->y + r->height), CV_RGB(255, 0, 0), 1, 8, 0);
//		cvShowImage("Modified", img);
//	}
//}
