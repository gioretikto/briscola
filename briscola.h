#include <stdio.h>
#include <gtk/gtk.h>

#define CARDS 40

enum suit {BASTONI, DENARI, COPPE, SPADE};
enum states {BLOCK, PLAY};
enum choice {MIN, MAX};

enum players {PLY0, PLY1};

struct player_data {

	int total;				/* total points */
	struct card *card[3];
	unsigned int slot;				/* card index  */
	
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
    GtkWidget *play_button;
	enum states status;
	enum players turn;
	enum players winner;
	enum suit briscola;
	unsigned int cards_dealt;
	unsigned int hand;
	unsigned int lim;
	int memo[4]; 	/* to memorize Aces and 3 played */
}table;

void create_window(void);
void activate_about(void);
void destroy (GtkWidget *window G_GNUC_UNUSED, gpointer data G_GNUC_UNUSED);
void get_number_players (GtkWidget *window);
void shuffle (struct card *xdeck);
void card1_clicked (GtkWidget *event_box G_GNUC_UNUSED, GdkEventButton *event G_GNUC_UNUSED, struct player_data *player);
void card2_clicked (GtkWidget *event_box G_GNUC_UNUSED, GdkEventButton *event G_GNUC_UNUSED, struct player_data *player);
void card3_clicked (GtkWidget *event_box G_GNUC_UNUSED, GdkEventButton *event G_GNUC_UNUSED, struct player_data *player);
void init_game(struct player_data *player);
void ply0_move(unsigned int index, struct player_data *player);
void move_reply(struct player_data *player);
void update_points(struct player_data *player, int index);
void display_first_hand (struct player_data *player);
unsigned int min_max (struct player_data *player, _Bool s, int m);
void assign_points(struct player_data *player);
void draw_cards(struct player_data *player);
void move(struct player_data *player);
gboolean clean_table (struct player_data *player);
unsigned int find_charge(struct player_data *player);
void update_cards_left(void);
void end_game(struct player_data *player);
