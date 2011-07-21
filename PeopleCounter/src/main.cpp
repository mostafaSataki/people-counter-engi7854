/*
 * main.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "CamCapture.h"
#include <iostream>
#include "windows.h"
#include "ProcessedVideo.h"
#include "CamCapture.h"

//class ProcessedVideo;
//class CamCapture;

using namespace std;

DWORD WINAPI ThreadFn(LPVOID vpParam);
bool done;
int main(){

	done = false;
	//create a thread
	unsigned int uiCounter = 0;
	DWORD qThreadID;
	// uiCounter is the paramter that can be passed into the thread upon execution if needed
	HANDLE hThread = CreateThread(0,0,ThreadFn,&uiCounter,0,&qThreadID);

	CamCapture *camera = new CamCapture();
	camera->displayVideo();
	CloseHandle(hThread);
	return 0;
}

DWORD WINAPI ThreadFn(LPVOID vpParam){
	//anything to be performed in the second thread goes in here
	ProcessedVideo *pVideo = new ProcessedVideo();
	pVideo->displayVideo();
	return 0;
}


