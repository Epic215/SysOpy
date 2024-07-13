#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <signal.h>

#define THREAD_COUNT 12


typedef struct {
    int cell_start;
    int cell_end;

    char** background;
    char** foreground;
} targs;

void nothing(int signo) { /*zz*/ }

void* threading(void * args){
    targs thread_info = *(targs*) args;
    while(true){
        pause();

        for (int i=thread_info.cell_start;i<thread_info.cell_end;i++){
            int row=i/grid_height;
            int col=i%grid_width;
            (*thread_info.background)[i]=is_alive(row, col, *(thread_info.foreground));
        }
    }
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = nothing;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

    pthread_t threads[THREAD_COUNT];


	init_grid(foreground);
    int count=(int)ceil((double)grid_height*(double)grid_width/THREAD_COUNT);

    for (int i=0 ;i<THREAD_COUNT;i++){
        targs args;
        args.cell_start=i*count;
        args.cell_end=i*count+count;
        args.foreground=&foreground;
        args.background=&background;
        pthread_create(&threads[i],NULL,threading,&args);
    }

	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

        for (int i=0 ;i<THREAD_COUNT;i++){
            pthread_kill(threads[i],SIGUSR1);
        }

		// Step simulation
		//update_grid(foreground, background);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
