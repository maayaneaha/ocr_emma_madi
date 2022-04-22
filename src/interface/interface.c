#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *about_button;
GtkWidget *grid_window;
GtkWidget *RM_label;

int main(int argc, char *argv[])
{
	gtk_init(&argc ,&argv);

	builder = gtk_builder_new_from_file("interface.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "main window"));

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	grid_window = GTK_WIDGET(gtk_builder_get_object(builder, "grid"));

	about_button = GTK_WIDGET(gtk_builder_get_object(builder, "about button"));

	RM_label = GTK_WIDGET(gtk_builder_get_object(builder, "open README label"));

	gtk_widget_show(window);

	gtk_main();

	return EXIT_SUCCESS;
}

void on_about_button_clicked(GtkButton *b)
{
	//TODO
}
