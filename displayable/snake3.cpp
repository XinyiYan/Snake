#include "snake3.h"
#include "snakenode.h"
#include "../common.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

Snake::Snake(int x, int y, int headDirX, int headDirY): Displayable(x, y), speed{5}, headDirX{headDirX}, headDirY{headDirY}, score{0}, lives{1} {
    for (int i = 0; i < 6; ++i) {
		SnakeNode block;
		block.setPosition(x + i * blockSize * headDirX, y);
		block.setDirection(headDirX, headDirY);
		body.push_back(block);
	}	
}

void Snake::paint(XInfo &xinfo) {
	for (vector<SnakeNode>::iterator it = body.begin(); it != body.end(); ++it) {
		XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], it->getPosX(), it->getPosY(), blockSize, blockSize);		
		if (x < 400) {
			XSetForeground(xinfo.display, xinfo.gc[0], red.pixel);
		} else {
			XSetForeground(xinfo.display, xinfo.gc[0], yellow.pixel);
		}
				
		XDrawRectangle(xinfo.display, xinfo.window, xinfo.gc[0], it->getPosX(), it->getPosY(), blockSize, blockSize);
		XSetForeground(xinfo.display, xinfo.gc[0], BlackPixel(xinfo.display, xinfo.screen));
		}
}

void Snake::move(XInfo &xinfo) {

	int tailPreX = body.begin()->getPosX();
	int tailPreY = body.begin()->getPosY();
				
	for (vector<SnakeNode>::iterator it = body.begin(); it != body.end() - 1; ++it) {
		it->setDirection((it+1)->getDirX(), (it+1)->getDirY());
		it->setPosition((it+1)->getPosX(), (it+1)->getPosY());
	}


	// update snake head
	SnakeNode &head = body.back();
	head.setDirection(headDirX, headDirY);
	head.setPosition(body.back().getPosX() + headDirX * blockSize, body.back().getPosY() + headDirY * blockSize);					

	if (head.getPosX() < 40 || head.getPosX() + blockSize > width - 40
			|| head.getPosY() < 40 || head.getPosY()+ blockSize > height - 40) {
		--lives;
		if (lives == 0) {
			gameOver = true;
		} else {
			int sz = body.size();
			for (int i = sz - 1; i > 0; --i) {
				body[i].setDirection(body[i - 1].getDirX(), body[i - 1].getDirY());			
				body[i].setPosition(body[i - 1].getPosX(), body[i - 1].getPosY());
			}
			body[0].setPosition(tailPreX, tailPreY);

			bool canFindDirToMove = false;
			for (vector<pair<int, int> >::iterator it = possibleDir.begin(); it != possibleDir.end(); ++it) {
				if (canMoveTo(it->first, it->second)) {
					headDirX = it->first;
					headDirY = it->second;
					canFindDirToMove = true;
    				break;
				}
			}
			
			if (!canFindDirToMove) {
				int sz = body.size();
				if (x < 400) {
					headDirX = 1;
					headDirY = 0;
				} else {
					headDirX = -1;
					headDirY = 0;
				}
				for (int i = 0; i < sz; ++i) {
					body[i].setPosition(x + i * blockSize * headDirX, y);
					body[i].setDirection(headDirX, headDirY);
				}
			}
		}
	}

	if (eatItself()) {
		--lives;
		if (lives == 0) {
			gameOver = true;
		}	
	}
}

int Snake::getHeadX() {
	return body.back().getPosX();
}
		
int Snake::getHeadY() {
	return body.back().getPosY();
}

int Snake::getHeadDirX() {
	return body.back().getDirX();
}

int Snake::getHeadDirY() {
	return body.back().getDirY();
}

int Snake::getSpeed() {
	return speed;
}

int Snake::getLives() {
	return lives;
}

int Snake::getScore() {
	return score;
}

void Snake::didEatFruit() {
	SnakeNode block;
	block.setPosition(body.front().getPosX() - body.front().getDirX() * blockSize,
                        body.front().getPosY() - body.front().getDirY() * blockSize);
	block.setDirection(body.front().getDirX(), body.front().getDirY());
	body.insert(body.begin(), block);
	incScore(1);
}

void Snake::didEatPotion() {
	if (lives < 3) {
		++lives;
	} else {
		incScore(2);
	} 
}

void Snake::didEatFrog() {
	SnakeNode block;
	block.setPosition(body.front().getPosX() - body.front().getDirX() * blockSize,
							  body.front().getPosY() - body.front().getDirY() * blockSize);
	block.setDirection(body.front().getDirX(), body.front().getDirY());
	body.insert(body.begin(), block);
	incScore(4);
}

bool Snake::eatItself() {
	for (vector<SnakeNode>::iterator it = body.begin(); it != body.end() - 1; ++it) {
		if (it->getPosX() == body.back().getPosX() && it->getPosY() == body.back().getPosY()) {
			if (lives > 1) {
				body.erase(body.begin(), it);
			}
			return true;
		}
	}
	return false;
}

bool Snake::eatByOtherSnake(Snake& another) {
	int anotherHeadX = another.getHeadX();
	int anotherHeadY = another.getHeadY();
	for (vector<SnakeNode>::iterator it = body.begin(); it != body.end() - 1; ++it) {
		if (it->getPosX() == anotherHeadX && it->getPosY() == anotherHeadY) {
			body.erase(body.begin(), it);
			--lives;
			if (lives <= 0) {
				gameOver = true;
			}
			return true;
		}
	}
	return false;
}

bool Snake::canMoveTo(int dirX, int dirY) {
	int headPosX = body.back().getPosX();
	int headPosY = body.back().getPosY();

	int newPosX = headPosX + dirX * blockSize * 2;
	int newPosY = headPosY + dirY * blockSize * 2;

	if (newPosX < 40 || newPosX + blockSize > width - 40
				|| newPosY < 40 || newPosY + blockSize > height - 40) {
		return false;
	} else {
		for (vector<SnakeNode>::iterator it = body.begin(); it != body.end() - 1; ++it) {
			if ((it->getPosX() == newPosX && it->getPosY() == newPosY) ||
				(it->getPosX() == newPosX - dirX * blockSize && it->getPosY() == newPosY - dirY * blockSize)) {
				return false;
			}
		}
		return true;		
	}	
}

void Snake::incScore(int inc) {
	score += inc;
	if (score > 20 && multiPlayer) {
		gameOver = true;
	}
}

void Snake::turnLeft() {
	headDirX = -1;
	headDirY = 0;
}

void Snake::turnRight() {
	headDirX = 1;
	headDirY = 0;
}

void Snake::goUp() {
	headDirX = 0;
	headDirY = -1;
}

void Snake::goDown() {
	headDirX = 0;
	headDirY = 1;
}

void Snake::setSpeed(int sp) {
	speed = sp;
}
		
void Snake::restart(int dirX, int dirY) {
	headDirX = dirX;
	headDirY = dirY;
	score = 0;
	lives = 1;
	body.clear();
	for (int i = 0; i < 6; ++i) {
		SnakeNode block;
		block.setPosition(x + i * blockSize * headDirX, y);
		block.setDirection(headDirX, headDirY);
		body.push_back(block);
	}
}

