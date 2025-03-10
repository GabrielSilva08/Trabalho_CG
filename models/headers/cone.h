#ifndef CONE_H
#define CONE_H

#include "./ponto.h"
#include "objeto.h"

class cone : public objeto {
private:
    ponto centroBase;
    ponto vertice;
    float radius;
public:
    bool colisaoBase;
    bool temBase;
    bool temAnel;
    cone(ponto centroBase, ponto vertice, float radius, tupla Kd, tupla Ke, tupla Ka, float M, bool temBase, bool temAnel);
    ~cone();
    ponto getCentroBase();
    ponto getVertice();
    float getRadius();
    float colisao(raio ray) override;
    void transladar(float x, float y, float z) override;
    void rotacionar(ponto p1, ponto p2, float theta) override;
};

#endif