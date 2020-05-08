#include "briscola.h"
#include <stdio.h>
#include <string.h>
#include <glib.h>

#define FAIL 5

extern struct card deck[CARDS];

void init_game(struct player_data *player) {

	game.hand = 1;
	game.lim = 3;
	player[PLY0].total = 0;
	player[PLY1].total = 0;

	shuffle(deck);
	
	game.briscola = deck[CARDS-1].suit;
	
	/* Deal first 6 cards */
	
	/* for player */
	
	dealCards(&player[PLY0], 0);
	
	/* for CPU */
	
	dealCards(&player[PLY1], 3);
	
	game.cards_dealt = 6;
	
	sortCards(&player[PLY1], SUB_ACES);
	
	unsigned int j;
	
	for (j = 0; j < game.lim; j++)
	 	printf("Card %u: value %d, suit %d\n", j+1, player[PLY1].card[j]->value, player[PLY1].card[j]->suit);
	 	
	printf("\n\n");
	
	gtk_image_set_from_resource (GTK_IMAGE(table.image_briscola), deck[CARDS-1].file);
	
	/* Set image for deck right to the briscola */
	gtk_image_set_from_resource (GTK_IMAGE(table.image_deck_pile), "/cards/deck.png");
	
	memset(game.memo, 0, sizeof(game.memo));
	
	update_cards_left();
	
	if (game.next_player == PLY0) {
		game.turn = PLY0;
		game.status = PLAY;
		game.next_player = PLY1;
	}
		
	else {
		game.status = BLOCK;
		game.turn = PLY1;
		game.next_player = PLY0;
		move(player);
	}
}

void ply0_move(unsigned int index, struct player_data *player) {

	game.status = BLOCK;
		
	player->box = index;
		
	/* Hide played card */
	gtk_widget_hide(table.PLY0_image[index]);
	
	if (game.turn == PLY0)
		move_reply(player);
			
	else {
		displayPlayedCard(player, 1);
		assign_points(player);
	}
}

void move(struct player_data *player) {

	unsigned int i;
	unsigned int j;
	
	sortCards(&player[PLY1], SUB_ACES);
	
	if (game.hand == LAST_HAND - 4)
		i = 0;
		
	else if ((i = verifyCombo(player)) != FAIL)
		;
		
	else
		i = 0;
	
	player[PLY1].box = i;
	
	/* Hide card Played by PLY1 */
	
  	if (game.hand == LAST_HAND-2)							/* In the last 3 hands start hiding from card 3 */
		gtk_widget_hide(table.PLY1_covered[2]);
		
	else if (game.hand == LAST_HAND-1)
		gtk_widget_hide(table.PLY1_covered[1]);
			
	else
		gtk_widget_hide(table.PLY1_covered[i]);
	
	displayPlayedCard(&player[PLY1], 0);

	game.status = PLAY;
	
	for (j = 0; j < game.lim; j++)
	 	printf("Card %u: value %d, suit %d\n", j+1, player[PLY1].card[j]->value, player[PLY1].card[j]->suit);
	 	
	printf("\n\n");
}

