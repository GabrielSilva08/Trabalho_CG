#include "../headers/cilindro.h"
#include "../headers/matriz.h"
#include <cmath>
#include "../headers/plano.h"
#include <cfloat>
#include <iostream>

cilindro::cilindro(ponto centroBase, ponto centroTopo, float radius, tupla Kd, tupla Ke, tupla Ka, float M) : centroBase(centroBase), centroTopo(centroTopo), radius(radius), objeto(Kd, Ke, Ka, M){colisaoBase = false; colisaoTopo = false;};

cilindro::~cilindro(){};

ponto cilindro::getcentroBase(){
    return this->centroBase;
}

ponto cilindro::getcentroTopo(){
    return this->centroTopo;
}

float cilindro::getRaio(){
    return this->radius;
}

float cilindro::colisao(raio raio){
    tupla dirCilindro = tupla::sub(centroTopo, centroBase, false);
    float H = std::sqrt(dirCilindro.dot(dirCilindro));
    dirCilindro.normalize();

    matriz I = matriz::identidade();
    matriz Q = matriz::multComTransposta(dirCilindro);
    matriz M = I.subMatriz(Q);

    tupla aux1 = M.multTupla(raio.getDist());
    tupla aux2 = M.multTupla(tupla::sub(raio.getP0(), this->centroBase, false));

    float a = aux1.dot(aux1);
    float b = 2.0f*(aux1.dot(aux2));
    float c = aux2.dot(aux2) - std::pow(radius, 2.0f);

    float T1 = -1000.0f;
    float T2 = -1000.0f;

    float delta = std::pow(b, 2)-4*a*c;

    if (delta >= 0)
    {
        T1 = (-b + std::sqrt(delta))/(2*a);
        T2 = (-b - std::sqrt(delta))/(2*a);
    }

    ponto P0 = raio.getP0();
    tupla aux3 = raio.getDist().multiplyByScalar(T1, false);
    tupla aux4 = raio.getDist().multiplyByScalar(T2, false);

    ponto Pi1(P0.getCoord().at(0) + aux3.element1, P0.getCoord().at(1) + aux3.element2, P0.getCoord().at(2)  + aux3.element3);
    ponto Pi2(P0.getCoord().at(0) + aux4.element1, P0.getCoord().at(1) + aux4.element2, P0.getCoord().at(2)  + aux4.element3);
    
    tupla projecaoCilindro1 = Q.multTupla(tupla::sub(Pi1, centroBase, false));
    tupla projecaoCilindro2 = Q.multTupla(tupla::sub(Pi2, centroBase, false));
    
    if (std::sqrt(projecaoCilindro1.dot(projecaoCilindro1)) < 0.0f || std::sqrt(projecaoCilindro1.dot(projecaoCilindro1)) > H)
    {
        T1 = -1000.0f;
    }  
    
    if (std::sqrt(projecaoCilindro2.dot(projecaoCilindro2)) < 0.0f || std::sqrt(projecaoCilindro2.dot(projecaoCilindro2)) > H)
    {
        T2 = -1000.0f;
    }

    float Tanel;

    if (T1 != -1000 && T2 != -1000)
    {
        Tanel = std::min(T1,T2);
    }
    else if (T1 == -1000 && T2 != -1000)
    {
        Tanel = T2;
    }
    else if (T1 != -1000 && T2 == -1000)
    {
        Tanel = T1;
    }
    else
    {
        Tanel = -1000;
    }
    
    plano base(centroBase, dirCilindro.multiplyByScalar(-1, false), getKd(), getKe(), getKa(), getM());
    plano topo(centroTopo, dirCilindro, getKd(), getKe(), getKa(), getM());

    float T3 = -1000;
    float T4 = -1000;

    T3 = base.colisao(raio);
    T4 = topo.colisao(raio);

    tupla aux5 = raio.getDist().multiplyByScalar(T3, false);
    tupla aux6 = raio.getDist().multiplyByScalar(T4, false);

    ponto Pi3(P0.getCoord().at(0) + aux5.element1, P0.getCoord().at(1) + aux5.element2, P0.getCoord().at(2)  + aux5.element3);
    ponto Pi4(P0.getCoord().at(0) + aux6.element1, P0.getCoord().at(1) + aux6.element2, P0.getCoord().at(2)  + aux6.element3);

    tupla projecaoCilindro3 = (tupla::sub(Pi3, centroBase, false));
    tupla projecaoCilindro4 = (tupla::sub(Pi4, centroTopo, false));

    if (std::sqrt(projecaoCilindro3.dot(projecaoCilindro3)) > radius)
    {
        T3 = -1000;
    }

    if (std::sqrt(projecaoCilindro4.dot(projecaoCilindro4)) > radius)
    {
        T4 = -1000;
    }

    if (Tanel == -1000 && T3 == -1000 && T4 == -1000)
    {
        return -1000;
    }
    
    if (Tanel == -1000)
    {
        Tanel = FLT_MAX;
    }
    if (T3 == -1000)
    {
        T3 = FLT_MAX;
    }
    if (T4 == -1000)
    {
        T4 = FLT_MAX;
    }
    
    float retorno = std::min(Tanel,T3);

    if (retorno > T4)
    {
        retorno = T4;
    }

    if (retorno == T3){
        colisaoBase = true;
    } else if (retorno == T4)
    {
        colisaoTopo = true;
    }
    
    return retorno;
}

void cilindro::transladar(float x, float y, float z){
    matriz M = matriz::translacao(x,y,z);
    centroBase = M.multPonto(centroBase);
    centroTopo = M.multPonto(centroTopo);
}

void cilindro::rotacionar(ponto p1, ponto p2, float theta){
    matriz M = matriz::rotacao(p1, p2, theta);
    centroBase = M.multPonto(centroBase);
    centroTopo = M.multPonto(centroTopo);
}