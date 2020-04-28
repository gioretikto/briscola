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
	memset(table.memo, 0, 4 * sizeof(int));
	table.hand = 1;
	
	player[PLY0].total = 0;
	player[PLY1].total = 0;
	
	display_cards(player);
}

void move(struct player_data *player) {

	int i;
	
	for (i = 0; i < 3; i++)
	 	printf("Cards %d: value %d , suit %d\n", i+1, player[PLY1].card[i]->value, player[PLY1].card[i]->suit);
	
	i = min_max(player, 0, -1);
	player->slot = i;
		
	gtk_widget_hide(table.PLY1_covered[i]);
	
	gtk_image_set_from_file(GTK_IMAGE(table.played_card[0]), player->card[i]->file);
	gtk_widget_show(table.played_card[0]);	

	table.status = PLAY;
}

void move_reply(struct player_data *player) {

	gtk_image_set_from_file(GTK_IMAGE(table.played_card[0]), player[PLY0].card[player[PLY0].slot]->file);
	gtk_widget_show(table.played_card[0]);

	int i;
	int tmp;
	int index = player[PLY0].slot;
	
	for (i = 0; i < 3; i++)
	 	printf("Cards %d: value %d , suit %d\n", i+1, player[PLY1].card[i]->value, player[PLY1].card[i]->suit);
	
	i = 0;
	
	if (player[PLY0].card[index]->suit != table.briscola) {
	
		if (find_charge(player) != -1)
				i = find_charge(player);		
		else
				i = min_max(&player[1], 0, 1);				/* if it is a scartina reply with another scartina */
	}		
	
	/* if it is an ace or three or 10 */
	else if ( (player[PLY0].card[index]->value == 11) || (player[PLY0].card[index]->value == 10) || (player[PLY0].card[index]->value == 4) ) {
		
		if (player[PLY0].card[index]->suit != table.briscola) { /* try to take with a card of min value 0, else with another card */
			
			tmp = min_max(&player[PLY1], 0, 1); 
				
			if (player[PLY1].card[tmp]->value > player[PLY0].card[index]->value) 	   /* if the min value cannot overtake*/			
				i = tmp;
			
			else {
				tmp = min_max(&player[1], 1, 1);   /* try with max */
						
				if (player[PLY1].card[tmp]->value > player[PLY0].card[index]->value)   /* Try to see if the max value can do it */
					i = tmp;
			}
		}		
	}
	
	else
		i = min_max(&player[1], 0, 1);
	
	player[PLY1].slot = i;
	
	/* Hide card Played by PLY1 */
	gtk_widget_hide(table.PLY1_covered[i]);
	
	printf("Briscola: %d\n", table.briscola);
 	printf("Played: value %d, suit %d, Index: %d\n", player[PLY0].card[index]->value, player[PLY0].card[index]->suit, index);
 	printf("Reply: value %d, suit %d, Index: %d\n", player[PLY1].card[i]->value, player[PLY1].card[i]->suit, player[PLY1].slot);
 	
 	if (player[PLY0].card[index]->suit != table.briscola)
 		if(player[PLY0].card[index]->value == 10 || player[PLY0].card[index]->value == 11)
 			table.memo[player[PLY0].card[index]->suit]++;
 			
 	if (player[PLY1].card[i]->suit != table.briscola)
 		if(player[PLY1].card[i]->value == 10 || player[PLY1].card[i]->value == 11)
 			table.memo[player[PLY1].card[i]->suit]++;
	
	/* Move card played by PLY1 on the table */
	
	gtk_image_set_from_file(GTK_IMAGE(table.played_card[PLY1]), player[PLY1].card[i]->file);
	gtk_widget_show(table.played_card[PLY1]);
	
	/* Assign points of the hand */
	assign_points(player);
}

void assign_points (struct player_data *player) {

	printf("Hand: %d\n", table.hand);
	printf("Cards dealt: %d\n", table.cards_dealt);

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
		player[PLYR].total += player[PLYM].card[indexM]->value + player[PLYR].card[indexR]->value;
		
	else
		player[PLYM].total +=  player[PLYM].card[indexM]->value + player[PLYR].card[indexR]->value;
	
	printf("Player who moved is: %d\n", PLYM);
	printf("Player who replied is: %d\n", PLYR);
	printf("Total Points PLY0: %d\n", player[PLY0].total);
	printf("Total Points PLY1: %d\n", player[PLY1].total);
	printf("Winner is: %d\n\n", table.winner);
		
	table.turn = table.winner;
	
	g_timeout_add(1000, (GSourceFunc)clean_table, player);
}

gboolean clean_table (struct player_data *player) {

	if (table.cards_dealt < CARDS) {	
		draw_cards(player);
		gtk_widget_show(table.PLY1_covered[player[PLY1].slot]);
		
		/* Set the new card for PLY0 */
		gtk_image_set_from_file(GTK_IMAGE(table.PLY0_image[player[PLY0].slot]), player[PLY0].card[player[PLY0].slot]->file);
		gtk_widget_show(table.PLY0_image[player[PLY0].slot]);
	}
	
	else {
		gtk_widget_hide(table.PLY1_covered[player[PLY1].slot]);
	}	 

	update_points(&player[table.winner], table.winner);
	update_cards_left();
	
	/* Hide card played */	
	
	gtk_widget_hide(table.played_card[PLY0]);
	gtk_widget_hide(table.played_card[PLY1]);
	
	table.hand++;
	
	if (player[PLY0].total + player[PLY1].total == 120)
		print_end_msg(player);
	
	else if (table.turn == PLY1)
		move(&player[PLY1]);
	else
		table.status = PLAY;

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
		player[PLY1].card[index1] = &deck[table.cards_dealt];
		player[PLY0].card[index0] = &deck[table.cards_dealt+1];		
	}
	
	table.cards_dealt += 2;
	
	if (table.cards_dealt == CARDS) {
		gtk_widget_hide(table.image_briscola);
		gtk_widget_hide(table.image_deck_pile);	
	}
}

int min_max (struct player_data *player, _Bool s, int m) { 	/* if s == 0 calculate min else max */

	int i, index;
	int a[3];
	  
	for (i = 0; i < 3; i++) {
	
		a[i] = player->card[i]->value;
				
		if (player->card[i]->suit == table.briscola) {
			if (player->card[i]->value == 0)
				a[i] += 2;
			else
				a[i] += 1;
		}
		
		if (player->card[i]->suit != table.briscola)
			a[i] = a[i] + m * table.memo[player->card[i]->suit];
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

int find_charge(struct player_data *player) {

	int i;
	
	for (i = 0; i < 3; i++) {
		if (player[PLY1].card[i]->suit == player[PLY0].card[player[PLY0].slot]->suit)
			if (player[PLY1].card[i]->value > player[PLY0].card[player[PLY0].slot]->value)
				return i;
	}
		
	return -1;
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
