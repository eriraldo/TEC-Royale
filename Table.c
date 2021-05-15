//
// Created by ericksalazargranera on 5/6/21.
//

#include <strings.h>
#include "Table.h"

//player1
struct Tower tower1;
struct Tower tower2;
struct Tower tower3;
//player2
struct Tower tower4;
struct Tower tower5;
struct Tower tower6;
//Warrior *WarrirorList[];
Warrior warrior1;
char* arr[21] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"
};

int iniciar(){
    int check = initValues(&warrior1,100,10,10,5,"P",3,5,2);
    return check;
}
WINDOW *screen1;
WINDOW *screen2;

struct Tower
{
    int health;
    char *towerSymbol;
    int posX;
    int posY;

};


void createTowers(int opcion){
    //---------------------------------------------------player1-----------------------
    tower1.health = 999;//vida
    tower1.towerSymbol = "-";//figura de la torre
    tower1.posX = 1; //posicion de x para guiarse
    tower1.posY = 1; //posicion de y para guiarse
    //=====================
    tower2.health = 999;//vida
    tower2.towerSymbol = "-";//figura de la torre
    tower2.posX = 1;
    tower2.posY = 5;
    //=====================
    tower3.health = 999;//vida
    tower3.towerSymbol = "-";//figura de la torre
    tower3.posX = 1;
    tower3.posY = 9;
    //---------------------------------------------------player2-----------------------
    int val;
    if(opcion == 1){
        val = 18;
    }
    else if (opcion == 2){
        val = 24;
    }
    tower4.health = 999;//vida
    tower4.towerSymbol = "-";//figura de la torre
    tower4.posX = val;
    tower4.posY = 1;
    //=====================
    tower5.health = 999;//vida
    tower5.towerSymbol = "-";//figura de la torre
    tower5.posX = val;
    tower5.posY = 5;
    //=====================
    tower6.health = 999;//vida
    tower6.towerSymbol = "-";//figura de la torre
    tower6.posX = val;
    tower6.posY = 9;
};

