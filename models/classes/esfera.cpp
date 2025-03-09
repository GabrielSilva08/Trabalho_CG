#include "../headers/esfera.h"
#include "../headers/matriz.h"
#include <cmath>
#include <iostream>

esfera::esfera(ponto centro, float radius, tupla Kd, tupla Ke, tupla Ka, float M)
    : centro(centro), radius(radius), objeto(Kd, Ke, Ka, M)
{
};

ponto esfera::getCentro(){
    return this->centro;
}

float esfera::getRaio(){
    return this->radius;
}

float esfera::colisao(raio raio){ 
    ponto centro = this->centro;
    float R = this->radius;
    tupla dist = raio.getDist();

    tupla visao = tupla::sub(centro, raio.getP0(), false);

    float a = dist.dot(dist);
    float b = -2*dist.dot(visao);
    float c = visao.dot(visao) - std::pow(R,2);
    
    float descriminante = std::pow(b, 2)-4*a*c;

    if (descriminante >= 0)
    {
        float T1 = (-b + std::sqrt(descriminante))/(2*a);
        float T2 = (-b - std::sqrt(descriminante))/(2*a);

        return std::min(T1, T2);
    }
    
    return -1000;
}

void esfera::transladar(float x, float y, float z){
    matriz M = matriz::translacao(x, y, z);
    centro = M.multPonto(centro);
}

void esfera::rotacionar(ponto p1, ponto p2, float theta){
    matriz M = matriz::rotacao(p1, p2, theta);
    centro = M.multPonto(centro);
}

esfera::~esfera(){
    
}