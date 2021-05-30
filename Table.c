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
int countwarriors = 0;
Warrior warrior;
int entry_limit = 2;
int depart_limit = 25;
extern sigset_t sigProcMask;

warriorQueue warriorQueue1;

Warrior warrior1;
WINDOW *screen1;
WINDOW *screen2;
WINDOW *terminal;
WINDOW *terminal2;
int lock, battleLock, bridge1Lock, bridge2Lock, partner1Lock, partner2Lock;
int partnerBridge1, partnerBridge2 = 0;
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
    int check = initValues(&warrior1,200,100,10,5,"P",1,0);
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
        val = 24;
    }
    else if (opcion == 2){
        val = 30;
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
            nextNode = null;
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

warrior_ptr NewThreadW(Warrior * warrior, int player, int posx, int posy)
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
    newThread->lock = 0;
    newThread->Posx = posx;
    newThread->Posy = posy;
    newThread->partner = 0;
    return newThread;
}
warrior_ptr GetThreadW( long idThread){
    warrior_ptr nextNode = GetCurrentThreadW(warriorQueue1);
    warrior_ptr head = nextNode;

    while((nextNode!=null))
    {
        if(nextNode->id == idThread) {
            break;
        }
        nextNode = nextNode->next;
        if (head == nextNode){

           return null;
        }

    }
    return nextNode;
}


