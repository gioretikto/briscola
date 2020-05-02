#include "briscola.h"
#include <stdio.h>
#include <string.h>
#include <glib.h>

extern struct card deck[CARDS];

void init_game(struct player_data *player) {

	shuffle(deck);
	
	table.briscola = deck[CARDS-1].suit;
	
	/* Deal first 6 cards */
	
	player[PLY0].card[0] = &deck[0];
	player[PLY0].card[1] = &deck[1];
	player[PLY0].card[2] = &deck[2];
	
	player[PLY1].card[0] = &deck[3];
	player[PLY1].card[1] = &deck[4];
	player[PLY1].card[2] = &deck[5];
	
	gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[0]), player[PLY0].card[0]->file);
	gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[1]), player[PLY0].card[1]->file);
	gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[2]), player[PLY0].card[2]->file);
	
	gtk_image_set_from_resource (GTK_IMAGE(table.image_briscola), deck[CARDS-1].file);
	
	table.cards_dealt = 6;
	
	memset(table.memo, 0, sizeof(table.memo));
	
	table.hand = 1;
	table.lim = 3;
	
	player[PLY0].total = 0;
	player[PLY1].total = 0;
	
	if (table.next_player == PLY0) {
		table.status = PLAY;
		table.next_player = PLY1;
	}
		
	else {
		table.status = BLOCK;
		table.next_player = PLY0;
		move(&player[PLY1]);
	}
}

void ply0_move(unsigned int index, struct player_data *player) {

	table.status = BLOCK;
		
	player[PLY0].slot = index;
		
	/* Hide played card */
	gtk_widget_hide(table.PLY0_image[index]);
	
	if (table.turn == PLY0)
		move_reply(player);
			
	else {
		gtk_image_set_from_resource (GTK_IMAGE(table.played_card[1]), player->card[player[PLY0].slot]->file);
		gtk_widget_show(table.played_card[1]);
		assign_points(player);
	}
}

void move(struct player_data *player) {

	unsigned int i;
	
	if ((i = verifyCombo(&player[PLY1])) != 5)
		;
	else
		i = min_max(player, MIN, -1);
	
	player->slot = i;
	
	/* Hide card Played by PLY1 */	
  	if (table.hand == 18)							/* In the last 3 hands start hiding from card 3 */
		gtk_widget_hide(table.PLY1_covered[2]);
		
	else if (table.hand == 19)
		gtk_widget_hide(table.PLY1_covered[1]);
			
	else
		gtk_widget_hide(table.PLY1_covered[i]);
	
	gtk_image_set_from_resource (GTK_IMAGE(table.played_card[0]), player->card[i]->file);
		
	gtk_widget_show(table.played_card[0]);

	table.status = PLAY;
}

