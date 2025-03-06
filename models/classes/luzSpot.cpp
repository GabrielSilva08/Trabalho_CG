#include "../headers/luzSpot.h"

luzSpot::luzSpot(float x, float y, float z, tupla intensidade, tupla direcao, int ang_abertura)
    : ponto(x, y, z), intensidade(intensidade), direcao(direcao), ang_abertura(ang_abertura){
}

luzSpot::~luzSpot(){}

tupla luzSpot::getIntensidade(){
    return this->intensidade;
}

tupla luzSpot::getDirecao(){
    return this->direcao;
}

float luzSpot::getAng_abertura(){
    return this->ang_abertura;
}