#ifndef PLANO_H
#define PLANO_H
#include "ponto.h"
#include "tupla.h"
#include "objeto.h"
#include "raio.h"
#include "../headers/textura.h"

class plano : public objeto
{
private:
    ponto Pi;
    tupla Normal;
    bool has_texture;
    double x_texture_scale;
    double y_texture_scale;

public:
    textura* textura_plano;
    plano(ponto Pi, tupla Normal, tupla Kd, tupla Ke, tupla Ka, float M);
    plano(ponto Pi, tupla Normal, tupla Kd, tupla Ke, tupla Ka, float M, textura* textura_plano, double x_texture_scale, double y_texture_scale);
    ponto getPi();
    tupla getNormal();
    bool getHas_texture();
    double getX_tex_scale();
    double getY_tex_scale();
    float colisao(raio raio) override;
    ~plano();
};

#endif