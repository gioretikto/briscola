#include <stdio.h>
#include <gtk/gtk.h>

#define CARDS 40

enum suit {BASTONI, DENARI, COPPE, SPADE};

struct player_data {

	int total;
	
	struct card *card;	
};

struct card {
   	const char *file;
   	int value;
   	enum suit suit;
};

void create_window();
void destroy (GtkWidget *window, gpointer data);
void get_number_players (GtkWidget *window);
void shuffle (struct card *my_game);
void card1_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player);
void card2_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player);
void card3_clicked (GtkWidget *event_box, GdkEventButton *event, struct player_data *player);
void activate_about();
void init_game(struct player_data *player);
void move_reply(struct player_data *player, int index);
void update_points(struct player_data *player);
void display_cards (struct player_data *player);