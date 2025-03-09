#include "../headers/utils.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cfloat> 
#include "../headers/cilindro.h"
#include "../headers/luzSpot.h"
#include "../headers/luzDirecional.h"
#include "../../src/include/SDL2/SDL_render.h"
#include "../headers/triangulo.h"
#include "../headers/plano.h"
#include "../headers/textura.h"

using namespace std;
tupla utils::calcularDiff(ponto Pi, tupla normal, ponto fonte, tupla Kdif, tupla I_PONTUAL){
    tupla luz = tupla::sub(fonte, Pi, true); 

    float luminosidade = fmaxf(0.0f, normal.dot(luz));

    tupla I_dif(Kdif.element1 * I_PONTUAL.element1 * luminosidade, 
    Kdif.element2 * I_PONTUAL.element2 * luminosidade,
    Kdif.element3 * I_PONTUAL.element3 * luminosidade);

    return I_dif;
}

tupla utils::calcularEspec(ponto Pi, ponto P0, tupla normal, ponto fonte, float REFLEXIVIDADE, tupla Kesp, tupla I_PONTUAL){
    tupla luz = tupla::sub(fonte, Pi, true);
    
    tupla reflexo = normal.multiplyByScalar(normal.dot(luz), false);
    reflexo = reflexo.multiplyByScalar(2, false);
    reflexo = tupla::subTupla(reflexo, luz, false);
    
    reflexo.normalize();

    tupla visao = tupla::sub(P0, Pi, true);

    float reflexao = powf(fmax(0.0f, reflexo.dot(visao)), REFLEXIVIDADE);

    tupla I_Espec(Kesp.element1 * I_PONTUAL.element1 * reflexao, 
    Kesp.element2 * I_PONTUAL.element2 * reflexao,
    Kesp.element3 * I_PONTUAL.element3 * reflexao);

    return I_Espec;
}