void move_reply(struct player_data *player) {

	gtk_image_set_from_resource (GTK_IMAGE(table.played_card[0]), player[PLY0].card[player[PLY0].slot]->file);
	gtk_widget_show(table.played_card[0]);

	unsigned int i;
	unsigned int tmp;	
	unsigned int index = player[PLY0].slot;
	
	/* if it is an ace or three or 10 */
	if (player[PLY0].card[index]->value == 11 || player[PLY0].card[index]->value == 10) {
	
		if (player[PLY0].card[index]->suit == table.briscola) {
			
			if (player[PLY0].card[index]->value == 11)	/* There is no way to reply to this card*/
				i = min_max(&player[1], MIN, 1);
				
			else {
		  		/* try to take with a card of max value first, if fails use the min card */
				tmp = min_max(&player[1], MAX, 1);	/* get max. card */
					
				if (player[PLY1].card[tmp]->value > player[PLY0].card[index]->value)
					i = tmp;
					
				else								 	/* if the max value cannot overtake*/
					i = min_max(&player[1], MIN, 1);   /* try with min */
			}
		}
			
		else { /* if the Ace of 3 has suit different from briscola */
			
			if ((i = findCharge(&player[PLY1])) != 5)
				;
			
			else {
				if ((i = findBriscola(&player[PLY1])) != 5)
					;
				else
					i = min_max(&player[PLY1], MIN, 1);
			}
		}

	} /*End of if (player[PLY0].card[index]->value == 11 || .. */
	
	else if ((player[PLY0].card[index]->suit != table.briscola) && (player[PLY0].card[index]->value !=0)) {
	
		if ((i = findCharge(player)) != 5)
			;
		else {
			if ((i = findBriscola(&player[PLY1])) != 5)
				;
			else
				i = min_max(&player[PLY1], MIN, 1);			/* reply with a card of lowest value */
		}
	}
	
	else	/* card played has 0 value */
		i = min_max(&player[PLY1], MIN, 1);				/* reply with a card of lowest value */

	
	player[PLY1].slot = i;
	
	/* Hide card Played by PLY1 */
	
  	if (table.hand == 18)							/* In the last 3 hands start hiding from card 3 */
		gtk_widget_hide(table.PLY1_covered[2]);
		
	else if (table.hand == 19)
		gtk_widget_hide(table.PLY1_covered[1]);
			
	else 
		gtk_widget_hide(table.PLY1_covered[i]);
	
	
 	if (player[PLY0].card[index]->suit != table.briscola) {
 		if (player[PLY0].card[index]->value == 10 || player[PLY0].card[index]->value == 11)
 			table.memo[player[PLY0].card[index]->suit]++;
 	}
 			
 	if (player[PLY1].card[i]->suit != table.briscola) {
 		if (player[PLY1].card[i]->value == 10 || player[PLY1].card[i]->value == 11)
 			table.memo[player[PLY1].card[i]->suit]++;
 	}
	
	/* Move card played by PLY1 on the table */
	
	gtk_image_set_from_resource (GTK_IMAGE(table.played_card[PLY1]), player[PLY1].card[i]->file);
	
	gtk_widget_show(table.played_card[PLY1]);
 	
	/* Assign points of the hand */
	assign_points(player);
}

void assign_points (struct player_data *player) {

	unsigned int i;

	printf("Hand: %d\n", table.hand);
	printf("Cards dealt: %d\n", table.cards_dealt);
	printf("Briscola: %d\n", table.briscola);
	
	for (i = 0; i < table.lim; i++)
	 	printf("Card %u: value %d, suit %d\n", i+1, player[PLY1].card[i]->value, player[PLY1].card[i]->suit);
	 	
	printf("\n\n");

	enum players PLYM;			/* Player who moved */
	enum players PLYR;			/* Player replying */
	
	unsigned int indexR;		/* Index of player replying */
	unsigned int indexM;		/* Index of player who moved */
	
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
	
	printf("Played: value %d, suit %d, Index: %d\n", player[PLYM].card[indexM]->value, player[PLYM].card[indexM]->suit,  player[PLYM].slot);
 	printf("Reply: value %d, suit %d, Index: %d\n", player[PLYR].card[indexR]->value, player[PLYR].card[indexR]->suit, player[PLYR].slot);
		
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
					
	else if ((player[PLYR].card[indexR]->suit == table.briscola) || (player[PLYM].card[indexM]->suit == table.briscola)) {
		
		if (player[PLYR].card[indexR]->suit == table.briscola)
			table.winner = PLYR;
		else
			table.winner = PLYM;
	}
			
	else
		table.winner = PLYM;

	/* Assign Points to the winner of the hand*/
		
	player[table.winner].total += player[PLYM].card[indexM]->value + player[PLYR].card[indexR]->value;
		
	printf("Player who moved is: %u\n", PLYM);
	printf("Player who replied is: %u\n", PLYR);
	printf("Total Points PLY0: %d\n", player[PLY0].total);
	printf("Total Points PLY1: %d\n", player[PLY1].total);
	printf("Winner is: %d\n\n", table.winner);
		
	table.turn = table.winner;
	
	g_timeout_add(1000, (GSourceFunc)clean_table, player);
}

