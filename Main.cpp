#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat> 
#include <SDL2/SDL.h>
#include "./models/headers/ponto.h"
#include "./models/headers/raio.h"
#include "./models/headers/tupla.h"
#include "./models/headers/esfera.h"
#include "./models/headers/objeto.h"
#include "./models/headers/pontoLuminoso.h"
#include "./models/headers/luzSpot.h"
#include "./models/headers/luzDirecional.h"
#include "./models/headers/utils.h"
#include "./models/headers/plano.h"
#include "./models/headers/cilindro.h"
#include "./models/headers/cone.h"
#include "./models/headers/triangulo.h"
#include "./models/headers/malha.h"
#include "./models/headers/matriz.h"
#include "./src/include/SDL2/SDL_events.h"
#include "./src/include/SDL2/SDL_keycode.h"
#include "./models/headers/textura.h"
using namespace std;

// Dimensões em pixels da window
const int NCOL = 500, NLINHA = 500;
// Dimensões reais da janela
const float DIST = 30.0f, WJANELA = 60.0f, HJANELA = 60.0f, DX = WJANELA/NCOL, DY = HJANELA/NLINHA;
// Dimensões dos objetos da cena
const float  RAIO = 40.0f, M_ESFERA = 10.0f, M_PLANO = 1.0f, P0Z = 0.0f, M_ESFERA2 = 100.0f;
const tupla K_ESFERA(0.7f,0.2f,0.2f), K_ESFERA2(0.8f,0.8f,0.8f), I_FONTE(0.6f, 0.6f, 0.6f), K_D_PLANO1(0.2f, 0.7f, 0.2f), K_D_PLANO2(0.3f, 0.3f, 0.7f), K_E_PLANO(0.0f, 0.0f, 0.0f), I_AMBIENTE(0.1f, 0.1f, 0.1f), KD_ESFERA3(0.87f, 0.97f, 0.28f), KE_ESFERA3(0.1f, 0.1f, 0.1f);
const tupla K_CILINDRO(0.0f, 0.0f, 0.8f), K_CILINDRO2(0.2f, 0.2f, 0.2f), K_CONE(0.2f, 0.8f, 0.2f);
ponto eye(10.0f, 35.0f, 60.0f), look_at(10.0f, 22.0f, 30.0f), ponto_up(10.0f, 23.0f, 30.0f); //cordenadas que definem a câmera
string projecao = "perspectiva";
textura floor_plane = textura("wood_texture.jpg");

const tupla X_AXIS(1.0f, 0.0f, 0.0f), Y_AXIS(0.0f, 1.0f, 0.0f), Z_AXIS(0.0f, 0.0f, 1.0f);

