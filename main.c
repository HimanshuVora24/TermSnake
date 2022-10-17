#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include "snake.h"

int main() {
    struct Game game;

    initGame(&game);

    
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    int i = 0;
    while (!(game.end)) {
        //mvprintw();
        erase();
        keyInput(&(game));
        if (!(game.end)) {
            moveSnake(&(game.snake));
            collision(&game);
        }
        
        render(&game);
        refresh();
        i++;
        sleep(1);
    }

    exitGame(&game);
    endwin();
    return 0; 
}


void initGame(struct Game * game) {
    /*  Initialize walls for a (20 cols)x(10 rows) simple board 
        Can later extend to be read from an external file for easier board generation.  
    */
    for (int i = 0; i < 20; i++) {
        WALLS[i].row = 0; 
        WALLS[i].col = i;
        WALLS[i + 20].row = 9; 
        WALLS[i + 20].col = i;
    }

    for (int i = 1; i < 9; i++) {
        WALLS[i + 39].row = i; 
        WALLS[i + 39].col = 0; 
        WALLS[i + 47].row = i;
        WALLS[i + 47].col = 19;
    }

    // Initializes a snake of length 2
    SNAKE.back = (SnakeNode *) malloc(sizeof(SnakeNode)); 
    SNAKE.front = (SnakeNode *) malloc(sizeof(SnakeNode)); 
    (SNAKE.back)->next = SNAKE.front; 
    (SNAKE.front)->next = NULL; 
    SNAKE.length = 2;  
    SNAKE.x_dir = 1; 
    SNAKE.y_dir = 0; 
    (SNAKE.front)->row = 5;
    (SNAKE.back)->row = 5;
    (SNAKE.front)->col = 10;
    (SNAKE.back)->col = 9;

     
    //Randomly places a food particle in the 'top-left quarter' of the board
    srand(time(NULL));
    FOOD.row = rand()%4 + 1; 
    FOOD.col = rand()%9 + 1;     

    game->end = 0; 
}

void moveSnake(struct Snake * snake) {
    snake->front->next = snake->back; //move the tail in front of head
    snake->back->row = snake->front->row; 
    snake->back->col = snake->front->col; 
    snake->front = snake->back; // make the former tail the new head
    snake->back = (snake->back)->next; //make the node in front of former tail the new tail
    snake->front->next = NULL; // make the former tail point to nothing (as it is the new head)

    //Now update the head position
    (snake->front)->col += snake->x_dir; 
    (snake->front)->row += snake->y_dir;
}

//check for collision of Snake.front with walls and food 
void collision(struct Game * game) {
    for (int i = 0; i < NUM_WALLS; i++) {
        if (((SNAKE.front)->row == WALLS[i].row) && ((SNAKE.front)->col == WALLS[i].col)) {
            game->end = 1;    
        }
    }
    /* SnakeNode * node = (SNAKE.back); 
    while (node != SNAKE.front) {
        if (SNAKE.front->row == node->row && SNAKE.front->col == node->col) {
            game->end = 1; 
            return; 
        }
    } */

    if (((SNAKE.front)->row == FOOD.row) && ((SNAKE.front)->col == FOOD.col)) {
        FOOD.row = rand()%4 + 1; 
        FOOD.col = rand()%9 + 1;  
        increaseLength(&(SNAKE));
    }
}

void increaseLength(struct Snake * snake) {
    SnakeNode * node = (SnakeNode *) malloc(sizeof(SnakeNode));

    node->next = snake->back; 
    node->row = snake->back->row;
    node->col = snake->back->col; 
    snake->back = node; 
    (snake->length)++;
}

void render(struct Game * game) {
    //render walls
    for (int i = 0; i < 56; i++) {
        mvprintw(WALLS[i].row, WALLS[i].col, "o");
    }
    
    //render snake
    SnakeNode * node = SNAKE.back;
    for (int i = 0; (i < SNAKE.length) && node; i++) {
        mvprintw(node->row, node->col, "*");
        node = node->next; 
    }

    //render food
    mvprintw(FOOD.row, FOOD.col, "+");
}

void exitGame(struct Game * game) {
    SnakeNode * node = SNAKE.back; 
    while (node) {
        SNAKE.back = (SNAKE.back)->next; 
        free(node); 
        node = SNAKE.back;
    }
    game->end = 1;
}

void keyInput(struct Game * game) {
    //noecho();
    struct Snake * snake = &(game->snake);
    char ch = getch();
    
    switch (ch) {
        case 'w':
        snake->x_dir = 0; 
        snake->y_dir = -1; 
        break; 

        case 'a':
        snake->x_dir = -1; 
        snake->y_dir = 0; 
        break; 

        case 's':
        snake->x_dir = 0; 
        snake->y_dir = 1; 
        break; 

        case 'd':
        snake->x_dir = 1; 
        snake->y_dir = 0;
        break;

        case 'q':
        game->end = 1;
        break; 

        default:
        break; 
    }

    flushinp();

}