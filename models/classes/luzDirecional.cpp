#include "../headers/luzDirecional.h"

luzDirecional::luzDirecional(tupla intensidade, tupla direcao)
    : intensidade(intensidade), direcao(direcao){
}

luzDirecional::~luzDirecional(){}

tupla luzDirecional::getIntensidade(){
    return this->intensidade;
}

tupla luzDirecional::getDirecao(){
    return this->direcao;
}