gboolean clean_table (struct player_data *player) {

		update_points(&player[table.winner], table.winner);

	if (table.cards_dealt <= CARDS-2) {
		draw_cards(player);
		gtk_widget_show(table.PLY1_covered[player[PLY1].slot]);
		
		/* Set the new card for PLY0 */
		gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[player[PLY0].slot]), player[PLY0].card[player[PLY0].slot]->file);
		gtk_widget_show(table.PLY0_image[player[PLY0].slot]);
		
		if (table.hand == 16) /*when there are only 2 cards left to play pile becomes back */
			gtk_image_set_from_resource (GTK_IMAGE(table.image_deck_pile), "/cards/back.png");
	}

	else { /* Do a card shifting to eliminate cards played in last 3 hands*/
	
		if (player[PLY1].slot == 0) {
			player[PLY1].card[0] = player[PLY1].card[1];
			player[PLY1].card[1] = player[PLY1].card[2];
			
		}
		
		if (player[PLY1].slot == 1)
			player[PLY1].card[1] = player[PLY1].card[2];
		
		table.lim--;
	}
	
	/* Hide card played */
	
	gtk_widget_hide(table.played_card[PLY0]);
	gtk_widget_hide(table.played_card[PLY1]);
	
	table.hand++;
	
	if (table.hand == 21)
		end_game(player);
		
	else if (table.turn == PLY1)
		move(&player[PLY1]);
		
	else
		table.status = PLAY;

	return FALSE;
}

void draw_cards (struct player_data *player) {

	unsigned int index0 = player[PLY0].slot;
	unsigned int index1 = player[PLY1].slot;
	
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
	
	update_cards_left();
	
	if (table.cards_dealt == CARDS) {
		gtk_label_set_text (GTK_LABEL(table.lbl_cards_left), "");
		gtk_widget_hide(table.image_briscola);
		gtk_widget_hide(table.image_deck_pile);
	}
}

unsigned int min_max (struct player_data *player, _Bool choice, int m) { 	/* if s == 0 calculate min else max */

	unsigned int i, index;
	int a[3];
	  
	for (i = 0; i < table.lim; i++) {
	
		a[i] = player->card[i]->value;
				
		if (player->card[i]->suit == table.briscola) {
		
			if (player->card[i]->value == 0)
				a[i] += 5;
			else
				a[i] += 4;
		}
		
		if (player->card[i]->suit != table.briscola)
			a[i] += m * table.memo[player->card[i]->suit];
	}
	
	index = 0;
 
	if (choice == MIN) {
 		int min = a[0];
		for (i = 1; i < table.lim; i++) {
			if (a[i] < min) {
			   index = i;
			   min = a[i];
			}
	  	}
	}
	
	else {
		int max = a[0];
		for (i = 1; i < table.lim; i++) {
			if (a[i] > max) {
			   index = i;
			   max = a[i];
			}
	  	}
	}
 
	return index;
}

unsigned int findCharge(struct player_data *player) {

	unsigned int i;
	
	for (i = 0; i < table.lim; i++) {
		if (player[PLY1].card[i]->suit == player[PLY0].card[player[PLY0].slot]->suit) {
			if (player[PLY1].card[i]->value > player[PLY0].card[player[PLY0].slot]->value)
				return i;
			}
	}
		
	return 5;
}

unsigned int findBriscola(struct player_data *player) {

	unsigned int i;
	
	for (i = 0; i < table.lim; i++) {
		if (player->card[i]->suit == table.briscola) 
				return i;
	}
		
	return 5;
}

unsigned int verifyCombo (struct player_data *player) {	/*Try to see if PLY0 has all aces and 3s */
	
	unsigned int i;
	
	if ((player->card[0]->value + player->card[1]->value + player->card[2]->value) >= 30)
		if ((i = findBriscola(player)) != 5)
			return i;

	return 5;
}
