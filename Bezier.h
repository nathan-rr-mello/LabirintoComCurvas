#ifndef Bezier_h
#define Bezier_h

#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <GL/glut.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

using namespace std;

#include "Ponto.h"

class Bezier{
    Ponto Coords[3];

public:
    float ComprimentoTotalDaCurva;
    int cor;

    Bezier();
    Bezier(Ponto P0, Ponto P1, Ponto P2);
    Bezier(Ponto V[]);
    Ponto Calcula(double t);
    Ponto getPC(int i);
    double CalculaT(double distanciaPercorrida);
    void Traca();
    void calculaComprimentoDaCurva();
};
#endif