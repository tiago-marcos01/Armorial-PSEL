#ifndef ROLES_H
#define ROLES_H
#include "src/entities/Behaviors/behavior.h"

void goleiro(Player* jogador, WorldMap* world);
void atacante(Player* jogador, WorldMap* world);
void meioCampo(Player* jogador, WorldMap* world);

#endif // ROLES_H
