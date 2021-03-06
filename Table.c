//
// Created by ericksalazargranera on 5/6/21.
//

#include <strings.h>
#include "Table.h"
#include "time.h"
#include "my_mutex.h"

//player1 Towers
struct Tower tower1;
struct Tower tower2;
struct Tower tower3;
//player2 Towers
struct Tower tower4;
struct Tower tower5;
struct Tower tower6;
//Times to start the bomb timer and when to explode
int entry_limit = 0;
int depart_limit = 0;
//damage that deals the bomb
int damage = 0;
//variables needed to destroy the threads
extern sigset_t sigProcMask;
//list of warriors that are alive
warriorQueue warriorQueue1;
//list of warriors needed to make interativity posible in this project
Warrior warriors[8];
Warrior warrior1;
//creating the windows for the game
WINDOW *screen1;
WINDOW *screen2;
WINDOW *terminal;
WINDOW *terminal2;
//variables needed to manage the mutex
int lock, battleLock, bridge1Lock, bridge2Lock, partner1Lock, partner2Lock;
int partnerBridge1, partnerBridge2 = 0;
//variables needed to make mutations
Warrior *lastDeadWarrior1 = null;
Warrior *lastDeadWarrior2 = null;
//struct needed to use the movePlayer with the thread
struct Params{
    int nextMove;
    Warrior * warrior;
    warrior_ptr node;
    int width;
};

//array used to set the level of the warrior
char* arr[21] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"
};

extern ini_t *config;

//function to initializes all  kinds of warriors and add them to a list
int start(){
    int check = initValues(&warrior1,200,100,10,5,"P",0);
    Warrior warrior0;
    Warrior warrior11;
    Warrior warrior2;
    Warrior warrior3;
    Warrior warrior4;
    Warrior warrior5;
    Warrior warrior6;
    Warrior warrior7;
    initValues(&warrior0,50,100,10,5,"R",0);
    initValues(&warrior11,50,100,10,5,"T",0);
    initValues(&warrior2,50,100,10,5,"Y",0);
    initValues(&warrior3,50,100,10,5,"U",1);
    initValues(&warrior4,50,100,10,5,"F",0);
    initValues(&warrior5,50,100,10,5,"G",0);
    initValues(&warrior6,50,100,10,5,"H",0);
    initValues(&warrior7,50,100,10,5,"J",1);
    bombWarrior(&warrior7);
    bombWarrior(&warrior3);
    warriors[0] = warrior0;
    warriors[1] = warrior11;
    warriors[2] = warrior2;
    warriors[3] = warrior3;
    warriors[4] = warrior4;
    warriors[5] = warrior5;
    warriors[6] = warrior6;
    warriors[7] = warrior7;
    warriorQueue1 = GetThreadQueueW();
    return check;
}
//function that initializes all the values from the towers
void createTowers(int opcion, struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){
    //---------------------------------------------------player1-----------------------
    tower1->health = 500;//health
    tower1->towerSymbol = "-";//tower figure
    tower1->posX = 1; //X position to guide the display
    tower1->posY = 1; //X position to guide the display
    tower1->attack = 15;//the amount of damage that the tower deals
    //=====================
    tower2->health = 600;
    tower2->towerSymbol = "-";
    tower2->posX = 1;
    tower2->posY = 5;
    tower2->attack = 15;
    //=====================
    tower3->health = 500;
    tower3->towerSymbol = "-";
    tower3->posX = 1;
    tower3->posY = 9;
    tower3->attack = 15;
    //---------------------------------------------------player2-----------------------
    int val;
    //we need a validation of the size of the window in order to draw the towers in the right position
    if(opcion == 1){
        val = 24;
    }
    else if (opcion == 2){
        val = 30;
    }
    tower4->health = 500;
    tower4->towerSymbol = "-";
    tower4->posX = val;
    tower4->posY = 1;
    tower4->attack = 15;
    //=====================
    tower5->health = 600;
    tower5->towerSymbol = "-";
    tower5->posX = val;
    tower5->posY = 5;
    tower5->attack = 15;
    //=====================
    tower6->health = 500;
    tower6->towerSymbol = "-";
    tower6->posX = val;
    tower6->posY = 9;
    tower6->attack = 15;
};
//This will take the current warrior of a warrior list
warrior_ptr GetCurrentThreadW(warriorQueue queue)
{
    if(queue == null)
    {
        return null;
    }
    return queue->head;
}

//This will remove the warrior which is pointed by head of the given queue
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

//Moves head of queue
int moveHeadW(warriorQueue queue)
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

//takes the next warrior id
int GetNextThreadIdW()
{
    static long currentThreadID=0;
    return ++currentThreadID;
}

