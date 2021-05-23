//
// Created by ericksalazargranera on 5/6/21.
//

#include <strings.h>
#include "Table.h"
#include "time.h"
#include "my_mutex.h"

//player1
struct Tower tower1;
struct Tower tower2;
struct Tower tower3;
//player2
struct Tower tower4;
struct Tower tower5;
struct Tower tower6;
int i ;
int entry_limit = 2;
int depart_limit = 3;

warriorQueue warriorQueue1;

Warrior warrior1;
WINDOW *screen1;
WINDOW *screen2;
WINDOW *terminal;
int lock;

struct Params{
    int nextMove;
    Warrior * warrior;
    warrior_ptr node;
    int width;
};
char* arr[21] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"
};

int iniciar(){
    int check = initValues(&warrior1,200,100,10,5,"P",6,2,1,0);
    warriorQueue1 = GetThreadQueueW();
    return check;
}

void createTowers(int opcion, struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){
    //---------------------------------------------------player1-----------------------
    tower1->health = 500;//vida
    tower1->towerSymbol = "-";//figura de la torre
    tower1->posX = 1; //posicion de x para guiarse
    tower1->posY = 1; //posicion de y para guiarse
    tower1->attack = 15;
    //=====================
    tower2->health = 600;//vida
    tower2->towerSymbol = "-";//figura de la torre
    tower2->posX = 1;
    tower2->posY = 5;
    tower2->attack = 15;
    //=====================
    tower3->health = 500;//vida
    tower3->towerSymbol = "-";//figura de la torre
    tower3->posX = 1;
    tower3->posY = 9;
    tower3->attack = 15;
    //---------------------------------------------------player2-----------------------
    int val;
    if(opcion == 1){
        val = 18;
    }
    else if (opcion == 2){
        val = 24;
    }
    tower4->health = 500;//vida
    tower4->towerSymbol = "-";//figura de la torre
    tower4->posX = val;
    tower4->posY = 1;
    tower4->attack = 15;
    //=====================
    tower5->health = 600;//vida
    tower5->towerSymbol = "-";//figura de la torre
    tower5->posX = val;
    tower5->posY = 5;
    tower5->attack = 15;
    //=====================
    tower6->health = 500;//vida
    tower6->towerSymbol = "-";//figura de la torre
    tower6->posX = val;
    tower6->posY = 9;
    tower6->attack = 15;
};
warrior_ptr GetCurrentThreadW(warriorQueue queue)
{
    if(queue == null)
    {
        return null;
    }
    return queue->head;
}

//This will remove the Thread which is pointed by head of the given queue
int Pop_QueueW(warriorQueue queue)
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


int MoveForwardW(warriorQueue queue)
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

int GetThreadCountW(warriorQueue queue)
{
    if(queue == null)
    {
        return 0;
    }
    return queue->count;
}
int GetNextThreadIdW()
{
    static long currentThreadID=0;
    return ++currentThreadID;
}


int Push_QueueW(warriorQueue queue,warrior_ptr node)
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

void PopNode_QueueW(warriorQueue queue, warrior_ptr node)
{
    warrior_ptr nextNode = GetCurrentThreadW(queue);
    warrior_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->id == node->id)
            break;
        else{
            MoveForwardW(queue);
        }
        nextNode = GetCurrentThreadW(queue);
        if (nextNode == head){
            break;
        }
    }
    if (nextNode!=null){
        Pop_QueueW(queue);
    }

}
warriorQueue GetThreadQueueW()
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

