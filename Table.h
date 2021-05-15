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
void createTable(int opcion);
void moveWarrior(int nextMove,Warrior *warrior);
int iniciar();
#endif //UNTITLED_TABLE_H
