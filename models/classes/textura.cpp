#include "../headers/textura.h"
#include <iostream>

using namespace std;

textura::textura() : width(0), height(0), pitch(0), bpp(0) {}

textura::textura(const std::string& file_name) {
    SDL_Surface* surface = IMG_Load(file_name.c_str());
    if (!surface) {
        std::cerr << "Erro ao carregar textura: " << IMG_GetError() << std::endl;
        return;
    }

    // Converter para 24 bits por pixel (RGB sem alfa)
    SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
    SDL_FreeSurface(surface); // Liberar a original
    surface = converted;

    width = surface->w;
    height = surface->h;
    pitch = surface->pitch;
    bpp = surface->format->BytesPerPixel; // Deve ser 3 (RGB)

    // Garantindo que a textura está em 24 bits
    if (bpp != 3) {
        std::cerr << "Formato inesperado de BPP: " << bpp << ". Esperado 3 (RGB24)." << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Copiar pixels manualmente para `texture_data`
    Uint8* pixels = static_cast<Uint8*>(surface->pixels);
    texture_data.resize(width * height); // Armazenar em um vetor de 32 bits (RGB packed)

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * pitch) + (x * 3); // Cada pixel tem 3 bytes
            Uint8 r = pixels[index];
            Uint8 g = pixels[index + 1];
            Uint8 b = pixels[index + 2];

            // Guardamos como um Uint32 (embora seja 24 bits, o vetor precisa de 32 bits)
            texture_data[y * width + x] = (r << 16) | (g << 8) | b;
        }
    }

    SDL_FreeSurface(surface);
}

textura::textura(SDL_Surface* surface) {
    if (!surface) {
        std::cerr << "Superfície inválida para carregar textura!" << std::endl;
        return;
    }

    // Converter para 24 bits por pixel (RGB sem alfa)
    SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
    surface = converted;

    width = surface->w;
    height = surface->h;
    pitch = surface->pitch;
    bpp = surface->format->BytesPerPixel; // Deve ser 3 (RGB)

    if (bpp != 3) {
        std::cerr << "Formato inesperado de BPP: " << bpp << ". Esperado 3 (RGB24)." << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    Uint8* pixels = static_cast<Uint8*>(surface->pixels);
    texture_data.resize(width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * pitch) + (x * 3);
            Uint8 r = pixels[index];
            Uint8 g = pixels[index + 1];
            Uint8 b = pixels[index + 2];

            texture_data[y * width + x] = (r << 16) | (g << 8) | b;
        }
    }

    SDL_FreeSurface(surface);
}

tupla textura::sample(double u, double v) const {
    if (texture_data.empty()) {
        return tupla(0, 0, 0);
    }
    
    int x = static_cast<int>(u * (width - 1)) % width;
    int y = static_cast<int>(v * (height - 1)) % height;

    Uint32 pixel = texture_data[y * width + x];
    Uint8 r, g, b;
    r = (pixel >> 16) & 0xFF;
    g = (pixel >> 8) & 0xFF;
    b = pixel & 0xFF;

    return tupla(r / 255.0, g / 255.0, b / 255.0);
}

int textura::getWidth() { return width; }
int textura::getHeight() { return height; }
int textura::getPitch() { return pitch; }
int textura::getBpp() { return bpp; }