warrior_ptr NewThreadW(Warrior * warrior, int player)
{
    warrior_ptr newThread = (warrior_ptr)malloc(sizeof(war));
    if(newThread == null)
    {

        return null;
    }
    newThread->next = null;

    //falta agregar todos los valores que vaya a agregar para inicializarlos
    newThread->warrior = warrior;
    newThread->id = GetNextThreadIdW();
    newThread->player = player;
    return newThread;
}
warrior_ptr GetThreadW( long idThread){
    warrior_ptr nextNode = GetCurrentThreadW(warriorQueue1);
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
    warrior_ptr nextNode = GetCurrentThreadW(warriorQueue1);
    warrior_ptr head = nextNode;

    while((nextNode!=null))
    {
        if((warrior->warrior->Posx+2 == nextNode->warrior->Posx) &&(warrior->warrior->Posy == nextNode->warrior->Posy) && (warrior->player != nextNode->player) && (warrior->warrior->screen == nextNode->warrior->screen)){

            return nextNode;
        } else if((warrior->warrior->Posx-2 == nextNode->warrior->Posx) &&(warrior->warrior->Posy == nextNode->warrior->Posy)&& (warrior->player != nextNode->player) &&(warrior->warrior->screen == nextNode->warrior->screen)){
            return nextNode;
        }
        else if((warrior->warrior->Posx == nextNode->warrior->Posx) &&(warrior->warrior->Posy+2 == nextNode->warrior->Posy)&& (warrior->player != nextNode->player) && (warrior->warrior->screen == nextNode->warrior->screen)){
            return nextNode;
        }
        else if((warrior->warrior->Posx == nextNode->warrior->Posx) &&(warrior->warrior->Posy-2 == nextNode->warrior->Posy)&& (warrior->player != nextNode->player) && (warrior->warrior->screen == nextNode->warrior->screen)){
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

        while(tower4->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->warrior->vitality = warrior->warrior->vitality - tower4->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->warrior->vitality);
            if(warrior->warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_exit();
                break;
            }
            //Warrior attacks the tower
            tower4->health = tower4->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower4->health);
            wattron(screen2,COLOR_PAIR(2));
            if(tower4->health < 100){
                mvwprintw(screen2,tower4->posY+1,tower4->posX,"0");
                mvwprintw(screen2,tower4->posY+1,tower4->posX+1,health4);

            }
            else{
                mvwprintw(screen2,tower4->posY+1,tower4->posX,health4);
            }
            wattroff(screen2,COLOR_PAIR(2));
            wrefresh(screen2);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }
        if(tower4->health <= 0){
            my_mutex_lock(&lock);
            wattron(screen2,COLOR_PAIR(2));
            mvwprintw(screen2,tower4->posY+1,tower4->posX,"0");
            mvwprintw(screen2,tower4->posY+1,tower4->posX+1," ");
            mvwprintw(screen2,tower4->posY+1,tower4->posX+2," ");
            wattroff(screen2,COLOR_PAIR(2));
            wrefresh(screen2);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }


    }
    else if((warrior->warrior->Posx+2 == tower5->posX) && ((warrior->warrior->Posy == tower5->posY) || (warrior->warrior->Posy == tower5->posY+1)) && (warrior->player ==1) && (warrior->warrior->screen==2)){

        while(tower5->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->warrior->vitality = warrior->warrior->vitality - tower5->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->warrior->vitality);
            if(warrior->warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_exit();
                break;
            }
            //Warrior attacks the tower
            tower5->health = tower5->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower5->health);
            wattron(screen2,COLOR_PAIR(2));
            if(tower5->health < 100){
                mvwprintw(screen2,tower5->posY+1,tower5->posX,"0");
                mvwprintw(screen2,tower5->posY+1,tower5->posX+1,health4);

            }
            else{
                mvwprintw(screen2,tower5->posY+1,tower5->posX,health4);
            }
            wattroff(screen2,COLOR_PAIR(2));
            wrefresh(screen2);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }
        if(tower5->health <= 0){
            my_mutex_lock(&lock);
            tower5->health = 0;
            wattron(screen2,COLOR_PAIR(2));
            mvwprintw(screen2,tower5->posY+1,tower5->posX,"0");
            mvwprintw(screen2,tower5->posY+1,tower5->posX+1," ");
            mvwprintw(screen2,tower5->posY+1,tower5->posX+2," ");
            wattroff(screen2,COLOR_PAIR(2));
            wrefresh(screen2);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }


    }
    else if((warrior->warrior->Posx+2 == tower6->posX) && ((warrior->warrior->Posy == tower6->posY) || (warrior->warrior->Posy == tower6->posY+1)) && (warrior->player ==1) && (warrior->warrior->screen==2)){

        while(tower6->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->warrior->vitality = warrior->warrior->vitality - tower6->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->warrior->vitality);
            if(warrior->warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_exit();
                break;
            }
            //Warrior attacks the tower
            tower6->health = tower6->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower6->health);
            wattron(screen2,COLOR_PAIR(2));
            if(tower6->health < 100){
                mvwprintw(screen2,tower6->posY+1,tower6->posX,"0");
                mvwprintw(screen2,tower6->posY+1,tower6->posX+1,health4);

            }
            else{
                mvwprintw(screen2,tower6->posY+1,tower6->posX,health4);
            }
            wattroff(screen2,COLOR_PAIR(2));
            wrefresh(screen2);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }
        if(tower6->health <= 0){
            my_mutex_lock(&lock);
            tower6->health = 0;
            wattron(screen2,COLOR_PAIR(2));
            mvwprintw(screen2,tower6->posY+1,tower6->posX,"0");
            mvwprintw(screen2,tower6->posY+1,tower6->posX+1," ");
            mvwprintw(screen2,tower6->posY+1,tower6->posX+2," ");
            wattroff(screen2,COLOR_PAIR(2));
            wrefresh(screen2);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }


    }
    else if((warrior->warrior->Posx-2 <= tower1->posX+2) && ((warrior->warrior->Posy == tower1->posY+1) || (warrior->warrior->Posy == tower1->posY+2)) && (warrior->player ==2) && (warrior->warrior->screen==1) ){

        while(tower1->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->warrior->vitality = warrior->warrior->vitality - tower1->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->warrior->vitality);
            if(warrior->warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_exit();
                break;
            }
            //Warrior attacks the tower
            tower1->health = tower1->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower1->health);
            wattron(screen1,COLOR_PAIR(1));


            if(tower1->health < 100){
                mvwprintw(screen1,tower1->posY+1,tower1->posX,"0");
                mvwprintw(screen1,tower1->posY+1,tower1->posX+1,health4);

            }
            else{
                mvwprintw(screen1,tower1->posY+1,tower1->posX,health4);
            }
            wattroff(screen1,COLOR_PAIR(1));
            wrefresh(screen1);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }
        if(tower1->health <= 0){
            my_mutex_lock(&lock);
            wattron(screen1,COLOR_PAIR(1));
            mvwprintw(screen1,tower1->posY+1,tower1->posX,"0");
            mvwprintw(screen1,tower1->posY+1,tower1->posX+1," ");
            mvwprintw(screen1,tower1->posY+1,tower1->posX+2," ");
            wattroff(screen1,COLOR_PAIR(1));
            wrefresh(screen1);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }


    }
    else if((warrior->warrior->Posx-2 <= tower2->posX+2) && ((warrior->warrior->Posy == tower2->posY) || (warrior->warrior->Posy == tower2->posY+1)) && (warrior->player ==2) && (warrior->warrior->screen==1)){

        while(tower2->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->warrior->vitality = warrior->warrior->vitality - tower2->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->warrior->vitality);
            if(warrior->warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_exit();
                break;
            }
            //Warrior attacks the tower
            tower2->health = tower2->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower2->health);
            wattron(screen1,COLOR_PAIR(1));
            if(tower2->health < 100){
                mvwprintw(screen1,tower2->posY+1,tower2->posX,"0");
                mvwprintw(screen1,tower2->posY+1,tower2->posX+1,health4);
            }
            else{
                mvwprintw(screen1,tower2->posY+1,tower2->posX,health4);
            }
            wattroff(screen1,COLOR_PAIR(1));
            wrefresh(screen1);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }
        if(tower2->health <= 0){
            my_mutex_lock(&lock);
            tower2->health = 0;
            wattron(screen1,COLOR_PAIR(1));
            mvwprintw(screen1,tower2->posY+1,tower2->posX,"0");
            mvwprintw(screen1,tower2->posY+1,tower2->posX+1," ");
            mvwprintw(screen1,tower2->posY+1,tower2->posX+2," ");
            wattroff(screen1,COLOR_PAIR(1));
            wrefresh(screen1);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }


    }
    else if((warrior->warrior->Posx-2 <= tower3->posX+2) && ((warrior->warrior->Posy == tower3->posY) || (warrior->warrior->Posy == tower3->posY+1)) && (warrior->player ==2) && (warrior->warrior->screen==1)){
        wattron(screen1,COLOR_PAIR(1));
        while(tower3->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->warrior->vitality = warrior->warrior->vitality - tower3->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->warrior->vitality);
            if(warrior->warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_exit();
                break;
            }
            //Warrior attacks the tower
            tower3->health = tower3->health-warrior->warrior->attack;
            char health4[4] ;
            sprintf(health4,"%d",tower3->health);
            wattron(screen1,COLOR_PAIR(1));
            if(tower3->health < 100){
                mvwprintw(screen1,tower3->posY+1,tower3->posX,"0");
                mvwprintw(screen1,tower3->posY+1,tower3->posX+1,health4);

            }
            else{
                mvwprintw(screen1,tower3->posY+1,tower3->posX,health4);
            }
            wattroff(screen1,COLOR_PAIR(1));

            wrefresh(screen1);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }
        if(tower3->health <= 0){
            my_mutex_lock(&lock);
            tower3->health = 0;
            wattron(screen1,COLOR_PAIR(1));
            mvwprintw(screen1,tower3->posY+1,tower3->posX,"0");
            mvwprintw(screen1,tower3->posY+1,tower3->posX+1," ");
            mvwprintw(screen1,tower3->posY+1,tower3->posX+2," ");
            wattroff(screen1,COLOR_PAIR(1));
            wrefresh(screen1);
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
        }


    }


}




