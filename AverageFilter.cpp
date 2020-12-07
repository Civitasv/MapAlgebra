#include "AverageFilter.h"

AverageFilter::AverageFilter(int size) {
	this->size = size;
	m_filter = new float[size * size];
	float Size = size;
	for (unsigned i = 0; i < size * size; i++)
	{
		m_filter[i] = 1.0 / Size * Size;
	}
}

unsigned int AverageFilter::Size() {
	return this->size * this->size;
}

char AverageFilter::getOffX(unsigned int i) {
	return i % this->size - this->size / 2;
}

char AverageFilter::getOffY(unsigned int i) {
	return i / this->size - this->size / 2;
}

float AverageFilter::getTmpDis(unsigned int i) {
	return m_filter[i];
}
