#ifndef TEXTURA_H
#define TEXTURA_H
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./tupla.h"

class textura
{
private:
    std::vector<int> texture_data;

public:
    textura();
    textura(const std::string& file_name);
    textura(SDL_Surface* surface);
    
    tupla sample(double u, double v) const;
    int width;
    int height;
    int pitch;
    int bpp;

    int getWidth();
    int getHeight();
    int getPitch();
    int getBpp();
};

#endif