void moveWarrior(int nextMove, Warrior *warrior, warrior_ptr node){

    char * lvl = arr[warrior->level];



    warrior_ptr check = checkCollision(node);

    if (check == null) {

        switch (nextMove) {
            case 1://movimiento a la derecha

                if ((warrior->Posx + 2 < screen1->_maxx) && (warrior->screen == 1)) {

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
            case 2://movimiento a la izquierda
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
            case 3://movimiento hacia arriba
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
            case 4://movimiento hacia abajo
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
                break;
        }
    }
    else{

        srand(time(NULL));   // Initialization, should only be called once.
        int r = rand();
        if(check->warrior->screen == 1){
            if(r%2 == 0){
                mvwprintw(screen1, check->warrior->Posy, check->warrior->Posx, " ");
                mvwprintw(screen1, check->warrior->Posy + 1, check->warrior->Posx, " ");
            }
            else{
                mvwprintw(screen1, warrior->Posy, warrior->Posx, " ");
                mvwprintw(screen1, warrior->Posy + 1, warrior->Posx, " ");
            }


        }
        else{
            if(r%2 == 0){
                mvwprintw(screen2, check->warrior->Posy, check->warrior->Posx, " ");
                mvwprintw(screen2, check->warrior->Posy + 1, check->warrior->Posx, " ");
            }
            else{
                mvwprintw(screen2, warrior->Posy, warrior->Posx, " ");
                mvwprintw(screen2, warrior->Posy + 1, warrior->Posx, " ");
            }

        }
        PopNode_QueueW(warriorQueue1,check);
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
        printw("colors arent available");
        getch();

    }
    start_color();
    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    height = 13;
    if(opcion == 1){
        width = 22;
    }
    else if( opcion == 2){
        width = 28;
    }
    createTowers(opcion,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);//se crean las 3 torres
    screen1 = newwin(height, width, y, x);
    screen2 = newwin(height, width, y, width);
    //terminal = newwin(3, width, height, x);
    wrefresh(screen1);
    wrefresh(screen2);
    //wrefresh(terminal);
    box(screen1,0,0);
    box(screen2,0,0);
    //box(terminal,0,0);
    //-------------------------------------------------------------------------------bridge screen1
    mvwprintw(screen1,(height/2)-4,screen1->_maxx,"-");
    mvwprintw(screen1,(height/2)-2,screen1->_maxx,"-");
    mvwprintw(screen1,(height/2)+2,screen1->_maxx,"-");
    mvwprintw(screen1,(height/2)+4,screen1->_maxx,"-");

    //-------------------------------------------------------------------------------bridge screen
    mvwprintw(screen2,(height/2)-4,0,"-");
    mvwprintw(screen2,(height/2)-2,0,"-");
    mvwprintw(screen2,(height/2)+2,0,"-");
    mvwprintw(screen2,(height/2)+4,0,"-");

    char health[40] ;
    sprintf(health,"%d",tower1.health);
    wattron(screen1,COLOR_PAIR(1));

    mvwprintw(screen1,tower1.posY,tower1.posX,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY,tower1.posX+1,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY,tower1.posX+2,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY+1,tower1.posX,health);
    mvwprintw(screen1,tower1.posY+2,tower1.posX,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY+2,tower1.posX+1,tower1.towerSymbol);
    mvwprintw(screen1,tower1.posY+2,tower1.posX+2,tower1.towerSymbol);

    char health2[40] ;
    sprintf(health2,"%d",tower2.health);
    mvwprintw(screen1,tower2.posY,tower2.posX,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY,tower2.posX+1,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY,tower2.posX+2,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY+1,tower2.posX,health2);
    mvwprintw(screen1,tower2.posY+2,tower2.posX,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY+2,tower2.posX+1,tower2.towerSymbol);
    mvwprintw(screen1,tower2.posY+2,tower2.posX+2,tower2.towerSymbol);

    char health3[40] ;
    sprintf(health3,"%d",tower3.health);
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

    mvwprintw(screen2,tower4.posY,tower4.posX,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY,tower4.posX+1,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY,tower4.posX+2,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY+1,tower4.posX,health4);
    mvwprintw(screen2,tower4.posY+2,tower4.posX,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY+2,tower4.posX+1,tower4.towerSymbol);
    mvwprintw(screen2,tower4.posY+2,tower4.posX+2,tower4.towerSymbol);

    char health5[40] ;
    sprintf(health5,"%d",tower5.health);
    mvwprintw(screen2,tower5.posY,tower5.posX,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY,tower5.posX+1,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY,tower5.posX+2,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY+1,tower5.posX,health5);
    mvwprintw(screen2,tower5.posY+2,tower5.posX,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY+2,tower5.posX+1,tower5.towerSymbol);
    mvwprintw(screen2,tower5.posY+2,tower5.posX+2,tower5.towerSymbol);

    char health6[40] ;
    sprintf(health6,"%d",tower6.health);
    mvwprintw(screen2,tower6.posY,tower6.posX,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY,tower6.posX+1,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY,tower6.posX+2,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY+1,tower6.posX,health6);
    mvwprintw(screen2,tower6.posY+2,tower6.posX,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY+2,tower6.posX+1,tower6.towerSymbol);
    mvwprintw(screen2,tower6.posY+2,tower6.posX+2,tower6.towerSymbol);
    wattroff(screen2,COLOR_PAIR(2));
    Warrior warrior2;
    Warrior warrior3;

    initValues(&warrior2,100,10,10,5,"X",17,5,2,1);
    initValues(&warrior3,100,10,10,5,"T",7,8,1,0);

    Push_QueueW(warriorQueue1,NewThreadW(&warrior1,1));
    Push_QueueW(warriorQueue1,NewThreadW(&warrior2,2));
    Push_QueueW(warriorQueue1,NewThreadW(&warrior3,1));

    int key = 0;
        nodelay(stdscr, TRUE);
        //key = getch();


        sleep(1/2);
        my_mutex_init(&lock);
        my_thread_t t1;
        my_thread_t t2;
        my_thread_t t3;
        my_thread_init(100);

        struct Params * par = (struct Params *)malloc(sizeof(struct Params));
        struct Params * par2 = (struct Params *)malloc(sizeof(struct Params));
        struct Params * par3 = (struct Params *)malloc(sizeof(struct Params));
        par->width = width;
        par->node = GetThreadW(1);
        par->warrior = &warrior1;
        par->nextMove = 1;
        //bombWarrior(&warrior1);
        my_thread_create(&t1,movePlayer1,(void*)par, 2);

        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);

        par2->width = width;
        par2->node = GetThreadW(2);
        par2->warrior = &warrior2;
        par2->nextMove = 2;
        bombWarrior(&warrior2);
        my_thread_create(&t2,movePlayer1,(void*)par2, 1);
        //my_thread_sleep(3);
        //exitWarriorThread(1);

        par3->width = width;
        par3->node = GetThreadW(3);
        par3->warrior = &warrior3;
        par3->nextMove = 1;
        my_thread_create(&t3,movePlayer1,(void*)par3, 1);
        while(1){
            int terminar =decidirGanador(&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
            if(terminar == 1){
                break;
            }};

        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
        //wrefresh(terminal);
        //decidirGanador();
    wgetch(screen1);
    delwin(screen1);
    delwin(screen2);
    //delwin(terminal);
};
//el deploy en el INIT tiene que ser en el x = 5
void* movePlayer1(void * parameters){

    int  width, nextMove;
    warrior_ptr node;
    Warrior * warrior;

    width = ((struct Params*)parameters)->width;
    nextMove = ((struct Params*)parameters)->nextMove;
    node = ((struct Params*)parameters)->node;
    warrior = ((struct Params*)parameters)->warrior;
    time_t t1;
    if (warrior->bomb ==1 ){
        my_thread_sleep(warrior->entry_limit);

        t1 = time(0) + warrior->depart_limit;
    }
    int stepsX = 0;
    int pathLength;
    if (width <= 27){
        pathLength =10;
    }
    if (width >= 28){
        pathLength =38;
    }
    wrefresh(screen1);
    wrefresh(screen2);

    while(stepsX < pathLength ){//solo tiene que moverse a la derecha
        if (time(0) > t1 && warrior->bomb == 1){
            my_mutex_lock(&lock);
            if(node->player == 1){
                if(warrior->screen == 1){
                    mvwprintw(screen1,warrior->Posy,warrior->Posx,"X");
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx,"X");
                    mvwprintw(screen1,warrior->Posy,warrior->Posx+1,"X");
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1,"X");
                    wrefresh(screen1);
                }
                else{
                    mvwprintw(screen2,warrior->Posy,warrior->Posx,"X");
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx,"X");
                    mvwprintw(screen2,warrior->Posy,warrior->Posx+1,"X");
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1,"X");
                    wrefresh(screen2);
                }
            }
            else{
                if(warrior->screen == 1){
                    mvwprintw(screen1,warrior->Posy,warrior->Posx,"X");
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx,"X");
                    mvwprintw(screen1,warrior->Posy,warrior->Posx-1,"X");
                    mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1,"X");
                    wrefresh(screen1);
                }
                else{
                    mvwprintw(screen2,warrior->Posy,warrior->Posx,"X");
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx,"X");
                    mvwprintw(screen2,warrior->Posy,warrior->Posx-1,"X");
                    mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1,"X");
                    wrefresh(screen2);
                }
            }

            my_thread_sleep(1);
            cleanWarrior( warrior, node);
            my_mutex_unlock(&lock);
            my_thread_exit();
        }
        if(node->finish ==1){
            my_mutex_lock(&lock);
            cleanWarrior( warrior, node);
            my_mutex_unlock(&lock);
            my_thread_exit();
        }
        checkTowerCollision(node,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
        my_mutex_lock(&lock);
        if(node->player == 1 && warrior->screen == 1){
            wattron(screen1, COLOR_PAIR(1));
        }
        else{
            wattron(screen2, COLOR_PAIR(1));
        }
        moveWarrior(nextMove,warrior,node);
        if(node->player == 1 && warrior->screen == 1){
            wattroff(screen1, COLOR_PAIR(1));
        }
        else{
            wattroff(screen2, COLOR_PAIR(1));
        }

        my_mutex_unlock(&lock);
        my_thread_sleep(1);
        stepsX +=1;
        //refresh();
        wrefresh(screen1);
        wrefresh(screen2);
    }
    int down = 0;

    while(down <4){
        if(node->finish ==1){
            my_mutex_lock(&lock);
            cleanWarrior( warrior, node);
            my_mutex_unlock(&lock);
            my_thread_exit();
        }
        checkTowerCollision(node,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
        my_mutex_lock(&lock);
        moveWarrior(4,warrior,node);
        my_mutex_unlock(&lock);
        my_thread_sleep(1);
        wrefresh(screen1);
        wrefresh(screen2);
        down +=1;
    }
};


