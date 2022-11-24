#ifndef Ponto_hpp
#define Ponto_hpp

#include <iostream>
#include <cmath>
using namespace std;

class Ponto {

public:
    float x,y,z;
    int cor;

    Ponto ();
    Ponto(float x, float y, float z=0);
    void rotacionaZ(float angulo);
} ;

Ponto ObtemMinimo (Ponto P1, Ponto P2);
Ponto ObtemMaximo (Ponto P1, Ponto P2);
Ponto operator+(Ponto P1, Ponto P2);
Ponto operator- (Ponto P1, Ponto P2);
Ponto operator* (Ponto P1, float k);

double calculaDistancia(Ponto P, Ponto Q);
bool operator==(Ponto P1, Ponto P2);
#endif