#ifndef OBJETO_H
#define OBJETO_H
#include "tupla.h"
#include "raio.h"

class objeto
{
private:
    tupla Kd;
    tupla Ke;
    tupla Ka;
    float M;
public:
    objeto();
    objeto(tupla Kd, tupla Ke, tupla Ka, float M);
    virtual ~objeto() = default;
    tupla getKd();
    tupla getKe();
    tupla getKa();
    float getM();
    virtual float colisao(raio raio) {
        return -1000;
    };
    virtual void transladar(float x, float y, float z){};
    virtual void rotacionar(ponto p1, ponto p2, float theta){};
};

#endif