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
inline position relativePosition( Point P0, Point P1, Point P2 )
{
    if (((P1.x() - P0.x()) * (P2.y() - P0.y())
           - (P2.x() - P0.x()) * (P1.y() - P0.y())) < 0)  //P2 lies to the right of line joining P0 and P1
		   return ABOVE;                    //implies that P1 lies above line joining P0 and P2
	else if (((P1.x() - P0.x()) * (P2.y() - P0.y())    
           - (P2.x() - P0.x()) * (P1.y() - P0.y())) > 0)   //P2 lies to the left of line joining P0 and P1
		   return BELOW;      //implies that P1 lies below line joining P0 and P2
	else
		   return ON;
} // End of relativePosition


// tests if a point P is above a line joining Vi and Vj  
inline bool isAbove(Point P, Point Vi, Point Vj)
{
	if(Vi.x() <= Vj.x())
	{
		return(relativePosition(Vi, P, Vj) == ABOVE); //checks if the point P is above the line joining Vi and Vj
	}
	else
    {
	     return(relativePosition(Vj, P, Vi) == ABOVE);   //checks if the point P is above the line joining Vj and Vi
	}
} //End of isAbove

// tests if a point P is below a line joining Vi and Vj  
inline bool isBelow(Point P, Point Vi, Point Vj)
{
	if(Vi.x() <= Vj.x())
	{
		return(relativePosition(Vi, P, Vj) == BELOW);   //checks if the point P is below the line joining Vi and Vj
	}
	else
    {
	     return(relativePosition(Vj, P, Vi) == BELOW);   //checks if the point P is below the line joining Vj and Vi
	}
} //End of isBelow



// tests if a point P lies on a line joining Vi and Vj  
inline bool isOn(Point P, Point Vi, Point Vj)
{
	if(Vi.x() <= Vj.x())
	{
		return(relativePosition(Vi, P, Vj) == ON);   //checks if the point P lies on the line joining Vi and Vj
	}
	else
    {
	     return(relativePosition(Vj, P, Vi) == ON);   //checks if the point P lies on the line joining Vj and Vi
	}
} //End of isOn





//if point V.at(k) is tangent point with respect to point P on the other upper hull
inline bool isTangentPoint(vector<Point> V, int k, int indexRange_Size, Point P)
{
	if  ( indexRange_Size > 2)
	{
		if (isBelow(V.at(k-1), V.at(k), P) && isBelow(V.at(k+1), V.at(k), P))
			return true;
		else
			return false;
	}
	else if ( indexRange_Size == 2)
	{
		if (isBelow(V.at(k+1), V.at(k), P))
			return true;
		else 
			return false;
	}
	else if ( indexRange_Size == 1)
		return true;
}

//if point V.at(k) lies in left half of one upper hull with respect to point P on the other upper hull
inline bool isLPoint(vector<Point> V, int k, int indexRange_Size, Point P)
{
	if  ( indexRange_Size > 2)
	{
		if (isBelow(V.at(k-1), V.at(k), P) && isAbove(V.at(k+1), V.at(k), P))
			return true;
		else
			return false;
	}
	else if ( indexRange_Size == 2)
	{
		if (isAbove(V.at(k+1), V.at(k), P))
			return true;
		else 
			return false;
	}
	else if ( indexRange_Size == 1)
	{
		return false;
	}
}

//if point V.at(k) lies in right half of one upper hull with respect to point P on the other upper hull
inline bool isRPoint(vector<Point> V, int k, int indexRange_Size, Point P)
{
	if  ( indexRange_Size > 2)
	{
		if (isAbove(V.at(k-1), V.at(k), P) && isBelow(V.at(k+1), V.at(k), P))
			return true;
		else
			return false;
	}
	else if ( indexRange_Size == 2)
	{
		if (isBelow(V.at(k+1), V.at(k), P))
			return true;
		else 
			return false;
	}
	else if ( indexRange_Size == 1)
	{
		return false;
	}
}









//COMPUTE THE COMMON TANGENT GIVEN TWO UPPER HULLS
void tangentBetweenTwoUpperHulls( int n, vector<Point> &leftHull, int m, vector<Point> &rightHull, vector<int> t1, vector<int> t2 )
{
    int ix1=0, ix2=0;      // search indices for upper hulls 1 and 2
	int leftHull_Lowest=0, leftHull_Highest=n-1, i=0;   //leftHull_Lowest and leftHull_Highest contain indices of leftmost and rightmost points of left upper hull
	int rightHull_Lowest=0, rightHull_Highest=m-1, j=0;   //rightHull_Lowest and rightHull_Highest contain indices of leftmost and rightmost points of right upper hull
	bool leftHull_flag = false, rightHull_flag = false;  //two flags for the two upper hulls
	int leftIndexRange=0,rightIndexRange=0;
	leftIndexRange=leftHull.size();
	rightIndexRange=rightHull.size();

	while((leftHull_flag == false) && (rightHull_flag == false))   //continue binary search until we find the tangent points for both the upper hulls
	{


	while(rightHull_flag == false)  //continue binary search until we find the tangent point for the right upper hull with respect to the tangent point obtained in the left upper hull
	{
	j = (rightHull_Lowest + rightHull_Highest)/2;   //compute mid-point of right upper hull i.e. j
	if(isTangentPoint(rightHull,j,rightIndexRange,leftHull.at(i)))   //check if j is Tangent Point
	{
		rightHull_flag = true;
	}
	else if(isLPoint(rightHull,j,rightIndexRange,leftHull.at(i)))   //check if j is an LPoint
	{
		rightHull_Lowest = j;
		rightIndexRange = rightHull_Highest - rightHull_Lowest + 1;    //reduce index range
	}
	else if(isRPoint(rightHull,j,rightIndexRange,leftHull.at(i)))    //check if j is an RPoint
	{
		rightHull_Highest = j;
		rightIndexRange = rightHull_Highest - rightHull_Lowest + 1;    //reduce index range
	}
	}



	while(leftHull_flag == false)    //continue binary search until we find the tangent point for the left upper hull with respect to the tangent point obtained in the right upper hull
	{
	i = (leftHull_Lowest + leftHull_Highest)/2;   //compute mid-point of right upper hull i.e. i
	if(isTangentPoint(leftHull,i,leftIndexRange,rightHull.at(j)))   //check if i is Tangent Point
	{
		leftHull_flag = true;
	}
	else if(isLPoint(leftHull,i,leftIndexRange,rightHull.at(j)))    //check if i is an LPoint
	{
		leftHull_Lowest = i;
		leftIndexRange = leftHull_Highest - leftHull_Lowest + 1;    //reduce index range
	}
	else if(isRPoint(leftHull,j,leftIndexRange,rightHull.at(j)))   //check if i is an RPoint
	{
		leftHull_Highest = i;
		leftIndexRange = leftHull_Highest - leftHull_Lowest + 1;    //reduce index range
	}
	}


	//check if line joining i and j is a common tangent line between the two upper hulls
	if(isTangentPoint(leftHull,i,leftIndexRange,rightHull.at(j)) && isTangentPoint(rightHull,j,rightIndexRange,leftHull.at(i)))
	   {
		   ix1=i;
		   ix2=j;
	   }
	else
	   {
		leftHull_flag == false;
		rightHull_flag == false;
	   }
	}
    
	
	t1.push_back(ix1);
    t2.push_back(ix2);
    return;
} // End of tangentBetweenTwoUpperHulls
