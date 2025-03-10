#include "../headers/matriz.h"
#include "../headers/ponto.h"
#include <cmath>

matriz::matriz(tupla linha1, tupla linha2, tupla linha3) : linha1(linha1), linha2(linha2), linha3(linha3){
    this->linha4 = tupla(0,0,0,1);
}

matriz::matriz(tupla linha1, tupla linha2, tupla linha3, tupla linha4) : linha1(linha1), linha2(linha2), linha3(linha3), linha4(linha4){}

matriz::~matriz(){

}

std::vector<tupla> matriz::getLinhas(){
    std::vector<tupla> linhas = {linha1, linha2, linha3, linha4};

    return linhas;
}

matriz matriz::identidade(){

    tupla linha1(1,0,0,0);
    tupla linha2(0,1,0,0);
    tupla linha3(0,0,1,0);
    tupla linha4(0,0,0,1);

    return matriz(linha1, linha2, linha3, linha4);
}

matriz matriz::multComTransposta(tupla Tupla){
    return matriz(
        tupla(Tupla.element1 * Tupla.element1, Tupla.element1 * Tupla.element2, Tupla.element1 * Tupla.element3, Tupla.element1 * Tupla.element4),
        tupla(Tupla.element2 * Tupla.element1, Tupla.element2 * Tupla.element2, Tupla.element2 * Tupla.element3, Tupla.element2 * Tupla.element4),
        tupla(Tupla.element3 * Tupla.element1, Tupla.element3 * Tupla.element2, Tupla.element3 * Tupla.element3, Tupla.element3 * Tupla.element4),
        tupla(Tupla.element4 * Tupla.element1, Tupla.element4 * Tupla.element2, Tupla.element4 * Tupla.element3, Tupla.element4 * Tupla.element4)
    );
}

matriz matriz::multMatriz(matriz Matriz){
    std::vector<tupla> colunas = Matriz.getLinhas();
    
    tupla coluna1(colunas[0].element1, colunas[1].element1, colunas[2].element1, colunas[3].element1);
    tupla coluna2(colunas[0].element2, colunas[1].element2, colunas[2].element2, colunas[3].element2);
    tupla coluna3(colunas[0].element3, colunas[1].element3, colunas[2].element3, colunas[3].element3);
    tupla coluna4(colunas[0].element4, colunas[1].element4, colunas[2].element4, colunas[3].element4);

    return matriz(
        tupla(linha1.dot(coluna1), linha1.dot(coluna2), linha1.dot(coluna3), linha1.dot(coluna4)),
        tupla(linha2.dot(coluna1), linha2.dot(coluna2), linha2.dot(coluna3), linha2.dot(coluna4)),
        tupla(linha3.dot(coluna1), linha3.dot(coluna2), linha3.dot(coluna3), linha3.dot(coluna4)),
        tupla(linha4.dot(coluna1), linha4.dot(coluna2), linha4.dot(coluna3), linha4.dot(coluna4))
    );
}

matriz matriz::addMatriz(matriz Matriz) {
    std::vector<tupla> linhasOutraMatriz = Matriz.getLinhas();
    return matriz(
        tupla(linha1.element1 + linhasOutraMatriz[0].element1, linha1.element2 + linhasOutraMatriz[0].element2, linha1.element3 + linhasOutraMatriz[0].element3, linha1.element4 + linhasOutraMatriz[0].element4),
        tupla(linha2.element1 + linhasOutraMatriz[1].element1, linha2.element2 + linhasOutraMatriz[1].element2, linha2.element3 + linhasOutraMatriz[1].element3, linha2.element4 + linhasOutraMatriz[1].element4),
        tupla(linha3.element1 + linhasOutraMatriz[2].element1, linha3.element2 + linhasOutraMatriz[2].element2, linha3.element3 + linhasOutraMatriz[2].element3, linha3.element4 + linhasOutraMatriz[2].element4),
        tupla(linha4.element1 + linhasOutraMatriz[3].element1, linha4.element2 + linhasOutraMatriz[3].element2, linha4.element3 + linhasOutraMatriz[3].element3, linha4.element4 + linhasOutraMatriz[3].element4)
    );
}

