#include "behavior.h"
#include <math.h>
#include <QDebug>
#define PI 3.14159

int estadoChute = corrigir;

void chute(Player* jogador, WorldMap* world){

    QVector2D ourPentalyMark(0.6,0);
    //QVector2D ourGoalCenter = world->ourGoalCenter();
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

    switch(estadoChute){
    case corrigir:{
        if(getPosition.x()<ballPosition.x() && distancia<=0.3){
            jogador->goTo(correcao);
            estadoChute=aproximar;
        }else{
            estadoChute=aproximar;;
        }
        break;
    }
    case aproximar:{ //Caso o robô não esteja alinhado com a bola e o gol adversário, ele se deslocará para o vetor alinhamento.
            if(getPosition!=alinhamento){
                jogador->goTo(ajuste);
                estadoChute=alinhar;
            }else{
                estadoChute=alinhar;
            }
            break;
            }
        case alinhar:{ //Ao assumir a posição correta, o robô executará uma rotação para que fique de frente para a bola.
            if(ballPosition.x()<=0){
                if(getOrientation!=((3*PI/2) -(alinhamento.y()/alinhamento.x())) && getPosition.x() <= ajuste.x()+0.05 && getPosition.y() >= ajuste.y()-0.05){
                    jogador->rotateTo((3*PI/2)-(alinhamento.y()/alinhamento.x()));
                    estadoChute=chutar;
                }else{
                    estadoChute=aproximar;
                }
            }else{
                if(getOrientation!=(+(alinhamento.y()/alinhamento.x())) && getPosition.x() <= ajuste.x()+0.05 && getPosition.y() >= ajuste.y()-0.05){
                    jogador->rotateTo(+(alinhamento.y()/alinhamento.x()));
                    estadoChute=chutar;
                }else{
                    estadoChute=aproximar;
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
                estadoChute=retornar;
            }

            break;
            }
        case retornar:{ // Caso a bola entre no gol, ou se o robô errar o gol, ele se deslocará para a marca de penalty.
            if(ballPosition.x()<=theirGoalCenter.x()){
                    jogador->goTo(ourPentalyMark);
                    estadoChute=recomecar;
            }else{
                //posicao=recomecar;
                jogador->goTo(ballPosition);
            }
            break;
            }
        case recomecar:{ //Uma vez no gol aliado, o robô reiniciará o processo.
            if(getPosition.x()>=ourPentalyMark.x()){
                estadoChute=aproximar;
            }else{
                jogador->goTo(ourPentalyMark);
            }
            break;
            }
            }
            }


int estadoPreditor = espera;

void preditor(Player* jogador, WorldMap* world){


    QVector2D ballPosition = world->ballPosition();
    QVector2D ourGoalCenter = world->ourGoalCenter();
    QVector2D getPosition = jogador->getPosition();
    QVector2D chute;


    //float distanciaX = abs(abs(getPosition.x())-abs(ballPosition.x()));
    //float distanciaY = abs(abs(getPosition.y())-abs(ballPosition.y()));
    //float distancia = sqrt(pow(distanciaX,2)+pow(distanciaY,2));

    chute.setX(ballPosition.x()-0.2);
    chute.setY(ballPosition.y());

    switch(estadoPreditor){
    case espera:{
        if(ballPosition.x()>=0.4 && (ballPosition.y())<= 0.2 && ballPosition.y()>= -0.2){
            jogador->goTo(ballPosition);
            estadoPreditor = defende;
        }else{
            estadoPreditor=acompanha;
        }
        break;
    }
    case defende:{
        if(ballPosition.x()<0.4 && getPosition.x()<=ourGoalCenter.x()){
            jogador->goTo(ourGoalCenter);
            estadoPreditor = acompanha;
        }else{
            estadoPreditor = espera;
        }
        break;
    }
    case acompanha:{
        if(ballPosition.x()<0.4 && getPosition.x()>= ourGoalCenter.x()){
            jogador->rotateTo(ballPosition);
            estadoPreditor = espera;
        }else{
            estadoPreditor = defende;
        }
        break;
    }
    }
}

int estadoMeia = aguarda;

void meia(Player* jogador, WorldMap* world){

    QVector2D ourGoalCenter = world->ourGoalCenter();
    QVector2D ballPosition = world->ballPosition();
    QVector2D getPosition = jogador->getPosition();
    QVector2D alinhamento(0, ballPosition.y()*1.2);
    QVector2D ajuste;
    QVector2D ajuste2;

    ajuste.setX(alinhamento.x());
    ajuste.setY(alinhamento.y());

    ajuste2.setX(0.65);
    ajuste2.setY(ballPosition.y()*0.5);

    qDebug()<<estadoMeia;

        switch(estadoMeia){

            case avanca:{

                if(ballPosition.x() >= -0.4 && ballPosition.x() <= 0.4){
                    jogador->goTo(ballPosition);

                }else if(ballPosition.x()> 0.4){
                    estadoMeia = defesa;
                }else{
                    estadoMeia = aguarda;
                }
            break;
            }

            case aguarda:{

                if(ballPosition.x() <= -0.4){
                    jogador->goTo(ajuste);
                }else{
                    estadoMeia = avanca;
                }
            break;
            }

            case defesa:{

                if(ballPosition.x() >= 0.4){
                    jogador->goTo(ajuste2);

                }else{
                    estadoMeia = aguarda;
                }
            break;
            }




}
}
