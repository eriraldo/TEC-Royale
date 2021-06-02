#include "Table.h"
#include "my_thread.h"
#include "Warrior.h"
#include <ncurses.h>
#include "ini.h"
extern WINDOW *screen1;
extern WINDOW *screen2;
extern Warrior warrior1;



int main(int argc, char** argv) {

    ini_t *config = ini_load("config.ini");//read info from config file
    const char *opcion = ini_get(config, "table", "opcion");//get table size
    if (strcmp(opcion,"1") == 0) {

        createTable(1);
    }
    else{
        createTable(2);
    }
    endwin();
}
