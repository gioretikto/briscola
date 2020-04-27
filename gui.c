#include "briscola.h"

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
    table.label_player[0] = gtk_label_new ("0");
    table.label_player[1] = gtk_label_new ("0");
    
   	/* Images for cards played */
   	table.played_card[PLY0] = gtk_image_new ();
	table.played_card[PLY1] = gtk_image_new ();
            	
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
   	
   	table.turn = PLY0;
   	table.status = PLAY;
   	
	init_game(player);
	
    gtk_container_add(GTK_CONTAINER (headbar), about_button);
	gtk_container_add(GTK_CONTAINER (window), vbox);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_dealer);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_table);
	gtk_container_add(GTK_CONTAINER (vbox), hbox_player);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box1);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box2);
	gtk_container_add(GTK_CONTAINER (hbox_player), event_box3);
	gtk_container_add(GTK_CONTAINER (hbox_player), table.label_player[0]);
	gtk_container_add(GTK_CONTAINER (event_box1), table.PLY0_image[0]);
	gtk_container_add(GTK_CONTAINER (event_box2), table.PLY0_image[1]);
	gtk_container_add(GTK_CONTAINER (event_box3), table.PLY0_image[2]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.PLY1_covered[0]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.PLY1_covered[1]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.PLY1_covered[2]);
	gtk_container_add(GTK_CONTAINER (hbox_dealer), table.label_player[1]);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.image_briscola);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.image_deck_pile);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.played_card[0]);
	gtk_container_add(GTK_CONTAINER (hbox_table), table.played_card[1]);
	
	GtkStyleContext *context1;
	context1 = gtk_widget_get_style_context(hbox_player);			/* Apply style to player's box */
	gtk_style_context_add_class(context1, "my_hbox_player");
	
	GtkStyleContext *context2;
	context2 = gtk_widget_get_style_context(hbox_dealer);			/* Apply style to dealer's box */
	gtk_style_context_add_class(context2, "my_hbox_dealer");
	
	GtkStyleContext *context3;
	context3 = gtk_widget_get_style_context(hbox_table);			/* Apply style to biscola's box */
	gtk_style_context_add_class(context3, "my_hbox_table");
	
	gtk_widget_set_name (table.played_card[0], "card1_tavolo");  	/* Apply CSS style to played_card[0] */
	gtk_widget_set_name (table.played_card[1], "card2_tavolo");  	/* Apply CSS style to played_card[0] */
	
	g_signal_connect (about_button, "clicked", G_CALLBACK (activate_about), NULL);
	g_signal_connect (G_OBJECT (event_box1), "button_press_event", G_CALLBACK (card1_clicked), player);
	g_signal_connect (G_OBJECT (event_box2), "button_press_event", G_CALLBACK (card2_clicked), player);
    g_signal_connect (G_OBJECT (event_box3), "button_press_event", G_CALLBACK (card3_clicked), player);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
	
	gtk_widget_show_all (window);

	gtk_widget_hide(table.played_card[PLY0]);
	gtk_widget_hide(table.played_card[PLY1]);
	    
	gtk_main();
}

void card1_clicked (GtkWidget *event_box1, GdkEventButton *event, struct player_data *player) {
		
	if (table.status == PLAY) {
	
		table.status = BLOCK;
		
		player[PLY0].slot = 0;		
		
		/* Display played card on table */
		gtk_image_set_from_file(GTK_IMAGE(table.played_card[PLY0]), player[PLY0].card[0]->file);
		gtk_widget_show(table.played_card[PLY0]);
		
		/* Hide played card */
		gtk_widget_hide(table.PLY0_image[0]);
		
		if (table.turn == PLY0)
			move_reply(player);
		else
			assign_points(player);
	}
}

void card2_clicked (GtkWidget *event_box2, GdkEventButton *event, struct player_data *player) {

	if (table.status == PLAY) {
	
		table.status = BLOCK;

		player[PLY0].slot = 1;

		/* Display played card on table */
		gtk_image_set_from_file(GTK_IMAGE(table.played_card[PLY0]), player[PLY0].card[1]->file);
		gtk_widget_show(table.played_card[PLY0]);
		
		/* Hide played card */
		gtk_widget_hide(table.PLY0_image[1]);
		
		if (table.turn == PLY0)
			move_reply(player);
		else
			assign_points(player);
	}
}

void card3_clicked (GtkWidget *event_box3, GdkEventButton *event, struct player_data *player) {

	if (table.status == PLAY) {
	
		table.status = BLOCK;
	
		player[PLY0].slot = 2;

		/* Display played card on table */
		gtk_image_set_from_file(GTK_IMAGE(table.played_card[PLY0]), player[PLY0].card[2]->file);
		gtk_widget_show(table.played_card[PLY0]);
		
		/* Hide played card */
		gtk_widget_hide(table.PLY0_image[2]);
		
		if (table.turn == PLY0)
			move_reply(player);
			
		else
			assign_points(player);
	}
}

void destroy (GtkWidget *window, gpointer data)
{	
	gtk_main_quit ();
}
