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
#define null  NULL
typedef struct Node
{
    int id;
    struct Node* next;
    struct warrior * warrior;
    int player;

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

};

void createTable(int opcion);
void moveWarrior(int nextMove,Warrior *warrior, warrior_ptr node);
int iniciar();
void createTowers(int opcion, struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6);
int decidirGanador(struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6);
int Pop_Queue(warriorQueue queue);
int Push_Queue(warriorQueue queue,warrior_ptr node);
warriorQueue GetThreadQueue();
int MoveForward(warriorQueue queue);
warrior_ptr NewThread(Warrior * warrior, int player);
void PopNode_Queue(warriorQueue queue, warrior_ptr node);
int GetNextThreadId();
warrior_ptr GetThread( long idThread);
warrior_ptr  checkCollision(warrior_ptr warrior);
void checkTowerCollision(warrior_ptr warrior,struct Tower *tower1,  struct Tower *tower2,  struct Tower *tower3,  struct Tower *tower4,  struct Tower *tower5,  struct Tower *tower6);

#endif //UNTITLED_TABLE_H
