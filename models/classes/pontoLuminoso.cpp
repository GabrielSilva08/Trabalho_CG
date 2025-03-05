#include "../headers/pontoLuminoso.h"

pontoLuminoso::pontoLuminoso(float x, float y, float z, tupla intensidade)
    : ponto(x, y, z), intensidade(intensidade) {
}

pontoLuminoso::~pontoLuminoso(){}

tupla pontoLuminoso::getIntensidade(){
    return this->intensidade;
}