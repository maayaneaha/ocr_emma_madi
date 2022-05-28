#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/*
struct GdkRGBA {
  gdouble red;
  gdouble green;
  gdouble blue;
  gdouble alpha;
}
*/
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
	GtkButton* menu_button_solver;
	GdkScreen *screen;
	gchar* result_file;
	GtkSwitch* switch_button;
	GtkAdjustment* binadj;
	GtkAdjustment* conadj;
	GtkScale* binarise_scale;
	GtkScale* contrast_scale;
	int activate;
}GUI;

int activate = 0;
int binarise = 350;
int contrast = 10;

void on_quit_clicked()
{
	gtk_main_quit();
}

void binarise_scale_update(GtkAdjustment* k, gpointer user)
{
	GUI* gui = user;
	binarise = gtk_adjustment_get_value(gui->binadj);
	printf("%i\n", binarise);
}

void contrast_scale_update(GtkAdjustment* i, gpointer user)
{
	GUI* gui = user;
	contrast = gtk_adjustment_get_value(gui->conadj);
	printf("%i\n", contrast);
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

size_t my_strlen(char str[])
{
	size_t i = 0;
	for (; str[i] != 0;i ++); 
	return i;
}

char *str_concat(char str1[], char str2[])
{
	char *res = calloc(my_strlen(str1) + my_strlen(str2)+1, sizeof(char));
	for (size_t i = 0; i < my_strlen(str1); i ++) {
		res[i] = str1[i];
	}
	for (size_t i = 0; i < my_strlen(str2); i ++) {
		res[i + my_strlen(str1)] = str2[i];
	}
	return res;
}

void menu_button_solver_clicked(GtkButton* b, gpointer user)
{
	GUI* gui = user;
	//gtk_widget_hide(GTK_WIDGET(gui->help_menu));
	gtk_widget_show(GTK_WIDGET(gui->load));
	gtk_widget_hide(GTK_WIDGET(gui->solver));

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

char * convertNumberIntoArray(unsigned int number) {
    unsigned int length = (int)(log10((float)number)) + 1;
    char * arr = (char *) malloc(length * sizeof(char)), * curr = arr;
    do {
        *curr++ = number % 10;
        number /= 10;
    } while (number != 0);
    return arr;
}

void refresh(GtkButton* b, gpointer user, int binarise, int contrast)
{
	GUI* gui = user;
	char *cmd = str_concat("cd ../processing/; ./processing ",gui->filename);
	printf("%i", binarise);
	printf("%i", contrast);
	if(activate == 1)
	{
		char *res = str_concat(cmd, " 10 10 1");
		if (system(res))
	{
		printf("Error");
	}
	}
	else
	{
		char *res = str_concat(cmd, " 1000 350 0");
		if (system(res))
	{
		printf("Error");
	}
	}
	

	//gui->filename = "../processing/output.bmp";
	display_image(gui->image_holder, "../processing/output.bmp");
	
}

void otsu_switch()
{
	if(activate == 0)
	{
		activate = 1;
	}
	else
	{
		activate = 0;
	}
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
	//system("GTK_THEME=Adwaita:dark");
	gtk_init(&argc ,&argv);

	GtkBuilder* builder = gtk_builder_new_from_file("interface.glade");

	// Load CSS
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "interface.css", NULL);

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
	GtkButton* menu_button_solver = GTK_BUTTON(gtk_builder_get_object(builder, "menu_button_solver"));
	GtkSwitch* switch_button = GTK_SWITCH(gtk_builder_get_object(builder, "switch_button"));
	GtkAdjustment* binadj = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "binadj"));
	GtkAdjustment* conadj = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "conadj"));

	GtkScale* binarise_scale = GTK_SCALE(gtk_builder_get_object(builder, "binarise_scale"));
	GtkScale* contrast_scale = GTK_SCALE(gtk_builder_get_object(builder, "contrast_scale"));

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
		.menu_button_solver = menu_button_solver,
		.screen = screen,
		.switch_button = switch_button,
		.activate = 0,
		.binadj = binadj,
		.conadj = conadj,
		.binarise_scale = binarise_scale,
		.contrast_scale = contrast_scale,
	};

	//gtk_color_chooser_set_rgba (GtkColorChooser* chooser,const GdkRGBA* color);

	//CONNECTION
	g_signal_connect(interface, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(QuitButton, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(QuitButton1, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(QuitButton2, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(StartButton, "clicked", G_CALLBACK(open_files_explorer), &gui);
	g_signal_connect(help_button, "clicked", G_CALLBACK(open_help_menu), &gui);
	g_signal_connect(refresh_button, "clicked", G_CALLBACK(refresh), &gui);
	g_signal_connect(menu_button, "clicked", G_CALLBACK(on_menu_clicked), &gui);
	g_signal_connect(menu_button_solver, "clicked", G_CALLBACK(menu_button_solver_clicked), &gui);
	g_signal_connect(switch_button, "state-set", G_CALLBACK(otsu_switch), NULL);
	g_signal_connect(switch_button, "activate", G_CALLBACK(otsu_switch), NULL);
	g_signal_connect(binadj, "value-changed", G_CALLBACK(binarise_scale_update), &gui);
	g_signal_connect(conadj, "value-changed", G_CALLBACK(contrast_scale_update), &gui);
	//g_signal_connect(resolve_button, "clicked", G_CALLBACK(display_result), &gui);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(GTK_WIDGET(interface));
	gtk_main();

	return EXIT_SUCCESS;
}
