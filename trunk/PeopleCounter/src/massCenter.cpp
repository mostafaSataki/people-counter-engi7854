/*
 * massCenter.cpp
 *
 *  Created on: 2011-07-25
 *      Author: Justin
 */

#include <highgui.h>
#include <cv.h>
#include "massCenter.h"

massCenter::massCenter(int x, int y){
	lastx = x;
	lasty = y;
	checkPos = true;
	checkNeg = true;
}
