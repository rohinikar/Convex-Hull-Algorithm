#ifndef POINT_H
#define POINT_H


#include <iostream>
#include <math.h>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>
#include <mutex>
#include <chrono>
#include <stdio.h>
using namespace std;

class Point
{
public:
	float xval,yval;
	Point(float x, float y);
	float x();
	float y();
	friend bool operator<(Point i, Point j);
};


#endif