#include <gtk/gtk.h>

int number_players;

void get_number_players(GtkWidget *window) {
	GtkWidget *dialog;
	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  
  	dialog = gtk_dialog_new_with_buttons ("My dialog",
                                     GTK_WINDOW(window),
                                      flags,
                                      ("Two Players"),
                                      2,
                                      ("Three Players"),
                                      3,
                                      ("Four Players"),
                                      4,                                      
                                      NULL);

	number_players = gtk_dialog_run (GTK_DIALOG (dialog));
	
	gtk_widget_destroy (dialog);
}   	

void activate_about() {

	const gchar *authors[] = {"Giovanni Resta", NULL};

	gtk_show_about_dialog (NULL,
                       "program-name", "Briscola",
                       "version", "0.0.1",
    				   "license-type", GTK_LICENSE_GPL_3_0,
    				   "website", "https://github.com/gioretikto/briscola",
					   "authors", authors,
    				   "logo-icon-name", "start-here",
    				   "comments", "reach me on #cansi Freenode IRC",
                       "title", ("BlackJack"),
                       NULL);
}