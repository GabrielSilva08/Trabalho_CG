#ifndef LUZSPOT_H
#define LUZSPOT_H

#include "./ponto.h"
#include "./tupla.h"
#include "luz.h"

class luzSpot : public luz, public ponto{
private:
    tupla intensidade;
    tupla direcao;
    int ang_abertura;
public:
    luzSpot(float x, float y, float z, tupla intensidade, tupla direcao, int ang_abertura);
    ~luzSpot();
    tupla getIntensidade();
    tupla getDirecao();
    float getAng_abertura();
};

#endif