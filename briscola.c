#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "briscola.h"

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