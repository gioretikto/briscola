#include "briscola.h"
#include <stdio.h>
#include <glib.h>

extern struct card deck[40];

/*extern int number_players;*/

void init_game(struct player_data *player) {

	shuffle(deck);
	
	table.briscola = deck[39].suit;		/* the 1st card in deck is briscola */
	
	/* Deal first 6 cards */
	
	player[PLY0].card[0] = &deck[0];
	player[PLY0].card[1] = &deck[1];
	player[PLY0].card[2] = &deck[2];
	
	player[PLY1].card[0] = &deck[3];
	player[PLY1].card[1] = &deck[4];
	player[PLY1].card[2] = &deck[5];
	
	table.cards_dealt = 6;
	
	player[PLY0].total = 0;
	player[PLY1].total = 0;
	
	display_cards(player);
}

void display_cards (struct player_data *player) {

	/* Player's cards */
	
	table.PLY0_image[0] = gtk_image_new_from_file (player[PLY0].card[0]->file);
	table.PLY0_image[1] = gtk_image_new_from_file (player[PLY0].card[1]->file);
	table.PLY0_image[2] = gtk_image_new_from_file (player[PLY0].card[2]->file);
	
	/* Image of Dealer's covered cards */
  	
	table.PLY1_covered[0] = gtk_image_new_from_file ("c/back.png");
	table.PLY1_covered[1] = gtk_image_new_from_file ("c/back.png");
	table.PLY1_covered[2] = gtk_image_new_from_file ("c/back.png");
  		
	/* Images for Briscola and deck */
	  	
  	table.image_briscola = gtk_image_new_from_file (deck[39].file);
	
  	table.image_deck_pile = gtk_image_new_from_file ("c/deck.png");
}

void move(struct player_data *player) {

	int i;
	
	i = min_max(player, 0);
	
	gtk_widget_hide(table.PLY1_covered[i]);
	
	gtk_image_set_from_file(GTK_IMAGE(table.image_table[PLY1]), player->card[i]->file);
	
	player->slot = i;

	table.status = PLAY;
}

void move_reply(struct player_data *player) {

	int i;
	int tmp;
	int index = player[PLY0].slot;
		
	for (i = 0; i < 3; i++)
	 	printf("value %d , suit %d\n", player[PLY1].card[i]->value, player[PLY1].card[i]->suit);
	
	i = 0;
	
	if (player[PLY0].card[index]->value == 0)		/* if it is a scartina reply with another scartina */
		i = min_max(&player[1], 0);
	
	/* if it is an ace or three or 10 */
	else if ( (player[PLY0].card[index]->value == 11) || (player[PLY0].card[index]->value == 10) || (player[PLY0].card[index]->value == 4) ) {	
		
		if (player[PLY0].card[index]->suit != table.briscola) { /* try to take with a card of min value 0, else with another card */
			
			tmp = i = min_max(&player[PLY1], 0); 
				
			if (player[PLY1].card[i]->value < player[PLY0].card[index]->value) { 	   /* if the min value cannot overtake*/
				i = min_max(&player[1], 1);   /* try with max */
				
				if (player[PLY1].card[i]->value > player[PLY0].card[index]->value)	   /* Try to see if the max value can do it */
					;
				else 			/* if not even the max can take use the min card */
					i = tmp;
			}
		}
		
	else {
		i = min_max(&player[PLY1], 1); 		/* Try to see if the max value can overtake ace or 3*/
				
		if (player[PLY1].card[i]->value < player[PLY0].card[index]->value)
			i = min_max(&player[PLY1], 0); 	/* If not Play the card with lowest value */
		}
	}
	
	player[PLY1].slot = i;
	
	/* Hide card Played by PLY1 */
	gtk_widget_hide(table.PLY1_covered[i]);
	
	printf("Briscola: %d\n", table.briscola);
 	printf("Played: value %d, suit %d, Index: %d \n", player[PLY0].card[index]->value, player[PLY0].card[index]->suit, index);
 	printf("Reply: value %d, suit %d, Index: %d \n", player[PLY1].card[i]->value, player[PLY1].card[i]->suit, player[PLY1].slot);
	
	/* Move card played by PLY1 on the table */
	
	gtk_image_set_from_file(GTK_IMAGE(table.image_table[PLY1]), player[PLY1].card[i]->file);
	
	/* Assign points of the hand */
	
	g_timeout_add(2000, (GSourceFunc)assign_points, player);
}