//add a warriorNode to a warrior list
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
//pops a warrior from a warrior list
void PopNode_QueueW(warriorQueue queue, warrior_ptr node)
{
    warrior_ptr nextNode = GetCurrentThreadW(queue);
    warrior_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->id == node->id)
            break;
        else{
            moveHeadW(queue);
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
//Initialize new warrior queue
warriorQueue GetThreadQueueW()
{
    warriorQueue newQueue = (warriorQueue)malloc(sizeof(struct warriorQueue));
    if(newQueue == null)
    {
        return null;
    }
    newQueue->count = 0;
    newQueue->head = newQueue->head_parent = null;
    return newQueue;
}

//creates a new warriorNode. Initialize every attribute needed
warrior_ptr NewThreadW(Warrior * warrior, int player, int posx, int posy, int screen)
{
    warrior_ptr newThread = (warrior_ptr)malloc(sizeof(war));
    if(newThread == null)
    {

        return null;
    }
    newThread->next = null;
    newThread->warrior = warrior;
    newThread->id = GetNextThreadIdW();
    newThread->player = player;
    newThread->lock = 0;
    newThread->Posx = posx;
    newThread->Posy = posy;
    newThread->partner = 0;
    newThread->vitality = warrior->vitality;
    newThread->screen = screen;
    return newThread;
}
//gets a warrior from a warriorList by its ID
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

//checks the collision between warriors, return the warrior that is in front of the current warrior
warrior_ptr checkCollision(warrior_ptr warrior, int samePlayer){
    warrior_ptr nextNode = GetCurrentThreadW(warriorQueue1);
    warrior_ptr head = nextNode;
    //goes through all the list of warriors looking if anyone is right in front of the current warrior
    while((nextNode!=null))
    {
        //also checks if the current warrior is in front of other warrior of the same player so it doesnt step over him
        if(warrior->player != samePlayer){
            if((warrior->Posx+2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy) && (samePlayer!= nextNode->player) && (warrior->screen == nextNode->screen) && (warrior->player == 1)){

                return nextNode;
            }else if((warrior->Posx-2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy)&& (samePlayer != nextNode->player) &&(warrior->screen == nextNode->screen) && (warrior->player == 2)){
                return nextNode;
            }
        }else{
            if((warrior->Posx+2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy) && (samePlayer!= nextNode->player) && (warrior->screen == nextNode->screen)){

                return nextNode;
            } else if((warrior->Posx-2 == nextNode->Posx) &&(warrior->Posy == nextNode->Posy)&& (samePlayer != nextNode->player) &&(warrior->screen == nextNode->screen)) {
                return nextNode;
            }
        }
        if((warrior->Posx == nextNode->Posx) &&(warrior->Posy+2 == nextNode->Posy)&& (samePlayer != nextNode->player) && (warrior->screen == nextNode->screen)){
            return nextNode;
        }
        else if((warrior->Posx == nextNode->Posx) &&(warrior->Posy-2 == nextNode->Posy)&& (samePlayer != nextNode->player) && (warrior->screen == nextNode->screen)){
            return nextNode;
        }


        nextNode = nextNode->next;
        if (head == nextNode)//
            break;
    }
    return null;
}
//This functions does almost the same comparisons as the checkCollision function, but this one is intended only for the bomb case
void checkBombCollision(warrior_ptr warrior){
    warrior_ptr nextNode = GetCurrentThreadW(warriorQueue1);
    warrior_ptr head = nextNode;

    while((nextNode!=null))
    {   // for each case, decrease the enemy life depending on the damage set to the bomb
        if(((warrior->Posx+2 == nextNode->Posx) || (warrior->Posx+3 == nextNode->Posx) || (warrior->Posx+4 == nextNode->Posx)) &&(warrior->Posy == nextNode->Posy) && (warrior->player!= nextNode->player) && (warrior->screen == nextNode->screen)){

            nextNode->vitality = nextNode->vitality - damage;
        } else if(((warrior->Posx-2 == nextNode->Posx) || (warrior->Posx-3 == nextNode->Posx) || (warrior->Posx-4 == nextNode->Posx)) &&(warrior->Posy == nextNode->Posy)&& (warrior->player != nextNode->player) &&(warrior->screen == nextNode->screen)) {
            nextNode->vitality = nextNode->vitality - damage;
        }
        if((warrior->Posx == nextNode->Posx) &&((warrior->Posy+2 == nextNode->Posy) || (warrior->Posy+3 == nextNode->Posy) || (warrior->Posy+4 == nextNode->Posy)) && (warrior->player != nextNode->player) && (warrior->screen == nextNode->screen)){
            nextNode->vitality = nextNode->vitality - damage;
        }
        else if((warrior->Posx == nextNode->Posx) &&((warrior->Posy-2 == nextNode->Posy) || (warrior->Posy-3 == nextNode->Posy) || (warrior->Posy-4 == nextNode->Posy))&& (warrior->player != nextNode->player) && (warrior->screen == nextNode->screen)){
            nextNode->vitality = nextNode->vitality - damage;
        }
        //if the health of the warrior that recieves the damage is <= 0, cleans the warrior
        if(nextNode->vitality<=0){
            if(nextNode->screen == 1){
                mvwprintw(screen1,nextNode->Posy,nextNode->Posx," ");
                mvwprintw(screen1,nextNode->Posy+1,nextNode->Posx," ");
                if(nextNode->player == 1){
                    mvwprintw(screen1,nextNode->Posy,nextNode->Posx+1," ");
                    mvwprintw(screen1,nextNode->Posy+1,nextNode->Posx+1," ");
                }
                else{
                    mvwprintw(screen1,nextNode->Posy,nextNode->Posx-1," ");
                    mvwprintw(screen1,nextNode->Posy+1,nextNode->Posx-1," ");
                }


            }
            else{
                mvwprintw(screen2,nextNode->Posy,nextNode->Posx," ");
                mvwprintw(screen2,nextNode->Posy+1,nextNode->Posx," ");
                if(nextNode->player == 1){
                    mvwprintw(screen2,nextNode->Posy,nextNode->Posx+1," ");
                    mvwprintw(screen2,nextNode->Posy+1,nextNode->Posx+1," ");
                }
                else{
                    mvwprintw(screen2,nextNode->Posy,nextNode->Posx-1," ");
                    mvwprintw(screen2,nextNode->Posy+1,nextNode->Posx-1," ");
                }
            }
            //we also need to take out the warrior that died from the bomb
            PopNode_QueueW(warriorQueue1,nextNode);
            //refreshes the screens
            wrefresh(screen1);
            wrefresh(screen2);
        }


        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }

};



//This function is needed to check if the warrior is in front of the tower
void checkTowerCollision(warrior_ptr warrior,struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){


    //if the warrior hit the Tower4
    if((warrior->Posx+2 == tower4->posX) && ((warrior->Posy == tower4->posY+1) || (warrior->Posy == tower4->posY+2)) && (warrior->player ==1) && (warrior->screen==2)){
        //while the health is more than 0, keep attacking the tower
        while(tower4->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->vitality = warrior->vitality - tower4->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->vitality);
            //if the tower kills the warrior:
            if(warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_end();
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
        //if the warrior does more damage to the tower than needed:
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
        //if the warrior hit the Tower5
    else if((warrior->Posx+2 == tower5->posX) && ((warrior->Posy == tower5->posY) || (warrior->Posy == tower5->posY+1)) && (warrior->player ==1) && (warrior->screen==2)){

        while(tower5->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->vitality = warrior->vitality - tower5->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->vitality);
            if(warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_end();
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
        //if the warrior hit the Tower6
    else if((warrior->Posx+2 == tower6->posX) && ((warrior->Posy == tower6->posY) || (warrior->Posy == tower6->posY+1)) && (warrior->player ==1) && (warrior->screen==2)){

        while(tower6->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->vitality = warrior->vitality - tower6->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->vitality);
            if(warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_end();
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
        //if the warrior hit the Tower1
    else if((warrior->Posx-2 <= tower1->posX+2) && ((warrior->Posy == tower1->posY+1) || (warrior->Posy == tower1->posY+2)) && (warrior->player ==2) && (warrior->screen==1) ){

        while(tower1->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->vitality = warrior->vitality - tower1->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->vitality);
            if(warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_end();
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
        //if the warrior hit the Tower2
    else if((warrior->Posx-4 <= tower2->posX+3) && ((warrior->Posy == tower2->posY) || (warrior->Posy == tower2->posY+1) || (warrior->Posy == tower2->posY+2)) && (warrior->player ==2) && (warrior->screen==1)){

        while(tower2->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->vitality = warrior->vitality - tower2->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->vitality);
            if(warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_end();
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
        //if the warrior hit the Tower3
    else if((warrior->Posx-2 <= tower3->posX+2) && ((warrior->Posy == tower3->posY) || (warrior->Posy == tower3->posY+1)) && (warrior->player ==2) && (warrior->screen==1)){
        wattron(screen1,COLOR_PAIR(1));
        while(tower3->health > 0){
            my_mutex_lock(&lock);
            //tower attacks the warrior
            warrior->vitality = warrior->vitality - tower3->attack;
            char healthW[4] ;
            sprintf(healthW,"%d",warrior->vitality);
            if(warrior->vitality <= 0){
                cleanWarrior(warrior->warrior,warrior);
                my_mutex_unlock(&lock);
                my_thread_end();
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

//this function is used to move the warrior in 4 different ways
void moveWarrior(int nextMove, Warrior *warrior, warrior_ptr node, int * stepX){

    char * lvl = arr[warrior->level];

    int player = node->player;
    int idWarrior = node->id;
    int partner = node->partner;
    warrior_ptr checkAlly = null;
    if(node->vitality <= 0){
        //if the warrior is dead, we have to unlock every mutex that it could have had
        my_mutex_unlock(&battleLock);
        my_mutex_unlock(&lock);
        my_mutex_unlock(&bridge1Lock);
        my_mutex_unlock(&bridge2Lock);
        my_mutex_unlock(&partnerBridge1);
        my_mutex_unlock(&partnerBridge2);
        if(partnerBridge1 == player && partner == 1)
            partnerBridge1 = 0;
        if(partnerBridge2 == player && partner == 1)
            partnerBridge2 = 0;
        if(node->player == 1){
            lastDeadWarrior1 = node->warrior;
            if(lastDeadWarrior2 != null){
                pairing(lastDeadWarrior2, lastDeadWarrior1);
            }
        }else{
            lastDeadWarrior2 = node->warrior;
            if(lastDeadWarrior1)
                pairing(lastDeadWarrior1, lastDeadWarrior2);
        }
        PopNode_QueueW(warriorQueue1,node);
        my_thread_end();
    }
    //checking if theres an Ally in front of the warrior
    if (node->player == 1){
        checkAlly = checkCollision(node, 2);
    }else{
        checkAlly = checkCollision(node, 1);
    }
    //if theres an Ally, dont step over him
    if(checkAlly){
        *stepX-=1;
        return;
    }

    warrior_ptr check = checkCollision(node, node->player);
    //if theres nobody in front of the warrior, keep moving
    if (check == null) {
        //case to see which move the warrior wants to do next
        switch (nextMove) {
            case 1://move to right

                if ((node->Posx + 2 < screen1->_maxx) && (node->screen == 1)) {
                    //before every move, the warrior cleans itself and his surroundings
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");
                    //prints new position
                    mvwprintw(screen1, node->Posy, node->Posx + 1, warrior->name);
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, lvl);
                    mvwprintw(screen1, node->Posy, node->Posx + 2, ">");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 2, ">");
                    //updates the warrior position
                    node->Posx = node->Posx + 1;


                }
                //changing to screen2
                if ((node->Posx + 2 >= screen1->_maxx) && (node->screen == 1)) {
                    //set the screen to 2
                    node->screen = 2;
                    //cleans the position
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    //start the position to the beginning of screen
                    node->Posx = 2;
                    mvwprintw(screen2, node->Posy, node->Posx, warrior->name);
                    mvwprintw(screen2, node->Posy + 1, node->Posx, lvl);
                    mvwprintw(screen2, node->Posy, node->Posx + 1, ">");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, ">");
                    wrefresh(screen2);
                }//check not to run over the tower and keep moving
                if ((node->screen == 2) && (node->Posx + 2 < tower5.posX)) {
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");

                    mvwprintw(screen2, node->Posy, node->Posx + 1, warrior->name);
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, lvl);
                    mvwprintw(screen2, node->Posy, node->Posx + 2, ">");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 2, ">");
                    node->Posx = node->Posx + 1;
                    wrefresh(screen2);
                }
                break;
            case 2://move left

                if ((node->Posx - 2 > 0) && (node->screen == 2)) {

                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");

                    mvwprintw(screen2, node->Posy, node->Posx - 2, "<");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 2, "<");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, warrior->name);
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, lvl);
                    node->Posx = node->Posx - 1;
                    wrefresh(screen2);
                }
                if ((node->Posx - 2 <= 0) && (node->screen == 2)) {
                    node->screen = 1;
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    wrefresh(screen2);
                    node->Posx = screen1->_maxx-2;

                    mvwprintw(screen1, node->Posy, node->Posx, warrior->name);
                    mvwprintw(screen1, node->Posy + 1, node->Posx, lvl);
                    mvwprintw(screen1, node->Posy, node->Posx - 1, "<");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, "<");
                    wrefresh(screen1);
                }
                if ((node->screen == 1) && (node->Posx - 2 > tower2.posX + 2)) {
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");

                    mvwprintw(screen1, node->Posy, node->Posx - 2, "<");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 2, "<");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, warrior->name);
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, lvl);
                    node->Posx = node->Posx - 1;
                    wrefresh(screen1);
                }
                break;
            case 3://move up
                if (node->screen == 1) {
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy - 2 > 0) {

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
                } else if (node->screen == 2) {
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy - 2 > 0) {

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
            case 4://move down
                if (node->screen == 1) {
                    mvwprintw(screen1, node->Posy, node->Posx, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen1, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen1, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy + 2 < screen1->_maxy) {

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
                } else if (node->screen == 2) {
                    mvwprintw(screen2, node->Posy, node->Posx, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx - 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx, " ");
                    mvwprintw(screen2, node->Posy, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy + 1, node->Posx + 1, " ");
                    mvwprintw(screen2, node->Posy - 1, node->Posx + 1, " ");
                    if (node->Posy + 2 < screen1->_maxy) {

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
    //in case there is a warrior in front of the current warrior
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
            my_mutex_unlock(&partnerBridge1);
            my_mutex_unlock(&partnerBridge2);
            if(partnerBridge1 == player && partner == 1)
                partnerBridge1 = 0;
            if(partnerBridge2 == player && partner == 1)
                partnerBridge2 = 0;
            if(node->player == 1){
                lastDeadWarrior1 = node->warrior;
                if(lastDeadWarrior2 != null){
                    pairing(lastDeadWarrior2, lastDeadWarrior1);
                }
            }else{
                lastDeadWarrior2 = node->warrior;
                if(lastDeadWarrior1)
                    pairing(lastDeadWarrior1, lastDeadWarrior2);
            }
            my_thread_end();
        }else{
            if(check->screen == 1){

                mvwprintw(screen1, checkExist->Posy, checkExist->Posx+1, " ");
                mvwprintw(screen1, checkExist->Posy + 1, checkExist->Posx+1, " ");
                mvwprintw(screen1, checkExist->Posy, checkExist->Posx-1, " ");
                mvwprintw(screen1, checkExist->Posy + 1, checkExist->Posx-1, " ");
            }
            else{

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
//this function is used for the mutation of the warriors
void pairing(Warrior* const cmp, Warrior* const depart){
    int* stats1 [4] = {&cmp->vitality, &cmp->attack, &cmp->defense, &cmp->speed};
    int* stats2 [4] = {&depart->vitality, &depart->attack, &depart->defense, &depart->speed};
    int avg1 = *stats1[0]+ *stats1[1]+ *stats1[2]+ *stats1[3];
    int avg2 = *stats2[0]+ *stats2[1]+ *stats2[2]+ *stats2[3];

    if(avg2 < avg1) {
        srand(time(NULL));
        int select_attr = rand() % 4;//randomly select attribute to swap
        *stats2[select_attr] = *stats1[select_attr];
        int check_mut = rand() % 100;
        if (check_mut < 10) {//check if eligible to mutation
            select_attr = rand() % 4;
            mutation(stats2[select_attr]);
        }
        getLevel(depart);
    }
}
//this is the main function, in here we will create everything
void createTable(int opcion){
    //we need to start the screens
    initscr();
    //active the keypad so that we can read the keys for keyboard as ASCII
    keypad(stdscr,TRUE);
    //function to exit with ctrl + C key
    cbreak();
    curs_set(0);
    //here we initialize de variables that are stored in the config.ini file
    const char *optionBomb = ini_get(config, "bombWarrior", "damage");
    damage = atoi(optionBomb);
    const char *optionBomb2 = ini_get(config, "bombWarrior", "entry_limit");
    entry_limit = atoi(optionBomb2);

    const char *optionBomb3 = ini_get(config, "bombWarrior", "depart_limit");
    depart_limit = atoi(optionBomb3);
    const char *interactive = ini_get(config, "table", "interactive");
    start();
    //here we difine the dimensions of the screen
    int x, y, height, width;
    x = 0;
    y = 0;
    if(!has_colors()){
        printw("colors arent available");
        getch();
    }
    //create 2 sets of colors for the screen
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
    //we have to refresh every screen so that it always display the newest
    wrefresh(screen1);
    wrefresh(screen2);
    wrefresh(terminal);
    wrefresh(terminal2);
    //we create a margin to each screen
    box(screen1,0,0);
    box(screen2,0,0);
    box(terminal,0,0);
    box(terminal2,0,0);
    wrefresh(terminal);
    wrefresh(terminal2);

    int count = 0;
    while(count < 7){

        // drawing bridge screen1
        mvwprintw(screen1,(height/2)-5,screen1->_maxx-count,"-");
        mvwprintw(screen1,(height/2)-1,screen1->_maxx-count,"-");
        mvwprintw(screen1,(height/2)+1,screen1->_maxx-count,"-");
        mvwprintw(screen1,(height/2)+5,screen1->_maxx-count,"-");
        // drawing bridge screen2
        mvwprintw(screen2,(height/2)-5,count,"-");
        mvwprintw(screen2,(height/2)-1,count,"-");
        mvwprintw(screen2,(height/2)+1,count,"-");
        mvwprintw(screen2,(height/2)+5,count,"-");
        count += 1;
    }
    char health[40] ;
    //changing to string the value of health
    sprintf(health,"%d",tower1.health);
    //turning on the color blue for the player 1 color
    wattron(screen1,COLOR_PAIR(1));
    //drawing the tower
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
    //turning off the color blue for the player 1 color
    wattroff(screen1,COLOR_PAIR(1));


    char health4[40] ;
    //turning on the color red for the player 2 color
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
    //turning off the color red for the player 2 color
    wattroff(screen2,COLOR_PAIR(2));
    //here we print the list of warriors that we can choose from the terminal
    printWarriorList();

    //starting all the mutexes
    sleep(1/2);
    my_mutex_init(&lock);
    my_mutex_init(&battleLock);
    my_mutex_init(&bridge1Lock);
    my_mutex_init(&bridge2Lock);
    my_mutex_init(&partner1Lock);
    my_mutex_init(&partner2Lock);
    //starting the threads
    my_thread_t t1;
    my_thread_t t2;
    my_thread_t t3;
    my_thread_t t4;
    my_thread_t t5;
    my_thread_init(100);

    wrefresh(screen1);
    wrefresh(screen2);
    wrefresh(terminal);
    wrefresh(terminal2);
    //selection between interactive and demo mode
    if(strcmp(interactive,"true") == 0){
        my_thread_t threadInputs;
        //creates a thread for the terminal inputs
        my_thread_create(&threadInputs,interactiveInputs,null, 0);

    }
    else{
        //demo of the game
        my_thread_t thread2;
        struct Params * par2 = (struct Params *)malloc(sizeof(struct Params));
        Warrior warrior2;
        initValues(&warrior2,50,10,10,5,"R",1);
        warrior_ptr pointer2 = NewThreadW(&warrior2, 1, 6, 3, 1);
        Push_QueueW(warriorQueue1, pointer2);
        par2->width = screen1->_maxx;
        par2->node = pointer2;
        par2->warrior = &warrior2;
        par2->nextMove = 1;
        bombWarrior(&warrior2);
        my_thread_create(&thread2,movePlayer1,(void*)par2, 0);
        my_thread_sleep(2);

        my_thread_t thread3;
        struct Params * par3 = (struct Params *)malloc(sizeof(struct Params));
        Warrior warrior3;
        initValues(&warrior3,200,100,10,5,"F",0);
        warrior_ptr pointer3 = NewThreadW(&warrior3, 2, 8, 3, 2);
        Push_QueueW(warriorQueue1, pointer3);
        par3->width = screen1->_maxx;
        par3->node = pointer3;
        par3->warrior = &warrior3;
        par3->nextMove = 2;
        my_thread_create(&thread3,movePlayer1,(void*)par3, 0);

        my_thread_sleep(7);

        my_thread_t thread4;
        struct Params * par4 = (struct Params *)malloc(sizeof(struct Params));
        Warrior warrior4;
        initValues(&warrior4,200,100,10,5,"F",0);
        warrior_ptr pointer4 = NewThreadW(&warrior4, 2, (screen1->_maxx-6), 9, 2);
        Push_QueueW(warriorQueue1, pointer4);
        par4->width = screen1->_maxx;
        par4->node = pointer4;
        par4->warrior = &warrior4;
        par4->nextMove = 2;

        my_thread_create(&thread4,movePlayer1,(void*)par4, 0);

        my_thread_sleep(1);

        my_thread_t thread5;
        struct Params * par5 = (struct Params *)malloc(sizeof(struct Params));
        Warrior warrior5;
        initValues(&warrior5,200,100,10,5,"G",0);
        warrior_ptr pointer5 = NewThreadW(&warrior5, 2, (screen1->_maxx-6), 3, 2);
        Push_QueueW(warriorQueue1, pointer5);
        par5->width = screen1->_maxx;
        par5->node = pointer5;
        par5->warrior = &warrior5;
        par5->nextMove = 2;

        my_thread_create(&thread5,movePlayer1,(void*)par5, 0);

        my_thread_sleep(2);

        my_thread_t thread6;
        struct Params * par6 = (struct Params *)malloc(sizeof(struct Params));
        Warrior warrior6;
        initValues(&warrior6,200,100,10,5,"T",0);
        warrior_ptr pointer6 = NewThreadW(&warrior6, 1, 6, 3, 1);
        Push_QueueW(warriorQueue1, pointer6);
        par6->width = screen1->_maxx;
        par6->node = pointer6;
        par6->warrior = &warrior6;
        par6->nextMove = 1;
        bombWarrior(&warrior6);
        my_thread_create(&thread6,movePlayer1,(void*)par6, 0);

        wrefresh(screen1);
        wrefresh(screen2);
        wrefresh(terminal);
        wrefresh(terminal2);
    }
    while(1){
        int terminar =decideWinner(&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
        if(terminar == 1){
            wgetch(screen1);
            break;
        }
    };

    wrefresh(screen1);
    wrefresh(screen2);
    wrefresh(terminal);
    wrefresh(terminal2);
    wgetch(screen1);
    delwin(screen1);
    delwin(screen2);
    delwin(terminal);
    delwin(terminal2);
};
//this function moves the warrior in an specific way
void* movePlayer1(void * parameters){

    int  width, nextMove, entryBridge, exitBridge;
    warrior_ptr node;
    Warrior * warrior;

    width = ((struct Params*)parameters)->width;
    nextMove = ((struct Params*)parameters)->nextMove;
    node = ((struct Params*)parameters)->node;
    warrior = ((struct Params*)parameters)->warrior;
    time_t t1;
    //in case the warrior is a bomb
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
        //in case the warrior will explode
        if (time(0) > t1 && warrior->bomb == 1){
            my_mutex_lock(&lock);
            if(node->player == 1){
                //drawing the simulation of a bomb depending on the screen where the bomb is
                if(node->screen == 1){
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
                if(node->screen == 1){
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
            //here we check if the bomb is dealing any damage to anybody
            checkBombCollision(node);
            my_thread_sleep(1);
            //we have to clean and destroy the bomb warrior and thread
            cleanWarrior( warrior, node);
            my_mutex_unlock(&battleLock);
            my_mutex_unlock(&lock);
            my_mutex_unlock(&bridge1Lock);
            my_mutex_unlock(&bridge2Lock);
            if(my_mutex_trylock(&partnerBridge1)){
                my_mutex_unlock(&partnerBridge1);
                partnerBridge1 = 0;
            }
            if(my_mutex_trylock(&partnerBridge2)){
                my_mutex_unlock(&partnerBridge2);
                partnerBridge2 = 0;
            }
            my_mutex_unlock(&partnerBridge1);
            my_mutex_unlock(&partnerBridge2);
            if(partnerBridge1 == node->player && node->partner == 1)
                partnerBridge1 = 0;
            if(partnerBridge2 == node->player && node->partner == 1)
                partnerBridge2 = 0;


            my_thread_end();
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
        //if the node is dead we need to clean it
        if(node->finish ==1){
            my_mutex_lock(&lock);
            cleanWarrior( warrior, node);
            my_mutex_unlock(&lock);
            my_thread_end();
        }
        //we have to check if the warrior has a collision with a tower
        checkTowerCollision(node,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
        my_mutex_lock(&lock);
        //all this next if conditions is because of the coloring of the warriors
        if(node->player == 1 && node->screen == 1){
            wattron(screen1, COLOR_PAIR(1));
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        else if(node->player == 1 && node->screen == 2){
            wattron(screen2, COLOR_PAIR(1));
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        if(node->player == 1 && node->screen == 1){
            wattroff(screen1, COLOR_PAIR(1));
        }
        else if(node->player == 1 && node->screen == 2){
            wattroff(screen2, COLOR_PAIR(1));
        }
        //-------------------------------------------------
        if(node->player == 2 && node->screen == 1){
            wattron(screen1, COLOR_PAIR(2));
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        else if(node->player == 2 && node->screen == 2){
            wattron(screen2, COLOR_PAIR(2));
            moveWarrior(nextMove,warrior,node, &stepsX);
        }
        if(node->player == 2 && node->screen == 1){
            wattroff(screen1, COLOR_PAIR(2));
        }
        else if(node->player == 2 && node->screen == 2){
            wattroff(screen2, COLOR_PAIR(2));
        }
        my_mutex_unlock(&lock);
        my_thread_sleep(1);
        stepsX +=1;

        wrefresh(screen1);
        wrefresh(screen2);
    }

    int down = 0;
    int up = 0;
    //in case the warrior is going in the top path of the screen
    if(node->Posy < (screen1->_maxy)/2){

        while(down <3){
            if(node->finish ==1){
                my_mutex_lock(&lock);
                cleanWarrior( warrior, node);
                my_mutex_unlock(&lock);
                my_thread_end();
            }
            checkTowerCollision(node,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
            my_mutex_lock(&lock);
            if(node->player == 1 && node->screen == 1){
                wattron(screen1, COLOR_PAIR(1));
                moveWarrior(4,warrior,node, &down);
            }
            else if(node->player == 1 && node->screen == 2){
                wattron(screen2, COLOR_PAIR(1));
                moveWarrior(4,warrior,node, &down);
            }
            if(node->player == 1 && node->screen == 1){
                wattroff(screen1, COLOR_PAIR(1));
            }
            else if(node->player == 1 && node->screen == 2){
                wattroff(screen2, COLOR_PAIR(1));
            }
            //-------------------------------------------------
            if(node->player == 2 && node->screen == 1){
                wattron(screen1, COLOR_PAIR(2));
                moveWarrior(4,warrior,node, &down);
            }
            else if(node->player == 2 && node->screen == 2){
                wattron(screen2, COLOR_PAIR(2));
                moveWarrior(4,warrior,node, &down);
            }
            if(node->player == 2 && node->screen == 1){
                wattroff(screen1, COLOR_PAIR(2));
            }
            else if(node->player == 2 && node->screen == 2){
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
        //in case the warrior is going in the bottom path of the screen
        while(up <3){
            if(node->finish ==1){
                my_mutex_lock(&lock);
                cleanWarrior( warrior, node);
                my_mutex_unlock(&lock);
                my_thread_end();
            }
            checkTowerCollision(node,&tower1,&tower2,&tower3,&tower4,&tower5,&tower6);
            my_mutex_lock(&lock);
            if(node->player == 1 && node->screen == 1){
                wattron(screen1, COLOR_PAIR(1));
                moveWarrior(3,warrior,node, &up);
            }
            else if(node->player == 1 && node->screen == 2){
                wattron(screen2, COLOR_PAIR(1));
                moveWarrior(3,warrior,node, &up);
            }
            if(node->player == 1 && node->screen == 1){
                wattroff(screen1, COLOR_PAIR(1));
            }
            else if(node->player == 1 && node->screen == 2){
                wattroff(screen2, COLOR_PAIR(1));
            }
            //-------------------------------------------------
            if(node->player == 2 && node->screen == 1){
                wattron(screen1, COLOR_PAIR(2));
                moveWarrior(3,warrior,node, &up);
            }
            else if(node->player == 2 && node->screen == 2){
                wattron(screen2, COLOR_PAIR(2));
                moveWarrior(3,warrior,node, &up);
            }
            if(node->player == 2 && node->screen == 1){
                wattroff(screen1, COLOR_PAIR(2));
            }
            else if(node->player == 2 && node->screen == 2){
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
//This function checks which of the central towers is destroyed first and decide a winner
int decideWinner(struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6){
    my_mutex_lock(&lock);
    if((tower2->health <= 0) )
    {
        mvwprintw(screen2,(screen1->_maxy)/2,10,"GANADOR ");
        wrefresh(screen1);
        wrefresh(screen2);
        my_mutex_unlock(&lock);
        return 1;
    }
    else if((tower5->health <= 0))
    {
        //clear();
        mvwprintw(screen1,4,(screen1->_maxy)/2,"GANADOR ");
        wrefresh(screen1);
        wrefresh(screen2);
        my_mutex_unlock(&lock);
        return 1;
    }
    my_mutex_unlock(&lock);
    return 0;
}
//this function cleans the warrior that is received from parameter
void cleanWarrior(Warrior * warrior, warrior_ptr node){
    if(node->player == 1){
        lastDeadWarrior1 = node->warrior;
        if(lastDeadWarrior2 != null){
            pairing(lastDeadWarrior2, lastDeadWarrior1);
        }
    }else{
        lastDeadWarrior2 = node->warrior;
        if(lastDeadWarrior1 != null)
            pairing(lastDeadWarrior1, lastDeadWarrior2);
    }
    if(node->screen == 1){
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
//this function activates the timer for the bomb
void bombWarrior(Warrior * warrior){
    if(warrior->bomb == 1){
        warrior->entry_limit = entry_limit;
        warrior->depart_limit = depart_limit;

    }
}
//this function prints the keys that you need to press in order to choose a warrior in the interactive mode
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
//this function defines the kind of warrior that is going to be sent to the thread
warrior_ptr selectWarrior( int opcion,int localizacion){
    warrior_ptr pointer = null;
    switch (opcion) {
        case  114:// r
            pointer = NewThreadW(&warriors[0], 1, 6, localizacion,1);
            break;
        case  116:// t
            pointer = NewThreadW(&warriors[1], 1, 6, localizacion,1);
            break;
        case  121:// y
            pointer = NewThreadW(&warriors[2], 1, 6, localizacion, 1);
            break;
        case  117:// u          ------> bomba del P1
            pointer = NewThreadW(&warriors[3], 1, 6, localizacion, 1);
            break;
        case  102:// f
            pointer = NewThreadW(&warriors[4], 2, screen2->_maxx-6, localizacion, 2);
            break;
        case  103:// g
            pointer = NewThreadW(&warriors[5], 2, screen2->_maxx-6, localizacion, 2);
            break;
        case  104:// h
            pointer = NewThreadW(&warriors[6], 2, screen2->_maxx-6, localizacion, 2);
            break;
        case  106:// j          ------> bomba del P2
            pointer = NewThreadW(&warriors[7], 2, screen2->_maxx-6, localizacion, 2);
            break;
    }
    Push_QueueW(warriorQueue1, pointer);
    my_thread_t thread;
    struct Params * par4 = (struct Params *)malloc(sizeof(struct Params));
    par4->width = screen1->_maxx;
    par4->node = pointer;
    par4->warrior = pointer->warrior;
    if(pointer->player == 1){
        par4->nextMove = 1;
    }
    else{
        par4->nextMove = 2;
    }
    my_thread_create(&thread,movePlayer1,(void*)par4, 0);
    return pointer;
}
//function to take the inputs from the terminal
void* interactiveInputs(){
    while(true){
        int key = 0;
        int posicion  =0;
        //nodelay(terminal, TRUE);
        //-----------------------------------------------
        my_mutex_lock(&lock);

        mvwprintw(terminal,1,2,"Warrior seleccionado: ");
        wmove(terminal,1,25);
        key = wgetch(terminal);

        mvwprintw(terminal,2,2,"posici??n a desplegar: ");
        wmove(terminal,2,25);
        posicion = wgetch(terminal);
        if(posicion == 49){
            posicion = 3;
        }
        else{
            posicion = 9;
        }

        wrefresh(terminal);
        my_mutex_unlock(&lock);
        //-----------------------------------------------
        warrior_ptr prueba = selectWarrior(key, posicion);
        my_mutex_lock(&lock);
        mvwprintw(terminal,1,25," ");
        mvwprintw(terminal,2,25," ");
        wrefresh(terminal);
        my_mutex_unlock(&lock);
        my_thread_sleep(10);
    }
    

}






