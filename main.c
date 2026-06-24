#include <gtk/gtk.h>
#include "scheduler.h"

#define MAX_GUI_ROWS 15

typedef struct {
    GtkWidget *name;
    GtkWidget *arrival;
    GtkWidget *burst;
    GtkWidget *priority;
    GtkWidget *type;
} ProcessRow;

typedef struct {
    GtkWidget *count_entry;
    GtkWidget *quantum_entry;

    GtkWidget *grid;
    //for output
    GtkWidget *output_view;
    GtkTextBuffer *output_buffer;
    ProcessRow rows[MAX_GUI_ROWS];

    int current_rows;
} AppData;

static const char *task_names[] = {
    "Message",
    "Notification",
    "Upload",
    "LiveStream",
    NULL
};

static void append_output(AppData *app, const char *text)
{
    GtkTextIter end;

    gtk_text_buffer_get_end_iter(
        app->output_buffer,
        &end
    );

    gtk_text_buffer_insert(
        app->output_buffer,
        &end,
        text,
        -1
    );
}

static void generate_rows(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    AppData *app = user_data;

    int n = atoi(
        gtk_editable_get_text(
            GTK_EDITABLE(app->count_entry)
        )
    );

    if(n < 1) n = 1;
    if(n > 15) n = 15;

    while(gtk_widget_get_first_child(app->grid))
{
    gtk_grid_remove(
        GTK_GRID(app->grid),
        gtk_widget_get_first_child(app->grid)
    );
}

app->current_rows = n;
    gtk_grid_attach(GTK_GRID(app->grid),
                    gtk_label_new("PID"),
                    0,0,1,1);

    gtk_grid_attach(GTK_GRID(app->grid),
                    gtk_label_new("Name"),
                    1,0,1,1);

    gtk_grid_attach(GTK_GRID(app->grid),
                    gtk_label_new("Type"),
                    2,0,1,1);

    gtk_grid_attach(GTK_GRID(app->grid),
                    gtk_label_new("Arrival"),
                    3,0,1,1);

    gtk_grid_attach(GTK_GRID(app->grid),
                    gtk_label_new("Burst"),
                    4,0,1,1);

    gtk_grid_attach(GTK_GRID(app->grid),
                    gtk_label_new("Priority"),
                    5,0,1,1);

    for(int i=0;i<n;i++)
    {
        char pid[10];
        sprintf(pid,"P%d",i+1);

        gtk_grid_attach(GTK_GRID(app->grid),
                        gtk_label_new(pid),
                        0,i+1,1,1);

        app->rows[i].name = gtk_entry_new();
        app->rows[i].arrival = gtk_entry_new();
        app->rows[i].burst = gtk_entry_new();
        app->rows[i].priority = gtk_entry_new();
   
        GtkStringList *list =
            gtk_string_list_new(task_names);

        app->rows[i].type =
            gtk_drop_down_new(
                G_LIST_MODEL(list),
                NULL
            );

        gtk_grid_attach(GTK_GRID(app->grid),
                        app->rows[i].name,
                        1,i+1,1,1);

        gtk_grid_attach(GTK_GRID(app->grid),
                        app->rows[i].type,
                        2,i+1,1,1);

        gtk_grid_attach(GTK_GRID(app->grid),
                        app->rows[i].arrival,
                        3,i+1,1,1);

        gtk_grid_attach(GTK_GRID(app->grid),
                        app->rows[i].burst,
                        4,i+1,1,1);

        gtk_grid_attach(GTK_GRID(app->grid),
                        app->rows[i].priority,
                        5,i+1,1,1);
    }
}

