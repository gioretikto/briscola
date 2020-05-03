#include "briscola.h"
#include <stdio.h>
#include <string.h>
#include <glib.h>

extern struct card deck[CARDS];

void init_game(struct player_data *player) {

	shuffle(deck);
	
	game.briscola = deck[CARDS-1].suit;
	
	/* Deal first 6 cards */
	
	player[PLY0].card[0] = &deck[0];
	player[PLY0].card[1] = &deck[1];
	player[PLY0].card[2] = &deck[2];
	
	/* for CPU */
	
	player[PLY1].card[0] = &deck[3];
	player[PLY1].card[1] = &deck[4];
	player[PLY1].card[2] = &deck[5];
	
	gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[0]), player->card[0]->file);
	gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[1]), player->card[1]->file);
	gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[2]), player->card[2]->file);
	
	gtk_image_set_from_resource (GTK_IMAGE(table.image_briscola), deck[CARDS-1].file);
	
	/* Set image for deck right to the briscola */
	gtk_image_set_from_resource (GTK_IMAGE(table.image_deck_pile), "/cards/deck.png");
	
	memset(game.memo, 0, sizeof(game.memo));
	
	player[PLY0].total = 0;
	player[PLY1].total = 0;
	game.hand = 1;
	game.lim = 3;
	game.cards_dealt = 6;
	
	update_cards_left();
	
	if (game.next_player == PLY0) {
		game.status = PLAY;
		game.next_player = PLY1;
	}
		
	else {
		game.status = BLOCK;
		game.next_player = PLY0;
		move(&player[PLY1]);
	}
}

void ply0_move(unsigned int index, struct player_data *player) {

	game.status = BLOCK;
		
	player->slot = index;
		
	/* Hide played card */
	gtk_widget_hide(table.PLY0_image[index]);
	
	if (game.turn == PLY0)
		move_reply(player);
			
	else {
		displayCard(player, 1);
		assign_points(player);
	}
}

void move(struct player_data *cpu) {

	unsigned int i;
	
	if ((i = verifyCombo(cpu)) != 5)
		;
	else
		i = min_max(cpu, MIN, -1);
	
	cpu->slot = i;
	
	/* Hide card Played by PLY1 */	
  	if (game.hand == LAST_HAND-2)							/* In the last 3 hands start hiding from card 3 */
		gtk_widget_hide(table.PLY1_covered[2]);
		
	else if (game.hand == LAST_HAND-1)
		gtk_widget_hide(table.PLY1_covered[1]);
			
	else
		gtk_widget_hide(table.PLY1_covered[i]);
	
	displayCard(cpu, 0);

	game.status = PLAY;
}

void move_reply(struct player_data *player) {

	struct player_data *pm, *pr; 	/* pm: player who moved*/
	
	pm = player;
	pr = player + 1;

	displayCard(pm, 0);

	unsigned int i;
	unsigned int tmp;	
	
	/* if it is an ace or three or 10 */
	if (pm->card[pm->slot]->value == 11 || pm->card[pm->slot]->value == 10) {
	
		if (pm->card[pm->slot]->suit == game.briscola) {
			
			if (pm->card[pm->slot]->value == 11)	/* There is no way to reply to this card*/
				i = min_max(pr, MIN, 1);
				
			else {
		  		/* try to take with a card of max value first, if fails use the min card */
				tmp = min_max(pr, MAX, 1);	/* get max. card */
					
				if (pr->card[tmp]->value > pm->card[pm->slot]->value)
					i = tmp;
					
				else								 	/* if the max value cannot overtake*/
					i = min_max(pr, MIN, 1);   /* try with min */
			}
		}
			
		else { /* if the Ace of 3 has suit different from briscola */
			
			if ((i = findCharge(pr)) != 5)
				;
			
			else {
				if ((i = findBriscola(pr)) != 5)
					;
				else
					i = min_max(pr, MIN, 1);
			}
		}

	} /*End of if (pm->card[index]->value == 11 || .. */
	
	else if ((pm->card[pm->slot]->suit != game.briscola) && (pm->card[pm->slot]->value !=0)) {
	
		if ((i = findCharge(player)) != 5)
			;
		else {
			if ((i = findBriscola(pr)) != 5)
				;
			else
				i = min_max(pr, MIN, 1);			/* reply with a card of lowest value */
		}
	}
	
	else	/* card played has 0 value */
		i = min_max(pr, MIN, 1);				/* reply with a card of lowest value */

	
	pr->slot = i;
	
	/* Hide card Played by PLY1 */
	
  	if (game.hand == LAST_HAND-2)							/* In the last 3 hands start hiding from card 3 */
		gtk_widget_hide(table.PLY1_covered[2]);
		
	else if (game.hand == LAST_HAND-1)
		gtk_widget_hide(table.PLY1_covered[1]);
			
	else 
		gtk_widget_hide(table.PLY1_covered[i]);
	
	
 	if (pm->card[pm->slot]->suit != game.briscola) {
 		if (pm->card[pm->slot]->value == 10 || pm->card[pm->slot]->value == 11)
 			game.memo[pm->card[pm->slot]->suit]++;
 	}
 			
 	if (pr->card[i]->suit != game.briscola) {
 		if (pr->card[i]->value == 10 || pr->card[i]->value == 11)
 			game.memo[pr->card[i]->suit]++;
 	}
	
	/* Move card played by PLY1 on the table */
	
	displayCard(pr, 1);
 	
	/* Assign points of the hand */
	assign_points(player);
}

