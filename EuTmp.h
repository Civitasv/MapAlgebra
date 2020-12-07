#pragma once
#include "DistanceTmp.h"
#include <math.h>
/**
 * @brief ŷʽ����ģ�壬�ɶ�̬ѡ��ģ���С
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