void moveWarrior(int nextMove, Warrior *warrior){ //----------------movimientos del guerrero en la pantalla izquierda

    char * lvl = arr[warrior->level];


    //bzero(lvl, sizeof(lvl));

 //   sprintf(lvl,"%d",warrior20->level);


    switch (nextMove) {
        case KEY_RIGHT://movimiento a la derecha
            //validar si en la posicion en donde se va a mover no hay nadie
            if ((warrior->Posx+2 < screen1->_maxx ) && (warrior->screen == 1)){
                mvwprintw(screen1,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx+1," ");
                //se imprime la nueva posicion
                //sprintf(lvl,"%d",warrior->level);

                mvwprintw(screen1,warrior->Posy,warrior->Posx+1,warrior->name);
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1,lvl);
                mvwprintw(screen1,warrior->Posy,warrior->Posx+2,">");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx+2,">");
                //se tiene que actualizar la posicion del warrior->x
                warrior->Posx  = warrior->Posx +1;
                wrefresh(screen1);
                wrefresh(screen2);
            }
                //se cambia a movimientos en pantalla 2 a la derecha
            if((warrior->Posx+2 >= screen1->_maxx ) && (warrior->screen == 1)){


                warrior->screen = 2;
                mvwprintw(screen1,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1," ");


                warrior->Posx = 2;

                mvwprintw(screen2,warrior->Posy,warrior->Posx,warrior->name);
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx,lvl);
                mvwprintw(screen2,warrior->Posy,warrior->Posx+1,">");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1,">");
                wrefresh(screen1);
                wrefresh(screen2);
            }
            if((warrior->screen == 2) && (warrior->Posx+2 < tower5.posX)){ //-------------------------------------------aqui se tiene que hacer la validacion de que dibuje solo si el puente está habilitado
                mvwprintw(screen2,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx+1," ");

                mvwprintw(screen2,warrior->Posy,warrior->Posx+1,warrior->name);
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1,lvl);
                mvwprintw(screen2,warrior->Posy,warrior->Posx+2,">");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx+2,">");
                warrior->Posx  = warrior->Posx +1;
                wrefresh(screen1);
                wrefresh(screen2);
            }
            break;
        case KEY_LEFT://movimiento a la izquierda
            //validar si en la posicion en donde se va a mover no hay nadie
            if((warrior->Posx-2 > 0) && (warrior->screen == 2)){
                //se imprime la nueva posicion
                mvwprintw(screen2,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx+1," ");

                mvwprintw(screen2,warrior->Posy,warrior->Posx-2,"<");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx-2,"<");
                mvwprintw(screen2,warrior->Posy,warrior->Posx-1,warrior->name);
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1,lvl);
                warrior->Posx = warrior->Posx-1;
                wrefresh(screen2);
            }
            if((warrior->Posx-2 <= 0 ) && (warrior->screen == 2)){
                warrior->screen = 1;
                mvwprintw(screen2,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1," ");
                //wrefresh(screen1);
                wrefresh(screen2);

                warrior->Posx = 19;

                mvwprintw(screen1,warrior->Posy,warrior->Posx,warrior->name);
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx,lvl);
                mvwprintw(screen1,warrior->Posy,warrior->Posx-1,"<");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1,"<");
                wrefresh(screen1);
                //wrefresh(screen2);
            }
            if((warrior->screen == 1) && (warrior->Posx-2 > tower2.posX+2)){
                mvwprintw(screen1,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx+1," ");

                mvwprintw(screen1,warrior->Posy,warrior->Posx-2,"<");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx-2,"<");
                mvwprintw(screen1,warrior->Posy,warrior->Posx-1,warrior->name);
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1,lvl);
                warrior->Posx  = warrior->Posx -1;
                wrefresh(screen1);
                //wrefresh(screen2);
            }
            break;
        case KEY_UP://movimiento hacia arriba
            if(warrior->screen == 1){
                mvwprintw(screen1,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx+1," ");
                if(warrior->Posy-2 > 0){

                    //se imprime la nueva posicion
                    mvwprintw(screen1,warrior->Posy-2,warrior->Posx,"/");
                    mvwprintw(screen1,warrior->Posy-2,warrior->Posx+1,"\\");
                    mvwprintw(screen1,warrior->Posy-1,warrior->Posx,warrior->name);
                    mvwprintw(screen1,warrior->Posy-1,warrior->Posx+1,lvl);
                    warrior->Posy = warrior->Posy-1;
                }
                else{
                    mvwprintw(screen1,warrior->Posy-1,warrior->Posx,"/");
                    mvwprintw(screen1,warrior->Posy-1,warrior->Posx+1,"\\");
                    mvwprintw(screen1,warrior->Posy,warrior->Posx,warrior->name);
                    mvwprintw(screen1,warrior->Posy,warrior->Posx+1,lvl);
                }
            }
            else if(warrior->screen == 2 ){
                mvwprintw(screen2,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx+1," ");
                if (warrior->Posy-2 > 0){

                    mvwprintw(screen2,warrior->Posy-2,warrior->Posx,"/");
                    mvwprintw(screen2,warrior->Posy-2,warrior->Posx+1,"\\");
                    mvwprintw(screen2,warrior->Posy-1,warrior->Posx,warrior->name);
                    mvwprintw(screen2,warrior->Posy-1,warrior->Posx+1,lvl);
                    warrior->Posy = warrior->Posy-1;
                }
                else{
                    mvwprintw(screen2,warrior->Posy-1,warrior->Posx,"/");
                    mvwprintw(screen2,warrior->Posy-1,warrior->Posx+1,"\\");
                    mvwprintw(screen2,warrior->Posy,warrior->Posx,warrior->name);
                    mvwprintw(screen2,warrior->Posy,warrior->Posx+1,lvl);
                }
            }


            break;
        case KEY_DOWN://movimiento hacia abajo
            if(warrior->screen == 1){
                mvwprintw(screen1,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen1,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen1,warrior->Posy-1,warrior->Posx+1," ");
                if(warrior->Posy+2 < screen1->_maxy){
                    //se imprime la nueva posicion
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx,warrior->name);
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1,lvl);
                    mvwprintw(screen1,warrior->Posy+2,warrior->Posx,"\\");
                    mvwprintw(screen1,warrior->Posy+2,warrior->Posx+1,"/");
                    warrior->Posy = warrior->Posy+1;
                }
                else{
                    mvwprintw(screen1,warrior->Posy,warrior->Posx,warrior->name);
                    mvwprintw(screen1,warrior->Posy,warrior->Posx+1,lvl);
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx,"\\");
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1,"/");
                }
            }
            else if(warrior->screen == 2){
                mvwprintw(screen2,warrior->Posy,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx," ");
                mvwprintw(screen2,warrior->Posy,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1," ");
                mvwprintw(screen2,warrior->Posy-1,warrior->Posx+1," ");
                if(warrior->Posy+2 < screen1->_maxy){
                    //se imprime la nueva posicion
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx,warrior->name);
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1,lvl);
                    mvwprintw(screen2,warrior->Posy+2,warrior->Posx,"\\");
                    mvwprintw(screen2,warrior->Posy+2,warrior->Posx+1,"/");
                    warrior->Posy = warrior->Posy+1;
                }
                else{
                    mvwprintw(screen2,warrior->Posy,warrior->Posx,warrior->name);
                    mvwprintw(screen2,warrior->Posy,warrior->Posx+1,lvl);
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx,"\\");
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1,"/");
                }
            }

            break;
        default:break;//no haga nada si mete otra tecla
    }
};