warrior_ptr checkCollision(warrior_ptr warrior, int samePlayer){
    warrior_ptr nextNode = GetCurrentThreadW(warriorQueue1);
    warrior_ptr head = nextNode;

    while((nextNode!=null))
    {

        if(warrior->player != samePlayer){
            if((warrior->Posx+2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy) && (samePlayer!= nextNode->player) && (warrior->warrior->screen == nextNode->warrior->screen) && (warrior->player == 1)){

                return nextNode;
            }else if((warrior->Posx-2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy)&& (samePlayer != nextNode->player) &&(warrior->warrior->screen == nextNode->warrior->screen) && (warrior->player == 2)){
                return nextNode;
            }
        }else{
            if((warrior->Posx+2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy) && (samePlayer!= nextNode->player) && (warrior->warrior->screen == nextNode->warrior->screen)){

                return nextNode;
            } else if((warrior->Posx-2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy)&& (samePlayer != nextNode->player) &&(warrior->warrior->screen == nextNode->warrior->screen)) {
                return nextNode;
            }
        }
        if((warrior->Posx == nextNode->Posx) &&(warrior->Posy+2 == nextNode->Posy)&& (samePlayer != nextNode->player) && (warrior->warrior->screen == nextNode->warrior->screen)){
            return nextNode;
        }
        else if((warrior->Posx == nextNode->Posx) &&(warrior->Posy-2 == nextNode->Posy)&& (samePlayer != nextNode->player) && (warrior->warrior->screen == nextNode->warrior->screen)){
            return nextNode;
        }


        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
    return null;
}
void checkTowerCollision(warrior_ptr warrior,struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){



    if((warrior->Posx+2 == tower4->posX) && ((warrior->Posy == tower4->posY+1) || (warrior->Posy == tower4->posY+2)) && (warrior->player ==1) && (warrior->warrior->screen==2)){

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
    else if((warrior->Posx+2 == tower5->posX) && ((warrior->Posy == tower5->posY) || (warrior->Posy == tower5->posY+1)) && (warrior->player ==1) && (warrior->warrior->screen==2)){

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
    else if((warrior->Posx+2 == tower6->posX) && ((warrior->Posy == tower6->posY) || (warrior->Posy == tower6->posY+1)) && (warrior->player ==1) && (warrior->warrior->screen==2)){

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
    else if((warrior->Posx-2 <= tower1->posX+2) && ((warrior->Posy == tower1->posY+1) || (warrior->Posy == tower1->posY+2)) && (warrior->player ==2) && (warrior->warrior->screen==1) ){

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
    else if((warrior->Posx-2 <= tower2->posX+2) && ((warrior->Posy == tower2->posY) || (warrior->Posy == tower2->posY+1)) && (warrior->player ==2) && (warrior->warrior->screen==1)){

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
    else if((warrior->Posx-2 <= tower3->posX+2) && ((warrior->Posy == tower3->posY) || (warrior->Posy == tower3->posY+1)) && (warrior->player ==2) && (warrior->warrior->screen==1)){
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




void moveWarrior(int nextMove, Warrior *warrior, warrior_ptr node, int * stepX){

    char * lvl = arr[warrior->level];

    int player = node->player;
    int idWarrior = node->id;
    int partner = node->partner;
    warrior_ptr checkAlly = null;
    if (node->player == 1){
        checkAlly = checkCollision(node, 2);
    }else{
        checkAlly = checkCollision(node, 1);
    }

    if(checkAlly){
        *stepX-=1;
        return;
    }

    warrior_ptr check = checkCollision(node, node->player);

    if (check == null) {

        switch (nextMove) {
            case 1://movimiento a la derecha

                if ((node->Posx + 2 < screen1->_maxx) && (warrior->screen == 1)) {

                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen1, node->Posy, node->Posx + 1, warrior->name);
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, lvl);
                    mvwprintw(screen1, node->Posy, node->Posx + 2, ">");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 2, ">");
                    //se tiene que actualizar la posicion del warrior->x
                    node->Posx = node->Posx + 1;


                }
                //se cambia a movimientos en pantalla 2 a la derecha
                if ((node->Posx + 2 >= screen1->_maxx) && (warrior->screen == 1)) {
                    node->warrior->screen = 2;
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    node->Posx = 2;
                    mvwprintw(screen2, node->Posy, node->Posx, warrior->name);
                    mvwprintw(screen2, node->Posy + 1, node->Posx, lvl);
                    mvwprintw(screen2, node->Posy, node->Posx + 1, ">");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, ">");
                    wrefresh(screen2);
                }
                if ((warrior->screen == 2) && (node->Posx + 2 < tower5.posX)) { //-------------------------------------------aqui se tiene que hacer la validacion de que dibuje solo si el puente está habilitado
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen2, node->Posy, node->Posx + 1, warrior->name);
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, lvl);
                    mvwprintw(screen2, node->Posy, node->Posx + 2, ">");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 2, ">");
                    node->Posx = node->Posx + 1;
                    wrefresh(screen2);
                }
                break;
            case 2://movimiento a la izquierda
                //validar si en la posicion en donde se va a mover no hay nadie
                if ((node->Posx - 2 > 0) && (warrior->screen == 2)) {
                    //se imprime la nueva posicion
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen2, node->Posy, node->Posx - 2, "<");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 2, "<");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, warrior->name);
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, lvl);
                    node->Posx = node->Posx - 1;
                    wrefresh(screen2);
                }
                if ((node->Posx - 2 <= 0) && (warrior->screen == 2)) {
                    node->warrior->screen = 1;
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    wrefresh(screen2);
                    node->Posx = screen1->_maxx-2;
                    //se imprime la nueva posicion
                    mvwprintw(screen1, node->Posy, node->Posx, warrior->name);
                    mvwprintw(screen1, node->Posy + 1, node->Posx, lvl);
                    mvwprintw(screen1, node->Posy, node->Posx - 1, "<");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, "<");
                    wrefresh(screen1);
                }
                if ((warrior->screen == 1) && (node->Posx - 2 > tower2.posX + 2)) {
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");
                    //se imprime la nueva posicion
                    mvwprintw(screen1, node->Posy, node->Posx - 2, "<");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 2, "<");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, warrior->name);
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, lvl);
                    node->Posx = node->Posx - 1;
                    wrefresh(screen1);
                }
                break;
            case 3://movimiento hacia arriba
                if (warrior->screen == 1) {
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy - 2 > 0) {
                        //se imprime la nueva posicion
                        mvwprintw(screen1, node->Posy - 2, node->Posx, "/");
                        mvwprintw(screen1, node->Posy - 2, node->Posx + 1, "\\");
                        mvwprintw(screen1, node->Posy - 1, node->Posx, warrior->name);
                        mvwprintw(screen1, node->Posy - 1, node->Posx + 1, lvl);
                        node->Posy = node->Posy - 1;
                    } else {
                        mvwprintw(screen1, node->Posy - 1, node->Posx, "/");
                        mvwprintw(screen1, node->Posy - 1, node->Posx + 1, "\\");
                        mvwprintw(screen1, node->Posy, node->Posx, warrior->name);
                        mvwprintw(screen1, node->Posy, node->Posx + 1, lvl);
                    }
                } else if (warrior->screen == 2) {
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy - 2 > 0) {
                        //se imprime la nueva posicion
                        mvwprintw(screen2, node->Posy - 2, node->Posx, "/");
                        mvwprintw(screen2, node->Posy - 2, node->Posx + 1, "\\");
                        mvwprintw(screen2, node->Posy - 1, node->Posx, warrior->name);
                        mvwprintw(screen2, node->Posy - 1, node->Posx + 1, lvl);
                        node->Posy = node->Posy - 1;
                    } else {
                        mvwprintw(screen2, node->Posy - 1, node->Posx, "/");
                        mvwprintw(screen2, node->Posy - 1, node->Posx + 1, "\\");
                        mvwprintw(screen2, node->Posy, node->Posx, warrior->name);
                        mvwprintw(screen2, node->Posy, node->Posx + 1, lvl);
                    }
                }


                break;
            case 4://movimiento hacia abajo
                if (warrior->screen == 1) {
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy + 2 < screen1->_maxy) {
                        //se imprime la nueva posicion
                        mvwprintw(screen1, node->Posy + 1, node->Posx, warrior->name);
                        mvwprintw(screen1, node->Posy + 1, node->Posx + 1, lvl);
                        mvwprintw(screen1, node->Posy + 2, node->Posx, "\\");
                        mvwprintw(screen1, node->Posy + 2, node->Posx + 1, "/");
                        node->Posy = node->Posy + 1;
                    } else {
                        mvwprintw(screen1, node->Posy, node->Posx, warrior->name);
                        mvwprintw(screen1, node->Posy, node->Posx + 1, lvl);
                        mvwprintw(screen1, node->Posy + 1, node->Posx, "\\");
                        mvwprintw(screen1, node->Posy + 1, node->Posx + 1, "/");
                    }
                } else if (warrior->screen == 2) {
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy + 2 < screen1->_maxy) {
                        //se imprime la nueva posicion
                        mvwprintw(screen2, node->Posy + 1, node->Posx, warrior->name);
                        mvwprintw(screen2, node->Posy + 1, node->Posx + 1, lvl);
                        mvwprintw(screen2, node->Posy + 2, node->Posx, "\\");
                        mvwprintw(screen2, node->Posy + 2, node->Posx + 1, "/");
                        node->Posy = node->Posy + 1;
                    } else {
                        mvwprintw(screen2, node->Posy, node->Posx, warrior->name);
                        mvwprintw(screen2, node->Posy, node->Posx + 1, lvl);
                        mvwprintw(screen2, node->Posy + 1, node->Posx, "\\");
                        mvwprintw(screen2, node->Posy + 1, node->Posx + 1, "/");
                    }
                }

                break;
            default:
                break;
        }
    }
    else{
        my_mutex_unlock(&lock);
        my_mutex_lock(&battleLock);
        my_mutex_lock(&lock);
        warrior_ptr checkExist = GetThreadW(idWarrior);
        if(checkExist == null){
            my_mutex_unlock(&battleLock);
            my_mutex_unlock(&lock);
            my_mutex_unlock(&bridge1Lock);
            my_mutex_unlock(&bridge2Lock);
            if(partnerBridge1 == player && partner == 1)
                partnerBridge1 = 0;
            if(partnerBridge2 == player && partner == 1)
                partnerBridge2 = 0;
            my_thread_exit();
        }else{
            if(check->warrior->screen == 1){
//                mvwprintw(screen1, checkExist->Posy, checkExist->Posx, " ");
//                mvwprintw(screen1, checkExist->Posy + 1, checkExist->Posx, " ");
                mvwprintw(screen1, checkExist->Posy, checkExist->Posx+1, " ");
                mvwprintw(screen1, checkExist->Posy + 1, checkExist->Posx+1, " ");
                mvwprintw(screen1, checkExist->Posy, checkExist->Posx-1, " ");
                mvwprintw(screen1, checkExist->Posy + 1, checkExist->Posx-1, " ");
            }
            else{
//                mvwprintw(screen2, checkExist->Posy, checkExist->Posx, " ");
//                mvwprintw(screen2, checkExist->Posy + 1, checkExist->Posx, " ");
                mvwprintw(screen2, checkExist->Posy, checkExist->Posx+1, " ");
                mvwprintw(screen2, checkExist->Posy + 1, checkExist->Posx+1, " ");
                mvwprintw(screen2, checkExist->Posy, checkExist->Posx-1, " ");
                mvwprintw(screen2, checkExist->Posy + 1, checkExist->Posx-1, " ");
            }
            *stepX-=1;
            sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
            PopNode_QueueW(warriorQueue1,check);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            my_mutex_unlock(&battleLock);
        }
    }

};


