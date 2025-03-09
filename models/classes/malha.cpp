#include "../headers/malha.h"
#include<bits/stdc++.h>
#include <cfloat>

malha::malha(vector<triangulo*> faces) : faces(faces) {
    // Calcula a normal média das faces da malha
    normal = tupla(0, 0, 0);
    defineMinMax();
}

malha::~malha() {
}

tupla malha::getNormal() {
    return normal;
}

triangulo* malha::getFaceIntersec(){
    return faces[intersecI];
}

void malha::defineMinMax(){
    minX = FLT_MAX;
    maxX = FLT_MIN;
    minY = FLT_MAX;
    maxY = FLT_MIN;
    minZ = FLT_MAX;
    maxZ = FLT_MIN;

    for(triangulo* f : faces){
        ponto v1 = f->v1;
        ponto v2 = f->v2;
        ponto v3 = f->v3;

        if (min({v1.x,v2.x,v3.x}) < minX)
        {
            minX = min({v1.x,v2.x,v3.x});
        }
        if (min({v1.y,v2.y,v3.y}) < minY)
        {
            minY = min({v1.y,v2.y,v3.y});
        }
        if (min({v1.z,v2.z,v3.z}) < minZ)
        {
            minZ = min({v1.z,v2.z,v3.z});
        }
        if (max({v1.x,v2.x,v3.x}) > maxX)
        {
            maxX = max({v1.x,v2.x,v3.x});
        }
        if (max({v1.y,v2.y,v3.y}) > maxY)
        {
            maxY = max({v1.y,v2.y,v3.y});
        }
        if (max({v1.z,v2.z,v3.z}) > maxZ)
        {
            maxZ = max({v1.z,v2.z,v3.z});
        }
    }
}

float malha::colisao(raio ray) {
    // Primeiro, verificamos se o raio intersecta a Bounding Box
    float tMin = (minX - ray.getP0().x) / ray.getDist().element1;
    float tMax = (maxX - ray.getP0().x) / ray.getDist().element1;
    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (minY - ray.getP0().y) / ray.getDist().element2;
    float tyMax = (maxY - ray.getP0().y) / ray.getDist().element2;
    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax)) return -1000.0f;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    float tzMin = (minZ - ray.getP0().z) / ray.getDist().element3;
    float tzMax = (maxZ - ray.getP0().z) / ray.getDist().element3;
    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax)) return -1000.0f;

    // Se passou na Bounding Box, testamos com as faces
    float menorDistancia = FLT_MAX;
    intersecI = -1;

    for (size_t i = 0; i < faces.size(); i++) {
        float distancia = faces[i]->colisao(ray);
        if (distancia > 0 && distancia < menorDistancia) {
            menorDistancia = distancia;
            intersecI = static_cast<int>(i);
        }
    }

    return (intersecI != -1) ? menorDistancia : -1000.0f;
}

void malha::transladar(float x, float y, float z){
    for (triangulo* face : faces)
    {
        face->transladar(x,y,z); //chamando a função de translação para todas as faces
    }
    defineMinMax();
}

void malha::rotacionar(ponto p1, ponto p2, float theta){
    for (triangulo* face : faces)
    {
        face->rotacionar(p1,p2,theta); //chamando a função de rotação para todas as faces
    }
    defineMinMax();
}

void malha::escalar(float x, float y, float z){
    ponto p = faces.at(0)->getV1(); //ponto arbitrário para fixar

    for (triangulo* face: faces)
    {
        face->escalarT(x,y,z,p);
    }
    defineMinMax();
}