#ifndef SNAKENODE_H
#define SNAKENODE_H
#include <utility>

class SnakeNode {
	 std::pair<int, int> pos;	// position: <x, y>
	 std::pair<int, int> dir;	// direction: <dirX, dirY>

	 public:
	 	void setPosition(int x, int y);

		void setDirection(int dirX, int dirY);

		int getPosX();

		int getPosY();

		int getDirX();

		int getDirY();
 };
#endif