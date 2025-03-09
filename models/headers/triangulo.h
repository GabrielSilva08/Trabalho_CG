#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "ponto.h"
#include "objeto.h"
#include "raio.h"

class triangulo : public objeto
{
private:
    tupla normal;
    tupla calcNormal();
public:
    ponto v1;
    ponto v2;
    ponto v3;
    triangulo(ponto v1, ponto v2, ponto v3, tupla Kd, tupla Ke, tupla Ka, float M);
    ~triangulo();
    ponto getV1();
    ponto getV2();
    ponto getV3();
    tupla getNormal();
    float colisao(raio ray) override;
    void transladar(float x, float y, float z) override;
    void rotacionar(ponto p1, ponto p2, float theta) override;
    void escalarT(float x, float y, float z, ponto p);
};

#endif