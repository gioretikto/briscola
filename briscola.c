#include <stdlib.h>
#include <time.h>

#include "briscola.h"

struct card	deck[] = {
	{"c/1.png", 11, BASTONI},
    {"c/2.png", 0, BASTONI},
    {"c/3.png", 10, BASTONI},
    {"c/4.png", 0, BASTONI},
	{"c/5.png", 0, BASTONI},
	{"c/6.png", 0, BASTONI},
	{"c/7.png", 0, BASTONI},
	{"c/8.png", 2, BASTONI},
	{"c/9.png", 3, BASTONI},
	{"c/10.png", 4, BASTONI},
	{"c/11.png", 11, DENARI},
    {"c/12.png", 0, DENARI},
    {"c/13.png", 10, DENARI},
    {"c/14.png", 0, DENARI},
	{"c/15.png", 0, DENARI},
	{"c/16.png", 0, DENARI},
	{"c/17.png", 0, DENARI},
	{"c/18.png", 2, DENARI},
	{"c/19.png", 3, DENARI},
	{"c/20.png", 4, DENARI},
	{"c/21.png", 11, COPPE},
    {"c/22.png", 0, COPPE},
    {"c/23.png", 10, COPPE},
    {"c/24.png", 0, COPPE},
	{"c/25.png", 0, COPPE},
	{"c/26.png", 0, COPPE},
	{"c/27.png", 0, COPPE},
	{"c/28.png", 2, COPPE},
	{"c/29.png", 3, COPPE},
	{"c/30.png", 4, COPPE},
	{"c/31.png", 11, SPADE},
    {"c/32.png", 0, SPADE},
    {"c/33.png", 10, SPADE},
    {"c/34.png", 0, SPADE},
	{"c/35.png", 0, SPADE},
	{"c/36.png", 0, SPADE},
	{"c/37.png", 0, SPADE},
	{"c/38.png", 2, SPADE},
	{"c/39.png", 3, SPADE},
	{"c/40.png", 4, SPADE}
    };

void shuffle (struct card *deck) {

	int i, j;
	struct card temp;

	for (i = 0; i < CARDS - 1; i++) {
	    j = rand() % CARDS; 
	    temp = deck[j];
	    deck[j] = deck[i];
	    deck[i] = temp;
    }  
}