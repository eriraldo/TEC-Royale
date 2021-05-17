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
int i;
warriorQueue warriorQueue1;

Warrior warrior1;
WINDOW *screen1;
WINDOW *screen2;
WINDOW *terminal;
char* arr[21] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"
};

int iniciar(){
    int check = initValues(&warrior1,100,100,10,5,"P",3,5,2);
    warriorQueue1 = GetThreadQueue();
    return check;
}

void createTowers(int opcion, struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){
    //---------------------------------------------------player1-----------------------
    tower1->health = 999;//vida
    tower1->towerSymbol = "-";//figura de la torre
    tower1->posX = 1; //posicion de x para guiarse
    tower1->posY = 1; //posicion de y para guiarse
    //=====================
    tower2->health = 999;//vida
    tower2->towerSymbol = "-";//figura de la torre
    tower2->posX = 1;
    tower2->posY = 5;
    //=====================
    tower3->health = 999;//vida
    tower3->towerSymbol = "-";//figura de la torre
    tower3->posX = 1;
    tower3->posY = 9;
    //---------------------------------------------------player2-----------------------
    int val;
    if(opcion == 1){
        val = 18;
    }
    else if (opcion == 2){
        val = 24;
    }
    tower4->health = 999;//vida
    tower4->towerSymbol = "-";//figura de la torre
    tower4->posX = val;
    tower4->posY = 1;
    //=====================
    tower5->health = 999;//vida
    tower5->towerSymbol = "-";//figura de la torre
    tower5->posX = val;
    tower5->posY = 5;
    //=====================
    tower6->health = 999;//vida
    tower6->towerSymbol = "-";//figura de la torre
    tower6->posX = val;
    tower6->posY = 9;
};
warrior_ptr GetCurrentThread(warriorQueue queue)
{
    if(queue == null)
    {
        return null;
    }
    return queue->head;
}

//This will remove the Thread which is pointed by head of the given queue
int Pop_Queue(warriorQueue queue)
{
    int failure = 0;
    warrior_ptr next_node,prev_node,head_node;
    if(queue == null)
    {
        failure = -1;
    }
    else
    {
        head_node = queue->head;
        prev_node = queue->head_parent;
        if(head_node != null)
        {

            next_node = head_node->next;
            if(queue->count == 1)
            {
                //There is only one node in the queue
                //clear the pointers
                queue->head = queue->head_parent = null;
            }
            else
            {
                //More Than one node in queue
                //Make the parent node point to next_node
                queue->head = next_node;
                prev_node->next = queue->head;
            }
            free(head_node);
            queue->count--;
        }
        else
        {
            //Queue Is Empty
            failure = -1;
        }
    }
    return failure;
}


int MoveForward(warriorQueue queue)
{
    int failure = -1;
    if(queue != null)
    {
        warrior_ptr current_head = queue->head;
        if(current_head != null)
        {
            queue->head_parent = current_head;
            queue->head = current_head->next;
            failure = 0;
        }
    }
    return failure;
}

int GetThreadCount(warriorQueue queue)
{
    if(queue == null)
    {
        return 0;
    }
    return queue->count;
}
int GetNextThreadId()
{
    static long currentThreadID=0;
    return ++currentThreadID;
}


int Push_Queue(warriorQueue queue,warrior_ptr node)
{
    int failure = 0;
    if(queue ==null || node== null)
    {
        failure = -1;
    }
    else
    {
        if(queue->head == null)
        {
            //first node
            //make next of it point to itself as we are implementing circular list
            node->next = node;
            queue->head_parent = node;
            queue->head = node;
        }
        else
        {
            //Add the node before head so that it will be at the end of the queue
            node->next= queue->head;
            queue->head_parent->next = node;
            queue->head_parent = node;
        }
        queue->count++;
    }

    return failure;
}

void PopNode_Queue(warriorQueue queue, warrior_ptr node)
{
    warrior_ptr nextNode = GetCurrentThread(queue);
    warrior_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->id == node->id)
            break;
        else{
            MoveForward(queue);
        }
        nextNode = GetCurrentThread(queue);
        if (nextNode == head){
            break;
        }
    }
    if (nextNode!=null){
        Pop_Queue(queue);
    }

}
warriorQueue GetThreadQueue()
{
    warriorQueue newQueue = (warriorQueue)malloc(sizeof(struct warriorQueue));
    if(newQueue == null)
    {
        //Memory Exhaustion
        return null;
    }
    newQueue->count = 0;
    newQueue->head = newQueue->head_parent = null;
    return newQueue;
}

