#ifndef SNAKE_H
#define SNAKE_H

#define WALLS (game->walls)
#define SNAKE (game->snake)
#define FOOD (game->food)
#define NUM_WALLS 56

typedef struct SnakeNode {
    struct SnakeNode * next;
    int row; 
    int col; 
} SnakeNode;

struct Snake {
    SnakeNode * back; // back is the head of the list 
    SnakeNode * front; // front is the tail of the list 
    int length; 
    int x_dir;
    int y_dir; 
};

struct Food {
    int row;
    int col; 
};

struct Wall {
    int row; 
    int col; 
};

struct Game {
    struct Snake snake; 
    struct Food food; 
    struct Wall walls[56];
    int end; 
};

void initGame(struct Game * game); //initialize game elements
void moveSnake(struct Snake * snake); //moves back node to front node 
void collision(struct Game * game); 
void increaseLength(struct Snake * snake);

void render(struct Game * game);
void exitGame(struct Game * game); //free snake nodes

void keyInput(struct Game * game);


#endif