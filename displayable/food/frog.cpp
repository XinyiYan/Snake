#include "frog.h"
#include <stdlib.h>
#include "../../common.h"

using namespace std;

Frog::Frog(): Food() {}

void Frog::paint(XInfo &xinfo) {
	if (isAvaliable()) {
		XSetForeground(xinfo.display, xinfo.gc[0], green.pixel);
		XFillArc(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10, 0, 360 * 64);
		XSetForeground(xinfo.display, xinfo.gc[0], BlackPixel(xinfo.display, xinfo.screen));
	}
}

void Frog::jump() {
	int loop = 0;
	while (true && loop < 100) {
		++loop;
		int randomDir = rand() % 4;
		int possibleX = x + possibleDir[randomDir].first * 2 * blockSize;
		int possibleY = y + possibleDir[randomDir].second * 2 * blockSize;
		if (possibleX >= 40 && possibleX + blockSize <= width - 40
				&& possibleY >= 40 && possibleY + blockSize <= height - 40) {
				x = possibleX;
				y = possibleY;
				break;
		}
	}
}