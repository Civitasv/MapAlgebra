#pragma once
#include "DistanceTmp.h"
/**
 * @brief °Ë±ßÐÎ¾àÀëÄ£°å
 */
class OctagonTmp :
	public CDistanceTmp
{
private:
	int m_OffX[38] = { -1,  0, 1, -2, -1,0,1,2,-3,-2,-1,0,1,2,3,-3,-2,-1,0,1,2,3,-3,-2,-1,0,1,2,3,-2,-1,0,1,2,-1,0,1 };
	int m_OffY[38] = { -3,-3,-3,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3 };
	unsigned int m_DisTmp[38] = { 3,3,3,3,2,2,2,3,3,2,2,1,2,2,3,3,2,1,0,1,2,3,3,2,2,1,2,2,3,3,2,2,2,3,3,3,3 };
public:
	OctagonTmp();
	unsigned int Size();
	int getOffX(unsigned int i);
	int getOffY(unsigned int i);
	float getTmpDis(unsigned int i);
};

