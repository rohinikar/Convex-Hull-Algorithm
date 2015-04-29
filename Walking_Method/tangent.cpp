#include "tangent.h"
#include "Point.h"
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
using namespace std;





//CHECK IF A POINT P2 LIES TO THE LEFT/RIGHT/ON THE LINE JOINING TWO OTHER POINTS P0 and P1
inline float orient( Point P0, Point P1, Point P2 )
{
    return (P1.x() - P0.x()) * (P2.y() - P0.y())
           - (P2.x() - P0.x()) * (P1.y() - P0.y());  
} // End of orient













//COMPUTE THE COMMON TANGENT GIVEN TWO UPPER HULLS
void tangentBetweenTwoUpperHulls( int n, vector<Point> &leftHull, int m, vector<Point> &rightHull, int* t1, int* t2 )
{
    int ix1=0, ix2=0;      // search indices for upper hulls 1 and 2
	int p=n-1;
	int q=n-2;
	//int leftSentinel=round_toward_infinity;
	//int rightSentinel=round_toward_infinity;
	int pp=0;
	int qq=1;

	
	
	
	
	while(orient(leftHull.at(p),rightHull.at(pp),rightHull.at(qq))>=0 && orient(rightHull.at(pp),leftHull.at(p),leftHull.at(q))<=0)
	{
		while(orient(leftHull.at(p),rightHull.at(pp),rightHull.at(qq))>=0)
		{
			pp=qq; //advance pp to its successor
			qq=qq+1;  //advance qq to its successor
			if(pp==m-1)  //if pp reaches the rightmost point of the hull
				break;
		}
		while(orient(rightHull.at(pp),leftHull.at(p),leftHull.at(q))<=0)
		{
			p=q;   //advance p to its predecessor
			q=q-1;   //advance q to its predecessor
			if(p==0)  //if p reaches the leftmost point of the hull
				break;
		}
		if(pp==m-1)
			break;
		if(p==0)
		{
			while(orient(leftHull.at(p),rightHull.at(pp),rightHull.at(qq))>=0)
		{
			pp=qq;
			qq=qq+1;
			if(pp==m-1)
				break;
		}
			break;
		}
	}
		
   		ix1 = p;
		ix2 = pp;
    
	
	*t1=ix1;
    *t2=ix2;
    return;
} // End of tangentBetweenTwoUpperHulls


//COMPUTE THE COMMON TANGENT GIVEN TWO LOWER HULLS
void tangentBetweenTwoLowerHulls( int n, vector<Point> &leftHull, int m, vector<Point> &rightHull, int* t1, int* t2 )
{
    int ix1=0, ix2=0;      // search indices for upper hulls 1 and 2
	int p=n-1;
	int q=n-2;
	//int leftSentinel=round_toward_infinity;
	//int rightSentinel=round_toward_infinity;
	int pp=0;
	int qq=1;

	
	
	
	
	while(orient(leftHull.at(p),rightHull.at(pp),rightHull.at(qq))<=0 && orient(rightHull.at(pp),leftHull.at(p),leftHull.at(q))>=0)
	{
		while(orient(leftHull.at(p),rightHull.at(pp),rightHull.at(qq))<=0)
		{
			pp=qq;
			qq=qq+1;
			if(pp==m-1)
				break;
		}
		while(orient(rightHull.at(pp),leftHull.at(p),leftHull.at(q))>=0)
		{
			p=q;
			q=q-1;
			if(p==0)
				break;
		}
		if(pp==m-1)
			break;
		if(p==0)
		{
			while(orient(leftHull.at(p),rightHull.at(pp),rightHull.at(qq))<=0)
		{
			pp=qq;
			qq=qq+1;
			if(pp==m-1)
				break;
		}
			break;
		}
	}
		
   		ix1 = p;
		ix2 = pp;
    
	
	*t1=ix1;
    *t2=ix2;
    return;
} // End of tangentBetweenTwoLowerHulls

