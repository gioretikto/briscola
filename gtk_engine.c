#include "briscola.h"
#include <stdio.h>
#include <glib.h>

enum players {PLAYER, DEALER};
extern struct card deck[40];
/*extern int number_players;*/
int briscola;
int cards_dealt;
enum players to_play;

GtkWidget *label_player[2];

GtkWidget *image_table[2];
GtkWidget *image_briscola, *image_deck_pile;
GtkWidget *dealer_covered[3];
GtkWidget *image_player[3];

void create_window() {

	GtkWidget *window;
	GtkWidget *headbar;
    GtkWidget *vbox;
    GtkWidget *hbox_dealer;
	GtkWidget *hbox_table;
    GtkWidget *hbox_player;
    GtkWidget *about_button;
    GtkWidget *event_box1, *event_box2, *event_box3;
        
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	headbar = gtk_header_bar_new();
	about_button = gtk_button_new_with_mnemonic("_About");
   	event_box1 = gtk_event_box_new ();
   	event_box2 = gtk_event_box_new ();
	event_box3 = gtk_event_box_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 30);   
    hbox_dealer = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 35);
    hbox_table = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    hbox_player = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 35);   
    label_player[0] = gtk_label_new ("0");
    label_player[1] = gtk_label_new ("0");
            	
    gtk_header_bar_set_title (GTK_HEADER_BAR (headbar), "Briscola");
    gtk_window_set_title (GTK_WINDOW (window), "Briscola");    
	gtk_window_set_titlebar (GTK_WINDOW (window), headbar);
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headbar), TRUE);
	gtk_window_maximize (GTK_WINDOW (window));
	
	GtkCssProvider *css_provider = gtk_css_provider_new();		/* Apply style */
	gtk_css_provider_load_from_path (css_provider, "style.css", NULL);
  		
	GdkScreen *myScreen = gdk_screen_get_default();
	gtk_style_context_add_provider_for_screen
		    (myScreen,
            GTK_STYLE_PROVIDER (css_provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
            
   	/*get_number_players(window);*/
   	
   	struct player_data player[2];
   	
   	to_play = PLAYER;

	init_game(player);
	
    gtk_container_add(GTK_CONTAINER (headbar), about_button);
	gtk_container_add(GTK_CONTAINER (window), vbox);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_dealer);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_table);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_player);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box1);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box2);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box3);
	gtk_container_add(GTK_CONTAINER (hbox_player), label_player[0]);
	gtk_container_add(GTK_CONTAINER (event_box1), image_player[0]);
	gtk_container_add(GTK_CONTAINER (event_box2), image_player[1]);
	gtk_container_add(GTK_CONTAINER (event_box3), image_player[2]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), dealer_covered[0]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), dealer_covered[1]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), dealer_covered[2]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), label_player[1]);
	gtk_container_add(GTK_CONTAINER (hbox_table), image_briscola);
	gtk_container_add(GTK_CONTAINER (hbox_table), image_deck_pile);
	gtk_container_add(GTK_CONTAINER (hbox_table), image_table[0]);
	gtk_container_add(GTK_CONTAINER (hbox_table), image_table[1]);
	
	GtkStyleContext *context1;
	context1 = gtk_widget_get_style_context(hbox_player);		/* Apply style to player's box */
	gtk_style_context_add_class(context1, "my_hbox_player");
	
	GtkStyleContext *context2;
	context2 = gtk_widget_get_style_context(hbox_dealer);		/* Apply style to dealer's box */
	gtk_style_context_add_class(context2, "my_hbox_dealer");
	
	GtkStyleContext *context3;
	context3 = gtk_widget_get_style_context(hbox_table);		/* Apply style to biscola's box */
	gtk_style_context_add_class(context3, "my_hbox_table");
	
	gtk_widget_set_name (image_table[0], "card1_tavolo");  		/* Apply CSS style to image_table[0] */
	
	g_signal_connect (about_button, "clicked", G_CALLBACK (activate_about), NULL);
	g_signal_connect (G_OBJECT (event_box1), "button_press_event", G_CALLBACK (card1_clicked), player);
	g_signal_connect (G_OBJECT (event_box2), "button_press_event", G_CALLBACK (card2_clicked), player);
    g_signal_connect (G_OBJECT (event_box3), "button_press_event", G_CALLBACK (card3_clicked), player);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
	
	gtk_widget_show_all (window);
	    
	gtk_main();

}

void init_game(struct player_data *player) {

	shuffle(deck);
	
	briscola = deck[39].suit;	/* the 1st card in deck is briscola */
	
	player[0].card[0] = &deck[0];
	player[0].card[1] = &deck[1];
	player[0].card[2] = &deck[2];
	
	player[1].card[0] = &deck[3];
	player[1].card[1] = &deck[4];
	player[1].card[2] = &deck[5];
	
	cards_dealt = 6;
	
	player[0].total = 0;
	player[1].total = 0;
	
	display_cards(player);
}

void display_cards (struct player_data *player) {

	/* Player's cards */
	
	image_player[0] = gtk_image_new_from_file (player[0].card[0]->file);
	image_player[1] = gtk_image_new_from_file (player[0].card[1]->file);
	image_player[2] = gtk_image_new_from_file (player[0].card[2]->file);
	
	/* Image of Dealer's covered cards */
  	
	dealer_covered[0] = gtk_image_new_from_file ("c/back.png");
	dealer_covered[1] = gtk_image_new_from_file ("c/back.png");
	dealer_covered[2] = gtk_image_new_from_file ("c/back.png");
		
	/* Images for cards played */
	image_table[0] = gtk_image_new ();
	image_table[1] = gtk_image_new ();
  		
	/* Images for Briscola and deck */
	  	
  	image_briscola = gtk_image_new_from_file (deck[39].file);
	
  	image_deck_pile = gtk_image_new_from_file ("c/deck.png");
}

