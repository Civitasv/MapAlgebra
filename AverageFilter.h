#pragma once
#include <math.h>
/**
 * @brief ��ֵ�˲���
 */
class AverageFilter
{
public:
private:
    int size;
    float* m_filter;
public:
    AverageFilter(int size);
    unsigned int Size();
    char getOffX(unsigned int i);
    char getOffY(unsigned int i);
    float getTmpDis(unsigned int i);
};

