/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "coach.h"
#include "math.h"
#include <QDebug>

#define PI 3.14159

Coach::Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap)
    : _players(players), _worldMap(worldMap)
{
    // Create QTimer and connects to the runCoach() slot
    _actuatorTimer = new QTimer(this);
    QObject::connect(_actuatorTimer, &QTimer::timeout, this, &Coach::runCoach);
    _actuatorTimer->start(COACH_ITERATION_INTERVAL_MS);
}

std::optional<Player*> Coach::getPlayer(const bool& isTeamBlue, const quint8& playerId) {
    // Get list of players
    QList<Player*> playersForColor = _players.value(isTeamBlue);

    // Iterate searching for playerId
    for(const auto& player : playersForColor) {
        if(player->getPlayerId() == playerId) {
            return player;
        }
    }

    // If could not found, just return std::nullopt (should trait later)
    return std::nullopt;
}

WorldMap* Coach::getWorldMap() {
    return _worldMap;
}





int posicao = corrigir;

void Coach::chute(void){

    Player* jogador = getPlayer(YELLOW,0).value(); // O jogador utilizado para os testes é o Yellow 0.
    WorldMap* world = getWorldMap();

    QVector2D ourGoalCenter = world->ourGoalCenter();
    float getOrientation = jogador->getOrientation();
    QVector2D getPosition = jogador->getPosition();
    QVector2D ballPosition = world->ballPosition();
    QVector2D theirGoalCenter = world->theirGoalCenter();

    QVector2D alinhamento = (-theirGoalCenter + ballPosition)*1.4;
    QVector2D ajuste;

    float length = world->length();             //O vetor alinhamento descreve um vetor que contenha as coordenadas da bolla e do dentro do goll adversário.
    ajuste.setX(alinhamento.x()-(length/2));
    ajuste.setY(alinhamento.y());

    float distanciaX = abs(abs(getPosition.x())-abs(ballPosition.x())); //Distância entre a bola e o robô na componente X.
    float distanciaY = abs(abs(getPosition.y())-abs(ballPosition.y())); //Distância entre a bola e o robô na componente Y.
    float distancia = sqrt(pow(distanciaX,2)+pow(distanciaY,2));

    QVector2D correcao;
    correcao.setX(ballPosition.x());
    correcao.setY(ballPosition.y()+0.17);
    qDebug()<<posicao;
    switch(posicao){
    case corrigir:{
        if(getPosition.x()<ballPosition.x() && distancia<=0.3){
            jogador->goTo(correcao);
            posicao=aproximar;
        }else{
            posicao=aproximar;;
        }
        break;
    }
    case aproximar:{ //Caso o robô não esteja alinhado com a bola e o gol adversário, ele se deslocará para o vetor alinhamento.
            if(getPosition!=alinhamento){
                jogador->goTo(ajuste);
                posicao=alinhar;
            }else{
                posicao=alinhar;
            }
            break;
            }
        case alinhar:{ //Ao assumir a posição correta, o robô executará uma rotação para que fique de frente para a bola.
            if(ballPosition.x()<=0){
                if(getOrientation!=((3*PI/2) -(alinhamento.y()/alinhamento.x())) && getPosition.x() <= ajuste.x()+0.05 && getPosition.y() >= ajuste.y()-0.05){
                    jogador->rotateTo((3*PI/2)-(alinhamento.y()/alinhamento.x()));
                    posicao=chutar;
                }else{
                    posicao=aproximar;
                }
            }else{
                if(getOrientation!=(+(alinhamento.y()/alinhamento.x())) && getPosition.x() <= ajuste.x()+0.05 && getPosition.y() >= ajuste.y()-0.05){
                    jogador->rotateTo(+(alinhamento.y()/alinhamento.x()));
                    posicao=chutar;
                }else{
                    posicao=aproximar;
                }
            }
            break;
            }
        case chutar:{ //Quando estiver na posição correta e virado para a bola, o robô chutará a bola.
            if(ballPosition.x()>theirGoalCenter.x() && getPosition.x()>ballPosition.x()){
                if(distancia>=0.1){
                    jogador->goTo(ballPosition);
                }
            }else{
                posicao=retornar;
            }

            break;
            }
        case retornar:{ // Caso a bola entre no gol, ou se o robô errar o gol, ele se deslocará para o gol aliado.
            if(ballPosition.x()<=theirGoalCenter.x()){
                    jogador->goTo(ourGoalCenter);
                    posicao=recomecar;
            }else{
                posicao=recomecar;
            }
            break;
            }
        case recomecar:{ //Uma vez no gol aliado, o robô reiniciará o processo.
            if(getPosition.x()>=ourGoalCenter.x()){
                posicao=aproximar;
            }else{
                jogador->goTo(ourGoalCenter);
            }
            break;
            }
            }
            }


int proceder = espera;

void Coach::preditor(){
    Player* jogador = getPlayer(YELLOW,1).value();
    WorldMap* world = getWorldMap();

    QVector2D ballPosition = world->ballPosition();
    QVector2D ourGoalCenter = world->ourGoalCenter();
    QVector2D getPosition = jogador->getPosition();
    QVector2D chute;


    float distanciaX = abs(abs(getPosition.x())-abs(ballPosition.x()));
    float distanciaY = abs(abs(getPosition.y())-abs(ballPosition.y()));
    float distancia = sqrt(pow(distanciaX,2)+pow(distanciaY,2));

    chute.setX(ballPosition.x()-0.2);
    chute.setY(ballPosition.y());

    switch(proceder){
    case espera:{
        if(ballPosition.x()>=0){
            jogador->goTo(ballPosition);
            proceder = defende;
        }else{
            proceder=acompanha;
        }
        break;
    }
    case defende:{
        if(ballPosition.x()<0 && getPosition.x()<=ourGoalCenter.x()){
            jogador->goTo(ourGoalCenter);
            proceder = acompanha;
        }else{
            proceder = espera;
        }
        break;
    }
    case acompanha:{
        if(ballPosition.x()<0 && getPosition.x()>= ourGoalCenter.x()){
            jogador->rotateTo(ballPosition);
            proceder = espera;
        }else{
            proceder = defende;
        }
        break;
    }
    }




}
void Coach::runCoach() {

    chute();
    preditor();

    Player* jogador = getPlayer(BLUE,1).value();
    WorldMap* world = getWorldMap();

    QVector2D ballPosition = world->ballPosition();
    jogador->goTo(ballPosition);



    // Here you can control the robots freely.
    // Remember that the getPlayer(color, id) function can return a std::nullopt object, so
    // be careful when you use it (remember to only use ids from 0-2 and the BLUE and YELLOW
    // defines).




}
