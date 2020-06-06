#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <time.h>
#include <stdbool.h>
// kompilacja: gcc `pkg-config --cflags gtk+-3.0` -o example-0 Lab_4.c `pkg-config --libs gtk+-3.0` -lpthread 

struct int_arg{
    int number;
};

const bool LEFT_TO_RIGHT = true; 
const bool RIGHT_TO_LEFT = false; 

GtkWidget *number1,*permutations;
long long int permutations_numer=1;
int stopped=0;
long long int factor = 100000;
gfloat progress;
time_t start,working_time, estimated_time;
pthread_t producent;
struct int_arg *arg_n;


long long int fact(int n) 
{ 
    long long int res = 1; 
    for (int i = 1; i <= n; i++) 
        res = res * i; 
    return res; 
} 

int searchArr(int a[], int n, int mobile) 
{ 
    for (int i = 0; i < n; i++) 
        if (a[i] == mobile) 
           return i + 1; 
} 
  
int getMobile(int a[], bool dir[], int n) 
{ 
    int mobile_prev = 0, mobile = 0; 
    for (int i = 0; i < n; i++) 
    { 
        if (dir[a[i]-1] == RIGHT_TO_LEFT && i!=0) 
        { 
            if (a[i] > a[i-1] && a[i] > mobile_prev) 
            { 
                mobile = a[i]; 
                mobile_prev = mobile; 
            } 
        } 
        if (dir[a[i]-1] == LEFT_TO_RIGHT && i!=n-1) 
        { 
            if (a[i] > a[i+1] && a[i] > mobile_prev) 
            { 
                mobile = a[i]; 
                mobile_prev = mobile; 
            } 
        } 
    } 
  
    if (mobile == 0 && mobile_prev == 0) 
        return 0; 
    else
        return mobile; 
} 
  
int printOnePerm(int a[], bool dir[], int n) 
{ 
    int mobile = getMobile(a, dir, n); 
    int pos = searchArr(a, n, mobile); 
    int buf;

    if (dir[a[pos - 1] - 1] ==  RIGHT_TO_LEFT) 
    {
        buf = a[pos-1];
        a[pos-1] = a[pos-2]; 
        a[pos-2] = buf;
    }
 
    else if (dir[a[pos - 1] - 1] == LEFT_TO_RIGHT) 
    {
        buf = a[pos];
        a[pos] = a[pos-1]; 
        a[pos-1] =buf;
    }
  
    for (int i = 0; i < n; i++) 
    { 
        if (a[i] > mobile) 
        { 
            if (dir[a[i] - 1] == LEFT_TO_RIGHT) 
                dir[a[i] - 1] = RIGHT_TO_LEFT; 
            else if (dir[a[i] - 1] == RIGHT_TO_LEFT) 
                dir[a[i] - 1] = LEFT_TO_RIGHT; 
        } 
    } 
  
   /* for (int i = 0; i < n; i++) 
        printf("%d",a[i]);  
    printf("\n");  */
} 
  
void* printPermutation(void *args) 
{   
    int* old_type;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, old_type);

    int n = ((struct int_arg*)args)->number;
    int a[n]; 
    bool dir[n]; 
  
 
    for (int i = 0; i < n; i++) 
    { 
        a[i] = i + 1; 
        //printf("%d",a[i]); 
    } 
    //printf("\n"); 
  
    for (int i = 0; i < n; i++) 
        dir[i] =  RIGHT_TO_LEFT; 
  
    for (permutations_numer = 1; permutations_numer < factor; permutations_numer++)
    {
        printOnePerm(a, dir, n);
        while (stopped)
        {
            usleep(10000);
        }
    } 
    pthread_exit(NULL);	
} 

void do_permutiation()
{   
    if (producent)
    {
    pthread_cancel(producent);
    }
    int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    arg_n = (struct int_arg *)malloc(sizeof(struct int_arg));
    factor = fact(num1);
    char buffer[64];
    char val[16];
    strcpy(buffer, (const char*)"n!: ");
    snprintf(val, 16, "%lld", factor);
    strcat(buffer, val);
    gtk_label_set_text(GTK_LABEL(permutations), buffer);
    arg_n->number = num1;
    pthread_create(&producent, NULL, printPermutation, (void*)arg_n);
    start = time(NULL);  
}

void pause_permutation()
{
    if (stopped==0) stopped = 1;
    else stopped = 0;
}

void* Progress_monitor()
{   
    start = time(NULL);
    while(1)
    {   
        progress = (gfloat)(permutations_numer)/factor;
        if(factor > permutations_numer)
        {
            working_time = time(NULL) - start;
            estimated_time = working_time*(1-progress)/progress;
        }
        usleep(50000);
    }      
}

gboolean update_progress_bar(gpointer progress_bar)
{   
    //printf("progres %lf\n", progress);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), progress);
    return G_SOURCE_CONTINUE;
}
gboolean update_time_to_end(gpointer time_to_end)
{
    char buf[64];
    char val[16];
    strcpy(buf, (const char*)"czas do końca: ");
    snprintf(val, 16, "%ld", estimated_time);
    strcat(buf, val);
    gtk_label_set_text(GTK_LABEL(time_to_end), buf);
return G_SOURCE_CONTINUE;
}
gboolean update_time_from_start(gpointer time_from_start)
{
    char buf[64];
    char val[16];
    strcpy(buf, (const char*)"czas od rozpoczęcia: ");
    snprintf(val, 16, "%ld", working_time);
    strcat(buf, val);
    gtk_label_set_text(GTK_LABEL(time_from_start), buf);
return G_SOURCE_CONTINUE;
}

int main(int argc,char **argv)
{
    pthread_t progress_monitor;
    int result_code=0;

    GtkWidget *window,*grid,*start_producer,*pause_producer;
    GtkWidget *progress_bar,*time_from_start,*time_to_end; 

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 640, 360); 
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    grid = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    number1 = gtk_entry_new();
    gtk_widget_set_size_request(number1, 240, 60); 
    gtk_fixed_put(GTK_FIXED(grid), number1, 20, 40); 

    start_producer = gtk_button_new_with_label("start");
    g_signal_connect(start_producer, "clicked", G_CALLBACK(do_permutiation), NULL);
    gtk_widget_set_size_request(start_producer, 100, 100); 
    gtk_fixed_put(GTK_FIXED(grid), start_producer, 300, 20);

    pause_producer = gtk_button_new_with_label("pause/resume");
    g_signal_connect(pause_producer, "clicked", G_CALLBACK(pause_permutation), NULL);
    gtk_widget_set_size_request(pause_producer, 100, 100);
    gtk_fixed_put(GTK_FIXED(grid), pause_producer, 460, 20);

    progress_bar = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar, 600, 20);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
    gtk_fixed_put(GTK_FIXED(grid), progress_bar, 20, 140);

    permutations = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(permutations), "n!= ");
    gtk_fixed_put(GTK_FIXED(grid), permutations, 30, 110);

    time_from_start = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(time_from_start), "czas od rozpoczęcia:");
    gtk_fixed_put(GTK_FIXED(grid), time_from_start, 160, 200);

    time_to_end = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(time_to_end), "czas do końca:");
    gtk_fixed_put(GTK_FIXED(grid), time_to_end, 160, 240);

    g_timeout_add(50, update_progress_bar, progress_bar);
    g_timeout_add(200, update_time_from_start, time_from_start);
    g_timeout_add(200, update_time_to_end, time_to_end);
   
    gtk_widget_show_all(window);

    result_code = pthread_create(&progress_monitor, NULL, Progress_monitor, NULL);
    gtk_main();  
}





