tupla utils::calcularCores(float T, raio raio, tupla normal, std::vector<luz*>& luzes, tupla I_am, objeto* objeto){
    ponto P0 = raio.getP0();
    tupla aux = raio.getDist().multiplyByScalar(T, false);
    ponto Pi(P0.getCoord().at(0) + aux.element1, P0.getCoord().at(1) + aux.element2, P0.getCoord().at(2)  + aux.element3);
    tupla Ka(objeto->getKa().element1 * I_am.element1, 
             objeto->getKa().element2 * I_am.element2,
             objeto->getKa().element3 * I_am.element3);
    tupla cores = Ka;

    // Adicionando textura ao plano (se o for)
    if(plano* PL = dynamic_cast<plano*>(objeto)){
        // Verificando se o plano possui textura
        if(PL->getHas_texture()){
            // Cálculo da base
            tupla base1 = (PL->getNormal().cross(tupla(1,0,0), true));
            if(base1.magnitude() < 1e-6) base1 = (PL->getNormal().cross(tupla(0,1,0), true));
            tupla base2 = PL->getNormal().cross(base1, false);

            tupla aux = (tupla::sub(Pi, P0, false));
            
            // Cálculo das coordenadas da textura
            double u = aux.dot(base1); 
            u *= PL->getX_tex_scale(); // eixo z
            
            double v = aux.dot(base2);
            v *= PL->getY_tex_scale(); // eixo z

            u = u - floor(u);
            v = v - floor(v);

            //cout << "(u,v) = " << "(" << u << "," << v << ")";
            cores = PL->textura_plano->sample(u, v);
            //cout << "Cheguei aqui cores!";
        }
    }

    for (luz* luzPtr : luzes){
        // Luz pontual
        if(pontoLuminoso* posLuz = dynamic_cast<pontoLuminoso*>(luzPtr)){
            ponto pontoLuz(posLuz->getCoord().at(0), posLuz->getCoord().at(1), posLuz->getCoord().at(2));
            tupla I_dif = utils::calcularDiff(Pi, normal, pontoLuz, objeto->getKd(), posLuz->getIntensidade());
            tupla I_espec = utils::calcularEspec(Pi, P0, normal, pontoLuz, objeto->getM(), objeto->getKe(), posLuz->getIntensidade());
            
            cores = tupla::addTupla(cores, I_dif, false);
            cores = tupla::addTupla(cores, I_espec, false);
        }
        // Luz Spot
        else if(luzSpot* spotLuz = dynamic_cast<luzSpot*>(luzPtr)){
            tupla l = tupla::sub(*spotLuz, Pi, true); // Vetor luz
            tupla d = spotLuz->getDirecao(); // Vetor direção principal da luz spot
            float angulo = acos((-1) * l.dot(d)) * 180.0f / M_PI; // Ângulo entre alpha e -df (graus)
            if(angulo < spotLuz->getAng_abertura()){
                ponto pontoLuz(spotLuz->getCoord().at(0), spotLuz->getCoord().at(1), spotLuz->getCoord().at(2));
                tupla I_dif = utils::calcularDiff(Pi, normal, pontoLuz, objeto->getKd(), spotLuz->getIntensidade());
                tupla I_espec = utils::calcularEspec(Pi, P0, normal, pontoLuz, objeto->getM(), objeto->getKe(), spotLuz->getIntensidade());
                
                I_dif = I_dif.multiplyByScalar((-1)*l.dot(d), false);
                cores = tupla::addTupla(cores, I_dif, false);
                cores = tupla::addTupla(cores, I_espec, false);
            }
        }
        // Luz Direcional
        else if(luzDirecional* direcionalLuz = dynamic_cast<luzDirecional*>(luzPtr)){
            tupla l = direcionalLuz->getDirecao(); // Vetor luz
            // Calculando I_dif diretamente
            tupla Kdif = objeto->getKd();
            tupla I_PONTUAL = direcionalLuz->getIntensidade();
            float luminosidade = fmaxf(0.0f, normal.dot(l));

            tupla I_dif(Kdif.element1 * I_PONTUAL.element1 * luminosidade, Kdif.element2 * I_PONTUAL.element2 * luminosidade, Kdif.element3 * I_PONTUAL.element3 * luminosidade);
            // Calculando I_espec diretamente
            float REFLEXIVIDADE = objeto->getM();
            tupla Kesp = objeto->getKe();           
            tupla reflexo = normal.multiplyByScalar(normal.dot(l), false);

            reflexo = reflexo.multiplyByScalar(2, false);
            reflexo = tupla::subTupla(reflexo, l, false);
            reflexo.normalize();

            tupla visao = tupla::sub(P0, Pi, true);

            float reflexao = powf(fmax(0.0f, reflexo.dot(visao)), REFLEXIVIDADE);

            tupla I_espec(Kesp.element1 * I_PONTUAL.element1 * reflexao, Kesp.element2 * I_PONTUAL.element2 * reflexao, Kesp.element3 * I_PONTUAL.element3 * reflexao);
        
            cores = tupla::addTupla(cores, I_dif, false);
            cores = tupla::addTupla(cores, I_espec, false);
        }
    }

    cores = cores.multiplyByScalar(255, false);                    
    utils::clamp(&cores);

    return cores;
}

