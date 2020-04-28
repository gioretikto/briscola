#include <stdio.h>
#include <gtk/gtk.h>

#define CARDS 40

enum suit {BASTONI, DENARI, COPPE, SPADE};
enum states {BLOCK, PLAY};

enum players {PLY0, PLY1};

struct player_data {

	int total;				/* total points */
	struct card *card[3];
	int slot;				/* card index  */
	
};

struct card {
	int index;
   	const char *file;
   	int value;
   	enum suit suit;
};

struct game {
	GtkWidget *label_player[2];
	GtkWidget *lbl_cards_left;
	GtkWidget *played_card[2];
	GtkWidget *image_briscola, *image_deck_pile;
	GtkWidget *PLY1_covered[3];
	GtkWidget *PLY0_image[3];
	enum states status;
	enum players turn;
	enum players winner;
	enum suit briscola;
	int cards_dealt;
	int hand;
	int memo[4]; /* to memorize  Aces and 3 played */
}table;

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
void update_points(struct player_data *player, int index);
void display_cards (struct player_data *player);
int min_max (struct player_data *player, _Bool s, int m);
void assign_points(struct player_data *player);
void draw_cards(struct player_data *player);
void move(struct player_data *player);
gboolean clean_table (struct player_data *player);
int find_charge(struct player_data *player);
void update_cards_left();
void print_end_msg(struct player_data *player);
