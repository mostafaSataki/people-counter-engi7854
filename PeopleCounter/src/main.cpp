/*
 * main.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
 int main() {

   //seclect best available video capture device
   CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
   if ( !capture ) {
	 // stop if there is no appropriate device
     fprintf( stderr, "ERROR: capture is NULL \n" );
     getchar();
     return -1;
   }

   // Create a window to display video
   cvNamedWindow( "People Counter", CV_WINDOW_AUTOSIZE );
   // Show the image captured from the camera in the window and repeat
   while ( 1 ) {
     // Get one frame
     IplImage* frame = cvQueryFrame( capture );

     if ( !frame ) {
    	//stop is there is no frame captured
       fprintf( stderr, "ERROR: frame is null...\n" );
       getchar();
       break;
     }
     //display image/frame
     cvShowImage( "People Counter", frame );
     //If ESC key pressed exit
     if ( (cvWaitKey(10) & 255) == 27 ) break;
   }
   // Release the capture device housekeeping
   cvReleaseCapture( &capture );
   cvDestroyWindow( "People Counter" );
   return 0;
 }
