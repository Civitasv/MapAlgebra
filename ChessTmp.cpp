#include "ChessTmp.h"

CChessTmp::CChessTmp(int size) {
	this->size = size;
	m_DisTmp = new float[size * size];
	for (unsigned i = 0; i < size * size; i++)
	{
		float x = abs(getOffX(i));
		float y = abs(getOffY(i));
		m_DisTmp[i] = x > y ? x : y;
	}
}

unsigned int CChessTmp::Size() {
	return this->size * this->size;
}

int CChessTmp::getOffX(unsigned int i) {
	return i % this->size - this->size / 2;
}

int CChessTmp::getOffY(unsigned int i) {
	return i / this->size - this->size / 2;
}

float CChessTmp::getTmpDis(unsigned int i) {
	return m_DisTmp[i];
}