void assign_points (struct player_data *player) {

	unsigned int i;
	struct player_data *pm, *pr, *winner;
	
	printf("Hand: %d\n", game.hand);
	printf("Cards dealt: %d\n", game.cards_dealt);
	printf("Briscola: %d\n", game.briscola);
	
	for (i = 0; i < game.lim; i++)
	 	printf("Card %u: value %d, suit %d\n", i+1, (player+1)->card[i]->value, (player+1)->card[i]->suit);
	 	
	printf("\n\n");
	
	if (game.turn & PLY0) {
		pm = player;
		pr = pm + 1;
	}
	
	else {
		pr = player;
		pm = pr + 1;
	}
	
	printf("Played: value %d, suit %d, Index: %d\n", pm->card[pm->slot]->value, pm->card[pm->slot]->suit, pm->slot);
 	printf("Reply: value %d, suit %d, Index: %d\n", pr->card[pr->slot]->value, pr->card[pr->slot]->suit, pr->slot);
		
	/* Evaluate the winner of the hand */
		
	if (pr->card[pr->slot]->suit == pm->card[pm->slot]->suit) {
		
		if (pr->card[pr->slot]->value == pm->card[pm->slot]->value) {	/* if they are both cards of 0 value */
		
			if (pr->card[pr->slot]->index > pm->card[pm->slot]->index)
				winner = pr;
			else
				winner = pm;
		}
		
		else if (pr->card[pr->slot]->value > pm->card[pm->slot]->value)
			winner = pr;
			
		else
			winner = pm;
	}
					
	else if ((pr->card[pr->slot]->suit == game.briscola) || (pm->card[pm->slot]->suit == game.briscola)) {
		
		if (pr->card[pr->slot]->suit == game.briscola)
			winner = pr;
		else
			winner = pm;
	}
			
	else
		winner = pm;

	/* Assign Points to the winner of the hand*/
		
	winner->total += pm->card[pm->slot]->value + pr->card[pr->slot]->value;
		
	printf("Player who moved is: %u\n", pm->flag);
	printf("Player who replied is: %u\n", pr->flag);
	printf("Total Points PLY0: %d\n", player->total);
	printf("Total Points PLY1: %d\n", (player+1)->total);
	printf("Winner is: %d\n\n", winner->flag);
		
	game.turn = winner->flag;
	
	update_points(winner);
	
	g_timeout_add(1000, (GSourceFunc)clean_table, player);
}

gboolean clean_table (struct player_data *player) {

	if (game.cards_dealt <= CARDS-2) {
		draw_cards(player);
		gtk_widget_show(table.PLY1_covered[(player+1)->slot]);
		
		/* Set the new card for PLY0 */
		gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[player->slot]), player->card[player->slot]->file);
		gtk_widget_show(table.PLY0_image[player->slot]);
		
		if (game.hand == 16) /*when there are only 2 cards left to play pile becomes back */
			gtk_image_set_from_resource (GTK_IMAGE(table.image_deck_pile), "/cards/back.png");
	}

	else { /* Do a card shifting to eliminate cards played in last 3 hands*/
	
		if (player[PLY1].slot == 0) {
			player[PLY1].card[0] = player[PLY1].card[1];
			player[PLY1].card[1] = player[PLY1].card[2];			
		}
		
		if (player[PLY1].slot == 1)
			player[PLY1].card[1] = player[PLY1].card[2];
		
		game.lim--;
	}
	
	/* Hide card played */
	
	gtk_widget_hide(table.played_card[PLY0]);
	gtk_widget_hide(table.played_card[PLY1]);
	
	game.hand++;
	
	if (game.hand == LAST_HAND + 1)
		end_game(player);
		
	else if (game.turn == PLY1)
		move(player+1);
		
	else
		game.status = PLAY;

	return FALSE;
}

void draw_cards (struct player_data *player) {
	
	/* Draw new card from deck */

	if (game.turn & PLY0) {
		player->card[player->slot] = &deck[game.cards_dealt];
		(player+1)->card[(player+1)->slot] = &deck[game.cards_dealt+1];
	}
	
	else {
		(player+1)->card[(player+1)->slot] = &deck[game.cards_dealt];
		player->card[player->slot] = &deck[game.cards_dealt+1];
	}
	
	game.cards_dealt += 2;
	
	update_cards_left();
	
	if (game.cards_dealt == CARDS) {
		gtk_label_set_text (GTK_LABEL(table.lbl_cards_left), "");
		gtk_widget_hide(table.image_briscola);
		gtk_widget_hide(table.image_deck_pile);
	}
}

unsigned int min_max (struct player_data *player, _Bool choice, int m) { 	/* if s == 0 calculate min else max */

	unsigned int i, index;
	int a[3];
	  
	for (i = 0; i < game.lim; i++) {
	
		a[i] = player->card[i]->value;
				
		if (player->card[i]->suit == game.briscola) {
		
			if (player->card[i]->value == 0)
				a[i] += 5;
			else
				a[i] += 4;
		}
		
		if (player->card[i]->suit != game.briscola)
			a[i] += m * game.memo[player->card[i]->suit];
	}
	
	index = 0;
 
	if (choice == MIN) {
 		int min = a[0];
		for (i = 1; i < game.lim; i++) {
			if (a[i] < min) {
			   index = i;
			   min = a[i];
			}
	  	}
	}
	
	else {
		int max = a[0];
		for (i = 1; i < game.lim; i++) {
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
	
	for (i = 0; i < game.lim; i++) {
		if ( (player+1)->card[i]->suit == player->card[player->slot]->suit) {
			if ((player+1)->card[i]->value > player->card[player->slot]->value)
				return i;
			}
	}
		
	return 5;
}

unsigned int findBriscola(struct player_data *player) {

	unsigned int i;
	
	for (i = 0; i < game.lim; i++) {
		if (player->card[i]->suit == game.briscola) 
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
