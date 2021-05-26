//
// Created by ericksalazargranera on 5/6/21.
//

#ifndef UNTITLED_TABLE_H
#define UNTITLED_TABLE_H
#include <stdlib.h>
#include "stdio.h"
#include <ncurses.h>
#include "Warrior.h"
#include "unistd.h"
#include "my_thread.h"
#include <time.h>
#define null  NULL
typedef struct Node2
{
    int id;
    struct Node2* next;
    struct warrior * warrior;
    int player;
    int finish;

} war,*warrior_ptr;

typedef struct warriorQueue
{
    warrior_ptr head;
    warrior_ptr head_parent;
    long count;
} *warriorQueue;

struct Tower
{
    int health;
    char *towerSymbol;
    int posX;
    int posY;
    int attack;

};

void createTable(int opcion);
void moveWarrior(int nextMove,Warrior *warrior, warrior_ptr node, int* stepX);
int iniciar();
void createTowers(int opcion, struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6);
int decidirGanador(struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6);
int Pop_QueueW(warriorQueue queue);
int Push_QueueW(warriorQueue queue,warrior_ptr node);
warriorQueue GetThreadQueueW();
int MoveForwardW(warriorQueue queue);
warrior_ptr NewThreadW(Warrior * warrior, int player);
void PopNode_QueueW(warriorQueue queue, warrior_ptr node);
int GetNextThreadIdW();
warrior_ptr GetThreadW( long idThread);
warrior_ptr  checkCollision(warrior_ptr warrior);
void checkTowerCollision(warrior_ptr warrior,struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6);
void* movePlayer1(void * Params);
void* movePlayer2(void * parameters);
void exitWarriorThread(int id);
void cleanWarrior(Warrior * warrior, warrior_ptr node);
void bombWarrior(Warrior * warrior);
#endif //UNTITLED_TABLE_H