void createTable(int opcion){

    initscr();
    iniciar();
    keypad(stdscr,TRUE);
    cbreak();
    //noecho();
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
        width = 28;
    }
    else if( opcion == 2){
        width = 34;
    }
    createTowers(opcion,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);//se crean las 3 torres
    screen1 = newwin(height, width, y, x);
    screen2 = newwin(height, width, y, width);
    terminal = newwin(9, width, height+1, x);
    terminal2 = newwin(9, width, height+1, width);
    wrefresh(screen1);
    wrefresh(screen2);
    wrefresh(terminal);
    wrefresh(terminal2);
    box(screen1,0,0);
    box(screen2,0,0);
    box(terminal,0,0);
    box(terminal2,0,0);
    wrefresh(terminal);
    wrefresh(terminal2);
    //-------------------------------------------------------------------------------bridge screen1
    int count = 0;
    while(count < 7){
        mvwprintw(screen1,(height/2)-5,screen1->_maxx-count,"-");
        mvwprintw(screen1,(height/2)-1,screen1->_maxx-count,"-");
        mvwprintw(screen1,(height/2)+1,screen1->_maxx-count,"-");
        mvwprintw(screen1,(height/2)+5,screen1->_maxx-count,"-");
        //-------------------------------------------------------------------------------bridge screen2
        mvwprintw(screen2,(height/2)-5,count,"-");
        mvwprintw(screen2,(height/2)-1,count,"-");
        mvwprintw(screen2,(height/2)+1,count,"-");
        mvwprintw(screen2,(height/2)+5,count,"-");
        count += 1;
    }
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

    printWarriorList();
    //Warrior warrior2;
    //Warrior warrior3;

    //initValues(&warrior2,100,10,10,5,"X",26,3,2,0);
    //initValues(&warrior3,200,100,10,5,"T",1,3,1,0);

    //Push_QueueW(warriorQueue1,NewThreadW(&warrior1,1));
    //Push_QueueW(warriorQueue1,NewThreadW(&warrior2,2));
    //Push_QueueW(warriorQueue1,NewThreadW(&warrior3,1));


    //nodelay(stdscr, TRUE);
    //key = getch();


    sleep(1/2);
    my_mutex_init(&lock);
    my_mutex_init(&battleLock);
    my_mutex_init(&bridge1Lock);
    my_mutex_init(&bridge2Lock);
    my_mutex_init(&partner1Lock);
    my_mutex_init(&partner2Lock);
    my_thread_t t1;
    my_thread_t t2;
    my_thread_t t3;
    my_thread_t t4;
    my_thread_t t5;
    my_thread_init(100);