tupla utils::calcularSombra(ponto Pi, std::vector<luz*>& luzes, tupla Ka, tupla I_am, tupla cores, std::vector<objeto*>& objetos){    
    for(luz* luzPtr: luzes){
        bool emSombra = true; // Flag para verificar se o ponto está sombreado

        // Luz pontual
        if(pontoLuminoso* posLuz = dynamic_cast<pontoLuminoso*>(luzPtr)){
            tupla dist = tupla::sub(*posLuz, Pi, false);
            float comprimento = sqrt(pow(dist.element1, 2.0f) + pow(dist.element2, 2.0f) + pow(dist.element3, 2.0f));

            raio raio(Pi, tupla::sub(*posLuz, Pi, true));
            float t = -1000;

            for(int i = 0; i < objetos.size(); i++){
                float aux = objetos[i]->colisao(raio);

                if ((t == -1000 && aux != -1000) || (t != -1000 && aux != -1000 && aux < t))
                {
                    t = aux;
                }      
            }

            if (t == -1000 || t > comprimento || t < 1)
            {
                return cores;
            }
        }
        // Luz Spot 
        else if(luzSpot* posLuz = dynamic_cast<luzSpot*>(luzPtr)){
            tupla dist = tupla::sub(*posLuz, Pi, false);
            float comprimento = sqrt(pow(dist.element1, 2.0f) + pow(dist.element2, 2.0f) + pow(dist.element3, 2.0f));

            raio raio(Pi, tupla::sub(*posLuz, Pi, true));
            float t = -1000;

            for(int i = 0; i < objetos.size(); i++){
                float aux = objetos[i]->colisao(raio);

                if ((t == -1000 && aux != -1000) || (t != -1000 && aux != -1000 && aux < t))
                {
                    t = aux;
                }      
            }

            if (t == -1000 || t > comprimento || t < 1)
            {
                return cores;
            }
        }
        // Luz Direcional
        else if(luzDirecional* posLuz = dynamic_cast<luzDirecional*>(luzPtr)){
            return cores;
        }
    }
    tupla FinalKa(Ka.element1 * I_am.element1, Ka.element2 * I_am.element2, Ka.element3 * I_am.element3);
    FinalKa = FinalKa.multiplyByScalar(255, false);
    utils::clamp(&FinalKa);

    return FinalKa;
}

void utils::clamp(tupla* cores){
    if (cores->element1 > 255)
    {
        cores->element1 = 255;
    }

    if (cores->element2 > 255)
    {
        cores->element2 = 255;
    }

    if (cores->element3 > 255)
    {
        cores->element3 = 255;
    }
}

objeto* utils::pick(raio ray, std::vector<objeto*>& objetos){
    float t = -1000.0;
    objeto* retorno = nullptr;
    for (objeto* o : objetos)
    {
        float aux = o->colisao(ray);

        if ((t == -1000 && aux != -1000) || (t != -1000 && aux != -1000 && aux < t))
            {
                t = aux;
                retorno = o;
            }
    }
    
    return retorno;
}

void utils::deletar(objeto* o, vector<objeto*>& objetos){
    auto it = find(objetos.begin(), objetos.end(), o);

    if (it != objetos.end())
    {
        objetos.erase(it);
        cout << "objeto deletado com sucesso" << endl;
    } else{
        cerr << "elemento não encontrado" << endl;
    }
    
}

void utils::transformar(objeto* o){
    cout << "Escolha a transformação desejada, 1 para translação, 2 para rotação. 3 para escala (escala só disponível para malha)" << endl;
    int t;
    cin >> t;
    switch (t)
    {
    case 1:
    {
        float x,y,z;
        cout << "Digite as componentes x y z da translação" << endl;
        cin >> x >> y >> z;

        o->transladar(x,y,z);

        cout << "objeto transladado com sucesso" << endl;
        
        break;
    }
    case 2:
    {
        float p1x, p1y, p1z, p2x, p2y, p2z, theta;
        cout << "Digite o x, y e z do p1 do eixo de rotação:" << endl;
        cin >> p1x >> p1y >> p1z;
        cout << "Agora, digite o x, y e z do p2 do eixo de rotação:" << endl;
        cin >> p2x >> p2y >> p2z;
        cout << "Por fim, digite o ângulo theta de rotação (a rotação será no sentido anti-horário):" << endl;
        cin >> theta;

        ponto p1(p1x, p1y, p1z);
        ponto p2(p2x, p2y, p2z);

        o->rotacionar(p1, p2, theta);

        cout << "objeto rotacionado com sucesso" << endl;

        break;
    }
    case 3:
    {
        float x,y,z;
        cout << "Digite as componentes x y z da escala" << endl;
        cin >> x >> y >> z;

        o->escalar(x,y,z);

        cout << "Objeto escalado com sucesso" << endl;

        break;
    }
    default:
        cerr << "transformação inválida" << endl;
        break;
    }
}