#include "fruit.h"
#include "../../common.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

Fruit::Fruit(): Food() {}

void Fruit::paint(XInfo &xinfo) {
	XSetForeground(xinfo.display, xinfo.gc[0], red.pixel);
	XFillArc(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10, 0, 360 * 64);
	XSetForeground(xinfo.display, xinfo.gc[0], BlackPixel(xinfo.display, xinfo.screen));
}