//    struct Params * par = (struct Params *)malloc(sizeof(struct Params));
//    struct Params * par2 = (struct Params *)malloc(sizeof(struct Params));
//    struct Params * par3 = (struct Params *)malloc(sizeof(struct Params));
//    par->width = width;
//    par->node = GetThreadW(1);
//    par->warrior = &warrior1;
//    par->nextMove = 1;
//    //bombWarrior(&warrior1);
//    my_thread_create(&t1,movePlayer1,(void*)par, 2);

    wrefresh(screen1);//se refresca la ventana
    wrefresh(screen2);
    wrefresh(terminal);
    wrefresh(terminal2);
//    par2->width = width;
//    par2->node = GetThreadW(2);
//    par2->warrior = &warrior2;
//    par2->nextMove = 2;
//    //bombWarrior(&warrior2);
//    my_thread_create(&t2,movePlayer1,(void*)par2, 1);
//    //my_thread_sleep(3);
//    //exitWarriorThread(1);
//
//    par3->width = width;
//    par3->node = GetThreadW(3);
//    par3->warrior = &warrior3;
//    par3->nextMove = 1;
//    my_thread_create(&t3,movePlayer1,(void*)par3, 1);
//    my_thread_sleep(20);
//    Warrior warrior4;
//    Warrior warrior5;
//    initValues(&warrior4,200,100,10,5,"P",6,3,1,0);
//    initValues(&warrior5,200,100,10,5,"T",1,3,1,0);
//    Push_QueueW(warriorQueue1,NewThreadW(&warrior4,1));
//    Push_QueueW(warriorQueue1,NewThreadW(&warrior5,1));
//    struct Params * par4 = (struct Params *)malloc(sizeof(struct Params));
//    struct Params * par5 = (struct Params *)malloc(sizeof(struct Params));
//
//    par4->width = width;
//    par4->node = GetThreadW(4);
//    par4->warrior = &warrior4;
//    par4->nextMove = 1;
//
//    par5->width = width;
//    par5->node = GetThreadW(5);
//    par5->warrior = &warrior5;
//    par5->nextMove = 1;
//    my_thread_create(&t4,movePlayer1,(void*)par4, 1);
//    my_thread_create(&t5,movePlayer1,(void*)par5, 1);

    my_mutex_lock(&bridge1Lock);
    partnerBridge1 = 1;

    int key;
    int posicion ;
    //nodelay(terminal, TRUE);
    //-----------------------------------------------
    my_mutex_lock(&lock);

    mvwprintw(terminal,1,2,"Warrior seleccionado: ");
    wmove(terminal,1,25);
    key = wgetch(terminal);

    mvwprintw(terminal,2,2,"posición a desplegar: ");
    wrefresh(terminal);
    mvwprintw(terminal,2,2,"posición a desplegar: ");
    wmove(terminal,2,25);
    posicion = wgetch(terminal);
    if(posicion == 49){
        posicion = 3;
    }
    else{
        posicion = 9;
    }
    //printf("%d",posicion);
    wrefresh(terminal);
    my_mutex_unlock(&lock);
    //-----------------------------------------------
    warrior_ptr prueba = selectWarrior(key, posicion);
    my_thread_sleep(5);
    my_thread_t thread2;
    struct Params * par2 = (struct Params *)malloc(sizeof(struct Params));

    Warrior warrior2;
    initValues(&warrior2,50,10,10,5,"R",1,0);
    warrior_ptr pointer2 = NewThreadW(&warrior2, 1, 6, 3);
    Push_QueueW(warriorQueue1, pointer2);
    par2->width = screen1->_maxx;
    par2->node = pointer2;
    par2->warrior = &warrior2;
    par2->nextMove = 1;
    my_thread_create(&thread2,movePlayer1,(void*)par2, 0);

    my_thread_sleep(30);
    my_mutex_unlock(&bridge1Lock);
