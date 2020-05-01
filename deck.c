#include <stdlib.h>
#include <time.h>

#include "briscola.h"

struct card	deck[] = {
	{1, "c/1.png", 11, BASTONI},
    {2, "c/2.png", 0, BASTONI},
    {3, "c/3.png", 10, BASTONI},
    {4, "c/4.png", 0, BASTONI},
	{5, "c/5.png", 0, BASTONI},
	{6, "c/6.png", 0, BASTONI},
	{7, "c/7.png", 0, BASTONI},
	{8, "c/8.png", 2, BASTONI},
	{9, "c/9.png", 3, BASTONI},
	{10, "c/10.png", 4, BASTONI},
	{11, "c/11.png", 11, DENARI},
    {12, "c/12.png", 0, DENARI},
    {13, "c/13.png", 10, DENARI},
    {14, "c/14.png", 0, DENARI},
	{15, "c/15.png", 0, DENARI},
	{16, "c/16.png", 0, DENARI},
	{17, "c/17.png", 0, DENARI},
	{18, "c/18.png", 2, DENARI},
	{19, "c/19.png", 3, DENARI},
	{20, "c/20.png", 4, DENARI},
	{21, "c/21.png", 11, COPPE},
    {22, "c/22.png", 0, COPPE},
    {23, "c/23.png", 10, COPPE},
    {24, "c/24.png", 0, COPPE},
	{25, "c/25.png", 0, COPPE},
	{26, "c/26.png", 0, COPPE},
	{27, "c/27.png", 0, COPPE},
	{28, "c/28.png", 2, COPPE},
	{29, "c/29.png", 3, COPPE},
	{30, "c/30.png", 4, COPPE},
	{31, "c/31.png", 11, SPADE},
    {32, "c/32.png", 0, SPADE},
    {33, "c/33.png", 10, SPADE},
    {34, "c/34.png", 0, SPADE},
	{35, "c/35.png", 0, SPADE},
	{36, "c/36.png", 0, SPADE},
	{37, "c/37.png", 0, SPADE},
	{38, "c/38.png", 2, SPADE},
	{39, "c/39.png", 3, SPADE},
	{40, "c/40.png", 4, SPADE}
    };

void shuffle (struct card *xdeck) {

	int i, j;
	struct card temp;

	for (i = 0; i < CARDS - 1; i++) {
	    j = rand() % CARDS; 
	    temp = xdeck[j];
	    xdeck[j] = xdeck[i];
	    xdeck[i] = temp;
    }  
}
