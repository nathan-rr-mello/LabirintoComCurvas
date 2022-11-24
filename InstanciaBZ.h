#ifndef Instancia_hpp
#define Instancia_hpp

#include <iostream>
#include <math.h>

using namespace std;

#include "Bezier.h"
#include "ListaDeCoresRGB.h"
#include "Poligono.h"

typedef void TipoFuncao();

class InstanciaBZ{
public:
    InstanciaBZ();
    InstanciaBZ(Bezier *Curva);
    
    Bezier *Curva;
    TipoFuncao *modelo; 
    Poligono* poly;
    Ponto Posicao, Escala;

    float Rotacao, Velocidade, tAtual;  
    int cor, direcao, nroDaCurva, proxCurva;

    Ponto ObtemPosicao();
    void desenha();
    void AtualizaPosicao(float tempoDecorrido);
};
#endif