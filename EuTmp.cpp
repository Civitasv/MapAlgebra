#include "EuTmp.h"

CEuTmp::CEuTmp(int size) {
	this->size = size;
	m_DisTmp = new float[size * size];
	for (unsigned i = 0; i < size * size; i++)
	{
		float x = getOffX(i);
		float y = getOffY(i);
		m_DisTmp[i] = sqrt(x * x + y * y);
	}
}

unsigned int CEuTmp::Size() {
	return this->size * this->size;
}

int CEuTmp::getOffX(unsigned int i) {
	return i % this->size - this->size / 2;
}

int CEuTmp::getOffY(unsigned int i) {
	return i / this->size - this->size / 2;
}

float CEuTmp::getTmpDis(unsigned int i) {
	return m_DisTmp[i];
}