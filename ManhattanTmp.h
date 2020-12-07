#pragma once
#include "DistanceTmp.h"
/**
 * @brief Âü¹ş¶Ù¾àÀëÄ£°å
 */
class CManhattanTmp :
	public CDistanceTmp
{
private:
	int m_OffX[13] = { 0,  -1, 0, 1,  -2,-1, 0, 1, 2,  -1,0,1,0 };
	int m_OffY[13] = { -2, -1,-1,-1,   0, 0, 0, 0, 0,   1,1,1,2 };
	unsigned int m_DisTmp[13] = { 2,   2, 1, 2,   2, 1, 0, 1, 2,   2,1,2,2 };
public:
	CManhattanTmp();
	unsigned int Size();
	int getOffX(unsigned int i);
	int getOffY(unsigned int i);
	float getTmpDis(unsigned int i);
};

