#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <string>
#include <cstring>
#include "common.h"
#include "displayable.h"
#include "displayable/snake3.h"
#include "displayable/food/frog.h"
#include "displayable/food/fruit.h"
#include "displayable/food/potion.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;


/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 40;
int FPS = 100;
const int width = 800;
const int height = 600;
bool isPaused = false;
bool gameOver = false;
bool onStartPage = true;
bool seeInstruction = false;
bool multiPlayer = false;
bool bothLose = false;
vector<pair<int, int> > possibleDir; 
XColor red;
XColor yellow;
XColor green;
list<Displayable *> dList;           // list of Displayables
Snake snake(100, 450, 1, 0);
Snake snakeTwo(700, 450, -1, 0);
Fruit fruit;
Potion potion;
Frog frog;
XFontStruct* font;
unsigned long lastMove = 0;
unsigned long lastJump = 0;
unsigned long canCreatePotion = 0;
unsigned long canCreateFrog = 0;

/*
 * Information to draw on the window.
 
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;
};
*/

/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {
	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xInfo.display = XOpenDisplay( "" );
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.min_width = 800;
	hints.max_width = 800;
	hints.min_height = 600;
	hints.max_height = 600; 
	hints.flags = PPosition | PSize | PMinSize | PMaxSize;

	XColor palegreen;
	Colormap screen_colormap = DefaultColormap(xInfo.display, DefaultScreen(xInfo.display));;
	XAllocNamedColor(xInfo.display, screen_colormap, "palegreen", &palegreen, &palegreen);
	XAllocNamedColor(xInfo.display, screen_colormap, "red", &red, &red);
	XAllocNamedColor(xInfo.display, screen_colormap, "yellow", &yellow, &yellow);
	XAllocNamedColor(xInfo.display, screen_colormap, "forestgreen", &green, &green);

	xInfo.window = XCreateSimpleWindow( 
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		palegreen.pixel );					// window background colour
		
	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"animation",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window


	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], palegreen.pixel); //WhitePixel(xInfo.display, xInfo.screen)
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);
	
  	// load a larger font

  	font = XLoadQueryFont(xInfo.display, "10x20");
  	XSetFont(xInfo.display, xInfo.gc[i], font->fid);


	XSelectInput(xInfo.display, xInfo.window, 
		ButtonPressMask | KeyPressMask | 
		PointerMotionMask | 
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}