matriz matriz::subMatriz(matriz Matriz) {
    std::vector<tupla> linhasOutraMatriz = Matriz.getLinhas();
    return matriz(
        tupla(linha1.element1 - linhasOutraMatriz[0].element1, linha1.element2 - linhasOutraMatriz[0].element2, linha1.element3 - linhasOutraMatriz[0].element3, linha1.element4 - linhasOutraMatriz[0].element4),
        tupla(linha2.element1 - linhasOutraMatriz[1].element1, linha2.element2 - linhasOutraMatriz[1].element2, linha2.element3 - linhasOutraMatriz[1].element3, linha2.element4 - linhasOutraMatriz[1].element4),
        tupla(linha3.element1 - linhasOutraMatriz[2].element1, linha3.element2 - linhasOutraMatriz[2].element2, linha3.element3 - linhasOutraMatriz[2].element3, linha3.element4 - linhasOutraMatriz[2].element4),
        tupla(linha4.element1 - linhasOutraMatriz[3].element1, linha4.element2 - linhasOutraMatriz[3].element2, linha4.element3 - linhasOutraMatriz[3].element3, linha4.element4 - linhasOutraMatriz[3].element4)
    );
}

matriz matriz::worldToCamera(ponto eye, ponto at, ponto up){
    tupla camera3 = tupla::sub(eye, at, true);
    tupla vup = tupla::sub(up, eye, false);
    tupla camera1 = vup.cross(camera3, true);
    tupla camera2 = camera3.cross(camera1, false);

    float aux1 = -camera1.dot(tupla::sub(eye, ponto(0,0,0), false));
    float aux2 = -camera2.dot(tupla::sub(eye, ponto(0,0,0), false));
    float aux3 = -camera3.dot(tupla::sub(eye, ponto(0,0,0), false));

    camera1.element4 = aux1;
    camera2.element4 = aux2;
    camera3.element4 = aux3;

    return matriz(camera1, camera2, camera3);
}

matriz matriz::cameraToWorld(ponto eye, ponto at, ponto up) {
    // 1. Eixo Z da câmera (direção da câmera, apontando para trás)
    tupla camera3 = tupla::sub(eye, at, true);

    // 2. Eixo X da câmera (direção direita, perpendicular ao up e à direção da câmera)
    tupla vup = tupla::sub(up, eye, false);
    tupla camera1 = vup.cross(camera3, true);

    // 3. Eixo Y da câmera (direção "para cima", perpendicular aos dois anteriores)
    tupla camera2 = camera3.cross(camera1, false);

    // 4. Posição da câmera
    array<float, 3> cordEye = eye.getCoord();

    return matriz(
        tupla(camera1.element1, camera2.element1, camera3.element1, cordEye.at(0)),
        tupla(camera1.element2, camera2.element2, camera3.element2, cordEye.at(1)),
        tupla(camera1.element3, camera2.element3, camera3.element3, cordEye.at(2))
    );
}

matriz matriz::translacao(float x, float y, float z){
    return matriz(
        tupla(1,0,0,x),
        tupla(0,1,0,y),
        tupla(0,0,1,z),
        tupla(0,0,0,1)
    );
}

matriz matriz::rotacao(ponto p1, ponto p2, float theta){
    tupla U = tupla::sub(p2, p1, true);

    theta *= M_PI/180;

    float x = std::sin(theta/2)*U.element1;
    float y = std::sin(theta/2)*U.element2;
    float z = std::sin(theta/2)*U.element3;
    float w = std::cos(theta/2);

    float x2 = std::pow(x,2);
    float y2 = std::pow(y,2);
    float z2 = std::pow(z,2);
    float w2 = std::pow(w,2);
    float xy = x*y;
    float xz = x*z;
    float xw = x*w;
    float yz = y*z;
    float yw = y*w;
    float zw = z*w;

    matriz Q = matriz(
        tupla((w2 + x2 - y2 - z2), 2*(xy - zw), 2*(xz + yw), 0),
        tupla(2*(xy + zw), (w2 - x2 + y2 - z2), 2*(yz - xw), 0),
        tupla(2*(xz - yw), 2*(yz + xw), (w2 - x2 - y2 + z2), 0),
        tupla(0,0,0,1)
    );

    return matriz::translacao(p1.x, p1.y, p1.z).multMatriz(Q).multMatriz(matriz::translacao(-p1.x, -p1.y, -p1.z));
}

matriz matriz::escala(float x, float y, float z, ponto p){
    matriz Q(
        tupla(x,0,0,0),
        tupla(0,y,0,0),
        tupla(0,0,z,0),
        tupla(0,0,0,1)
    );

    return matriz::translacao(p.x, p.y, p.z).multMatriz(Q).multMatriz(matriz::translacao(-p.x, -p.y, -p.z));
}

tupla matriz::multTupla(tupla vetorColuna) {
    return tupla(
        linha1.dot(vetorColuna),
        linha2.dot(vetorColuna),
        linha3.dot(vetorColuna),
        linha4.dot(vetorColuna)
    );
}

ponto matriz::multPonto(ponto p) {
    return ponto(
        linha1.dotPonto(p),
        linha2.dotPonto(p),
        linha3.dotPonto(p)
    );
}