#pragma once
#include "DistanceTmp.h"
#include <math.h>
/**
 * @brief ���̾���ģ�壬�ɶ�̬ѡ��ģ���С
 */
class CChessTmp :
    public CDistanceTmp
{
private:
    int size;
    float* m_DisTmp;
public:
    CChessTmp(int size);
    unsigned int Size();
    int getOffX(unsigned int i);
    int getOffY(unsigned int i);
    float getTmpDis(unsigned int i);
};

