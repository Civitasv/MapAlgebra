#include "ManhattanTmp.h"
CManhattanTmp::CManhattanTmp() {
	
}

unsigned int CManhattanTmp::Size() {
	return 13;
}

int CManhattanTmp::getOffX(unsigned int i) {
	return m_OffX[i];
}


int CManhattanTmp::getOffY(unsigned int i) {
	return m_OffY[i];
}

float CManhattanTmp::getTmpDis(unsigned int i) {
	return m_DisTmp[i];
}