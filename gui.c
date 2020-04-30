#include "briscola.h"

void create_window() {

	GtkWidget *window;
	GtkWidget *headbar;
    GtkWidget *vbox;
    GtkWidget *hbox_dealer;
	GtkWidget *hbox_table;
    GtkWidget *hbox_player;
    GtkWidget *about_button;
    GtkWidget *event_box[3];
        
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	headbar = gtk_header_bar_new();
	about_button = gtk_button_new_with_label("About");
	table.play_button = gtk_button_new_with_label("Play Again");
   	event_box[0] = gtk_event_box_new ();
   	event_box[1] = gtk_event_box_new ();
	event_box[2] = gtk_event_box_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    hbox_dealer = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 35);
    hbox_table = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    hbox_player = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 35);
    table.label_player[PLY0] = gtk_label_new ("0");
    table.label_player[PLY1] = gtk_label_new ("0");
    table.lbl_cards_left = gtk_label_new ("");
        
   	/* Images for cards played */
   	table.played_card[0] = gtk_image_new ();
   	table.played_card[1] = gtk_image_new ();

	table.image_deck_pile = gtk_image_new_from_file ("c/deck.png");
	
	/* Image of Dealer's covered cards */
	table.PLY1_covered[0] = gtk_image_new_from_file ("c/back.png");
	table.PLY1_covered[1] = gtk_image_new_from_file ("c/back.png");
	table.PLY1_covered[2] = gtk_image_new_from_file ("c/back.png");
	
	table.image_briscola = gtk_image_new ();
	
	table.PLY0_image[0] = gtk_image_new();
	table.PLY0_image[1] = gtk_image_new();
	table.PLY0_image[2] = gtk_image_new();
            	
    gtk_header_bar_set_title (GTK_HEADER_BAR (headbar), "Briscola");
    gtk_window_set_title (GTK_WINDOW (window), "Briscola");    
	gtk_window_set_titlebar (GTK_WINDOW (window), headbar);
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headbar), TRUE);
	gtk_window_maximize (GTK_WINDOW (window));
	
	GtkCssProvider *css_provider = gtk_css_provider_new();		/* Apply style */
	gtk_css_provider_load_from_path (css_provider, "style.css", NULL);
  		
	GdkScreen *myScreen = gdk_screen_get_default();
	gtk_style_context_add_provider_for_screen (myScreen, GTK_STYLE_PROVIDER (css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
            
   	struct player_data player[2];
   	
   	table.turn = PLY0;
   	table.status = PLAY;
	
    gtk_container_add(GTK_CONTAINER (headbar), about_button);
	gtk_container_add(GTK_CONTAINER (window), vbox);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_dealer);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_table);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_player);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.PLY1_covered[0]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.PLY1_covered[1]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.PLY1_covered[2]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.label_player[PLY1]);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.image_briscola);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.image_deck_pile);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.lbl_cards_left);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.play_button);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.played_card[0]);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.played_card[1]);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box[0]);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box[1]);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box[2]);
	gtk_container_add(GTK_CONTAINER (event_box[0]), table.PLY0_image[0]);
	gtk_container_add(GTK_CONTAINER (event_box[1]), table.PLY0_image[1]);
	gtk_container_add(GTK_CONTAINER (event_box[2]), table.PLY0_image[2]);
	gtk_container_add(GTK_CONTAINER (hbox_player), table.label_player[PLY0]);
		
	GtkStyleContext *context1;										/* Apply style to player's box */
	context1 = gtk_widget_get_style_context(hbox_player);			
	gtk_style_context_add_class(context1, "my_hbox_player");
	
	GtkStyleContext *context2;
	context2 = gtk_widget_get_style_context(hbox_dealer);			/* Apply style to dealer's box */
	gtk_style_context_add_class(context2, "my_hbox_dealer");
	
	GtkStyleContext *context3;
	context3 = gtk_widget_get_style_context(hbox_table);			/* Apply style to card displayed on tables */
	gtk_style_context_add_class(context3, "my_hbox_table");

	gtk_widget_set_name (table.played_card[0], "card_played1");
	
	g_signal_connect (about_button, "clicked", G_CALLBACK (activate_about), NULL);
	g_signal_connect (G_OBJECT (event_box[0]), "button_press_event", G_CALLBACK (card1_clicked), player);
	g_signal_connect (G_OBJECT (event_box[1]), "button_press_event", G_CALLBACK (card2_clicked), player);
    g_signal_connect (G_OBJECT (event_box[2]), "button_press_event", G_CALLBACK (card3_clicked), player);
    g_signal_connect (G_OBJECT (table.play_button), "button_press_event", G_CALLBACK (init_game), player);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
	
	gtk_widget_show_all (window);
	
	gtk_widget_hide(table.played_card[PLY0]);
	gtk_widget_hide(table.played_card[PLY1]);
	
	/* initialize first hand */
	  	
	init_game(player);

	gtk_main();
}

void card1_clicked (GtkWidget *event_box1 G_GNUC_UNUSED, GdkEventButton *event G_GNUC_UNUSED, struct player_data *player) {
		
	if (table.status == PLAY)
		ply0_move(0, player);
}

void card2_clicked (GtkWidget *event_box2 G_GNUC_UNUSED, GdkEventButton *event G_GNUC_UNUSED, struct player_data *player) {
 
	if (table.status == PLAY)
		ply0_move(1, player);
}

void card3_clicked (GtkWidget *event_box3 G_GNUC_UNUSED, GdkEventButton *event G_GNUC_UNUSED, struct player_data *player) {

	if (table.status == PLAY)
		ply0_move(2, player);
}

void update_points(struct player_data *player, int index) {

	char *display;
    display = g_strdup_printf("%d", player->total);							/* convert num to str */
    gtk_label_set_text (GTK_LABEL(table.label_player[index]), display);		/* set label to "display */
    
    g_free(display);
}

void update_cards_left() {

   	char *display;
    display = g_strdup_printf("%d", CARDS - table.cards_dealt);
    gtk_label_set_text (GTK_LABEL(table.lbl_cards_left), display);		/* set label to "display */
	g_free(display);
}

void end_game(struct player_data *player) {

	gtk_widget_show(table.play_button);
	
	char *display;
	
	if (player[PLY0].total > player[PLY1].total)
	    display = g_strdup_printf("Congratulation you win %d to %d", player[PLY0].total, player[PLY1].total);
	    
	else if (player[PLY0].total == player[PLY1].total)
		display = g_strdup_printf("The game is a tie %d to %d", player[PLY0].total, player[PLY1].total);
		
	else
		display = g_strdup_printf("You loose %d to %d", player[PLY0].total, player[PLY1].total);
		
    gtk_label_set_text(GTK_LABEL(table.lbl_cards_left), display);
    
    g_free(display);
}

void activate_about(void) {

	const gchar *authors[] = {"Giovanni Resta", "giovannirestadev@gmail.com", NULL};
	
	gtk_show_about_dialog (NULL,
                       "program-name", "Briscola",
                       "version", "0.0.1",
    				   "license-type", GTK_LICENSE_GPL_3_0,
    				   "website", "https://github.com/gioretikto/briscola",
					   "authors", authors,
    				   "logo-icon-name", "start-here",
    				   "comments", "reach me on #cansi Freenode IRC",
                       "title", ("Briscola"),
                       NULL);                   

}

void destroy (GtkWidget *window G_GNUC_UNUSED, gpointer data G_GNUC_UNUSED)
{	
	gtk_main_quit ();
}
