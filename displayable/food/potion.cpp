#include "potion.h"
#include "../../common.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
using namespace std;

Potion::Potion(): Food() {}

void Potion::paint(XInfo &xinfo) {
	if (isAvaliable()) {
		XDrawRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10);
	}
}