void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	XClearWindow( xinfo.display, xinfo.window );
 
	if (onStartPage) {
		int str_height = font->ascent + font->descent;
		if (!seeInstruction) {
			int str1_width = XTextWidth(font, "Welcome to Snake Game", strlen("Welcome to Snake Game"));
			int str2_width = XTextWidth(font, "Designed and implemented by Nicole", strlen("Designed and implemented by Nicole"));
			int str3_width = XTextWidth(font, "userid: x46yan", strlen("userid: x46yan"));
			int str4_width = XTextWidth(font, "Press 'i' to see instructions", strlen("Press 'i' to see instructions"));
			int posx = (width - str1_width) / 2;
			int posy = (height - 4 * str_height) / 2;
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posx, posy, "Welcome to Snake Game", strlen("Welcome to Snake Game"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], (width - str2_width) / 2, (height - 2 * str_height) / 2 , "Designed and implemented by Nicole", strlen("Designed and implemented by Nicole"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], (width - str3_width) / 2, (height) / 2, "userid: x46yan", strlen("userid: x46yan"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], (width - str4_width) / 2, (height + 2 * str_height) / 2, "Press 'i' to see instructions", strlen("Press 'i' to see instructions"));
		} else {
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40, "Instructions:", strlen("Instructions:"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + str_height, "1. Use WASD to control the snake's movements", strlen("1. Use WASD to control the snake's movements"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 2 * str_height, "2. Eat an apple(red circle): gain 1 point, snake grows in length", strlen("2. Eat an apple(red circle): gain 1 point, snake grows in length"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 3 * str_height, "3. Eat a potion(white rec): gain an extra life", strlen("3. Eat a potion(white rec): gain an extra life"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 4 * str_height, "4. Eat a frog(green circle): gain 4 points", strlen("4. Eat a frog(green circle): gain 4 points"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 5 * str_height, "5. Snake can have a total of 3 lives, then eating a potion will gain 2 points", strlen("5. Snake can have a total of 3 lives, then eating a potion will gain 2 points"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 6 * str_height, "6. You lose the game when you run out of lives", strlen("6. You lose the game when you run out of lives"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 7 * str_height, "7. Press 'p' to pause and 'r' to restart the game", strlen("7. Press 'p' to pause and 'r' to restart the game"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 9 * str_height, "Multi-Player Mode: ", strlen("Multi-Player Mode:"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 10 * str_height, "1. Player on the left: Use WASD to control the snake", strlen("1. Player on the left: Use WASD to control the snake"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 11 * str_height, "2. Player on the right: Use arrow keys to control the snake", strlen("2. Player on the right: Use arrow keys to control the snake"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 12 * str_height, "3. Player who first gain 20 points will win the game", strlen("3. Player who first gain 20 points will win the game"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 13 * str_height, "4. One snake can eat another snake's body to gain 5 points", strlen("4. One snake can eat another snake's body to gain 5 points"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 14 * str_height, "5. Snake whose body is eaten will lose one life", strlen("5. Snake whose body is eaten will lose one life"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 25, 40 + 15 * str_height, "6. If two snakes meet each other face-to-face, try and see what happens!", strlen("6. If two snakes meet each other face-to-face, try and see what happens!"));
			int str_width = XTextWidth(font, "To start this game, press 1(2) to play in single(multi-player) mode", strlen("To start this game, press '1'(or '2') to play in single(or multi-player) mode"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], (width - str_width) / 2, 40 + 18 * str_height, "To start this game, press '1'(or '2') to play in single(or multi-player) mode", strlen("To start this game, press '1'(or '2') to play in single(or multi-player) mode"));
		}
	} else {
    	XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], 40, 40, width - 40, 40);
		XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], 40, height - 40, width - 40, height - 40);
		XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], 40, 40, 40, height - 40);
		XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], width - 40, 40, width - 40, height - 40);

		// draw lives 
		if (multiPlayer) {
			for (int i = 0; i < snake.getLives(); ++i) {
				int posX = 40 + i * 15;
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posX, 35, "S", strlen("S"));
			}

			for (int i = 0; i < snakeTwo.getLives(); ++i) {
				int posX = (width - 50) - i * 15;
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posX, 35, "S", strlen("S"));
			}
		} else {
			for (int i = 0; i < snake.getLives(); ++i) {
				int posX = (width - 60) - i * 15;
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posX, 35, "S", strlen("S"));
			}
		}

		// draw Score
		string sc = "Score: " + to_string(snake.getScore());
		XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 5, 590, sc.c_str(), sc.length());
		if (multiPlayer) {
			sc = "Score: " + to_string(snakeTwo.getScore());
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], 680, 590, sc.c_str(), sc.length());
		}

		// draw display list
		while( begin != end ) {
			Displayable *d = *begin;
			d->paint(xinfo);
			begin++;
		}

		if (gameOver && !multiPlayer) {
			string lose = "Game Over!";
			string options = "Press r to restart or q to quit";
			int lose_width = XTextWidth(font, lose.c_str(), lose.length());
			int lose_height = font->ascent + font->descent;
			int options_width = XTextWidth(font, options.c_str(), options.length());
			int posx = (width - lose_width) / 2;
			int posy = (height - 2 * lose_height) / 2;
			int posx2 = (width - options_width) / 2;
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posx, posy, lose.c_str(), lose.length());
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posx2, posy + lose_height, options.c_str(), options.length());
		} else if (gameOver && !bothLose) {
			int playerOneLives = snake.getLives();
			int playerTwoLives = snakeTwo.getLives();
			int playerOneScore = snake.getScore();
			int playerTwoScore = snakeTwo.getScore();
				
			string winMsg = "You Win!";
			int str_width = XTextWidth(font, winMsg.c_str(), winMsg.length());
			int posx;
			int posy = (height - font->ascent - font->descent) / 2;
			if (playerOneLives <= 0 && playerTwoLives > 0) {
				posx = 3 * width / 4 - str_width / 2;
			} else if (playerOneLives > 0 && playerTwoLives <= 0) {
				posx = width / 4 - str_width / 2;
			} else if (playerOneScore < playerTwoScore){
				posx = 3 * width / 4 - str_width / 2;
			} else if (playerOneScore > playerTwoScore) {
				posx = width / 4 - str_width / 2;
			} else {
				winMsg = "Tie";
				str_width = XTextWidth(font, winMsg.c_str(), winMsg.length());
				posx = (width - str_width) / 2;
			}
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posx, posy, winMsg.c_str(), winMsg.length());
		} else if (gameOver && bothLose) {
			string s2 = "Both Died!";
			string s3 = "You got snake meat!";
			int str1_width = XTextWidth(font, "Two snakes fought with each other and...", strlen("Two snakes fought with each other and..."));
			int str2_width = XTextWidth(font, "Both Died!", strlen("Both Died!"));
			int str3_width = XTextWidth(font, "You got snake meat!", strlen("You got snake meat!"));
			int str_height = font->ascent + font->descent;
			int posx = (width - str1_width) / 2;
			int posy = (height - 3 * str_height) / 2;
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], posx, posy, "Two snakes fought with each other and...", strlen("Two snakes fought with each other and..."));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], (width - str2_width) / 2, (height - str_height) / 2 , "Both Died!", strlen("Both Died!"));
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0], (width - str3_width) / 2, (height + str_height) / 2, "You got snake meat!", strlen("You got snake meat!"));
		}
	}
	XFlush( xinfo.display );
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		}

		if (text[0] == 'p') {
			// pause the game
			if (!onStartPage) {
				if (isPaused) {
					isPaused = false;
				} else {
					isPaused = true;
				}
			}
		} else if (text[0] == 'r') {
			// restart the level
			if (!onStartPage) {
				snake.restart(1, 0);
				fruit.regenerate();
				potion.isEaten();
				frog.isEaten();
				isPaused = false;
				gameOver = false;
				bothLose = false;
				lastMove = now();
				lastJump = lastMove;
				canCreatePotion = lastMove;
				canCreateFrog = lastMove;
				if (multiPlayer) {
					snakeTwo.restart(-1, 0);
				}
			}
		} else if (text[0] == '1') {
			// single mode
			onStartPage = false;
		} else if (text[0] == '2') {
			// multi-player mode
			if (onStartPage) {
				multiPlayer = true;
				dList.push_back(&snakeTwo);
			}
			onStartPage = false;
		} else if (text[0] == 'w') {
			if (!onStartPage && !isPaused && !gameOver) {
				if (snake.getHeadDirX() != 0 ) {
					cout << "Up" << endl;
					snake.goUp();
				}
			}
		} else if (text[0] == 's') {
			if (!onStartPage && !isPaused && !gameOver) {
				if (snake.getHeadDirX() != 0) {
					cout << "Down" << endl;
					snake.goDown();
				}
			}
		} else if (text[0] == 'a') {
			if (!onStartPage && !isPaused && !gameOver) {
				if (snake.getHeadDirY() != 0) {
					cout << "Left" << endl;
					snake.turnLeft();
				}
			}
		} else if (text[0] == 'd') {
			if (!onStartPage && !isPaused && !gameOver) {
				if (snake.getHeadDirY() != 0) {
					cout << "Right" << endl;
					snake.turnRight();
				}
			}
		} else if (text[0] == 'i') {
			if (onStartPage && !seeInstruction) {
				seeInstruction = true;
			}
		}
	}

	if (!isPaused && !gameOver && !onStartPage && multiPlayer) {
		switch(key){
        	case XK_Up:
				if (snakeTwo.getHeadDirX() != 0 ) {
					cout << "Up" << endl;
					snakeTwo.goUp();
				}
				break;
            
        	case XK_Down:
            	if (snakeTwo.getHeadDirX() != 0) {
					cout << "Down" << endl;
					snakeTwo.goDown();
				}
				break;
        	case XK_Left:
				if (snakeTwo.getHeadDirY() != 0) {
					cout << "Left" << endl;
					snakeTwo.turnLeft();
				}
				break;
        	case XK_Right:
				if (snakeTwo.getHeadDirY() != 0) {
					cout << "Right" << endl;
					snakeTwo.turnRight();
				}
				break;
		}
	}
}