void move_reply(struct player_data *player) {

	struct player_data *pm, *pr; 	/* pm: player who moved, pr: player who replied*/
	
	pm = player;
	pr = player + 1;

	displayPlayedCard(pm, 0);

	unsigned int i;
	unsigned int j;
	
	sortCards(pr, ADD_ACES);
	
	i = 0;
	
	for (j = 0; j < game.lim; j++)
	 	printf("Card %u: value %d, suit %d\n", j+1, pr->card[j]->value, pr->card[j]->suit);
	 	
	printf("\n\n");
	
	/* if it is an ace or three or 10 */
	if (pm->card[pm->box]->value >= 10) {
	
		if (pm->card[pm->box]->value == 11)	/* There is no way to reply to this card*/
			;
		
		else if ((i = catchWithNoBriscola(player)) != FAIL)
			;			
				
		else if ((i = catchWithBriscola(player)) != FAIL)
			;
		
		else
			i = 0;

	}
	
	else if ((pm->card[pm->box]->value != 0) ) {
	
		if ((i = catchWithNoBriscola(player)) != FAIL)
			;
			
		else if ((i = verifyCombo(pr)) != FAIL)
			;
			
		else if ((game.memo[game.briscola] == 2) && (pr->card[0]->value + pr->card[1]->value + pr->card[2]->value >= 16)) {
		
			if ((i = catchWithBriscola(player)) != FAIL)
				;
			else
			  i = 0;
		}
		
		else
			i = 0;
	}
	
	else {				/* card played has 0 value */
		if ((i = catchWithNoBriscola(player)) != FAIL)
			;
		else
			i = 0;
	}
	
	if (game.hand == LAST_HAND -4) {
		
		if (pm->card[pm->box]->value + pr->card[i]->value <= 8)
			i = 0;
	}		
	
	pr->box = i;
	
	/* Hide card Played by PLY1 */
	
  	if (game.hand == LAST_HAND-2)							/* In the last 3 hands start hiding from card 3 */
		gtk_widget_hide(table.PLY1_covered[2]);
		
	else if (game.hand == LAST_HAND-1)
		gtk_widget_hide(table.PLY1_covered[1]);
			
	else 
		gtk_widget_hide(table.PLY1_covered[i]);
	
	
 	if (pm->card[pm->box]->suit != game.briscola) {
 		if (pm->card[pm->box]->value >= 10)
 			game.memo[pm->card[pm->box]->suit]++;
 	}
 			
 	if (pr->card[i]->suit != game.briscola) {
 		if (pr->card[i]->value >= 10)
 			game.memo[pr->card[i]->suit]++;
 	}
	
	/* Move card played by PLY1 on the table */
	
	displayPlayedCard(pr, 1);
 	
	/* Assign points of the hand */
	assign_points(player);
}

void assign_points (struct player_data *player) {

	struct player_data *pm, *pr, *winner;
	
	printf("Hand: %d\n", game.hand);
	printf("Cards dealt: %d\n", game.cards_dealt);
	printf("Briscola: %d\n", game.briscola);
	
	if (game.turn == PLY0) {
		pm = player;
		pr = pm + 1;
	}
	
	else {
		pr = player;
		pm = pr + 1;
	}
	
	printf("Played: value %d, suit %d, Index: %d\n", pm->card[pm->box]->value, pm->card[pm->box]->suit, pm->box);
 	printf("Reply: value %d, suit %d, Index: %d\n", pr->card[pr->box]->value, pr->card[pr->box]->suit, pr->box);
		
	/* Evaluate the winner of the hand */
		
	if (pr->card[pr->box]->suit == pm->card[pm->box]->suit) {
		
		if (pr->card[pr->box]->value == pm->card[pm->box]->value) {	/* if they are both cards of 0 value */
		
			if (pr->card[pr->box]->index > pm->card[pm->box]->index)
				winner = pr;
			else
				winner = pm;
		}
		
		else if (pr->card[pr->box]->value > pm->card[pm->box]->value)
			winner = pr;
			
		else
			winner = pm;
	}
					
	else if ((pr->card[pr->box]->suit == game.briscola) || (pm->card[pm->box]->suit == game.briscola)) {
		
		if (pr->card[pr->box]->suit == game.briscola)
			winner = pr;
		else
			winner = pm;
	}
			
	else
		winner = pm;

	/* Assign Points to the winner of the hand*/
		
	winner->total += pm->card[pm->box]->value + pr->card[pr->box]->value;
		
	printf("Player who moved is: %u\n", pm->flag);
	printf("Player who replied is: %u\n", pr->flag);
	printf("Total Points PLY0: %d\n", player->total);
	printf("Total Points PLY1: %d\n", (player+1)->total);
	printf("Winner is: %d\n\n", winner->flag);
	printf("********************************\n");
		
	game.turn = winner->flag;
	
	game.hand++;
	
	update_points(winner);
	
	g_timeout_add(1000, (GSourceFunc)clean_table, player);
}

gboolean clean_table (struct player_data *player) {

	if (game.cards_dealt <= CARDS-2) {
	
		draw_cards(player);
		
		if (game.hand == LAST_HAND - 3) /* when there are only 2 cards left to play pile becomes back */
			gtk_image_set_from_resource (GTK_IMAGE(table.image_deck_pile), "/cards/back.png");
			
	}

	else { /* Do a card shifting to eliminate cards played in last 3 hands*/
	
		if (player[PLY1].box == 0) {
			player[PLY1].card[0] = player[PLY1].card[1];
			player[PLY1].card[1] = player[PLY1].card[2];		
		}
		
		if (player[PLY1].box == 1)
			player[PLY1].card[1] = player[PLY1].card[2];
		
		game.lim--;
	}
	
	/* Hide card played */
	
	gtk_widget_hide(table.played_card[PLY0]);
	gtk_widget_hide(table.played_card[PLY1]);
	
	if (game.hand == LAST_HAND + 1)
		end_game(player);
		
	else if (game.turn == PLY1)
		move(player);
		
	else
		game.status = PLAY;

	return FALSE;
}

