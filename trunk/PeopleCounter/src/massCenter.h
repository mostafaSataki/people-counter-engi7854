/*
 * massCenter.h
 *
 *  Created on: 2011-07-25
 *      Author: Justin
 */

#ifndef MASSCENTER_H_
#define MASSCENTER_H_

#include <highgui.h>
#include <cv.h>

class massCenter{

public :

	massCenter(int x, int y);
	~massCenter(){};

	int lastx;
	int lasty;
	std::vector<int> prevX;
	std::vector<int> prevY;
	bool checkPos, checkNeg;


private :

};
#endif /* MASSCENTER_H_ */