gboolean assign_points(struct player_data *player) {

	int PLYM;		/* Player who moved */
	int PLYR;		/* Player replying */
	int indexR;		/* Index of player replying */
	int indexM;		/* Index of player who moved */
		
	if (table.turn == PLY0) {
		PLYM = PLY0;
		PLYR = PLY1;
		indexM = player[PLY0].slot;
		indexR = player[PLY1].slot;
	}
		
	else {
		PLYM = PLY1;
		PLYR = PLY0;
		indexM = player[PLY1].slot;
		indexR = player[PLY0].slot;
	}
		
	/* Evaluate the table.winner of the hand */
		
	if (player[PLYR].card[indexR]->suit == player[PLYM].card[indexM]->suit)	{
		
		if (player[PLYR].card[indexR]->value == player[PLYM].card[indexM]->value) {	/* if they are both cards of 0 value */
		
				if (player[PLYR].card[indexR]->index > player[PLYM].card[indexM]->index)
					table.winner = PLYR;
				else
					table.winner = PLYM;
		}
		
		else if (player[PLYR].card[indexR]->value > player[PLYM].card[indexM]->value)
			table.winner = PLYR;
		else
			table.winner = PLYM;
	}
					
	else if ( (player[PLYR].card[indexR]->suit == table.briscola) || (player[PLYM].card[indexM]->suit == table.briscola) ) {
		
		if (player[PLYR].card[indexR]->suit == table.briscola)
			table.winner = PLYR;
		else
			table.winner = PLYM;
	}
		
	else
		table.winner = PLYM;

	/* Assign Points to the table.winner of the hand*/
		
	if (table.winner == PLYR)
		player[PLYR].total = player[PLYR].total + player[PLYM].card[indexM]->value + player[PLYR].card[indexR]->value;
		
	else
		player[PLYM].total = player[PLYM].total + player[PLYM].card[indexM]->value + player[PLYR].card[indexR]->value;
		
	printf("Player who moved is: %d\n", PLYM);
	printf("Player who replied is: %d\n", PLYR);
	printf("Card index played is: %d\n", indexM);
	printf("Card index replayed is: %d\n", indexR);
	printf("Total Points PLY0: %d\n", player[PLY0].total);
	printf("Total Points PLY1: %d\n", player[PLY1].total);
	printf("Winner is: %d\n", table.winner);
	printf("Player M hand points: %d\n", player[PLYM].card[indexM]->value);
	printf("Player R hand points: %d\n\n\n", player[PLYR].card[indexR]->value);
		
	table.turn = table.winner;
		
	update_points(&player[table.winner], table.winner);
		
	draw_cards(player);
	
	table.cards_dealt += 2;
	
	if (table.turn == PLY1)
		move(&player[PLY1]);
	else
		table.status = PLAY;
	
	printf("Table status %d\n", table.status);
	gtk_widget_show(table.PLY1_covered[player[PLY1].slot]);

	return FALSE;
}

void draw_cards (struct player_data *player) {

	int index0 = player[PLY0].slot;
	int index1 = player[PLY1].slot;
	
	/* Draw new card from deck */

	if (table.turn == PLY0) {
		player[PLY0].card[index0] = &deck[table.cards_dealt];
		player[PLY1].card[index1] = &deck[table.cards_dealt+1];
	}
	
	else {
		player[PLY0].card[index0] = &deck[table.cards_dealt+1];
		player[PLY1].card[index1] = &deck[table.cards_dealt];
	}
	
	/* Set the new card for PLY0 */
	gtk_image_set_from_file(GTK_IMAGE(table.PLY0_image[player[PLY0].slot]), player[PLY0].card[index0]->file);

	/* Display the cards */
	gtk_widget_show(table.PLY0_image[index0]);		
	gtk_widget_show(table.PLY1_covered[index1]);
	
	/* Hide played cards */
	gtk_widget_hide(table.image_table[PLY0]);
	gtk_widget_hide(table.image_table[PLY1]);
}

int min_max (struct player_data *player, _Bool s) { 	/* if s == 0 calculate min else max */

	int i, index;
	int a[3];
	  
	for (i = 0; i < 3; i++) {
	
		a[i] = player->card[i]->value;
				
		if (player->card[i]->suit == table.briscola)
			a[i] += 1;
	}
	
	index = 0;
 
	if (s == 0) {
 		int min = a[0];
		for (i = 1; i < 3; i++) {
			if (a[i] < min) {
			   index = i;
			   min = a[i];
			}
	  	}
	}
	
	else {
		int max = a[0];
		for (i = 1; i < 3; i++) {
			if (a[i] > max) {
			   index = i;
			   max = a[i];
			}
	  	}
	}
 
	return index;
}

void update_points(struct player_data *player, int index) {

	char *display;
    display = g_strdup_printf("%d", player[index].total);					/* convert num to str */
    gtk_label_set_text (GTK_LABEL(table.label_player[index]), display);		/* set label to "display */
    
    g_free(display);
}