int main(int argc, char* argv[]) {
    // Inicialização da API SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Erro ao inicializar SDL: " << SDL_GetError() << endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Erro ao inicializar SDL_image: " << IMG_GetError() << std::endl;
        return 1;
    }

    // Inicialização da janela SDL
    SDL_Window* window = SDL_CreateWindow("Ray Casting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, NCOL, NLINHA, 0);
    if (!window) {
        cerr << "Erro ao criar janela: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    // Inicialização do renderizador SDL
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Erro ao criar renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    vector<luz*> luzes = {
        new pontoLuminoso(10.0f, 50.0f, 100.0f, I_FONTE),
        //new luzSpot(10.0f, 52.0f, 30.0f, I_FONTE, tupla(0.0f, -1.0f, 0.0f), 30.0f),
        //new luzDirecional(I_FONTE, tupla(-1.0f, -1.0f, -1.0f))
    };

    vector<triangulo*> faces = {
        new triangulo(ponto(8.0f, 22.3f, 30.0f), ponto(3.0f, 22.3f, 30.0f), ponto(8.0f, 22.3f, 35.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(3.0f, 22.3f, 30.0f), ponto(3.0f, 22.3f, 35.0f), ponto(8.0f, 22.3f, 35.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(3.0f, 22.3f, 30.0f), ponto(8.0f, 22.3f, 30.0f), ponto(8.0f, 22.8f, 28.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(3.0f, 22.3f, 30.0f), ponto(8.0f, 22.8f, 28.0f), ponto(3.0f, 22.8f, 28.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(8.0f, 22.3f, 30.0f), ponto(8.0f, 22.3f, 35.0f), ponto(10.0f, 22.8f, 35.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(8.0f, 22.3f, 30.0f), ponto(10.0f, 22.8f, 35.0f), ponto(10.0f, 22.8f, 30.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(8.0f, 22.3f, 35.0f), ponto(3.0f, 22.3f, 35.0f), ponto(3.0f, 22.8f, 37.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(8.0f, 22.3f, 35.0f), ponto(3.0f, 22.8f, 37.0f), ponto(8.0f, 22.8f, 37.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(3.0f, 22.3f, 30.0f), ponto(1.0f, 22.8f, 30.0f), ponto(3.0f, 22.3f, 35.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(3.0f, 22.3f, 35.0f), ponto(1.0f, 22.8f, 30.0f), ponto(1.0f, 22.8f, 35.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(8.0f, 22.3f, 30.0f), ponto(10.0f, 22.8f, 30.0f), ponto(8.0f, 22.8f, 28.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(8.0f, 22.3f, 35.0f), ponto(8.0f, 22.8f, 37.0f), ponto(10.0f, 22.8f, 35.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(3.0f, 22.3f, 35.0f), ponto(1.0f, 22.8f, 35.0f), ponto(3.0f, 22.8f, 37.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA),
        new triangulo(ponto(3.0f, 22.3f, 30.0f), ponto(3.0f, 22.8f, 28.0f), ponto(1.0f, 22.8f, 30.0f), K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA)
    };
    
    vector<objeto*> objetos = {
        //mesa
        new cilindro(ponto(10.0f, 0.0f, 30.0f), ponto(10.0f, 20.0f, 30.0f), 3.0f, K_ESFERA, K_ESFERA, K_ESFERA, M_ESFERA, false, true, false),
        new cilindro(ponto(10.0f, 20.0f, 30.0f), ponto(10.0f, 22.0f, 30.0f), 15.0f, K_ESFERA, K_ESFERA, K_ESFERA, M_ESFERA, true, true, true),

        //prato e fruta
        new malha(faces),
        new esfera(ponto(5.0f, 23.2f, 32.0f), 1.5f, KD_ESFERA3, KE_ESFERA3, KD_ESFERA3, M_ESFERA2),

        //copo e bebida
        new cilindro(ponto(15.0f, 22.0f, 30.0f), ponto(15.0f, 25.0f, 30.0f), 2, K_ESFERA2, K_ESFERA2, K_ESFERA2, M_ESFERA, false, true, false),
        new cilindro(ponto(15.0f, 22.0f, 30.0f), ponto(15.0f, 24.5f, 30.0f), 2, K_CILINDRO, K_CILINDRO, K_CILINDRO, M_ESFERA, false, false, true),

        //guarda chuva
        new cilindro(ponto(14.8f, 24.0f, 30.0f), ponto(16.0f, 27.0f, 30.0f), 0.2, K_CILINDRO2, K_CILINDRO2, K_CILINDRO2, M_ESFERA, false, true, false),
        new cone(ponto(15.814f, 26.5355f, 30.0f), ponto(16.558f ,28.3935f, 30.0f), 3, K_CONE, K_CONE, K_CONE, M_ESFERA2, false, true),

        new plano(ponto(0.0f, 0.0f, 0.0f), Y_AXIS, tupla(0,0,0), tupla(0,0,0), K_D_PLANO1, M_PLANO, &floor_plane, 0.01, 0.01),
        new plano(ponto(0.0f, 0.0f, 0.0f), Z_AXIS, K_D_PLANO2, K_E_PLANO, K_D_PLANO2, M_PLANO)
    };

    vector<float> distancias(objetos.size());

    bool rodando = true;
    while (rodando) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Atalhos para encerrar a janela
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) { rodando = false; }
            
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    // Atalhos para movimentação da câmera
                    case SDLK_d: eye.x += 1.0f; break;
                    case SDLK_a: eye.x -= 1.0f; break;
                    case SDLK_w: eye.y += 1.0f; break;
                    case SDLK_s: eye.y -= 1.0f; break;
                    case SDLK_LSHIFT: eye.z -= 1.0f; break;
                    case SDLK_SPACE: eye.z += 1.0f; break;
                    
                   // Atalhos para a definição da perspectiva
                    case SDLK_1: projecao = "perspectiva"; break; // Perspectiva (padrão)
                    case SDLK_2: projecao = "ortografica"; break; // Ortográfica
                    case SDLK_3: projecao = "obliqua"; break; // Oblíqua
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                float x = event.button.x;
                float y = event.button.y;

                float Px = -WJANELA / 2.0f + x * DX + DX / 2.0f;
                float Py = HJANELA / 2.0f - y * DY - DY / 2.0f;

                ponto p_camera(Px, Py, P0Z - DIST);
                matriz cameraWorld = matriz::cameraToWorld(eye, look_at, ponto_up);

                ponto p_mundo = cameraWorld.multPonto(p_camera);

                raio raio(eye, tupla::sub(p_mundo, eye, true));

                objeto* o = utils::pick(raio, objetos);

                switch (event.button.button)
                {
                case SDL_BUTTON_RIGHT:
                    utils::deletar(o, objetos);
                    break;
                case SDL_BUTTON_LEFT:
                    utils::transformar(o);
                    break;
                }
            }
            
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Define a cor de fundo como preto
        SDL_RenderClear(renderer); // Limpa a tela

        for (int y = 0; y < NCOL; y++) {
            float Py = HJANELA / 2.0f - y * DY - DY / 2.0f;
            for (int x = 0; x < NLINHA; x++) {
                float Px = -WJANELA / 2.0f + x * DX + DX / 2.0f;
                // Definição da projeção
                ponto eye_atual = eye;
                matriz cameraWorld = matriz::cameraToWorld(eye, look_at, ponto_up);

                if(projecao == "ortografica"){ 
                    eye_atual = ponto(Px, Py, P0Z);
                    eye_atual = cameraWorld.multPonto(eye_atual); // Projeção ortográfica: Ponto parte do centro de cada píxel
                }
                else if(projecao == "obliqua"){
                    tupla direcao_obliqua(1.0f, 1.0f, -1.0f); // Vetor direção para a projeção oblíqua
                    direcao_obliqua.normalize(); // Normaliza para garantir proporção correta

                    eye_atual = ponto(Px + direcao_obliqua.element1, Py + direcao_obliqua.element2, P0Z + direcao_obliqua.element3); // Projeção oblíqua: Ponto parte do centro de cada píxel com direção igual ao vetor passado
                    eye_atual = cameraWorld.multPonto(eye_atual);
                }

                // Definição da câmera
                ponto p_camera(Px, Py, P0Z - DIST);

                ponto p_mundo = cameraWorld.multPonto(p_camera);

                raio ray(eye_atual, tupla::sub(p_mundo, eye_atual, true));

                // Detecção do objeto mais próximo ao observador com nenhuma obstrução
                int index = -1;
                float menorDistancia = FLT_MAX;
                for (size_t i = 0; i < objetos.size(); i++) {
                    float d = objetos[i]->colisao(ray);
                    if (d != -1000 && d < menorDistancia) {
                        menorDistancia = d;
                        index = static_cast<int>(i);
                    }
                }
                
                // Pinta o pixel com a cor de fundo caso não tenha sido detectado algum objeto

                if (index == -1) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                } else {
                    objeto* obj = objetos[index];
                    tupla aux = ray.getDist().multiplyByScalar(menorDistancia, false);
                    ponto Pi(eye.getCoord().at(0) + aux.element1, eye.getCoord().at(1) + aux.element2, eye.getCoord().at(2) + aux.element3);

                    // Pintura do objeto conforme seu tipo
                    // Cone
                    tupla normal;
                    if (auto* co = dynamic_cast<cone*>(obj)) {
                        tupla dirCone = tupla::sub(co->getVertice(), co->getCentroBase(), true);
                        if (co->colisaoBase) {
                            normal = dirCone.multiplyByScalar(-1, false);
                        } else {
                            matriz M = matriz::identidade().subMatriz(matriz::multComTransposta(dirCone));
                            normal = M.multTupla(tupla::sub(Pi, co->getCentroBase(), false));
                            normal.normalize();
                        }
                        co->colisaoBase = false;
                    }
                    // Cilindro
                    else if (auto ci = dynamic_cast<cilindro*>(obj)) {
                        tupla dirCilindro = tupla::sub(ci->getcentroTopo(), ci->getcentroBase(), true);
                        if (ci->colisaoBase)
                        {
                            normal = dirCilindro.multiplyByScalar(-1, false);
                        } else if (ci->colisaoTopo)
                        {
                            normal = dirCilindro;
                        } else{
                            matriz M = matriz::identidade().subMatriz(matriz::multComTransposta(dirCilindro));
                            normal = M.multTupla(tupla::sub(Pi, ci->getcentroBase(), false));
                            normal.normalize();
                        }
                        ci->colisaoBase = false;
                        ci->colisaoTopo = false;
                    }
                    // Plano
                    else if (auto* p = dynamic_cast<plano*>(obj)) {
                        normal = p->getNormal();
                    }
                    // Malha
                    else if (auto* m = dynamic_cast<malha*>(obj))
                    {
                        triangulo* t = m->getFaceIntersec();
                        normal = t->getNormal();
                        obj = t;
                    }
                    // Esfera
                    else if (auto* e = dynamic_cast<esfera*>(obj)) {
                        normal = tupla::sub(Pi, e->getCentro(), true);
                    }
                    tupla cor = utils::calcularCores(menorDistancia, ray, normal, luzes, I_AMBIENTE, obj);
                    cor = utils::calcularSombra(Pi, luzes, obj->getKa(), I_AMBIENTE, cor, objetos);
                    SDL_SetRenderDrawColor(renderer, cor.element1, cor.element2, cor.element3, 255);
                }
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Liberação de memória dos objetos alocados
    for (objeto* obj : objetos) delete obj;
    for (luz* l : luzes) delete l;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}