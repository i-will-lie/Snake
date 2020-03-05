/**
 * Implementation of a queue. Allows creation of a queue for integers 
 * with a specified capaacity
 */
#include <ncurses.h>


/**
 * Limited sized queue to hold integers.
 */
struct MoveQueue
{
  int maxLength; //Max number moves able to be held.
  int length;    //Number of moves currently on queue.
  int h; //Head of queue.
  int n; //Next postion of queue to add to.
  int *moves; //List of moves in queue.
};

/**
 * Initailse MoveQueue with given size.
 * @param q: Queue to initialise.
 * @param maxQueueSize: Size of queue to set as.
 */
void initialiseMoveBuffer(struct MoveQueue *q, int maxQueueSize)
{
  q->maxLength = maxQueueSize;
  q->length = 0;
  q->h = 0;
  q->n = 0;
  q->moves = (int *)malloc(q->maxLength * sizeof(int));
}

/**
 * Add a new move to end of the given queue if not full and given key is valid move.
 * @param q: Queue to add move to.
 * @param key: Key press representing the move.
 */
void moveOffer(struct MoveQueue *q, int key)
{

  if (q->length < q->maxLength && (key == KEY_DOWN || key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT))
  {
    q->moves[q->n] = key;
    q->n = (q->n + 1) % q->maxLength;
    (q->length)++;
  }
}

/**
 * Return the next move in the given queue and shift head to next postion.
 * @param q: Queue to get move from.
 * @returns: The next move or -1 if empty.
 */
int movePoll(struct MoveQueue *q)
{
  //Check for empty queue.
  if (q->length == 0)
  {
    return -1;
  }

  (q->length)--;

  int move = q->moves[q->h];

  //Shift head to next.
  q->h = (q->h + 1) % q->maxLength;

  return move;
}