#include "Table.h"
#include "my_thread.h"
#include "Warrior.h"
#include <ncurses.h>
#include "ini.h"
#include <stdio.h>
#include <unistd.h>
extern WINDOW *screen1;
extern WINDOW *screen2;
extern Warrior warrior1;
ini_t *config;


int main(int argc, char** argv) {

    int opt;
    opt = getopt(argc, argv, "abc:"); //parse command line arguments
    if(opt == 'c') {
        config = ini_load(optarg);//read info from config file
        if(config){
            const char *opcion = ini_get(config, "table", "opcion");//get table size
            if (strcmp(opcion,"1") == 0) {

                createTable(1);
            }
            else{
                createTable(2);
            }
            endwin();
        }else{
            printf("config file doesn't exist\n");
        }

    }else{
        printf("invalid arguments\n");

    }


}
