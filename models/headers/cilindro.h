#ifndef CILINDRO_H
#define CILINDRO_H

#include "./ponto.h"
#include "objeto.h"

class cilindro : public objeto{
private:
    ponto centroBase;
    ponto centroTopo;
    float radius;
public:
    bool colisaoBase;
    bool colisaoTopo;
    bool temBase;
    bool temAnel;
    bool temTopo;
    cilindro(ponto centroBase, ponto centroTopo, float radius, tupla Kd, tupla Ke, tupla Ka, float M, bool temBase, bool temAnel, bool temTopo);
    ~cilindro();
    ponto getcentroBase();
    ponto getcentroTopo();
    float getRaio();
    float colisao(raio ray) override;
    void transladar(float x, float y, float z) override;
    void rotacionar(ponto p1, ponto p2, float theta) override;
};


#endif