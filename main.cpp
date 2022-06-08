#include <iostream>
#include <time.h>
#include <conio.h>

using namespace std;

bool game_over = false;
const int width = 20;
const int height = 20;
const float FRAME_RATE = 5;

unsigned int player_x = 0, player_y = 0;
unsigned int fruit_x = 0, fruit_y = 0;
unsigned int score = 0;

class Point {
    public:
        unsigned int x;
        unsigned int y;
        Point() = default;
        Point(unsigned int _x, unsigned int _y): x(_x), y(_y) {};
};

// player length array
Point ** player_length_array;
unsigned int player_n = 1;

enum Direction {
    STOP,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};
Direction dir = STOP;

void set_up() {
    // player starts in the middle
    player_x = width / 2;
    player_y = height / 2;

    // randomise fruit location
    srand(time(NULL));
    fruit_x = rand() % (width-1);
    fruit_y = rand() % (height-1);

    // initialise player length
    player_length_array = new Point*[width * height];
    player_length_array[0] = new Point(player_x, player_y);
}

void draw() {
    system("cls");

    // draw structures
    for (int y = -1; y <= height; y++)
    {
        for (int x = -1; x <= width; x++)
        {
            if (y == -1 || y == height || x == -1 || x == width) {
                cout << "#";
            }
            else if (player_x == x && player_y == y) {
                cout << "O";
            }
            else if (fruit_x == x && fruit_y == y) {
                cout << "F";
            }
            else
            {
                bool occupied = false;
                // draw snake length
                for (size_t i = 0; i < player_n; i++)
                {
                    Point *pt = player_length_array[i];
                    if (x == pt->x && y == pt->y) {
                        cout << "o";
                        occupied = true;
                    }
                }
                if (!occupied) {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }

    // draw score
    cout << "score: " << score << endl;

    // debug
    // cout << "n = " << player_n << endl;
    // for (size_t i = 0; i < player_n; i++)
    // {
    //     Point * point = player_length_array[i];
    //     cout << point->x << "," << point->y << endl;
    // }
    
}
void read_input() {
    if (_kbhit()) {
        switch (_getch())
        {
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
                game_over = true;
                break;
        }
    }
}
void game_logic()
{
    int last_place = player_n - 1;

    // update snake length
    for (int i = last_place; i > 0; i--)
    {
        player_length_array[i] = player_length_array[i - 1];
    }

    // move by direction
    switch (dir) {
        case UP:
            player_y--;
            break;
        case DOWN:
            player_y++;
            break;
        case RIGHT:
            player_x++;
            break;
        case LEFT:
            player_x--;
            break;
    }

    player_length_array[0] = new Point(player_x, player_y);

    // out of bounds game over
    if (player_x < 0 || player_x >= width || player_y < 0 || player_y >= height) {
        game_over = true;
    }

    // eating fruit
    if (fruit_x == player_x && fruit_y == player_y) {
        score += 10;
        player_length_array[last_place+1] = new Point(fruit_x, fruit_y);
        fruit_x = rand() % (width - 1);
        fruit_y = rand() % (height - 1);
        player_n++;
    }
}

int main() {
    set_up();

    while (!game_over) {
        draw();
        read_input();
        game_logic();
        _sleep(1000 / FRAME_RATE);
    }

    for (size_t i = 1; i < player_n; i++)
    {
        free(player_length_array[i]);
    }
    free(player_length_array);

    return 0;
}