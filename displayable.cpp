#include "displayable.h"


Displayable::Displayable(int x, int y): x{x}, y{y}, blockSize{10} {}

int Displayable::getPosX() {
    return x;
}

int Displayable::getPosY() {
    return y;
}