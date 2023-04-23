#include "roles.h"
#include "src/entities/Behaviors/behavior.h"


void goleiro(Player* jogador, WorldMap* world){
    preditor(jogador, world);

}

void atacante(Player* jogador, WorldMap* world){
    chute(jogador, world);

}

void meioCampo(Player* jogador, WorldMap* world){
    meia(jogador, world);

}
