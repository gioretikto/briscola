#include <stdlib.h>
#include <time.h>

#include "briscola.h"

struct card	deck[] = {
	{1, "/cards/1.png", 11, BASTONI},
    {2, "/cards/2.png", 0, BASTONI},
    {3, "/cards/3.png", 10, BASTONI},
    {4, "/cards/4.png", 0, BASTONI},
	{5, "/cards/5.png", 0, BASTONI},
	{6, "/cards/6.png", 0, BASTONI},
	{7, "/cards/7.png", 0, BASTONI},
	{8, "/cards/8.png", 2, BASTONI},
	{9, "/cards/9.png", 3, BASTONI},
	{10, "/cards/10.png", 4, BASTONI},
	{11, "/cards/11.png", 11, DENARI},
    {12, "/cards/12.png", 0, DENARI},
    {13, "/cards/13.png", 10, DENARI},
    {14, "/cards/14.png", 0, DENARI},
	{15, "/cards/15.png", 0, DENARI},
	{16, "/cards/16.png", 0, DENARI},
	{17, "/cards/17.png", 0, DENARI},
	{18, "/cards/18.png", 2, DENARI},
	{19, "/cards/19.png", 3, DENARI},
	{20, "/cards/20.png", 4, DENARI},
	{21, "/cards/21.png", 11, COPPE},
    {22, "/cards/22.png", 0, COPPE},
    {23, "/cards/23.png", 10, COPPE},
    {24, "/cards/24.png", 0, COPPE},
	{25, "/cards/25.png", 0, COPPE},
	{26, "/cards/26.png", 0, COPPE},
	{27, "/cards/27.png", 0, COPPE},
	{28, "/cards/28.png", 2, COPPE},
	{29, "/cards/29.png", 3, COPPE},
	{30, "/cards/30.png", 4, COPPE},
	{31, "/cards/31.png", 11, SPADE},
    {32, "/cards/32.png", 0, SPADE},
    {33, "/cards/33.png", 10, SPADE},
    {34, "/cards/34.png", 0, SPADE},
	{35, "/cards/35.png", 0, SPADE},
	{36, "/cards/36.png", 0, SPADE},
	{37, "/cards/37.png", 0, SPADE},
	{38, "/cards/38.png", 2, SPADE},
	{39, "/cards/39.png", 3, SPADE},
	{40, "/cards/40.png", 4, SPADE}
    };

/* The following is The Fisher–Yates algorithm to shuffle the deck */

void swap (struct card *a, struct card *b) { 
	struct card temp = *a;
    *a = *b; 
    *b = temp; 
}

static int randgen(int lower, int upper) {
  int r;
  int span = upper - lower + 1;
  int cutoff = (RAND_MAX / span) * span;

  while ((r = rand()) >= cutoff)
    continue;

  return ((r % span) + lower);
}
  
void shuffle(struct card *xdeck) {
  for (int i = CARDS - 1; i >= 1; --i)
    swap(&xdeck[i],             	  // swap the current element
         &xdeck[randgen(0, i)]);      // with some random element
}