/*    Warrior * prueba = selectWarrior(key,3,1);

    Push_QueueW(warriorQueue1,NewThreadW(prueba,1, 2, 3));
    struct Params * par4 = (struct Params *)malloc(sizeof(struct Params));
    par4->width = width;
    par4->node = GetThreadW(countwarriors);
    par4->warrior = prueba;
    par4->nextMove = 2;
    my_thread_create(&t4,movePlayer1,(void*)par4, 1);*/

    //-----------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------
//    prueba = selectWarrior(114,9,1);
//
//    Push_QueueW(warriorQueue1,NewThreadW(prueba,1));
//    par4 = (struct Params *)malloc(sizeof(struct Params));
//    par4->width = width;
//    par4->node = GetThreadW(countwarriors);
//    par4->warrior = prueba;
//    par4->nextMove = 1;
//    my_thread_create(&t5,movePlayer1,(void*)par4, 1);


    while(1){
        int terminar =decidirGanador(&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
        if(terminar == 1){
            break;
        }};

    wrefresh(screen1);//se refresca la ventana
    wrefresh(screen2);
    wrefresh(terminal);
    wrefresh(terminal2);
    wgetch(screen1);
    delwin(screen1);
    delwin(screen2);
    delwin(terminal);
    delwin(terminal2);
};
//el deploy en el INIT tiene que ser en el x = 6
void* movePlayer1(void * parameters){

    int  width, nextMove, entryBridge, exitBridge;
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
    if (width <= 29){
        pathLength =38;
        entryBridge = 10;
        exitBridge = 12;
    }
    if (width > 30){
        pathLength =51;
        entryBridge = 18;
        exitBridge = 26;
    }
    wrefresh(screen1);
    wrefresh(screen2);
    while(stepsX < pathLength ){//solo tiene que moverse a la derecha
        if (time(0) > t1 && warrior->bomb == 1){
            my_mutex_lock(&lock);
            if(node->player == 1){
                if(warrior->screen == 1){
                    mvwprintw(screen1,node->Posy,node->Posx,"X");
                    mvwprintw(screen1,node->Posy+1,node->Posx,"X");
                    mvwprintw(screen1,node->Posy,node->Posx+1,"X");
                    mvwprintw(screen1,node->Posy+1,node->Posx+1,"X");

                    wrefresh(screen1);
                }
                else{
                    mvwprintw(screen2,node->Posy,node->Posx,"X");
                    mvwprintw(screen2,node->Posy+1,node->Posx,"X");
                    mvwprintw(screen2,node->Posy,node->Posx+1,"X");
                    mvwprintw(screen2,node->Posy+1,node->Posx+1,"X");
                    wrefresh(screen2);
                }
            }
            else{
                if(warrior->screen == 1){
                    mvwprintw(screen1,node->Posy,node->Posx,"X");
                    mvwprintw(screen1,node->Posy+1,node->Posx,"X");
                    mvwprintw(screen1,node->Posy,node->Posx-1,"X");
                    mvwprintw(screen1,node->Posy+1,node->Posx-1,"X");
                    wrefresh(screen1);
                }
                else{
                    mvwprintw(screen2,node->Posy,node->Posx,"X");
                    mvwprintw(screen2,node->Posy+1,node->Posx,"X");
                    mvwprintw(screen2,node->Posy,node->Posx-1,"X");
                    mvwprintw(screen2,node->Posy+1,node->Posx-1,"X");
                    wrefresh(screen2);
                }
            }

            my_thread_sleep(1);
            cleanWarrior( warrior, node);
            my_mutex_unlock(&battleLock);
            my_mutex_unlock(&lock);
            my_mutex_unlock(&bridge1Lock);
            my_mutex_unlock(&bridge2Lock);
            if(partnerBridge1 == node->player && node->partner == 1)
                partnerBridge1 = 0;
            if(partnerBridge2 == node->player && node->partner == 1)
                partnerBridge2 = 0;
            my_thread_exit();
        }

        if(stepsX == entryBridge) {
            if (node->Posy > 6) {
                if(partnerBridge2 == node->player){
                    if(my_mutex_trylock(&partner2Lock) == false){
                        continue;
                    }
                    node->partner = 1;
                }else{
                    if(partnerBridge2 == 0){
                        my_mutex_lock(&bridge2Lock);
                        node->lock = 1;
                        partnerBridge2 = node->player;
                    }else{
                        continue;
                    }

                }

            } else {
                if(partnerBridge1 == node->player){
                    if(my_mutex_trylock(&partner1Lock) == false){
                        continue;
                    }
                    node->partner = 1;
                }else{
                    if(partnerBridge1 == 0) {
                        my_mutex_lock(&bridge1Lock);
                        node->lock = 1;
                        partnerBridge1 = node->player;
                    }else{
                        continue;
                    }
                }
            }
        }



        if(stepsX == exitBridge){
            if (node->Posy > 6) {
                if(node->lock == 1){
                    node->lock = 0;
                    if(my_mutex_trylock(&partner2Lock)){
                        my_mutex_unlock(&partner2Lock);
                        partnerBridge2 = 0;
                    }
                    my_mutex_unlock(&bridge2Lock);
                }else{
                    node->partner = 0;
                    partnerBridge2 = 0;
                    my_mutex_unlock(&partner2Lock);
                }


            } else {
                if(node->lock == 1){
                    node->lock = 0;
                    if(my_mutex_trylock(&partner1Lock)){
                        my_mutex_unlock(&partner1Lock);
                        partnerBridge1 = 0;
                    }
                    my_mutex_unlock(&bridge1Lock);

                }else{
                    node->partner = 0;
                    partnerBridge1 = 0;
                    my_mutex_unlock(&partner1Lock);
                }
            }
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
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        else if(node->player == 1 && warrior->screen == 2){
            wattron(screen2, COLOR_PAIR(1));
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        if(node->player == 1 && warrior->screen == 1){
            wattroff(screen1, COLOR_PAIR(1));
        }
        else if(node->player == 1 && warrior->screen == 2){
            wattroff(screen2, COLOR_PAIR(1));
        }
        //-------------------------------------------------
        if(node->player == 2 && warrior->screen == 1){
            wattron(screen1, COLOR_PAIR(2));
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        else if(node->player == 2 && warrior->screen == 2){
            wattron(screen2, COLOR_PAIR(2));
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        if(node->player == 2 && warrior->screen == 1){
            wattroff(screen1, COLOR_PAIR(2));
        }
        else if(node->player == 2 && warrior->screen == 2){
            wattroff(screen2, COLOR_PAIR(2));
        }
        my_mutex_unlock(&lock);
        my_thread_sleep(1);
        stepsX +=1;
        //refresh();
        wrefresh(screen1);
        wrefresh(screen2);
    }

    int down = 0;
    int up = 0;
    if(node->Posy < (screen1->_maxy)/2){
        while(down <3){
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
                moveWarrior(4,warrior,node, &down);
            }
            else if(node->player == 1 && warrior->screen == 2){
                wattron(screen2, COLOR_PAIR(1));
                moveWarrior(4,warrior,node, &down);
            }
            if(node->player == 1 && warrior->screen == 1){
                wattroff(screen1, COLOR_PAIR(1));
            }
            else if(node->player == 1 && warrior->screen == 2){
                wattroff(screen2, COLOR_PAIR(1));
            }
            //-------------------------------------------------
            if(node->player == 2 && warrior->screen == 1){
                wattron(screen1, COLOR_PAIR(2));
                moveWarrior(4,warrior,node, &down);
            }
            else if(node->player == 2 && warrior->screen == 2){
                wattron(screen2, COLOR_PAIR(2));
                moveWarrior(4,warrior,node, &down);
            }
            if(node->player == 2 && warrior->screen == 1){
                wattroff(screen1, COLOR_PAIR(2));
            }
            else if(node->player == 2 && warrior->screen == 2){
                wattroff(screen2, COLOR_PAIR(2));
            }
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
            wrefresh(screen1);
            wrefresh(screen2);
            down +=1;
        }
    }
    else{
        while(up <3){
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
                moveWarrior(3,warrior,node, &up);
            }
            else if(node->player == 1 && warrior->screen == 2){
                wattron(screen2, COLOR_PAIR(1));
                moveWarrior(3,warrior,node, &up);
            }
            if(node->player == 1 && warrior->screen == 1){
                wattroff(screen1, COLOR_PAIR(1));
            }
            else if(node->player == 1 && warrior->screen == 2){
                wattroff(screen2, COLOR_PAIR(1));
            }
            //-------------------------------------------------
            if(node->player == 2 && warrior->screen == 1){
                wattron(screen1, COLOR_PAIR(2));
                moveWarrior(3,warrior,node, &up);
            }
            else if(node->player == 2 && warrior->screen == 2){
                wattron(screen2, COLOR_PAIR(2));
                moveWarrior(3,warrior,node, &up);
            }
            if(node->player == 2 && warrior->screen == 1){
                wattroff(screen1, COLOR_PAIR(2));
            }
            else if(node->player == 2 && warrior->screen == 2){
                wattroff(screen2, COLOR_PAIR(2));
            }
            my_mutex_unlock(&lock);
            my_thread_sleep(1);
            wrefresh(screen1);
            wrefresh(screen2);
            up +=1;
        }
    }

    return null;
};
int decidirGanador(struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){
    my_mutex_lock(&lock);
    if((tower2->health <= 0) )
    {
        //clear();

        mvwprintw(screen2,4,10,"GANADOR ");
        wrefresh(screen1);//se refresca la ventana
        wrefresh(screen2);
        my_mutex_unlock(&lock);
        return 1;
    }
    else if((tower5->health <= 0))
    {
        //clear();
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
        mvwprintw(screen1,node->Posy,node->Posx," ");
        mvwprintw(screen1,node->Posy+1,node->Posx," ");
        mvwprintw(screen1,node->Posy,node->Posx+1," ");
        mvwprintw(screen1,node->Posy+1,node->Posx+1," ");
        mvwprintw(screen1,node->Posy,node->Posx-1," ");
        mvwprintw(screen1,node->Posy+1,node->Posx-1," ");
    }
    else{
        mvwprintw(screen2,node->Posy,node->Posx," ");
        mvwprintw(screen2,node->Posy+1,node->Posx," ");
        mvwprintw(screen2,node->Posy,node->Posx+1," ");
        mvwprintw(screen2,node->Posy+1,node->Posx+1," ");
        mvwprintw(screen2,node->Posy,node->Posx-1," ");
        mvwprintw(screen2,node->Posy+1,node->Posx-1," ");
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

void printWarriorList(){
    mvwprintw(terminal,3,2,"Teclas jugador 1");
    mvwprintw(terminal,5,(terminal->_maxx)/4-2,"R");

    mvwprintw(terminal,5,(terminal->_maxx/4)*2-3,"T");

    mvwprintw(terminal,5,(terminal->_maxx/4)*3-3,"Y");

    mvwprintw(terminal,5,(terminal->_maxx)-4,"U");

    wrefresh(terminal);
    //---------------------------
    mvwprintw(terminal2,3,2,"Teclas jugador 2");
    mvwprintw(terminal2,5,(terminal2->_maxx)/4-2,"F");

    mvwprintw(terminal2,5,(terminal2->_maxx/4)*2-3,"G");

    mvwprintw(terminal2,5,(terminal2->_maxx/4)*3-3,"H");

    mvwprintw(terminal2,5,(terminal2->_maxx)-4,"J");

    wrefresh(terminal2);
}

warrior_ptr selectWarrior( int opcion,int localizacion){
    warrior_ptr pointer;
    switch (opcion) {
        case  114:// r
            initValues(&warrior,200,100,10,5,"R",1,0);
            pointer = NewThreadW(&warrior, 1, 6, localizacion);
            Push_QueueW(warriorQueue1, pointer);
            break;
        case  116:// t
            initValues(&warrior,50,10,10,5,"T",1,0);
            pointer = NewThreadW(&warrior, 1, 6, localizacion);
            Push_QueueW(warriorQueue1, pointer);
            break;
        case  121:// y
            initValues(&warrior,50,10,10,5,"Y",1,0);
            pointer = NewThreadW(&warrior, 1, 6, localizacion);
            Push_QueueW(warriorQueue1, pointer);
            break;
        case  117:// u          ------> bomba del P1
            initValues(&warrior,50,10,10,5,"U",1,1);
            pointer = NewThreadW(&warrior, 1, 6, localizacion);
            bombWarrior(&warrior);
            Push_QueueW(warriorQueue1, pointer);
            break;
        case  102:// f
            initValues(&warrior,50,10,10,5,"F",2,0);
            pointer = NewThreadW(&warrior, 2, screen2->_maxx-6, localizacion);
            Push_QueueW(warriorQueue1, pointer);
            break;
        case  103:// g
            initValues(&warrior,50,10,10,5,"G",2,0);
            pointer = NewThreadW(&warrior, 2, screen2->_maxx-6, localizacion);
            Push_QueueW(warriorQueue1, pointer);
            break;
        case  104:// h
            initValues(&warrior,50,10,10,5,"H",2,0);
            pointer = NewThreadW(&warrior, 2, screen2->_maxx-6, localizacion);
            Push_QueueW(warriorQueue1, pointer);
            break;
        case  106:// j          ------> bomba del P2
            initValues(&warrior,50,10,10,5,"J",2,1);
            pointer = NewThreadW(&warrior, 2, screen2->_maxx-6, localizacion);
            bombWarrior(&warrior);
            Push_QueueW(warriorQueue1, pointer);
            break;
    }

    if(pointer->player == 1){
        my_thread_t thread;
        struct Params * par4 = (struct Params *)malloc(sizeof(struct Params));
        par4->width = screen1->_maxx;
        par4->node = pointer;
        par4->warrior = pointer->warrior;
        par4->nextMove = 1;
        my_thread_create(&thread,movePlayer1,(void*)par4, 1);
    }
    else{
        my_thread_t thread;
        struct Params * par4 = (struct Params *)malloc(sizeof(struct Params));
        par4->width = screen1->_maxx;
        par4->node = pointer;
        par4->warrior = pointer->warrior;
        par4->nextMove = 2;
        my_thread_create(&thread,movePlayer1,(void*)par4, 1);
    }
    return pointer;
}






