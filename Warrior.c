#include "Warrior.h"


int initValues(Warrior* const warrior, int vitality, int attack, int defense, int speed, char* name, int bomb){
    if(!warrior){
        perror("malloc failed");
        return 0;
    }
    warrior->vitality = vitality;
    warrior->attack = attack;
    warrior->defense = defense;
    warrior->speed = speed;
    warrior->name = name;
    warrior->entry_limit = 0;
    warrior->depart_limit = 0;
    warrior->bomb = bomb;

    int attr = ((warrior->vitality + warrior->speed + warrior->defense + warrior->attack) / 4)/10 + 1;
    if (attr >= 10){
        attr = 9;
    }
    warrior->level = attr;
    return 1;
}

void printWarrior(Warrior* const warrior){
    printf("VIT: %d, ATT: %d, DEF: %d, SPD: %d, NAME: %s\n", warrior->vitality,warrior->attack, warrior->defense, warrior->speed, warrior->name);
}

void getLevel(Warrior* const warrior){
    int attr = ((warrior->vitality + warrior->speed + warrior->defense + warrior->attack) / 4)/10 + 1;
    if (attr >= 10){
        attr = 9;
    }
    warrior->level = attr;
}

void mutation(int* stat){
    //srand(time(NULL));
    *stat+= 40;
    /*int addOrSub = rand()%2;
    if(addOrSub){
        *stat+= rand()%40;
    }else{
        *stat-= rand()%40;
        if(*stat < 0)
            *stat = 0;
    }*/
}

