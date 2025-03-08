#ifndef TUPLA_H
#define TUPLA_H

#include "ponto.h"

class tupla
{
private:
public:
    float element1;
    float element2;
    float element3;
    float element4;
    tupla(float element1, float element2, float element3);
    tupla(float element1, float element2, float element3, float element4);
    tupla();
    void normalize();
    static tupla add(ponto P1, ponto P2, bool normalize);
    static tupla sub(ponto P1, ponto P2, bool normalize);
    static tupla addTupla(tupla T1, tupla T2, bool normalize);
    static tupla subTupla(tupla T1, tupla T2, bool normalize);
    tupla multiplyByScalar(float scalar, bool normalize) const;
    tupla cross(const tupla& other, bool normalize) const;
    float dot(const tupla& other) const;
    float dotPonto(ponto p) const;
    ~tupla();
};
#endif