int decidirGanador(struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){
    my_mutex_lock(&lock);
    if((tower2->health <= 0) )
    {
        clear();

        mvwprintw(screen2,4,10,"GANADOR ");
        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
        my_mutex_unlock(&lock);
        return 1;
    }
    else if((tower5->health <= 0))
    {
        clear();
        mvwprintw(screen1,4,10,"GANADOR ");
        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
        my_mutex_unlock(&lock);
        return 1;
    }
    my_mutex_unlock(&lock);
    return 0;
}
void exitWarriorThread(int id){
    warrior_ptr warriorToRemove =  GetThreadW(id);
    warriorToRemove->finish = 1;
    my_mutex_unlock(&lock);
}

void cleanWarrior(Warrior * warrior, warrior_ptr node){
    if(warrior->screen == 1){
        mvwprintw(screen1,warrior->Posy,warrior->Posx," ");
        mvwprintw(screen1,warrior->Posy+1,warrior->Posx," ");
        mvwprintw(screen1,warrior->Posy,warrior->Posx+1," ");
        mvwprintw(screen1,warrior->Posy+1,warrior->Posx+1," ");
        mvwprintw(screen1,warrior->Posy,warrior->Posx-1," ");
        mvwprintw(screen1,warrior->Posy+1,warrior->Posx-1," ");
    }
    else{
        mvwprintw(screen2,warrior->Posy,warrior->Posx," ");
        mvwprintw(screen2,warrior->Posy+1,warrior->Posx," ");
        mvwprintw(screen2,warrior->Posy,warrior->Posx+1," ");
        mvwprintw(screen2,warrior->Posy+1,warrior->Posx+1," ");
        mvwprintw(screen2,warrior->Posy,warrior->Posx-1," ");
        mvwprintw(screen2,warrior->Posy+1,warrior->Posx-1," ");
    }
    PopNode_QueueW(warriorQueue1,node);
    wrefresh(screen1);
    wrefresh(screen2);
}

void bombWarrior(Warrior * warrior){
    if(warrior->bomb == 1){
        warrior->entry_limit = entry_limit;
        warrior->depart_limit = depart_limit;

    }
}





