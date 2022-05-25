#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct GUI
{
	GtkWindow* interface;
	GtkStack* window_pages;
	GtkBox* load;
	GtkBox* solver;
	GtkButton* QuitButton;
	GtkButton* StartButton;
	gchar* filename;
	GtkImage* image_holder;
	GtkBox* help_menu;
	GtkButton* help_button;
	GtkButton* refresh_button;
	GtkButton* menu_button;
	GtkButton* resolve_button;
	GdkScreen *screen;
}GUI;


void on_quit_clicked()
{
	gtk_main_quit();
}


void display_image(GtkImage* image_holder, gchar* file_name)
{
	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(file_name, NULL);
	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(image_holder, pixbuf);
	//gtk_image_set_from_file(image_holder, file_name);
}

void display_result(GtkImage* image_holder, gchar* result_file)
{
	display_image(image_holder, result_file);
}

void open_files_explorer(GtkButton* b, gpointer user)
{
	GUI* gui = user;
	GtkWidget *p_dialog = NULL;
	p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"document-cancel", GTK_RESPONSE_CANCEL,
			"document-open", GTK_RESPONSE_ACCEPT,
			NULL);

	gchar *file_name = NULL;
	if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
	{

		file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
	}
	gtk_widget_destroy (p_dialog);
	gui->filename = file_name;
	gtk_widget_show(GTK_WIDGET(gui->solver));
	gtk_widget_hide(GTK_WIDGET(gui->load));
	display_image(gui->image_holder, file_name);
}

void refresh(GtkButton* b, gpointer user)
{
	GUI* gui = user;
	display_image(gui->image_holder, gui->filename);
}

void open_help_menu(GtkButton* b, gpointer user)
{
	GUI* gui = user;
	gtk_widget_show(GTK_WIDGET(gui->help_menu));
	gtk_widget_hide(GTK_WIDGET(gui->load));
	gtk_widget_hide(GTK_WIDGET(gui->solver));
}

void on_menu_clicked(GtkButton* b, gpointer user)
{
	GUI* gui = user;
	gtk_widget_hide(GTK_WIDGET(gui->help_menu));
	gtk_widget_show(GTK_WIDGET(gui->load));
	gtk_widget_hide(GTK_WIDGET(gui->solver));
}
int main(int argc, char *argv[])
{
	gtk_init(&argc ,&argv);

	GtkBuilder* builder = gtk_builder_new_from_file("interface.glade");

	// Load CSS
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);

	// Inject CSS
	GdkScreen *screen = gdk_screen_get_default();
	gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

	GtkWindow* interface = GTK_WINDOW(gtk_builder_get_object(builder, "interface"));
	GtkStack* window_pages = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
	GtkBox* load = GTK_BOX(gtk_builder_get_object(builder, "load"));
	GtkBox* solver = GTK_BOX(gtk_builder_get_object(builder, "solver"));
	GtkButton* QuitButton = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton"));
	GtkButton* QuitButton2 = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton2"));
	GtkButton* StartButton = GTK_BUTTON(gtk_builder_get_object(builder, "StartButton"));
	GtkImage* image_holder = GTK_IMAGE(gtk_builder_get_object(builder, "image_holder"));
	GtkBox* help_menu = GTK_BOX(gtk_builder_get_object(builder, "help_menu"));
	GtkButton* help_button = GTK_BUTTON(gtk_builder_get_object(builder, "HelpButton"));
	GtkButton* QuitButton1 = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton1"));
	GtkButton* refresh_button = GTK_BUTTON(gtk_builder_get_object(builder, "RefreshButton"));
	GtkButton* menu_button = GTK_BUTTON(gtk_builder_get_object(builder, "MenuButton"));
	GtkButton* resolve_button = GTK_BUTTON(gtk_builder_get_object(builder, "ResolveButton"));

	GUI gui={
		.interface = interface,
		.window_pages = window_pages,
		.load = load,
		.solver = solver,
		.QuitButton = QuitButton,
		.StartButton = StartButton,
		.filename = NULL,
		.image_holder = image_holder,
		.help_menu = help_menu,
		.help_button = help_button,
		.menu_button = menu_button,
		.screen = screen,
	};


	//CONNECTION
	g_signal_connect(interface, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(QuitButton, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(QuitButton1, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(QuitButton2, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(StartButton, "clicked", G_CALLBACK(open_files_explorer), &gui);
	g_signal_connect(help_button, "clicked", G_CALLBACK(open_help_menu), &gui);
	g_signal_connect(refresh_button, "clicked", G_CALLBACK(refresh), &gui);
	g_signal_connect(menu_button, "clicked", G_CALLBACK(on_menu_clicked), &gui);
	g_signal_connect(resolve_button, "clicked", G_CALLBACK(display_result), &gui);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(GTK_WIDGET(interface));

	gtk_main();

	return EXIT_SUCCESS;
}
