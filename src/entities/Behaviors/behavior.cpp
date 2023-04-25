#include "behavior.h"
#include <math.h>
#include <QDebug>
#define PI 3.14159

int estadoChute = corrigir;

void chute(Player* jogador, WorldMap* world){

    bool side = world->playingLeftSide();
    int a = side ? -1 : 1;
    QVector2D ourPentalyMark;
    ourPentalyMark.setX(a*0.6);
    ourPentalyMark.setY(0);
    //qDebug() << ourPentalyMark;
    //QVector2D ourGoalCenter = world->ourGoalCenter();
    float getOrientation = jogador->getOrientation();
    QVector2D getPosition = jogador->getPosition();
    QVector2D ballPosition = world->ballPosition();
    QVector2D theirGoalCenter = world->theirGoalCenter();

    QVector2D alinhamento = side ? (-theirGoalCenter + ballPosition)*1.4 : (-theirGoalCenter + ballPosition)*1.4;
    QVector2D ajuste;
    //qDebug() << alinhamento;

    float length = world->length();             //O vetor alinhamento descreve um vetor que contenha as coordenadas da bola e do dentro do goll adversário.
    side ? ajuste.setX(alinhamento.x()+(length/2)) : ajuste.setX(alinhamento.x()-(length/2));
    ajuste.setY(alinhamento.y());

    float distanciaX = abs(abs(getPosition.x())-abs(ballPosition.x())); //Distância entre a bola e o robô na componente X.
    float distanciaY = abs(abs(getPosition.y())-abs(ballPosition.y())); //Distância entre a bola e o robô na componente Y.
    float distancia = sqrt(pow(distanciaX,2)+pow(distanciaY,2));


    QVector2D correcao;
    correcao.setX(ballPosition.x());
    side ? correcao.setY(ballPosition.y()-0.17) :  correcao.setY(ballPosition.y()+0.17);
    //qDebug() << correcao;
    //qDebug() << ajuste;
    qDebug() << estadoChute;
    switch(estadoChute){
    case corrigir:{
        if(side ? getPosition.x()>ballPosition.x() && distancia<=0.3 : getPosition.x()<ballPosition.x() && distancia<=0.3){
            jogador->goTo(correcao);
            estadoChute=aproximar;
        }else{
            estadoChute=aproximar;
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
            if(side ? ballPosition.x()>=0 : ballPosition.x()<=0){
                if(getOrientation!=((3*PI/2) -(alinhamento.y()/alinhamento.x())) && side ? getPosition.x() >= ajuste.x()-0.05 && getPosition.y() <= ajuste.y()+0.05 : getPosition.x() <= ajuste.x()+0.05 && getPosition.y() >= ajuste.y()-0.05){
                    jogador->rotateTo((3*PI/2)-(alinhamento.y()/alinhamento.x()));
                    estadoChute=chutar;
                }else{
                    estadoChute=aproximar;
                }
            }else{
                if(getOrientation!= side ? -((alinhamento.y()/alinhamento.x())) : ((alinhamento.y()/alinhamento.x())) && side ? getPosition.x() <= ajuste.x()-0.05 && getPosition.y() <= ajuste.y()+0.05 : getPosition.x() <= ajuste.x()+0.05 && getPosition.y() >= ajuste.y()-0.05){
                    jogador->rotateTo(+(alinhamento.y()/alinhamento.x()));
                    estadoChute=chutar;
                }else{
                    estadoChute=aproximar;
                }
            }
            break;
            }
        case chutar:{ //Quando estiver na posição correta e virado para a bola, o robô chutará a bola.
            if(side ? ballPosition.x()<theirGoalCenter.x() && getPosition.x()<ballPosition.x() : ballPosition.x()>theirGoalCenter.x() && getPosition.x()>ballPosition.x()){
                if(distancia>=0.1){
                    jogador->goTo(ballPosition);
                }
            }else{
                estadoChute=retornar;
            }

            break;
            }
        case retornar:{ // Caso a bola entre no gol, ou se o robô errar o gol, ele se deslocará para a marca de penalty.
            if(side ? ballPosition.x()>=theirGoalCenter.x() : ballPosition.x()<=theirGoalCenter.x()){
                    jogador->goTo(ourPentalyMark);
                    estadoChute=recomecar;
            }else{
                estadoChute=recomecar;
                //jogador->goTo(ballPosition);
            }
            break;
            }
        case recomecar:{ //Uma vez no gol aliado, o robô reiniciará o processo.
            if(side ? getPosition.x()<=ourPentalyMark.x() : getPosition.x()>=ourPentalyMark.x()){
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

    bool side = world->playingLeftSide();
    int a = side ? -1 : 1;
    QVector2D posicao(0.74, 0);
    posicao.setX(a*0.74);
    posicao.setY(0);


    switch(estadoPreditor){
    case espera:{  //Caso a bola entre na área determinada, o jogador empurrará a bola.
        if(side ? ballPosition.x()<=-0.4 : ballPosition.x()>=0.4 && (ballPosition.y())<= 0.3 && ballPosition.y()>= -0.3){
            jogador->goTo(ballPosition);
            estadoPreditor = defende;
        }else{
            estadoPreditor=acompanha;
        }
        break;
    }
    case defende:{ //Após empurrar a bola, o jogador retornará para a posição de defesa.
        if(side ? ballPosition.x()>0.4 && getPosition.x()>=ourGoalCenter.x() : ballPosition.x()<0.4 && getPosition.x()<=ourGoalCenter.x()){
            jogador->goTo(posicao);
            estadoPreditor = acompanha;
        }else{
            estadoPreditor = espera;
        }
        break;
    }
    case acompanha:{ //Enquanto a bola estiver fora da área determinada, o jogador irá rotacionar, na posição, para a posição da bola.
        if(side ? ballPosition.x()>0.4 && getPosition.x()<= posicao.x() : ballPosition.x()<0.4 && getPosition.x()>= posicao.x()){
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


    QVector2D ballPosition = world->ballPosition();
    QVector2D alinhamento(0, ballPosition.y()*1.2);
    QVector2D ajuste;
    QVector2D ajuste2;

    ajuste.setX(alinhamento.x());
    ajuste.setY(alinhamento.y());

    bool side = world->playingLeftSide();
    int a = side ? -1 : 1;
    ajuste2.setX(a*0.65);
    ajuste2.setY(ballPosition.y()*0.5);

    //qDebug()<<estadoMeia;

        switch(estadoMeia){

            case avanca:{ //Caso a bola entre dentro da área de atuação o jogador irá empurrá-la de volta.

                if(ballPosition.x() >= 0 && ballPosition.x() <= 0.4){
                    jogador->goTo(ballPosition);

                }else if(side ? ballPosition.x()< -0.4 : ballPosition.x()> 0.4){
                    estadoMeia = defesa;
                }else{
                    estadoMeia = aguarda;
                }
            break;
            }

            case aguarda:{  // Caso a bola esteja fora da área de ação, o jogador irá acompanhar a bola no sentido da largura do campo.

                if(side ? ballPosition.x() >= 0.4 : ballPosition.x() <= -0.4){
                    jogador->goTo(ajuste);
                }else{
                    estadoMeia = avanca;
                }
            break;
            }

            case defesa:{  //Caso a bola esteja fora da área de ação, mas no lado aliado, o jogador irá perseguir a bola.

                if(side ? ballPosition.x() <= 0.4 : ballPosition.x() >= 0.4){
                    jogador->goTo(ajuste2);

                }else{
                    estadoMeia = aguarda;
                }
            break;
            }
}
}
