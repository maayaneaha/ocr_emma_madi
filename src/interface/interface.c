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
}GUI;


void on_quit_clicked()
{
	gtk_main_quit();
}

SDL_Surface* resize_img(SDL_Surface *image)
{
    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                700, 800, image->format->BitsPerPixel,0,0,0,0);
    SDL_SoftStretch(image, NULL, dest, NULL);

    return dest;
}

void load_img(GUI* ui, gchar* path)
{
    SDL_Surface *s = IMG_Load(path);
    if(s->w > 2000)
    {
        SDL_Surface *new = resize_img(s);
        SDL_SaveBMP(new,"resized_img");
        gtk_image_set_from_file(ui->image_holder, "resized_img");
            SDL_UnlockSurface(new);
        ui->sdlimage = s;
    }
    else
    {
        SDL_Surface *new = SDL_CreateRGBSurface(SDL_HWSURFACE,
                700, 800, s->format->BitsPerPixel,
                0,0,0,0);
        SDL_SoftStretch(s, NULL, new, NULL);
        SDL_SaveBMP(new, "image");
        ui->sdlimage = s;
        SDL_UnlockSurface(new);
        gtk_image_set_from_file (ui->image_holder, "image");
    }
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
	load_img(gui, file_name);
}




int main(int argc, char *argv[])
{
	gtk_init(&argc ,&argv);

	GtkBuilder* builder = gtk_builder_new_from_file("interface.glade");

	GtkWindow* interface = GTK_WINDOW(gtk_builder_get_object(builder, "interface"));
	GtkStack* window_pages = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
	GtkBox* load = GTK_BOX(gtk_builder_get_object(builder, "load"));
	GtkBox* solver = GTK_BOX(gtk_builder_get_object(builder, "solver"));
	GtkButton* QuitButton = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton"));
	GtkButton* QuitButton2 = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton2"));
	GtkButton* StartButton = GTK_BUTTON(gtk_builder_get_object(builder, "StartButton"));
	GtkImage* image_holder = GTK_IMAGE(gtk_builder_get_object(builder, "image_holder"));

	GUI gui={
		.interface = interface,
		.window_pages = window_pages,
		.load = load,
		.solver = solver,
		.QuitButton = QuitButton,
		.StartButton = StartButton,
		.filename = NULL,
		.image_holder = image_holder;
	};

	//CONNECTION
	g_signal_connect(interface, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(QuitButton, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(QuitButton2, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(StartButton, "clicked", G_CALLBACK(open_files_explorer), &gui);



	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(interface);

	gtk_main();

	return EXIT_SUCCESS;
}
