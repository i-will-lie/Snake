/**
 * Snake game.
 * Played on console.
 * Control snake with arrow keys to ate apples upto specified amount.
 * Lose if collide with tail.
 * '+' and '-' to increase and decrease speed respectively.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include "./moveQueue.c"
#include "./snake.c"


#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define MAX_SNAKE_LENGTH 20
#define MOVE_QUEUE_SIZE 3
#define DEFAULT_GAME_SPEED 500000
#define GAME_SPEED_ADJUST 50000

void init();
void initialiseBoard();
void updateApple(struct Snake *s, int *a);
void drawApple(int *a);
void drawSnake(struct Snake *s, int *t, int ateApple);
void drawScore();

/**
 * Initialise window with ncurses.
 * Hide cursor and enable arrow keys detection. 
 */
void init()
{
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  clear();
}

/**
 * Render game board.
 */
void initialiseBoard()
{
  //Borders.
  mvhline(0, 0, '-', BOARD_WIDTH + 2);
  mvhline(BOARD_HEIGHT + 1, 0, '-', BOARD_WIDTH + 2);
  mvvline(1, 0, '|', BOARD_HEIGHT);
  mvvline(1, BOARD_WIDTH + 1, '|', BOARD_HEIGHT);

  //Area.
  for (int i = 1; i < BOARD_HEIGHT + 1; i++)
  {
    mvhline(i, 1, '.', BOARD_WIDTH);
  }
}

/**
 * Generate are new apple on space on the board
 * not occupied by snake.
 */ 
void updateApple(struct Snake *s, int *a)
{
  do
  {
    a[0] = rand() % BOARD_WIDTH;
    a[1] = rand() % BOARD_HEIGHT;
  } while (onSnake(s, a[0], a[1]));
}

/**
 * Render apple of the board.
 * @param a: Coordinates to render apple.
 */
void drawApple(int *a)
{
  mvaddch(a[1] + 1, a[0] + 1, '@');
}

/**
 * Render snake on the board.
 * @param s: Snake to render.
 * @param t: Previous tail end of snake.
 * @param ateApple: Flag indicating if apple has been ate.
 */ 
void drawSnake(struct Snake *s, int *t, int ateApple)
{ 
  //Draw over previous position of snake head. 
  if (s->length > 1)
  {
    mvaddch(s->ys[1] + 1, s->xs[1] + 1, 'O');
  }
  //If apple not eaten, draw over to shorten length.
  if (!ateApple)
  {
    mvaddch(t[1] + 1, t[0] + 1, '.');
  }
  //Draw snake head.
  mvaddch(s->ys[0] + 1, s->xs[0] + 1, 'X');
}

/**
 * Render current score.
 * @param score: score to render.
 */
void drawScore(int score)
{
  char scoreStr[MAX_SNAKE_LENGTH];
  sprintf(scoreStr, "%d", score);
  mvaddstr(BOARD_HEIGHT+3, 8, scoreStr);
}

/**
 * Initialise and run the game.
 */
void runGame()
{
  clear();
  int game_speed = DEFAULT_GAME_SPEED;
  struct Snake s;
  struct MoveQueue moveBuffer;

  //x,y coordinates of tail location.
  int t[2];

  //x, y coodinates of apple location.
  int a[2];
  initialiseBoard();

  initialiseMoveBuffer(&moveBuffer, MOVE_QUEUE_SIZE);
  initialiseSnake(&s, MAX_SNAKE_LENGTH);

  time_t tt;
  srand((unsigned)time(&tt));

  updateApple(&s, a);
  drawApple(a);
  mvaddstr(23, 1, "Score: 0");
  refresh();

  int key; //User input.
  int tickCount = 0; //Counter to control game speed.
  int score = 0; //Player score.

  //Main game loop.
  while (1)
  {
    //Check for ket inputs.
    key = getch();
    if (key == '+')
    {
      game_speed -= GAME_SPEED_ADJUST;
    }
    else if (key == '-')
    {
      game_speed += GAME_SPEED_ADJUST;
    }
    else
    {
      moveOffer(&moveBuffer, key);
    }


    if (tickCount >= game_speed)
    {
      tickCount = 0;

      int movekey = movePoll(&moveBuffer);

      updateSnakeDirection(&s, movekey);

      int ateApple = updateSnake(&s, t, a, BOARD_WIDTH, BOARD_HEIGHT);

      //Update apple and score if eaten.
      if (ateApple)
      {
        updateApple(&s, a);
        drawApple(a);
        score++;
        drawScore(score);
      }

      drawSnake(&s, t, ateApple);

      //End game if eaten enough apples.
      if (ateApple == 2)
      {
        mvaddstr((BOARD_HEIGHT / 3), 2, "  CONGRATULATIONS, THE SNAKE IS FULL!!!  ");
        break;
      }
      refresh();
      //End game if self collision.
      if (collisionCheck(&s))
      {
        mvaddstr((BOARD_HEIGHT / 3), 2, "  YOU LOSE!  ");
        break;
      }
    }

    tickCount++;
  }
  //Free memory.
  free(s.xs);
  free(s.ys);
  free(moveBuffer.moves);
}

int main()
{
  init();
  while (1)
  {
    mvprintw(BOARD_HEIGHT / 2, 5, "  Press [space] to start or [x] to exit.  ");
    refresh();
    int input = getchar();
    if (input == ' ')
    {
      runGame();
    }
    else if (input == 'x')
    {
      break;
    }
  }
  system("clear");
  endwin();
  exit(0);
}