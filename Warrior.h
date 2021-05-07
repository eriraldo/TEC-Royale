#ifndef WARRIOR_H
#define WARRIOR_H
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    int vitality;
    int attack;
    int defense;
    int speed;
    int level;
    int entry_limit;
    int depart_limit;
    char* name;
}Warrior;


int initValues(Warrior* const warrior, int vitality, int attack, int defense, int speed, char* name);
void printWarrior(Warrior* const warrior);
void getLevel(Warrior* const warrior);
void mutation(int*);


#endif //WARRIOR_H
