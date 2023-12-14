#include <gtk/gtk.h>
#include <glib/gstdio.h>

//#include "optimizer_fio.h"
//#include "abatch.h"
#include "avarlogger.h"

static void print_hello_1 (GtkWidget *widget, gpointer data) {
  g_print ("File select\n");
}

static void print_hello_2 (GtkWidget *widget, gpointer data) {
  g_print ("Find route\n");
  
}

static void quit_cb (GtkWindow *window) {
  gtk_window_close (window);
}


/*void on_button_clicked(GtkButton *button, gpointer user_data) {
    // Get the GtkDrawingArea from user_data
    GtkWidget *drawingArea = GTK_WIDGET(user_data);

    // Force a redraw of the drawing area
    gtk_widget_queue_draw(drawingArea);
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    // Get the allocation (size) of the drawing area
    int width, height;
    //gtk_widget_get_allocation(widget, &width, &height);

    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);


    // Draw the left half with red color
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // Red color
    cairo_rectangle(cr, 0, 0, width / 2, height);
    cairo_fill(cr);

    // Draw the right half with green color
    cairo_set_source_rgb(cr, 0.0, 1.0, 0.0); // Green color
    cairo_rectangle(cr, width / 2, 0, width / 2, height);
    cairo_fill(cr);

    return FALSE; // Ensure the default draw handler is also called
}*/

static void
draw_function (GtkDrawingArea *area,
               cairo_t        *cr,
               int             width,
               int             height,
               gpointer        data)
{
  GdkRGBA color;

  cairo_arc (cr,
             width / 2.0, height / 2.0,
             MIN (width, height) / 2.0,
             0, 2 * G_PI);

  /*gtk_widget_get_color (GTK_WIDGET (area),
                        &color);*/
  gdk_cairo_set_source_rgba (cr, &color);

  cairo_fill (cr);
}

static void
activate (GtkApplication *app, gpointer user_data) {
  /* Construct a GtkBuilder instance and load our UI description */
  GtkBuilder *builder = gtk_builder_new ();

  GError * error = NULL;
  if(!gtk_builder_add_from_file (builder, "./gui.ui", &error)) {
    g_warning( "GtkBuilder error: %s", error->message );
    g_error_free( error );
    exit(1);
    //perror()
  }

  /* Connect signal handlers to the constructed widgets. */
  GObject *window = gtk_builder_get_object (builder, "window");
  gtk_window_set_application (GTK_WINDOW (window), app);

  GObject *obj = gtk_builder_get_object (builder, "fileSelectButton");
  g_signal_connect (obj, "clicked", G_CALLBACK (print_hello_1), NULL);

  obj = gtk_builder_get_object (builder, "findRouteButton");
  g_signal_connect (obj, "clicked", G_CALLBACK (print_hello_2), NULL);

  obj = gtk_builder_get_object (builder, "progressBar");
  gtk_progress_bar_set_fraction((GtkProgressBar*)obj, 0.6);

  /*obj = gtk_builder_get_object (builder, "drawingArea");
  GtkWidget* drawingArea = GTK_WIDGET (obj);
  g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(on_draw), NULL);
  gtk_widget_queue_draw(drawingArea);*/

  GtkWidget *area = gtk_drawing_area_new ();
  gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (area), 100);
  gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (area), 100);
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (area),
                                  draw_function,
                                  NULL, NULL);

  //gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (obj), 100);
  //gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (obj), 100);
  /*gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (obj),
                                  draw_function,
                                  NULL, NULL);*/



  /*button = gtk_builder_get_object (builder, "quit");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (quit_cb), window);*/

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
  ar2ceConfig.num_iters = 100;
  ar2ceConfig.evaporation_factor = 0.5;

  /*abatch_stat_t stats = abatch("TSP", 1000, path, &ar2ceConfig, VERBOSE);
  #print_stat(stats);*/


  avarlogger_config_t avarloggerConfig;
  avarloggerConfig.logger_name = "TSP";
  avarloggerConfig.path = (char*)path;
  avarloggerConfig.ar2ceConfig = &ar2ceConfig;
  init_avarlogger(&avarloggerConfig);
  varlog_metric_weight(0.1, 3.005, 0.005, "TSP_metric_weight.acolog");
  varlog_pheromone_weight(0.1, 3.005, 0.005, "TSP_pheromone_weight.acolog");
  varlog_num_ants(10, 51, 1, "TSP_num_ants.acolog");
  varlog_ant_randomness_factor(0, 1.005, 0.005, "TSP_ant_randomness_factor.acolog");
  varlog_num_iters(1, 101, 1, "TSP_num_iters.acolog");
  varlog_evaporation_factor(0, 1, 0.005, "TSP_evaporation_factor.acolog");

  /*aco_result_t result = find_best_route(path, &ar2ceConfig);
  aco_route_t shortest_route = result.route;
  printf("\nFound route: ");
  print_route(shortest_route);
  printf("\nLength: ");
  printf("%4.0f\n", result.route_length);*/
}

int main (int argc, char *argv[]) {
  //console_demo("/home/mundus/base/sem5/miz/acdata/P-n16-k8.txt");
  console_demo("/home/mundus/base/sem5/miz/acdata/A-n32-k5.txt");
  //console_demo("/home/mundus/base/sem5/miz/acdata/A-n80-k10.txt");
  //printf("\nHello!\n");
  return 0;

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