warrior_ptr NewThread(Warrior * warrior, int player)
{
    warrior_ptr newThread = (warrior_ptr)malloc(sizeof(war));
    if(newThread == null)
    {

        return null;
    }
    newThread->next = null;

    //falta agregar todos los valores que vaya a agregar para inicializarlos
    newThread->warrior = warrior;
    newThread->id = GetNextThreadId();
    newThread->player = player;
    return newThread;
}
warrior_ptr GetThread( long idThread){
    warrior_ptr nextNode = GetCurrentThread(warriorQueue1);
    warrior_ptr head = nextNode;

    while((nextNode!=null))
    {
        if(nextNode->id == idThread)
            break;
        nextNode = nextNode->next;
        if (head == nextNode){

           return null;
        }

    }
    return nextNode;
}


warrior_ptr checkCollision(warrior_ptr warrior){
    warrior_ptr nextNode = GetCurrentThread(warriorQueue1);
    warrior_ptr head = nextNode;

    while((nextNode!=null))
    {
        if((warrior->warrior->Posx+2 == nextNode->warrior->Posx) &&(warrior->warrior->Posy == nextNode->warrior->Posy) && (warrior->player != nextNode->player)){

            return nextNode;
        } else if((warrior->warrior->Posx-2 == nextNode->warrior->Posx) &&(warrior->warrior->Posy == nextNode->warrior->Posy)&& (warrior->player != nextNode->player)){
            return nextNode;
        }
        else if((warrior->warrior->Posx == nextNode->warrior->Posx) &&(warrior->warrior->Posy+2 == nextNode->warrior->Posy)&& (warrior->player != nextNode->player)){
            return nextNode;
        }
        else if((warrior->warrior->Posx == nextNode->warrior->Posx) &&(warrior->warrior->Posy-2 == nextNode->warrior->Posy)&& (warrior->player != nextNode->player)){
            return nextNode;
        }
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
    return null;
}
void checkTowerCollision(warrior_ptr warrior,struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){



    if((warrior->warrior->Posx+2 == tower4->posX) && ((warrior->warrior->Posy == tower4->posY+1) || (warrior->warrior->Posy == tower4->posY+2)) && (warrior->player ==1) && (warrior->warrior->screen==2)){
        wattron(screen2,COLOR_PAIR(2));
        while(tower4->health > 0){

            tower4->health = tower4->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower4->health);
            mvwprintw(screen2,tower4->posY+1,tower4->posX,health4);
            sleep(1);
            wrefresh(screen2);
        }
        if(tower4->health <= 0){
            mvwprintw(screen2,tower4->posY+1,tower4->posX,"0");
            mvwprintw(screen2,tower4->posY+1,tower4->posX+1," ");
            mvwprintw(screen2,tower4->posY+1,tower4->posX+2," ");
            wrefresh(screen2);
        }
        wattroff(screen2,COLOR_PAIR(2));

    }
    else if((warrior->warrior->Posx+2 == tower5->posX) && ((warrior->warrior->Posy == tower5->posY) || (warrior->warrior->Posy == tower5->posY+1)) && (warrior->player ==1) && (warrior->warrior->screen==2)){
        wattron(screen2,COLOR_PAIR(2));
        while(tower5->health > 0){

            tower5->health = tower5->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower5->health);
            mvwprintw(screen2,tower5->posY+1,tower5->posX,health4);
            sleep(1);
            wrefresh(screen2);
        }
        if(tower5->health <= 0){
            tower5->health = 0;
            mvwprintw(screen2,tower5->posY+1,tower5->posX,"0");
            mvwprintw(screen2,tower5->posY+1,tower5->posX+1," ");
            mvwprintw(screen2,tower5->posY+1,tower5->posX+2," ");
            wrefresh(screen2);
        }
        wattroff(screen2,COLOR_PAIR(2));

    }
    else if((warrior->warrior->Posx+2 == tower6->posX) && ((warrior->warrior->Posy == tower6->posY) || (warrior->warrior->Posy == tower6->posY+1)) && (warrior->player ==1) && (warrior->warrior->screen==2)){
        wattron(screen2,COLOR_PAIR(2));
        while(tower6->health > 0){

            tower6->health = tower6->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower6->health);
            mvwprintw(screen2,tower6->posY+1,tower6->posX,health4);
            sleep(1);
            wrefresh(screen2);
        }
        if(tower6->health <= 0){
            tower6->health = 0;
            mvwprintw(screen2,tower6->posY+1,tower6->posX,"0");
            mvwprintw(screen2,tower6->posY+1,tower6->posX+1," ");
            mvwprintw(screen2,tower6->posY+1,tower6->posX+2," ");
            wrefresh(screen2);
        }
        wattroff(screen2,COLOR_PAIR(2));

    }
    else if((warrior->warrior->Posx-2 <= tower1->posX+2) && ((warrior->warrior->Posy == tower1->posY+1) || (warrior->warrior->Posy == tower1->posY+2)) && (warrior->player ==2) && (warrior->warrior->screen==1) ){
        wattron(screen1,COLOR_PAIR(1));
        while(tower1->health > 0){

            tower1->health = tower1->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower1->health);
            mvwprintw(screen1,tower1->posY+1,tower1->posX,health4);
            sleep(1);
            wrefresh(screen1);
        }
        if(tower1->health <= 0){
            mvwprintw(screen1,tower1->posY+1,tower1->posX,"0");
            mvwprintw(screen1,tower1->posY+1,tower1->posX+1," ");
            mvwprintw(screen1,tower1->posY+1,tower1->posX+2," ");
            wrefresh(screen1);
        }
        wattroff(screen1,COLOR_PAIR(1));

    }
    else if((warrior->warrior->Posx-2 <= tower2->posX+2) && ((warrior->warrior->Posy == tower2->posY) || (warrior->warrior->Posy == tower2->posY+1)) && (warrior->player ==2) && (warrior->warrior->screen==1)){
        wattron(screen1,COLOR_PAIR(1));
        while(tower2->health > 0){

            tower2->health = tower2->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower2->health);
            mvwprintw(screen1,tower2->posY+1,tower2->posX,health4);
            sleep(1);
            wrefresh(screen1);
        }
        if(tower2->health <= 0){
            tower2->health = 0;
            mvwprintw(screen1,tower2->posY+1,tower2->posX,"0");
            mvwprintw(screen1,tower2->posY+1,tower2->posX+1," ");
            mvwprintw(screen1,tower2->posY+1,tower2->posX+2," ");
            wrefresh(screen1);
        }
        wattroff(screen1,COLOR_PAIR(1));

    }
    else if((warrior->warrior->Posx-2 <= tower3->posX+2) && ((warrior->warrior->Posy == tower3->posY) || (warrior->warrior->Posy == tower3->posY+1)) && (warrior->player ==2) && (warrior->warrior->screen==1)){
        wattron(screen1,COLOR_PAIR(1));
        while(tower3->health > 0){

            tower3->health = tower3->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower3->health);
            mvwprintw(screen1,tower3->posY+1,tower3->posX,health4);
            sleep(1);
            wrefresh(screen1);
        }
        if(tower3->health <= 0){
            tower3->health = 0;
            mvwprintw(screen1,tower3->posY+1,tower3->posX,"0");
            mvwprintw(screen1,tower3->posY+1,tower3->posX+1," ");
            mvwprintw(screen1,tower3->posY+1,tower3->posX+2," ");
            wrefresh(screen1);
        }
        wattroff(screen1,COLOR_PAIR(1));

    }

}




