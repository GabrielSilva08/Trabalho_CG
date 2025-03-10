#include "../headers/triangulo.h"
#include <iostream>
#include <math.h>
#include "../headers/matriz.h"

using namespace std;

triangulo::triangulo(ponto v1, ponto v2, ponto v3, tupla Kd, tupla Ke, tupla Ka, float M) 
    : v1(v1), v2(v2), v3(v3), objeto(Kd, Ke, Ka, M) {
    normal = this->calcNormal();
}

triangulo::~triangulo() {}

ponto triangulo::getV1() {
    return this->v1;
}

ponto triangulo::getV2() {
    return this->v2;
}

ponto triangulo::getV3() {
    return this->v3;
}

tupla triangulo::calcNormal() {
    tupla aux1 = tupla::sub(v2, v1, false);
    tupla aux2 = tupla::sub(v3, v1, false);
    return aux1.cross(aux2, true); // Produto vetorial para calcular a normal
}

tupla triangulo::getNormal() {
    return this->normal;
}

float triangulo::colisao(raio ray) {
    ponto P0 = ray.getP0();
    tupla v = tupla::sub(P0, v1, false);
    float epsilon = 0.0001f;
    
    float denom = ray.getDist().dot(normal);
    if (fabs(denom) < epsilon) {
        return -1000.0f; // O raio é paralelo ao triângulo
    }

    float t = -(v.dot(normal) / denom);
    if (t < 0) {
        return -1000.0f; // O triângulo está atrás da origem do raio
    }
    
    tupla aux = ray.getDist().multiplyByScalar(t, false);
    ponto Pi(
        P0.getCoord().at(0) + aux.element1,
        P0.getCoord().at(1) + aux.element2,
        P0.getCoord().at(2) + aux.element3
    );

    // Teste de bordas usando produto vetorial
    tupla edge1 = tupla::sub(v2, v1, false);
    tupla edge2 = tupla::sub(v3, v2, false);
    tupla edge3 = tupla::sub(v1, v3, false);

    tupla C1 = tupla::sub(Pi, v1, false);
    tupla C2 = tupla::sub(Pi, v2, false);
    tupla C3 = tupla::sub(Pi, v3, false);

    if (normal.dot(edge1.cross(C1, false)) < 0 ||
        normal.dot(edge2.cross(C2, false)) < 0 ||
        normal.dot(edge3.cross(C3, false)) < 0) {
        return -1000.0f; // O ponto está fora do triângulo
    }

    return t; // O ponto está dentro do triângulo
}

void triangulo::transladar(float x, float y, float z){
    matriz M = matriz::translacao(x,y,z);
    v1 = M.multPonto(v1);
    v2 = M.multPonto(v2);
    v3 = M.multPonto(v3);
}

void triangulo::rotacionar(ponto p1, ponto p2, float theta){
    
    matriz M = matriz::rotacao(p1, p2, theta);
    v1 = M.multPonto(v1);
    v2 = M.multPonto(v2);
    v3 = M.multPonto(v3);
    normal = calcNormal(); //recalculando a normal
}

void triangulo::escalarT(float x, float y, float z, ponto p){ 
    
    if ((v1.x == p.x && v1.y == p.y && v1.z == p.z))
    {
        cout << "Origem" << endl;
    }

    matriz M = matriz::escala(x,y,z,p);
    v1 = M.multPonto(v1);
    v2 = M.multPonto(v2);
    v3 = M.multPonto(v3);

    if ((v1.x == 0 && v1.y == 0 && v1.z == 0))
    {
        cout << "Origem2" << endl;
    }
    
}