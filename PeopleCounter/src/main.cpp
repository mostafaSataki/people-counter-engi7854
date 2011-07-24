/*
 * main.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */
#define BOOST_THREAD_USE_LIB

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>


using namespace cv;

using namespace std;

int main(){
	cvNamedWindow("Stock", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("BGHist", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("StockHist", CV_WINDOW_AUTOSIZE);
	std::string filename = "";
	//cin >> filename;
	filename = "video6.avi";
	CvCapture *fc = cvCreateFileCapture(filename.c_str());

	if(!fc)
		return 0;

	IplImage* frame = cvQueryFrame(fc);

	if (!frame) {
		return 0;
	}

	IplImage* hsv = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
	IplImage* h_plane = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* s_plane = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* v_plane = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	IplImage* planes[] = {h_plane, s_plane};
	IplImage* hist_img;
	int scale = 10;

	int frame_count = 0;
	int h_bins = 30, s_bins = 32;

	//CvHistogram* hists[10];
	CvHistogram* hist = NULL;
	CvHistogram* bg_hist = NULL;
	CvHistogram* prev_hist = NULL;
	CvHistogram* prev_prev_hist = NULL;

	while((frame = cvQueryFrame(fc)) != NULL){

		cvCvtColor(frame, hsv, CV_BGR2HSV);
		cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);
		{
			int	hist_size[] = {h_bins, s_bins};
			float h_ranges[] = {0, 180}; //hue is 0-180
			float s_ranges[] = {0, 255};
			float* ranges[] = {h_ranges, s_ranges};
			hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, ranges, 1);
		}

		cvCalcHist(planes, hist, 0, 0);
		cvNormalizeHist(hist, 1.0);


		hist_img = cvCreateImage(cvSize(h_bins*scale, s_bins*scale), 8, 3);
		cvZero(hist_img);
		float max_value = 0;
		cvGetMinMaxHistValue(hist, 0, &max_value, 0, 0);
		for(int h = 0; h < h_bins; h++){
			for(int s = 0; s < s_bins; s++){
				float bin_val = cvQueryHistValue_2D(hist, h, s);
				int intensity = cvRound(bin_val*255/max_value);
				cvRectangle(hist_img, cvPoint(h*scale, s*scale), cvPoint((h+1)*scale-1, (s+1)*scale-1), CV_RGB(intensity,intensity,intensity), CV_FILLED);

			}
		}

		cvShowImage("Stock", frame);
		if(frame_count >= 3 && frame_count <= 100){
			prev_prev_hist = prev_hist;
			double correl = cvCompareHist(hist,prev_hist,CV_COMP_CORREL);
			double chi = cvCompareHist(hist,prev_hist,CV_COMP_CHISQR);
			double inter = cvCompareHist(hist,prev_hist,CV_COMP_INTERSECT);
			double batt = cvCompareHist(hist,prev_hist,CV_COMP_BHATTACHARYYA);
			double correl2 = cvCompareHist(hist,prev_prev_hist,CV_COMP_CORREL);
			double chi2 = cvCompareHist(hist,prev_prev_hist,CV_COMP_CHISQR);
			double inter2 = cvCompareHist(hist,prev_prev_hist,CV_COMP_INTERSECT);
			double batt2 = cvCompareHist(hist,prev_prev_hist,CV_COMP_BHATTACHARYYA);
			if( (correl > 0.96) && (chi < 0.04 ) && (inter > 0.96 ) && (batt <0.04 ) &&
					(correl2 > 0.96) && (chi2 < 0.04 ) && (inter2 > 0.96 ) && (batt2 <0.04 )){
				std::cout<<frame_count<<"\n";
				bg_hist = hist;
				cvShowImage("BGHist",hist_img);
			}
		}
		prev_hist = hist;

		cvShowImage("StockHist", hist_img);

		frame_count++;
		char c = cvWaitKey(66);
		if (c == 27) {
			cvReleaseImage(&frame);
			cvReleaseImage(&hsv);
			cvReleaseImage(&h_plane);
			cvReleaseImage(&s_plane);
			cvReleaseImage(&v_plane);
			//cvReleaseImage(&planes);
			cvReleaseImage(&hist_img);
			cvReleaseHist(&hist);
			cvReleaseHist(&bg_hist);
			cvReleaseCapture(&fc);
			cvDestroyWindow("Stock");
			cvDestroyWindow("BGHist");
			cvDestroyWindow("StockHist");
			return 0;
		}
	}
}
