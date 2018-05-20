#include "snakenode.h"

using namespace std;

void SnakeNode::setPosition(int x, int y) {
    pos.first = x;
	pos.second = y;
}

void SnakeNode::setDirection(int dirX, int dirY) {
	dir.first = dirX;
	dir.second = dirY;
}

int SnakeNode::getPosX() {
	return pos.first;
}

int SnakeNode::getPosY() {
	return pos.second;
}

int SnakeNode::getDirX() {
	return dir.first;
}

int SnakeNode::getDirY() {
	return dir.second;
}