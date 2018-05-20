#ifndef SNAKE3_H
#define SNAKE3_H
#include "../displayable.h"
#include "snakenode.h"
#include <vector>

struct XInfo;

class Snake : public Displayable {
    int speed;
    int headDirX;
	int headDirY;
	int score;
	int lives;
	std::vector<SnakeNode> body;

	public:
        Snake(int x, int y, int headDirX, int headDirY);
        void paint(XInfo &xinfo) override;
        void move(XInfo &xinfo);
        int getHeadX();
        int getHeadY();
        int getHeadDirX();
        int getHeadDirY();
        int getSpeed();
        int getLives();
        int getScore();
        void didEatFruit();
        void didEatPotion();
        void didEatFrog();
        bool eatItself();
        bool eatByOtherSnake(Snake& another);
        bool canMoveTo(int dirX, int dirY);
        void incScore(int inc);
        void turnLeft();
        void turnRight();
        void goUp();
        void goDown();
        void setSpeed(int sp);
        void restart(int dirX, int dirY);
};
#endif

