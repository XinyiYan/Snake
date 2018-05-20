#ifndef COMMON_H
#define COMMON_H
#include <vector>
#include <utility>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/*
 * Information to draw on the window.
 */
extern const int width;
extern const int height;
extern XColor red;
extern XColor yellow;
extern XColor green;
extern bool gameOver;
extern bool multiPlayer;
extern std::vector<std::pair<int, int> > possibleDir;

struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;
};
#endif

