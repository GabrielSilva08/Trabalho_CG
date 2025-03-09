#ifndef UTILS_H
#define UTILS_H
#include "./tupla.h"
#include "./ponto.h"
#include "./pontoLuminoso.h"
#include "./raio.h"
#include "./objeto.h"
#include "./luz.h"
#include <vector>

class utils
{
public:
    static tupla calcularDiff(ponto Pi, tupla normal, ponto fonte, tupla K, tupla I_PONTUAL);

    static tupla calcularEspec(ponto Pi, ponto P0, tupla normal, ponto fonte, float REFLEXIVIDADE, tupla K, tupla I_PONTUAL);

    static tupla calcularCores(float T, raio raio, tupla normal, std::vector<luz*>& luzes, tupla I_am, objeto* objeto);

    static tupla calcularSombra(ponto Pi, std::vector<luz*>& luzes, tupla Ka,tupla I_am, tupla cores, std::vector<objeto*>& objetos);

    static objeto* pick(raio ray, std::vector<objeto*>& objetos);

    static void deletar(objeto* o, std::vector<objeto*>& objetos);
    
    static void transformar(objeto* o);

    static void clamp(tupla* cores);
};

#endif