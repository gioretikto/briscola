#include <stdio.h>
#include <gtk/gtk.h>

#define CARDS 40

enum suit {BASTONI, DENARI, COPPE, SPADE};

struct player_data {

	int total;
	
	struct card *card[3];
	
	int index;	/*index for card played */
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
void move_reply(struct player_data *player);
void update_points(struct player_data *player);
void display_cards (struct player_data *player);
int min_max (struct player_data *player, _Bool s);
gboolean assign_points(struct player_data *player);
void draw_cards(struct player_data *player);
