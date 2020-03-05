/**
 * Implementaion of a snake object for snake game.
 * Allows for creation and update of snake.
 */

#include <ncurses.h>

/**
 * Represents are snake controlled by player.
 */
struct Snake
{
  int dir; //current direction snake is moving towards. 
  int maxLength; //Max length allowed for snake.
  int length; //current size
  int *xs; //x coordinates of snake.
  int *ys; //y coordinates of snake
};

void initialiseSnake(struct Snake *s, int maxSnakeLength)
{
  s->dir = KEY_RIGHT;
  s->maxLength = maxSnakeLength;
  s->length = 1;
  s->xs = (int *)malloc(s->maxLength * sizeof(int));
  s->ys = (int *)malloc(s->maxLength * sizeof(int));
  s->xs[0] = 10;
  s->ys[0] = 10;
}

/**
 * Update snake direction to given direction.
 * @param s: Snake to update direction for.
 * @param newKey: Input key from user.
 */
void updateSnakeDirection(struct Snake *s, int newKey)
{
  //Ignore key if no change to direction.
  if (s->dir != newKey)
  {
    //Only update if given key is valid arrow key.
    if (newKey == KEY_UP && s->dir != KEY_DOWN)
    {
      s->dir = KEY_UP;
    }
    else if (newKey == KEY_DOWN && s->dir != KEY_UP)
    {
      s->dir = KEY_DOWN;
    }
    else if (newKey == KEY_LEFT && s->dir != KEY_RIGHT)
    {
      s->dir = KEY_LEFT;
    }

    else if (newKey == KEY_RIGHT && s->dir != KEY_LEFT)
    {
      s->dir = KEY_RIGHT;
    }
  }
}

/**
 * Update x and y positions of the snake to reflect move based on its direction.
 * Store tail coordinates so board can be updated.
 * @param s: Snake to move.
 * @param t: Array to hold coordinates of tail, [x, y].
 * @param a: Array containing coordinates of apple, [x, y].
 * @param boardWidth: Width of board.
 * @param boardHeight: Height of board.
 * @returns: 1 if collision with apple , 2 if snake is at max length else 0. 
 */
int updateSnake(struct Snake *s, int *t, int *a, int boardWidth, int boardHeight)
{


  //Store x and y coordinates to tail.
  t[0] = s->xs[s->length - 1];
  t[1] = s->ys[s->length - 1];

  //Update tail. Shifting tail values of snake up towards head.
  for (int i = s->length - 1; i > 0; i--)
  {
    s->xs[i] = s->xs[i - 1];
    s->ys[i] = s->ys[i - 1];
  }

  //Update head
  int xOffset = 0;
  int yOffset = 0;

  //Shift x or y values based on direction.
  if (s->dir == KEY_UP)
  {
    yOffset = -1;
  }
  else if (s->dir == KEY_DOWN)
  {
    yOffset = 1;
  }
  else if (s->dir == KEY_LEFT)
  {
    xOffset = -1;
  }
  else if (s->dir == KEY_RIGHT)
  {
    xOffset = 1;
  }

  //Allow moving through walls to opposite walls.
  s->xs[0] = (s->xs[0] + xOffset + boardWidth) % boardWidth;
  s->ys[0] = (s->ys[0] + yOffset + boardHeight) % boardHeight;

  //Determine if head has collided with apple.
  int ateApple = a[0] == s->xs[0] && a[1] == s->ys[0] ? 1 : 0;

  //If apple, 'eaten' add tail back on and increase length of snake by 1.
  if (ateApple)
  {
    
    s->xs[s->length] = t[0];
    s->ys[s->length] = t[1];
    (s->length)++;
    
  }
  return s->length < s->maxLength? ateApple: 2;
}

/**
 * Determine if the head of snake collided with any part of its tail.
 * @param s: Snake to check for collision.
 * @returns: 1 if collided else 0.
 */
int collisionCheck(struct Snake *s)
{
  for (int i = 1; i < s->length; i++)
  {
    if (s->xs[0] == s->xs[i] && s->ys[0] == s->ys[i])
    {
      return 1;
    }
  }
  return 0;
}

/**
 * Determine whether given coordinates is on given snake.
 * @param s: Snake to check.
 * @param x: x coord.
 * @param y: y coord.
 * @returns: 1 if coordinate on snake else 0.
 */
int onSnake(struct Snake *s, int x, int y)
{

  for (int i = 0; i < s->length; i++)
  {
    if (s->xs[i] == x && s->ys[i] == y)
    {
      return 1;
    }
  }
  return 0;
}