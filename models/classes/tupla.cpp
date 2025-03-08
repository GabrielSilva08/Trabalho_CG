 #include "../headers/tupla.h"
#include <cmath>

// Construtor
tupla::tupla(float element1, float element2, float element3) 
    : element1(element1), element2(element2), element3(element3) {
        this->element4 = 1;
    }

tupla::tupla(float element1, float element2, float element3, float element4) 
    : element1(element1), element2(element2), element3(element3), element4(element4) {}

tupla::tupla(){
    element1 = 0;
    element2 = 0;
    element3 = 0;
    element4 = 1;
}

// Comprimento da tupla
float tupla::magnitude(){
    return std::sqrt(element1 * element1 + element2 * element2 + element3 * element3);
}


// Normaliza a tupla
void tupla::normalize() {
    float magnitude = this->magnitude();
    if (magnitude > 0) {
        element1 /= magnitude;
        element2 /= magnitude;
        element3 /= magnitude;
    }
}

float tupla::dot(const tupla& other) const {
    return element1 * other.element1 + element2 * other.element2 + element3 * other.element3;
}


// Adiciona duas tuplas e normaliza o resultado
tupla tupla::add(ponto P1, ponto P2, bool normalize) {
    std::array<float, 3> coords1 = P1.getCoord();
    std::array<float, 3> coords2 = P2.getCoord();

    tupla result(coords1[0] + coords2[0], coords1[1] + coords2[1], coords1[2] + coords2[2]);
    if (normalize)
    {
        result.normalize();  // Normaliza o resultado
    }
    return result;
}

tupla tupla::addTupla(tupla T1, tupla T2, bool normalize) {

    tupla result(T1.element1 + T2.element1, T1.element2+ T2.element2, T1.element3 + T2.element3);
    if (normalize)
    {
        result.normalize();  // Normaliza o resultado
    }
    return result;
}

// Subtrai duas tuplas e normaliza o resultado
tupla tupla::sub(ponto P1, ponto P2, bool normalize) {
    std::array<float, 3> coords1 = P1.getCoord();
    std::array<float, 3> coords2 = P2.getCoord();

    tupla result(coords1[0] - coords2[0], coords1[1] - coords2[1], coords1[2] - coords2[2]);
    if (normalize)
    {
        result.normalize();  // Normaliza o resultado
    }
    return result;
}

tupla tupla::subTupla(tupla T1, tupla T2, bool normalize) {

    tupla result(T1.element1 - T2.element1, T1.element2 - T2.element2, T1.element3 - T2.element3);
    if (normalize)
    {
        result.normalize();  // Normaliza o resultado
    }
    return result;
}

tupla tupla::multiplyByScalar(float scalar, bool normalize) const {
    tupla result(element1 * scalar, element2 * scalar, element3 * scalar);

    if (normalize)
    {
        result.normalize(); // Normaliza o resultado antes de retorná-lo
    }
    return result;
}

tupla tupla::cross(const tupla& other, bool normalize) const {
    tupla result(
        element2 * other.element3 - element3 * other.element2,
        element3 * other.element1 - element1 * other.element3,
        element1 * other.element2 - element2 * other.element1
    );
    
    if (normalize) {
        result.normalize(); // Normaliza o resultado antes de retorná-lo
    }
    
    return result;
}


// Destrutor
tupla::~tupla() {}
