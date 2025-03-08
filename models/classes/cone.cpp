#include "../headers/cone.h"
#include "../headers/matriz.h"
#include <cmath>
#include "../headers/plano.h"
#include <cfloat>
#include <iostream>

using namespace std;
cone::cone(ponto centroBase, ponto vertice, float radius, tupla Kd, tupla Ke, tupla Ka, float M) : centroBase(centroBase), vertice(vertice), radius(radius), objeto(Kd, Ke, Ka, M){colisaoBase = false;};

cone::~cone(){};

ponto cone::getCentroBase(){
    return centroBase;
}

ponto cone::getVertice(){
    return vertice;
}

float cone::getRadius(){
    return radius;
}

float cone::colisao(raio raio){
    tupla dist = raio.getDist();
    ponto P0 = raio.getP0();
    tupla dirCone = tupla::sub(vertice, centroBase, false);
    float H = std::sqrt(dirCone.dot(dirCone));
    dirCone.normalize();

    matriz I = matriz::identidade();
    matriz Q = matriz::multComTransposta(dirCone);
    matriz M = I.subMatriz(Q);

    tupla S = tupla::sub(raio.getP0(), centroBase, false);
    float H2 = std::pow(H,2);
    float R2 = std::pow(radius,2);
    
    /*Calculando as possíveis distâncias para a superfície do cone*/

    float aux1 = dist.dot(M.multTupla(dist.multiplyByScalar(H2, false)));
    float aux2 = dist.dot(Q.multTupla(dist.multiplyByScalar(R2, false)));
    float aux3 = 2*(dist.dot(M.multTupla(S.multiplyByScalar(H2, false))));
    float aux4 = 2*(dirCone.dot(Q.multTupla(dist.multiplyByScalar(H*R2, false))));
    float aux5 = 2*(dist.dot(Q.multTupla(S.multiplyByScalar(R2, false))));
    float aux6 = S.dot(M.multTupla(S.multiplyByScalar(H2,false)));
    float aux7 = 2*(dirCone.dot(Q.multTupla(S.multiplyByScalar(H*R2, false))));
    float aux8 = S.dot(Q.multTupla(S.multiplyByScalar(R2,false))); 

    float a = aux1 - aux2;
    float b = aux3 + aux4 - aux5;
    float c = aux6 + aux7 - aux8 - H2*R2;

    float delta = std::pow(b, 2)-4*a*c;

    float T1 = -1000.0f;
    float T2 = -1000.0f;

    if (delta >= 0)
    {
        T1 = (-b + std::sqrt(delta))/(2*a);
        T2 = (-b - std::sqrt(delta))/(2*a);
    }

    /*Checando se as distâncias encontradas são válidas*/

    tupla aux9 = raio.getDist().multiplyByScalar(T1, false);
    ponto Pi1(P0.getCoord().at(0) + aux9.element1, P0.getCoord().at(1) + aux9.element2, P0.getCoord().at(2) + aux9.element3);

    tupla aux10 = raio.getDist().multiplyByScalar(T2, false);
    ponto Pi2(P0.getCoord().at(0) + aux10.element1, P0.getCoord().at(1) + aux10.element2, P0.getCoord().at(2) + aux10.element3);

    float projecao1 = tupla::sub(Pi1, centroBase, false).dot(dirCone);
    float projecao2 = tupla::sub(Pi2, centroBase, false).dot(dirCone);

    /*Checando se os pontos de intercessão estão na superfície do cone*/
    if (projecao1 < 0 || projecao1 > H)
    {
        T1 = -1000.0f;
    }

    if (projecao2 < 0 || projecao2 > H)
    {
        T2 = -1000.0f;
    }
    
    /*Criando TSup e pegando o menor T1 ou T2 existente*/

    float TSup;

    if (T1 != -1000 && T2 != -1000)
    {
        TSup = std::min(T1,T2);
    }
    else if (T1 == -1000 && T2 != -1000)
    {
        TSup = T2;
    }
    else if (T1 != -1000 && T2 == -1000)
    {
        TSup = T1;
    }
    else
    {
        TSup = -1000;
    }
    
    /*Criando o plano da base do cone*/
    plano base(centroBase, dirCone.multiplyByScalar(-1, false), getKd(), getKe(), getKa(), getM());

    /*Pegando a possível intercessão com a base do cone*/
    float T3 = -1000;
    T3 = base.colisao(raio);

    /*Checando se T3 é válido*/

    tupla aux11 = raio.getDist().multiplyByScalar(T3, false);
    ponto Pi3(P0.getCoord().at(0) + aux11.element1, P0.getCoord().at(1) + aux11.element2, P0.getCoord().at(2)  + aux11.element3);

    tupla projecao3 = (tupla::sub(Pi3, centroBase, false));

    /*Se a distância entre a intercessão e o centro da base for maior do que o raio, T3 é inválido*/
    if (std::sqrt(projecao3.dot(projecao3)) > radius)
    {
        T3 = -1000;
    }

    if (TSup == -1000)
    {
        TSup = FLT_MAX;
    }
    if (T3 == -1000)
    {
        T3 = FLT_MAX;
    }
    
    return min(T3, TSup);
}

void cone::transladar(float x, float y, float z){
    matriz M = matriz::translacao(x,y,z);
    centroBase = M.multPonto(centroBase);
    vertice = M.multPonto(vertice);
}

void cone::rotacionar(ponto p1, ponto p2, float theta){
    matriz M = matriz::rotacao(p1, p2, theta);
    centroBase = M.multPonto(centroBase);
    vertice = M.multPonto(vertice);
}