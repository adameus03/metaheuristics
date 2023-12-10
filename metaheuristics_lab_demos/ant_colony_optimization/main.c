#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include "optimizer_fio.h"

static void print_hello_1 (GtkWidget *widget, gpointer data) {
  g_print ("Hello Europe\n");
}

static void print_hello_2 (GtkWidget *widget, gpointer data) {
  g_print ("Hello America\n");
}

static void quit_cb (GtkWindow *window) {
  gtk_window_close (window);
}

static void
activate (GtkApplication *app, gpointer user_data) {
  /* Construct a GtkBuilder instance and load our UI description */
  GtkBuilder *builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "./builder.ui", NULL);

  /* Connect signal handlers to the constructed widgets. */
  GObject *window = gtk_builder_get_object (builder, "window");
  gtk_window_set_application (GTK_WINDOW (window), app);

  GObject *button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello_1), NULL);

  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello_2), NULL);

  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (quit_cb), window);

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

  /* We do not need the builder any more */
  g_object_unref (builder);
}


void console_demo(const char* path) {
  aco_r2_cart_euc_config_t ar2ceConfig;
  ar2ceConfig.metric_weight = 1;
  ar2ceConfig.pheromone_weight = 1;
  ar2ceConfig.num_ants = 30;
  ar2ceConfig.ant_randomness_factor = 0.3;
  ar2ceConfig.num_iters = 1000;

  aco_result_t result = find_best_route(path, &ar2ceConfig);
  aco_route_t shortest_route = result.route;
  printf("\nFound route: ");
  print_route(shortest_route);
  printf("\nLength: ");
  printf("%4.0f\n", result.route_length);
}

int main (int argc, char *argv[]) {
  console_demo("/home/mundus/base/sem5/miz/acdata/P-n16-k8.txt");
  //console_demo("/home/mundus/base/sem5/miz/acdata/A-n32-k5.txt");
  //console_demo("/home/mundus/base/sem5/miz/acdata/A-n80-k10.txt");
  //printf("\nHello!\n");
  //return 0;

#ifdef GTK_SRCDIR
  g_chdir (GTK_SRCDIR);
#endif

  GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}


/*#include <gtk/gtk.h>

static void
activate (GtkApplication* app, gpointer user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;;
}*/