void createTable(int opcion){

    initscr();
    iniciar();
    keypad(stdscr,TRUE);
    cbreak();
    noecho();
    curs_set(0);

    int x, y, height, width;
    x = 0;
    y = 0;
    if(!has_colors()){
        printw("no se pueden usar los colores");
        getch();
        //return -1;
    }
    start_color();
    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    if(opcion == 1){
        height = 13;
        width = 22;
    }
    else if( opcion == 2){
        height = 13;
        width = 28;
    }

    createTowers(opcion);//se crean las 3 torres
    screen1 = newwin(height, width, y, x);
    screen2 = newwin(height, width, y, width);
    wrefresh(screen1);
    wrefresh(screen2);
    box(screen1,0,0);//se le define un borde al cuadro donde se puede jugar
    box(screen2,0,0);
    char health[40] ;
    sprintf(health,"%d",tower1.health);//se cambia a string la vida para poder mostrarla
    wattron(screen1,COLOR_PAIR(1));
    //se posiciona la torre en la ventana
    mvwprintw(screen1,tower1.posY,tower1.posX,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY,tower1.posX+1,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY,tower1.posX+2,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY+1,tower1.posX,health);
    mvwprintw(screen1,tower1.posY+2,tower1.posX,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY+2,tower1.posX+1,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY+2,tower1.posX+2,tower1.towerSymbol);
    //se posiciona la torre en la ventana
    char health2[40] ;
    sprintf(health2,"%d",tower2.health);//se cambia a string la vida para poder mostrarla
    mvwprintw(screen1,tower2.posY,tower2.posX,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY,tower2.posX+1,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY,tower2.posX+2,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY+1,tower2.posX,health2);
    mvwprintw(screen1,tower2.posY+2,tower2.posX,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY+2,tower2.posX+1,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY+2,tower2.posX+2,tower2.towerSymbol);
    //se posiciona la torre en la ventana
    char health3[40] ;
    sprintf(health3,"%d",tower3.health);//se cambia a string la vida para poder mostrarla
    mvwprintw(screen1,tower3.posY,tower3.posX,tower3.towerSymbol);
    mvwprintw(screen1,tower3.posY,tower3.posX+1,tower3.towerSymbol);
    mvwprintw(screen1,tower3.posY,tower3.posX+2,tower3.towerSymbol);
    mvwprintw(screen1,tower3.posY+1,tower3.posX,health3);
    mvwprintw(screen1,tower3.posY+2,tower3.posX,tower3.towerSymbol);
    mvwprintw(screen1,tower3.posY+2,tower3.posX+1,tower3.towerSymbol);
    mvwprintw(screen1,tower3.posY+2,tower3.posX+2,tower3.towerSymbol);
    wattroff(screen1,COLOR_PAIR(1));


    char health4[40] ;
    wattron(screen2,COLOR_PAIR(2));
    sprintf(health4,"%d",tower4.health);//se cambia a string la vida para poder mostrarla
    //se posiciona la torre en la ventana
    mvwprintw(screen2,tower4.posY,tower4.posX,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY,tower4.posX+1,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY,tower4.posX+2,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY+1,tower4.posX,health4);
    mvwprintw(screen2,tower4.posY+2,tower4.posX,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY+2,tower4.posX+1,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY+2,tower4.posX+2,tower4.towerSymbol);
    //se posiciona la torre en la ventana
    char health5[40] ;
    sprintf(health5,"%d",tower5.health);//se cambia a string la vida para poder mostrarla
    mvwprintw(screen2,tower5.posY,tower5.posX,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY,tower5.posX+1,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY,tower5.posX+2,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY+1,tower5.posX,health5);
    mvwprintw(screen2,tower5.posY+2,tower5.posX,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY+2,tower5.posX+1,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY+2,tower5.posX+2,tower5.towerSymbol);
    //se posiciona la torre en la ventana
    char health6[40] ;
    sprintf(health6,"%d",tower6.health);//se cambia a string la vida para poder mostrarla
    mvwprintw(screen2,tower6.posY,tower6.posX,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY,tower6.posX+1,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY,tower6.posX+2,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY+1,tower6.posX,health6);
    mvwprintw(screen2,tower6.posY+2,tower6.posX,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY+2,tower6.posX+1,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY+2,tower6.posX+2,tower6.towerSymbol);
    wattroff(screen2,COLOR_PAIR(2));

    int key = 0;
    while(key != 'q'){
        //move(5,5);
        nodelay(stdscr, TRUE);
        key = getch();
        //sleep(1);
        moveWarrior(key,&warrior1);
        refresh();
        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
    }



    /*

    else if(opcion == 2){//segundo tamanio del tablero de juego
        if(user ==1){
            height = 20;
            width = 60;
            createTowers();//se crean las 3 torres

            screen1 = newwin(height, width, y, x);
            refresh();
            box(screen1,0,0);
            char health[40] ;
            //sprintf(health,"%d",player.towers->health);//se cambia a string la vida para poder mostrarla
            mvwprintw(screen1,4,6,player.towers[0].towerSymbol);
            mvwprintw(screen1,4,7,player.towers[0].towerSymbol);
            mvwprintw(screen1,4,8,player.towers[0].towerSymbol);
            mvwprintw(screen1,5,6,health);
            mvwprintw(screen1,6,6,player.towers[0].towerSymbol);
            mvwprintw(screen1,6,7,player.towers[0].towerSymbol);
            mvwprintw(screen1,6,8,player.towers[0].towerSymbol);

            char health2[40] ;
            //sprintf(health2,"%d",player.towers->health);//se cambia a string la vida para poder mostrarla
            mvwprintw(screen1,9,9,player.towers[1].towerSymbol);
            mvwprintw(screen1,9,10,player.towers[1].towerSymbol);
            mvwprintw(screen1,9,11,player.towers[1].towerSymbol);
            mvwprintw(screen1,10,9,health2);
            mvwprintw(screen1,11,9,player.towers[1].towerSymbol);
            mvwprintw(screen1,11,10,player.towers[1].towerSymbol);
            mvwprintw(screen1,11,11,player.towers[1].towerSymbol);

            char health3[40] ;
            //sprintf(health3,"%d",player.towers->health);//se cambia a string la vida para poder mostrarla
            mvwprintw(screen1,14,6,player.towers[2].towerSymbol);
            mvwprintw(screen1,14,7,player.towers[2].towerSymbol);
            mvwprintw(screen1,14,8,player.towers[2].towerSymbol);
            mvwprintw(screen1,15,6,health3);
            mvwprintw(screen1,16,6,player.towers[2].towerSymbol);
            mvwprintw(screen1,16,7,player.towers[2].towerSymbol);
            mvwprintw(screen1,16,8,player.towers[2].towerSymbol);
            move(21,61);
            wrefresh(screen1);
        }
        else if (user ==2){
            height = 20;
            width = 60;
            createTowers();//se crean las 3 torres

            screen1 = newwin(height, width, y, x);
            refresh();
            box(screen1,0,0);
            char health[40] ;
            //sprintf(health,"%d",player.towers->health);//se cambia a string la vida para poder mostrarla
            mvwprintw(screen1,4,58,player.towers[0].towerSymbol);
            mvwprintw(screen1,4,57,player.towers[0].towerSymbol);
            mvwprintw(screen1,4,56,player.towers[0].towerSymbol);
            mvwprintw(screen1,5,56,health);
            mvwprintw(screen1,6,58,player.towers[0].towerSymbol);
            mvwprintw(screen1,6,57,player.towers[0].towerSymbol);
            mvwprintw(screen1,6,56,player.towers[0].towerSymbol);

            char health2[40] ;
            //sprintf(health2,"%d",player.towers->health);//se cambia a string la vida para poder mostrarla
            mvwprintw(screen1,9,58,player.towers[1].towerSymbol);
            mvwprintw(screen1,9,57,player.towers[1].towerSymbol);
            mvwprintw(screen1,9,56,player.towers[1].towerSymbol);
            mvwprintw(screen1,10,56,health2);
            mvwprintw(screen1,11,58,player.towers[1].towerSymbol);
            mvwprintw(screen1,11,57,player.towers[1].towerSymbol);
            mvwprintw(screen1,11,56,player.towers[1].towerSymbol);

            char health3[40] ;
            //sprintf(health3,"%d",player.towers->health);//se cambia a string la vida para poder mostrarla
            mvwprintw(screen1,14,58,player.towers[2].towerSymbol);
            mvwprintw(screen1,14,57,player.towers[2].towerSymbol);
            mvwprintw(screen1,14,56,player.towers[2].towerSymbol);
            mvwprintw(screen1,15,56,health3);
            mvwprintw(screen1,16,58,player.towers[2].towerSymbol);
            mvwprintw(screen1,16,57,player.towers[2].towerSymbol);
            mvwprintw(screen1,16,56,player.towers[2].towerSymbol);
            move(21,61);
            wrefresh(screen1);
        }
    */
};