void moveWarrior(int nextMove, Warrior *warrior, warrior_ptr node){ //----------------movimientos del guerrero en la pantalla izquierda

    char * lvl = arr[warrior->level];



    warrior_ptr check = checkCollision(node);

    if (check == null) {

        switch (nextMove) {
            case KEY_RIGHT://movimiento a la derecha

                if ((warrior->Posx + 2 < screen1->_maxx) && (warrior->screen == 1)) {

                    if (warrior->Posx + 2)
                        mvwprintw(screen1, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, warrior->name);
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, lvl);
                    mvwprintw(screen1, warrior->Posy, warrior->Posx + 2, ">");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 2, ">");
                    //se tiene que actualizar la posicion del warrior->x
                    warrior->Posx = warrior->Posx + 1;
                    wrefresh(screen1);
                }
                //se cambia a movimientos en pantalla 2 a la derecha
                if ((warrior->Posx + 2 >= screen1->_maxx) && (warrior->screen == 1)) {
                    warrior->screen = 2;
                    mvwprintw(screen1, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, " ");
                    warrior->Posx = 2;
                    mvwprintw(screen2, warrior->Posy, warrior->Posx, warrior->name);
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, lvl);
                    mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, ">");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, ">");
                    wrefresh(screen2);
                }
                if ((warrior->screen == 2) && (warrior->Posx + 2 < tower5.posX)) { //-------------------------------------------aqui se tiene que hacer la validacion de que dibuje solo si el puente está habilitado
                    mvwprintw(screen2, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, warrior->name);
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, lvl);
                    mvwprintw(screen2, warrior->Posy, warrior->Posx + 2, ">");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 2, ">");
                    warrior->Posx = warrior->Posx + 1;
                    wrefresh(screen2);
                }
                break;
            case KEY_LEFT://movimiento a la izquierda
                //validar si en la posicion en donde se va a mover no hay nadie
                if ((warrior->Posx - 2 > 0) && (warrior->screen == 2)) {
                    //se imprime la nueva posicion
                    mvwprintw(screen2, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen2, warrior->Posy, warrior->Posx - 2, "<");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx - 2, "<");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx - 1, warrior->name);
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx - 1, lvl);
                    warrior->Posx = warrior->Posx - 1;
                    wrefresh(screen2);
                }
                if ((warrior->Posx - 2 <= 0) && (warrior->screen == 2)) {
                    warrior->screen = 1;
                    mvwprintw(screen2, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx - 1, " ");
                    wrefresh(screen2);
                    warrior->Posx = 19;
                    //se imprime la nueva posicion
                    mvwprintw(screen1, warrior->Posy, warrior->Posx, warrior->name);
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, lvl);
                    mvwprintw(screen1, warrior->Posy, warrior->Posx - 1, "<");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx - 1, "<");
                    wrefresh(screen1);
                }
                if ((warrior->screen == 1) && (warrior->Posx - 2 > tower2.posX + 2)) {
                    mvwprintw(screen1, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen1, warrior->Posy, warrior->Posx - 2, "<");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx - 2, "<");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx - 1, warrior->name);
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx - 1, lvl);
                    warrior->Posx = warrior->Posx - 1;
                    wrefresh(screen1);
                }
                break;
            case KEY_UP://movimiento hacia arriba
                if (warrior->screen == 1) {
                    mvwprintw(screen1, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx + 1, " ");
                    if (warrior->Posy - 2 > 0) {
                        //se imprime la nueva posicion
                        mvwprintw(screen1, warrior->Posy - 2, warrior->Posx, "/");
                        mvwprintw(screen1, warrior->Posy - 2, warrior->Posx + 1, "\\");
                        mvwprintw(screen1, warrior->Posy - 1, warrior->Posx, warrior->name);
                        mvwprintw(screen1, warrior->Posy - 1, warrior->Posx + 1, lvl);
                        warrior->Posy = warrior->Posy - 1;
                    } else {
                        mvwprintw(screen1, warrior->Posy - 1, warrior->Posx, "/");
                        mvwprintw(screen1, warrior->Posy - 1, warrior->Posx + 1, "\\");
                        mvwprintw(screen1, warrior->Posy, warrior->Posx, warrior->name);
                        mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, lvl);
                    }
                } else if (warrior->screen == 2) {
                    mvwprintw(screen2, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx + 1, " ");
                    if (warrior->Posy - 2 > 0) {
                        //se imprime la nueva posicion
                        mvwprintw(screen2, warrior->Posy - 2, warrior->Posx, "/");
                        mvwprintw(screen2, warrior->Posy - 2, warrior->Posx + 1, "\\");
                        mvwprintw(screen2, warrior->Posy - 1, warrior->Posx, warrior->name);
                        mvwprintw(screen2, warrior->Posy - 1, warrior->Posx + 1, lvl);
                        warrior->Posy = warrior->Posy - 1;
                    } else {
                        mvwprintw(screen2, warrior->Posy - 1, warrior->Posx, "/");
                        mvwprintw(screen2, warrior->Posy - 1, warrior->Posx + 1, "\\");
                        mvwprintw(screen2, warrior->Posy, warrior->Posx, warrior->name);
                        mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, lvl);
                    }
                }


                break;
            case KEY_DOWN://movimiento hacia abajo
                if (warrior->screen == 1) {
                    mvwprintw(screen1, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen1, warrior->Posy - 1, warrior->Posx + 1, " ");
                    if (warrior->Posy + 2 < screen1->_maxy) {
                        //se imprime la nueva posicion
                        mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, warrior->name);
                        mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, lvl);
                        mvwprintw(screen1, warrior->Posy + 2, warrior->Posx, "\\");
                        mvwprintw(screen1, warrior->Posy + 2, warrior->Posx + 1, "/");
                        warrior->Posy = warrior->Posy + 1;
                    } else {
                        mvwprintw(screen1, warrior->Posy, warrior->Posx, warrior->name);
                        mvwprintw(screen1, warrior->Posy, warrior->Posx + 1, lvl);
                        mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, "\\");
                        mvwprintw(screen1, warrior->Posy + 1, warrior->Posx + 1, "/");
                    }
                } else if (warrior->screen == 2) {
                    mvwprintw(screen2, warrior->Posy, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx - 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx, " ");
                    mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, " ");
                    mvwprintw(screen2, warrior->Posy - 1, warrior->Posx + 1, " ");
                    if (warrior->Posy + 2 < screen1->_maxy) {
                        //se imprime la nueva posicion
                        mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, warrior->name);
                        mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, lvl);
                        mvwprintw(screen2, warrior->Posy + 2, warrior->Posx, "\\");
                        mvwprintw(screen2, warrior->Posy + 2, warrior->Posx + 1, "/");
                        warrior->Posy = warrior->Posy + 1;
                    } else {
                        mvwprintw(screen2, warrior->Posy, warrior->Posx, warrior->name);
                        mvwprintw(screen2, warrior->Posy, warrior->Posx + 1, lvl);
                        mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, "\\");
                        mvwprintw(screen2, warrior->Posy + 1, warrior->Posx + 1, "/");
                    }
                }

                break;
            default:
                break;//no haga nada si mete otra tecla
        }
    }
    else{
        //hacer random para definir quien muere
        if(check->warrior->screen == 1){
            mvwprintw(screen1, check->warrior->Posy, check->warrior->Posx, " ");
            mvwprintw(screen1, check->warrior->Posy + 1, check->warrior->Posx, " ");
        }
        else{
            mvwprintw(screen2, check->warrior->Posy, check->warrior->Posx, " ");
            mvwprintw(screen2, check->warrior->Posy + 1, check->warrior->Posx, " ");

        }
        PopNode_Queue(warriorQueue1,check);
        moveWarrior(nextMove,warrior,node);

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
    createTowers(opcion,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);//se crean las 3 torres
    screen1 = newwin(height, width, y, x);
    screen2 = newwin(height, width, y, width);
    //terminal = newwin(3, width, height+1, x);
    wrefresh(screen1);
    wrefresh(screen2);
    //wrefresh(terminal);
    box(screen1,0,0);//se le define un borde al cuadro donde se puede jugar
    box(screen2,0,0);
    //box(terminal,0,0);
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
    Warrior warrior2;

    initValues(&warrior2,100,10,10,5,"X",7,5,1);
    moveWarrior(KEY_DOWN,&warrior2, GetThread(1));
    Push_Queue(warriorQueue1,NewThread(&warrior2,1));
    Push_Queue(warriorQueue1,NewThread(&warrior1,2));


    int key = 0;
    while(key != 'q'){
        //move(5,5);
        nodelay(stdscr, TRUE);
        key = getch();
        //sleep(1);

        checkTowerCollision(GetThread(2),&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
        int terminar =decidirGanador(&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
        if(terminar == 1){
            sleep(10);
            break;
        }

        moveWarrior(key,&warrior1, GetThread(2));
        refresh();
        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
        //wrefresh(terminal);

        //tengo que meterle guerreros a la lista warriorQueue

        //decidirGanador();
    }
    delwin(screen1);
    delwin(screen2);
    //delwin(terminal);


};


int decidirGanador(struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){
    if((tower1->health <= 0)&&(tower2->health <= 0) && (tower3->health <= 0))
    {
        clear();
        mvwprintw(screen2,4,10,"GANADOR ");
        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
        return 1;
    }
    else if((tower4->health <= 0)&&(tower5->health <= 0) && (tower6->health <= 0))
    {
        clear();
        mvwprintw(screen1,4,10,"GANADOR ");
        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
        return 1;
    }
    return 0;
}


