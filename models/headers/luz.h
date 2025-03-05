#ifndef LUZ_H
#define LUZ_H
#include "tupla.h"
#include "raio.h"

class luz
{
private:
    tupla intensidade;
public:
    luz();
    luz(tupla intensidade);
    virtual ~luz() = default;
    tupla getIntensidade();
};

#endif