static void run_scheduler(GtkButton *btn, gpointer user_data)
{

    (void)btn;
    AppData *app = user_data;

    Process original[MAX_PROCESSES];
    Process working[MAX_PROCESSES];

    int n = app->current_rows;

    int quantum =
        atoi(
            gtk_editable_get_text(
                GTK_EDITABLE(app->quantum_entry)
            )
        );

    if(quantum < 1)
        quantum = 2;

    for(int i=0;i<n;i++)
    {
        original[i].pid = i + 1;

        strcpy(
            original[i].name,
            gtk_editable_get_text(
                GTK_EDITABLE(app->rows[i].name)
            )
        );

        original[i].arrival =
            atoi(
                gtk_editable_get_text(
                    GTK_EDITABLE(app->rows[i].arrival)
                )
            );

        original[i].burst =
            atoi(
                gtk_editable_get_text(
                    GTK_EDITABLE(app->rows[i].burst)
                )
            );

        original[i].priority =
            atoi(
                gtk_editable_get_text(
                    GTK_EDITABLE(app->rows[i].priority)
                )
            );

        original[i].type =
            gtk_drop_down_get_selected(
                GTK_DROP_DOWN(app->rows[i].type)
            );

        original[i].remaining =
            original[i].burst;

        original[i].started = 0;
    }

    gtk_text_buffer_set_text(
    app->output_buffer,
    "",
    -1
);

append_output(
    app,
    "=================================\n"
);

append_output(
    app,
    "RUNNING ALL ALGORITHMS\n"
);

append_output(
    app,
    "=================================\n"
);
 
//write to file
freopen("output.txt","w",stdout);

    reset_processes(original, working, n);
    fcfs(working, n);


    reset_processes(original, working, n);
    sjf(working, n);


    reset_processes(original, working, n);
    round_robin(working, n, quantum);

    reset_processes(original, working, n);
    priority_sched(working, n);

    compare_all(original, n, quantum);
   append_output(
    app,
    "\nComparision report generated\n"
);
fflush(stdout);
fclose(stdout);
FILE *fp = fopen("output.txt", "r");

if(fp)
{
    char buffer[50000];

    size_t len =
        fread(buffer,
              1,
              sizeof(buffer)-1,
              fp);

    buffer[len] = '\0';

    fclose(fp);

    gtk_text_buffer_set_text(
        app->output_buffer,
        buffer,
        -1
    );
}
}

static void activate(GtkApplication *gtk_app,
                     gpointer user_data)
{
    (void)user_data;
    AppData *app =g_malloc0(sizeof(AppData));
    GtkWidget *window;
    GtkWidget *main_box;

    window =
        gtk_application_window_new(
            gtk_app
        );

    gtk_window_set_title(
        GTK_WINDOW(window),
        "Social Media Scheduler"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        1000,700
    );

    main_box =
        gtk_box_new(
            GTK_ORIENTATION_VERTICAL,
            10
        );

    gtk_window_set_child(
        GTK_WINDOW(window),
        main_box
    );

    gtk_box_append(
        GTK_BOX(main_box),
        gtk_label_new(
            "Number of Processes (1-15)"
        )
    );

    app->count_entry =
        gtk_entry_new();

    gtk_box_append(
        GTK_BOX(main_box),
        app->count_entry
    );

    GtkWidget *gen_btn =
        gtk_button_new_with_label(
            "Generate Tasks"
        );

    gtk_box_append(
        GTK_BOX(main_box),
        gen_btn
    );

    app->grid =
        gtk_grid_new();

    gtk_box_append(
        GTK_BOX(main_box),
        app->grid
    );

    gtk_box_append(
        GTK_BOX(main_box),
        gtk_label_new(
            "Round Robin Quantum"
        )
    );

    app->quantum_entry =
        gtk_entry_new();

    gtk_editable_set_text(
        GTK_EDITABLE(app->quantum_entry),
        "2"
    );

    gtk_box_append(
        GTK_BOX(main_box),
        app->quantum_entry
    );

    GtkWidget *run_btn =
        gtk_button_new_with_label(
            "Run Scheduler"
        );

    gtk_box_append(
        GTK_BOX(main_box),
        run_btn
    );

    g_signal_connect(
        gen_btn,
        "clicked",
        G_CALLBACK(generate_rows),
        app
    );

    g_signal_connect(
        run_btn,
        "clicked",
        G_CALLBACK(run_scheduler),
        app
    );
    GtkWidget *scroll;

scroll = gtk_scrolled_window_new();

gtk_widget_set_size_request(
    scroll,
    900,
    250
);

app->output_view =
    gtk_text_view_new();

gtk_text_view_set_editable(
    GTK_TEXT_VIEW(app->output_view),
    FALSE
);

gtk_text_view_set_cursor_visible(
    GTK_TEXT_VIEW(app->output_view),
    FALSE
);

app->output_buffer =
    gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(app->output_view)
    );

gtk_scrolled_window_set_child(
    GTK_SCROLLED_WINDOW(scroll),
    app->output_view
);

gtk_box_append(
    GTK_BOX(main_box),
    scroll
);

    gtk_widget_show(window);
}

int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app =
        gtk_application_new(
            "org.scheduler.social",
            G_APPLICATION_FLAGS_NONE
        );

    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        NULL
    );

    status =
        g_application_run(
            G_APPLICATION(app),
            argc,
            argv
        );

    g_object_unref(app);

    return status;
}
