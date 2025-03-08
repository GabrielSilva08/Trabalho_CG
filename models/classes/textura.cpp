#include "../headers/textura.h"

textura::textura(){}

textura::textura(const std::string& file_name){
    SDL_Surface* surface = IMG_Load(file_name.c_str());
    *this = textura(surface);
    SDL_FreeSurface(surface);
}

textura::textura(SDL_Surface* surface){
    width =surface->w;
    height = surface->h;
    pitch = surface->pitch;
    bpp = pitch/width;

    texture_data = std::vector<int>(width*height*bpp);
    memcpy(texture_data.data(), surface->pixels, texture_data.size());
}

tupla textura::sample(double u, double v) const{
    int x = static_cast<int>(u*width);
    int y = static_cast<int>(v*height);

    size_t index = (y * pitch + x * bpp);
    int r = texture_data[index];
    int g = texture_data[index+1];
    int b = texture_data[index+2];

    return tupla(r/255.0, g/255.0, b/255.0);
}