//------------------------------------------- prueba de movimiento derecha
//printWarrior(1,7,10,9,screen1);
//printWarrior(1,7,12,9,screen1);
//------------------------------------------- prueba de movimiento izquierda
//printWarrior(2,7,14,9,screen1);
//printWarrior(2,7,12,9,screen1);
//------------------------------------------- prueba de movimiento arriba
//printWarrior(3,7,14,9,screen1);
//printWarrior(3,7,14,7,screen1);
//------------------------------------------- prueba de movimiento abajo
//printWarrior(4,7,14,2,screen1);
//printWarrior(4,7,14,4,screen1);
//printWarrior(4,7,14,6,screen1);
//------------------------------------------- prueba de movimiento abajo/izq
//printWarrior(4,7,14,4,screen1);
//printWarrior(2,7,14,6,screen1);
//printWarrior(4,7,12,2,screen1);


//------------------------------------------- prueba de movimiento abajo/izq
//------------------------------------------- prueba de movimiento derecha
//printWarrior(1,7,10,9,screen1);
//printWarrior(1,7,12,9,screen1);
//wattron(screen1,COLOR_PAIR(1));//-------------------------------------------------se inicia el coloreo de azul para el player 1
//printWarrior(3,7,18,4,screen1);
//printWarrior(4,7,14,4,screen1);
//printWarrior(2,7,14,5,screen1);
//wattroff(screen1,COLOR_PAIR(1));//-------------------------------------------------se finaliza el coloreo de azul para el player 1
//wattron(screen1,COLOR_PAIR(2)); //-------------------------------------------------se inicia el coloreo de rojo para el player 2
//printWarrior(2,7,14,9,screen1);
//wattroff(screen1,COLOR_PAIR(2));//-------------------------------------------------se finaliza el coloreo de rojo para el player 2
//decidirGanador()

//terminarJuego()

//inciarJuego()