void card1_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player)
{
	if (to_play == PLAYER) {

		/* Display played card on table */
		
		player[0].index = 0;
		
		gtk_image_set_from_file(GTK_IMAGE(image_table[0]), player[0].card[0]->file);
		gtk_widget_hide(image_player[0]);
		
		move_reply(player);
	}
}

void card2_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player)
{
	if (to_play == PLAYER) {

		/* Display played card on table */
		
		player[0].index = 1;
		
		gtk_image_set_from_file(GTK_IMAGE(image_table[0]), player[0].card[1]->file);
		gtk_widget_hide(image_player[1]);
		
		move_reply(player);
	}
}

void card3_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player)
{
	if (to_play == PLAYER) {
	
		player[0].index = 2;

		/* Display played card on table */
		
		gtk_image_set_from_file(GTK_IMAGE(image_table[0]), player[0].card[2]->file);
		gtk_widget_hide(image_player[2]);
		
		move_reply(player);
	}
}

void move_reply(struct player_data *player) {

	int i, tmp;
	int index = player[0].index;
	
	for (i = 0; i < 3; i++)
	 	printf("value %d , suit %d\n", player->card[i]->value, player->card[i]->suit);
	
	i = 0;
	
	if (player[0].card[index]->value == 0)		/* if it is a scartina reply with another scartina */
		i = min_max(&player[1], 0);		
	
	/* if it is an ace or three or 10*/
	else if ( (player[0].card[index]->value == 11) || (player[0].card[index]->value == 10) || (player[0].card[index]->value == 4) ) {	
		
		if (player[0].card[index]->suit != briscola) { /* try to take with a card of min value 0, else with another card */
			
			tmp = i = min_max(&player[1], 0); 
				
			if (player[1].card[i]->value < player[0].card[index]->value) { /* if the min value cannot overtake*/
				i = min_max(&player[1], 1);   /* try with max */
				
				if (player[1].card[i]->value > player[0].card[index]->value)	   /* Try to see if the max value can do it */
					;
				else 			/* if not even the max can take use the min card */
					i = tmp;
			}
		}
		
		else {
			i = min_max(&player[1], 1); 		/* Try to see if the max value can overtake ace or 3*/
				
			if (player[1].card[i]->value < player[0].card[index]->value)
				i = min_max(&player[1], 0); 	/* If not Play the card with lower value */
		}
	}
	
	else
		i = 0;

 	printf("Played \n value %d , suit %d\n", player[1].card[i]->value, player[1].card[i]->suit);
	player[1].index = i;
	gtk_widget_hide(dealer_covered[i]);
	gtk_image_set_from_file(GTK_IMAGE(image_table[1]), player[1].card[i]->file);
	g_timeout_add(3000, (GSourceFunc)assign_points, player);

}

gboolean assign_points(struct player_data *player) {

	/*Assing new cards */		

	int index0 = player[0].index;
	int index1 = player[1].index;
	
	/* Assign Points */	
	
	if ( (player[1].card[index1]->suit != briscola) && (player[1].card[index1]->suit != player[0].card[index0]->suit) ) {
		player[0].total = player[0].card[index0]->value + player[1].card[index1]->value;
		to_play = PLAYER;
	}
	
	else if ( (player[0].card[index0]->value > player[1].card[index1]->value) && (player[1].card[index1]->suit == player[0].card[index0]->suit)  ) {
		player[0].total = player[0].card[index0]->value + player[1].card[index1]->value;
		to_play = PLAYER;
	}
	
	else if ( (player[1].card[index1]->suit == briscola) && (player[0].card[index0]->suit != briscola) ) {
		player[1].total = player[0].card[index0]->value + player[1].card[index1]->value;
		to_play = DEALER;
	}

	else
		;
		
	update_points(player);
	
	draw_cards(player);
	
	printf("NEw File %s\n", player[0].card[index0]->file);
	printf("Index is %d\n", index0);
	gtk_image_set_from_file(GTK_IMAGE(image_player[0]), player[0].card[index0]->file);
		
	gtk_widget_show(image_player[index0]);
	gtk_widget_show(dealer_covered[index1]);
	
	gtk_widget_hide(image_table[0]);
	gtk_widget_hide(image_table[1]);
	
	cards_dealt += 2;	

	return FALSE;
}

void draw_cards (struct player_data *player) {

	int index0 = player[0].index;
	int index1 = player[1].index;
	
	/* Draw new card from deck */

	if (to_play == PLAYER) {
		printf("All right1\n");
		player[0].card[index0] = &deck[cards_dealt];
		player[1].card[index1] = &deck[cards_dealt+1];
	}
	
	else {
		printf("All right2\n");						
		player[0].card[index0] = &deck[cards_dealt+1];
		player[1].card[index1] = &deck[cards_dealt];
	}
}

int min_max (struct player_data *player, _Bool s) { 	/* if s == 0 calculate min else max */

	int i, index;
	int a[3];
	  
	for (i = 0; i < 3; i++) {
	
		a[i] = player->card[i]->value;
				
		if (player->card[i]->suit == briscola)
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

void update_points(struct player_data *player)
{
	char *display;
    display = g_strdup_printf("%d", player[0].total);					/* convert num to str */
    gtk_label_set_text (GTK_LABEL(label_player[0]), display);			/* set label to "display */
    
    g_free(display);
    
    display = g_strdup_printf("%d", player[1].total);					/* convert num to str */
    gtk_label_set_text (GTK_LABEL(label_player[1]), display);	
        
    g_free(display);		                    						/* free display */
}

void destroy (GtkWidget *window, gpointer data)
{	
	gtk_main_quit ();
}
