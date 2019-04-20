# Snake
Snake is a classic 2D video game where the main objective is to control a line (snake) as it grows 
in length while preventing it from hitting the bounds or itself.

The rules of this game are:

    - A snake can eat a fruit to gain 1 point and increase its length
    - A snake can eat a frog to gain 4 points and increase its length
    - A snake can gain extra lives by eating potions. It can have up to three lives
    - If a snake already has three lives, eating potion will increase its scores
    - A snake will lose one life when it hits the bounds or itself
    - Game ends if a snake runs out of its lives

Extra rules for multi-player mode:

    - The player who first gains 20 points will win the game
    - A snake can eat another snake's body to gain points
    - The snake that is eaten by another snake will lose one life
    - Game ends if one of the players runs out of lives


CONTROLS

Use the following commands to play the game:

    - Use WASD to control the snake's movements
    - Press 'p' to pause, 'r' to restart and 'q' to quit the game
    - Press 'i' to see instructions
    - Press '1' to play in single-player mode while on the splash green
    - Press '2' to play in multi-player mode while on the splash green
    - For multi-player mode, player on the left uses WASD to control the snake with red skin,
                             player on the right uses arrow keys to control the snake with yellow skin



This game includes the following new features:

    Two power-ups:
        - Potion: Eating a potion can give a snake an extra life.
              Every 5 seconds, there is 20% chance that a potion will appear
              If a snake already has three lives and eats a potion again, it will gain 2 points
              A potion is denoted by a white rectangle
        - Frog(denoted by a green circle): Eating a frog can give a snake 4 points
            A frog will jump over one block in a random direction every 2 seconds
            A frog can eat fruits and potions
            Every 10 seconds, there is 40% chance that a frog will spawn in a random location
    
    Multi-player Support:
        - This game supports multi-player mode. To play in multi-player, press '2' while on the splash green
        - Player on the left uses WASD to control a snake with red borders
        - Player on the right uses arrow keys to control a snake with yellow borders
        - The first player who achieves 20 points will win the game
        - Game ends whenever one of the two players runs out of lives
        - A snake can eat another snake's body to gain 5 points
        - The snake who is eaten by another will lose one life
