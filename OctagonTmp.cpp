#include "OctagonTmp.h"
OctagonTmp::OctagonTmp() {
	// ×ó¸ºÓÒÕı
}

unsigned int OctagonTmp::Size() {
	return 38;
}

int OctagonTmp::getOffX(unsigned int i) {
	return m_OffX[i];
}


int OctagonTmp::getOffY(unsigned int i) {
	return m_OffY[i];
}

float OctagonTmp::getTmpDis(unsigned int i) {
	return m_DisTmp[i];
}