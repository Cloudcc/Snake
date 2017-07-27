#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

bool gameOver;
const int width = 20;   //screenwidth
const int height = 4;   //screenheight
int score;      //score
int x, y, foodY, foodX; //Head Position (x,y) Food Position (foodX, foodY)
int tailX[100];
int tailY[100];
int nTail;      //schwanz
int speed;      //difficulty
enum eDirection {
    STOP = 0, LEFT, RIGHT, UP, DOWN
} dir;

int khBit(void);

void Setup() {

    gameOver = false;
    nTail = 0;
    dir = STOP;
    x = width / 2; //player Start x
    y = height / 2; //player start y
    foodX = rand() % width; //0 to width -1
    foodY = rand() % height;
    score = 0;

}

void Draw() {
    /* for(int i = 0; i < width; i++){
         printf("#");
     }
     printf("\n");
     */

    system("clear"); //Linux clear


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            //Print Walls
            /* if (j == 0) {
                 printf("x");
             }
             if (j == width -1) {
                 printf("x");
             }*/
            if (i == y && j == x) {
                printf("O");
            } else if (i == foodY && j == foodX) {
                printf("F");
            } else {
                bool print = false;

                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        print = true;
                    }
                }
                if (!print) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    /*   for (int i = 0; i < height; i++) { // Kann  man wohl wegmachen
           printf("X");
       }*/
    printf("%i\n", score); // 7Segment

}

//logic of snake
void Logic() {

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    //Movement Tail
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    //movement head
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    //collision at walls
    /*   if(x > width ||  x < 0 || y > height || y < 0)
           gameOver = true;*/

    //no collision at walls
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    //collision with tail
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
        }
    }

    //score +10, spawn new food, increment tail
    if (x == foodX && y == foodY && !(nTail == 'o')) {
        score += 10;
        foodX = rand() % width;
        foodY = rand() % height;
        nTail++;
    }

    //difficulty
    if (score >= 0) {
        speed = 200000;
    }
    if (score > 40) {
        speed = 150000;
    }
    if (score > 80) {
        speed = 150000;
    }
    if (score > 120) {
        speed = 100000;
    }
    if (score > 160) {
        speed = 50000;
    }

    //gameover screen and restart
    if (gameOver == true) {
        system("clear");
        printf("--------------------\n");
        printf("-------~Game~-------\n");
        printf("-------~Over~-------\n");
        printf("--Restart-ANY-KEY---\n");
        getchar();
        Setup();
    }
}


void Input() { //Buttons a:left,d:right,s:down,w:up,x:stop game

    if (khBit()) {
        switch (getchar()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }

}

int main() {

    Setup();
    while (!gameOver) {
        Input();        //input
        Logic();        //logic
        Draw();
        usleep(speed); //difficulty
    }

    return 0;
}


//linux khBit no "conio.h" on linux :(

int khBit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}