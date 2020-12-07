#pragma once
#include "DistanceTmp.h"
#include <math.h>
/**
 * @brief 欧式距离模板，可动态选择模板大小
 */
class CEuTmp :
	public CDistanceTmp
{
private:
    int size;
    float *m_DisTmp;
public:
    CEuTmp(int size);
    unsigned int Size();
    int getOffX(unsigned int i);
    int getOffY(unsigned int i);
    float getTmpDis(unsigned int i);
};

