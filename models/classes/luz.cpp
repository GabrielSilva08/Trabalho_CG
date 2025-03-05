#include "../headers/luz.h"

tupla luz::getIntensidade(){
    return this->intensidade;
}

luz::luz(){}

luz::luz(tupla intensidade): intensidade(intensidade){
    this->intensidade = intensidade;
}