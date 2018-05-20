#include "food.h"
#include <stdlib.h>

using namespace std;

Food::Food(): Displayable((rand() % 72 + 4) * 10, (rand() % 52 + 4) * 10), avaliable{false} {}

void Food::regenerate() {
	x = (rand() % 72 + 4) * 10;
    y = (rand() % 52 + 4) * 10;
	avaliable = true;
}

bool Food::isAvaliable() {
    return avaliable;
}

void Food::setStatus(bool st) {
    avaliable = st;
}

void Food::isEaten() {
    x = -1;
	y = -1;
	avaliable = false;
}