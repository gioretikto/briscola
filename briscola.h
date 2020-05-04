#include <stdio.h>
#include <gtk/gtk.h>

#define CARDS 40

enum suit {BASTONI, DENARI, COPPE, SPADE};
enum states {BLOCK, PLAY, LAST_HAND=20};
enum choice {SUB_ACES =-1, MIN, MAX, ADD_ACES=1};

enum players {PLY0, PLY1};

struct player_data {
	int total;				/* total points */
	struct card *card[3];
	unsigned int slot;		/* card index  */
	unsigned int flag;
};

struct card {
	int index;
   	const char *file;
   	int value;
   	enum suit suit;
};

struct glob_briscola {
	GtkWidget *headbar;
	GtkWidget *lbl_points_player[2];
	GtkWidget *lbl_msg;
	GtkWidget *lbl_cards_left;
	GtkWidget *btn_play;
	GtkWidget *played_card[2];
	GtkWidget *image_briscola, *image_deck_pile;
	GtkWidget *PLY1_covered[3];
	GtkWidget *PLY0_image[3];
}table;

struct glob_struct{
	enum states status;
	enum players turn;
	enum players next_player;
	enum suit briscola;
	unsigned int cards_dealt;
	unsigned int hand;
	unsigned int lim;
	unsigned int match_won[2];
	int memo[4]; 	/* to memorize Aces and 3 played */
}game;

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
void update_points(struct player_data *player);
unsigned int min_max (struct player_data *player, _Bool s, int unit);
void assign_points(struct player_data *player);
void draw_cards(struct player_data *player);
void move(struct player_data *cpu);
gboolean clean_table (struct player_data *player);
unsigned int findCharge(struct player_data *player);
void update_cards_left(void);
void end_game(struct player_data *player);
void start (GtkWidget *widget G_GNUC_UNUSED, struct player_data *player);
unsigned int findBriscola(struct player_data *player);
unsigned int verifyCombo (struct player_data *player);
void displayCard(struct player_data *player, int index);
