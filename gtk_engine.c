#include "briscola.h"

enum players {PLAYER, DEALER};
extern int number_players;
int hand;
_Bool status;
int briscola;
enum players who_moved;

GtkWidget *label_player[2];

GtkWidget *image_table[2];
GtkWidget *image_briscola, *image_deck_pile;
GtkWidget *dealer_covered[3];
GtkWidget *image_player[3];

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
            
   	get_number_players(window);
   	
   	struct player_data player[number_players];

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
	
	briscola = deck[0].suit;	/* the 1st card in deck is briscola */
	
	player[0].card = &deck[2];
	player[1].card = &deck[21];	
	
	who_moved = PLAYER;
	hand = 0;
	player[0].total = 0;
	player[1].total = 0;
	
	display_cards(player);
}

void display_cards (struct player_data *player) {

	/* Player's cards */
	
	image_player[0] = gtk_image_new_from_file (player[0].card[hand].file);
	image_player[1] = gtk_image_new_from_file (player[0].card[hand+1].file);
	image_player[2] = gtk_image_new_from_file (player[0].card[hand+2].file);	
	
  	/* Image of Dealer's covered cards */
  	
	dealer_covered[0] = gtk_image_new_from_file ("c/back.png");
	dealer_covered[1] = gtk_image_new_from_file ("c/back.png");
	dealer_covered[2] = gtk_image_new_from_file ("c/back.png");
		
	/* Images for cards played */
	image_table[0] = gtk_image_new ();
	image_table[1] = gtk_image_new ();
  		
	/* Deck with Briscola */
	  	
  	image_briscola = gtk_image_new_from_file (deck[0].file);
	
  	image_deck_pile = gtk_image_new_from_file ("c/deck.png");
}

void destroy (GtkWidget *window, gpointer data)
{	
	gtk_main_quit ();
}

void card1_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player)
{
	status = 1;

	/* Display played card on table */
	
	gtk_image_set_from_file(GTK_IMAGE(image_table[0]), player[0].card[hand].file);
	gtk_widget_hide(image_player[0]);
	
	who_moved = PLAYER;
	
	move_reply(player, 0);
}

void card2_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player)
{
	status = 1;

	/* Display played card on table */
	
	gtk_image_set_from_file(GTK_IMAGE(image_table[0]), player[0].card[hand+1].file);
	gtk_widget_hide(image_player[1]);
	who_moved = PLAYER;
	
	move_reply(player, 1);
}

void card3_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player)
{
	status = 1;

	/* Display played card on table */
	
	gtk_image_set_from_file(GTK_IMAGE(image_table[0]), player[0].card[hand+2].file);
	gtk_widget_hide(image_player[2]);
	
	who_moved = PLAYER;
	move_reply(player, 2);
}

void move_reply(struct player_data *player, int index) {

	int i;
	
	if (player[who_moved].card[hand+index].value == 0) {		/* if it is a scartina reply with another scartina */
	
		for (i = 0; i < 3; i++) {
		
			if ( (player[!who_moved].card[hand+i].value == 0) && (player[!who_moved].card[hand+i].suit != briscola) )
				break;
		}
	}
	
	else if ( (player[who_moved].card[hand+index].value == 11) || (player[who_moved].card[hand+index].value == 10) ) {	/*if it is an ace or three*/
	
		for (i = 0; i < 3; i++) { /* take with a briscola of value 0 */
			if (player[!who_moved].card[hand+i].value == 0 && player[!who_moved].card[hand+i].suit == briscola)
				break;
		}
	}
	
	else
		i = 0;	

	/* Assign Points */
	
	if ( (player[!who_moved].card[hand+i].suit != briscola) && (player[!who_moved].card[hand+i].suit != player[who_moved].card[hand+index].suit) )  {
		player[who_moved].total = player[who_moved].card[hand+index].value + player[!who_moved].card[hand+i].value;		
	}
	
	else if ( (player[!who_moved].card[hand+i].suit == briscola) && (player[who_moved].card[hand+index].suit != briscola) ) {
		player[!who_moved].total = player[who_moved].card[hand+index].value + player[!who_moved].card[hand+i].value;
	}	
	
	else if ( (player[who_moved].card[hand+index].value > player[!who_moved].card[hand+i].value) && (player[!who_moved].card[hand+i].suit == player[who_moved].card[hand+index].suit)  ) {
		player[who_moved].total = player[who_moved].card[hand+index].value + player[!who_moved].card[hand+i].value;
	}
	
	else
		;	

	update_points(player);
	gtk_widget_hide(dealer_covered[i]);
	gtk_image_set_from_file(GTK_IMAGE(image_table[!who_moved]), player[!who_moved].card[hand+i].file);	

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