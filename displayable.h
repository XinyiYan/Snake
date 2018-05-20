#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H
struct XInfo;

class Displayable {

    protected:
        int x;
        int y;
        int blockSize;

	public:
        Displayable(int x, int y);
		virtual void paint(XInfo &xinfo) = 0;
        int getPosX();
        int getPosY();
};
#endif