void draw_cards (struct player_data *player) {
	
	/* Draw new card from deck */

	if (game.turn == PLY0) {
		dealCards(&player[PLY0], game.cards_dealt);
		dealCards(&player[PLY1], game.cards_dealt+1);
	}
	
	else {
		dealCards(&player[PLY1], game.cards_dealt);
		dealCards(&player[PLY0], game.cards_dealt+1);
	}
	
	game.cards_dealt += 2;
	
	update_cards_left();
	
	if (game.cards_dealt == CARDS) {
		gtk_label_set_text (GTK_LABEL(table.lbl_cards_left), "");
		gtk_widget_hide(table.image_briscola);
		gtk_widget_hide(table.image_deck_pile);
	}
}

void sortCards(struct player_data *cpu, int unit) {

	unsigned int i;
	
	int a[3];
	
	game.memo[game.briscola] = 0;
	
	/* Assign card values */
	
	for (i = 0; i < game.lim; i++) {
	
		a[i] = cpu->card[i]->value;
				
		if (cpu->card[i]->suit == game.briscola) {
		
			game.memo[game.briscola]++;
		
			if (cpu->card[i]->value == 0)
				a[i] += 5;
			else
				a[i] += 4;
		}
		
		if (cpu->card[i]->suit != game.briscola)
			a[i] += unit * game.memo[cpu->card[i]->suit];			
	}
	
	/* Find the minimum element */
	
	unsigned int min = 0;
	
	for (i = 1; i < game.lim; i++) {
	
		if (a[i] < a[min]) {
		   swap(cpu->card[i], cpu->card[min]);
		   min = i;
		}
  	}
}

unsigned int catchWithNoBriscola(struct player_data *player) {

	unsigned int i;
	
	for (i = game.lim -1; i > 0; i--) {
	
		if (player[PLY1].card[i]->suit == game.briscola)
			continue;
			
		else {
		
			if (player[PLY1].card[i]->suit == player[PLY0].card[player[PLY0].box]->suit) {
		
				if (player[PLY1].card[i]->value > player[PLY0].card[player[PLY0].box]->value)
					return i;
				else
					break;
			}
		}
	}
		
	return FAIL;
}

unsigned int catchWithBriscola(struct player_data *player) {

	unsigned int i;
	
	for (i = 0; i < game.lim; i++) {
	
		if (player[PLY1].card[i]->suit != game.briscola)
			continue;
		
		else {
		
			if (player[PLY0].card[player[PLY0].box]->suit == game.briscola) {
			
				if (player[PLY1].card[i]->value > player[PLY0].card[player[PLY0].box]->value)
						return i;
			}

		}
	}
		
	return FAIL;
}

unsigned int verifyCombo (struct player_data *player) {	/*Try to see if PLY0 has all aces and 3s */
	
	unsigned int i;
	
	if ((player[PLY1].card[0]->value + player[PLY1].card[1]->value + player[PLY1].card[2]->value) >= 30) {
	
		for (i = game.lim -1; i > 0; i--) {
		
			if ((player[PLY1].card[i]->suit == game.briscola) && (player[PLY1].card[i]->value > player[PLY0].card[player[PLY0].box]->value))
				return i;
			else
				return FAIL;
		}
	}
		
	return FAIL;
}

void dealCards (struct player_data *player, unsigned int card_index) {

	unsigned int i;

	if (game.hand == 1) {
	
		for (i = 0; i < game.lim; i++) {
	
			player->card[i] = &deck[card_index];
	
			if (player->flag == PLY0) {
				/* Set the new card for PLY0 */
				gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[i]), player->card[i]->file);
				gtk_widget_show(table.PLY0_image[i]);
			}
			
			else
				gtk_widget_show(table.PLY1_covered[i]);
				
			card_index++;
					
		}
	}
		
	else {
	
		player->card[player->box] = &deck[card_index];
				
		if (player->flag == PLY0) {
			gtk_image_set_from_resource (GTK_IMAGE(table.PLY0_image[player->box]), player->card[player->box]->file);
			gtk_widget_show(table.PLY0_image[player->box]);
		}
			
		else
			gtk_widget_show(table.PLY1_covered[player->box]);
	}
}