void handleAnimation(XInfo &xinfo, int inside) {

	snake.move(xinfo);

	int snakeHeadX = snake.getHeadX();
	int snakeHeadY = snake.getHeadY();
	if (snakeHeadX == fruit.getPosX() && snakeHeadY == fruit.getPosY()) {
		snake.didEatFruit();
		fruit.regenerate();
	}

	if (potion.isAvaliable() && snakeHeadX == potion.getPosX() && snakeHeadY == potion.getPosY()) {
		snake.didEatPotion();
		potion.isEaten();
		canCreatePotion = now();
	}

	if (frog.isAvaliable() && snakeHeadX == frog.getPosX() && snakeHeadY == frog.getPosY()) {
		snake.didEatFrog();
		frog.isEaten();
		canCreateFrog = now();
	}

	if (frog.isAvaliable() && frog.getPosX() == fruit.getPosX() && frog.getPosY() == fruit.getPosY()) {
		fruit.regenerate();
	}

	if (frog.isAvaliable() && potion.isAvaliable() && frog.getPosX() == potion.getPosX() && frog.getPosY() == potion.getPosY()) {
		potion.isEaten();
		canCreatePotion = now();
	}

	if (multiPlayer) {
		snakeTwo.move(xinfo);
		int snakeTwoHeadX = snakeTwo.getHeadX();
		int snakeTwoHeadY = snakeTwo.getHeadY();
		if (snakeTwoHeadX == fruit.getPosX() && snakeTwoHeadY == fruit.getPosY()) {
			snakeTwo.didEatFruit();
			fruit.regenerate();
		}

		if (potion.isAvaliable() && snakeTwoHeadX == potion.getPosX() && snakeTwoHeadY == potion.getPosY()) {
			snakeTwo.didEatPotion();
			potion.isEaten();
			canCreatePotion = now();
		}
		if (frog.isAvaliable() && snakeTwoHeadX == frog.getPosX() && snakeTwoHeadY == frog.getPosY()) {
			snakeTwo.didEatFrog();
			frog.isEaten();
			canCreateFrog = now();
		}

		if (snake.eatByOtherSnake(snakeTwo)) {
			snakeTwo.incScore(5);
		} else if (snakeTwo.eatByOtherSnake(snake)) {
			snake.incScore(5);
		} else if (snakeHeadX == snakeTwoHeadX && snakeHeadY == snakeTwoHeadY) { 	 // two snakes bump their heads
			gameOver = true;
			bothLose = true;
		}
	}
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	dList.push_front(&frog);
	dList.push_front(&snake);
	
    dList.push_front(&fruit);
	dList.push_front(&potion);

	possibleDir.push_back(make_pair(1,0));
	possibleDir.push_back(make_pair(0,1));
	possibleDir.push_back(make_pair(-1,0));
	possibleDir.push_back(make_pair(0,-1));
	
	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while(true) {

		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			//cout << "event.type=" << event.type << "\n";
			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
			}
		} 

		unsigned long end = now();

		if (onStartPage) {
			lastMove = now();
			lastJump = lastMove;
			canCreatePotion = lastMove;
			canCreateFrog = lastMove;
		} else {

			if (!frog.isAvaliable() || isPaused || gameOver) {
				lastJump = now();
			} else {
				if (end - lastJump > 2000000) {
					cout << "two sec passed?" << endl;
					frog.jump();
					lastJump = now();
				}
			}
			//cout << "repaint" << "end: " << end << endl;
			int distanceSinceLastMove = floor((end - lastMove) / (1000000 / snake.getSpeed()));
			//cout << "end: " << end << "lastmove: " << lastMove << "distancesince last move: " << distanceSinceLastMove << endl;
			if (distanceSinceLastMove != 0 && !isPaused && !gameOver) {
				//cout << "move " << distanceSinceLastMove << " blocks ahead" << endl;
				for (int i = 0; i < distanceSinceLastMove; ++i) {
				//	cout << "handle anim" << endl;
					handleAnimation(xinfo, inside);
					if (gameOver) {
						break;
					}
				}
				lastMove = now();
			} else if (isPaused || gameOver) {
				lastMove = now();
			}  
	
			if (potion.isAvaliable() || isPaused || gameOver) {
				canCreatePotion = now();
			} else if (end - canCreatePotion > 5000000) {		
			// every 5 sec, there is 20% chance that a potion will be generated
			//	cout << "five sec passed? " << endl;
				bool generate = rand() % 10 > 7 ? true : false;
				if (generate) {
					potion.regenerate();
				}
				canCreatePotion = now();
			}

			if (frog.isAvaliable() || isPaused || gameOver) {
				canCreateFrog = now();
			} else if (end - canCreateFrog > 10000000) {
				// every 10 sec, there is 50% chance that a frog will show up
				bool generate = rand() % 10 > 4 ? true : false;
				if (generate) {
					frog.regenerate();
				}
				canCreateFrog = now();
			}
		}
  
		unsigned long cur = now();
		if (cur - lastRepaint > 1000000 / FPS) {
			cout << "end - lastp" << cur - lastRepaint << endl;
			repaint(xinfo);
			lastRepaint = now();
		}

		if (XPending(xinfo.display) == 0) {
			//cout << "pending " << 1000000/FPS - (end - lastRepaint) << endl;
			usleep(1000000/FPS - (end - lastRepaint));
		}  

	/*	repaint(xinfo);
		lastRepaint = now();
		if (lastRepaint - startTime < 1000000 / FPS) {
			cout << "lastrp - start " << startTime - lastRepaint << " " << endl; 
			usleep(1000000/FPS - (lastRepaint - startTime));
		} else {
			cout << "error, take more time" << lastRepaint - startTime << " should take" << 1000000 / FPS << endl;
		} */


	}
}

int main ( int argc, char *argv[] ) {

	if (argc == 3) {
		FPS = stoi(argv[1]);
		int sp = stoi(argv[2]);
		snake.setSpeed(sp);
	}
	XInfo xInfo;

	initX(argc, argv, xInfo);
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
