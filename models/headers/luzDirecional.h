#ifndef LUZDIRECIONAL_H
#define LUZDIRECIONAL_H

#include "./ponto.h"
#include "./tupla.h"
#include "luz.h"

class luzDirecional : public luz{
private:
    tupla intensidade;
    tupla direcao;
public:
    luzDirecional(tupla intensidade, tupla direcao);
    ~luzDirecional();
    tupla getIntensidade();
    tupla getDirecao();
};

#endif