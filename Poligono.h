#ifndef Poligono_hpp
#define Poligono_hpp

#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <GL/glut.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <cstring>
#endif
#ifdef __linux__
#include <GL/glut.h>
#include <cstring>
#endif
#include "Ponto.h"
#include <vector>

using namespace std;

class Poligono{
    vector<Ponto> Vertices;
    Ponto Min, Max;

public:
    Poligono();
    Poligono createEnvelope();
    Ponto getVertice(int);
    unsigned long getNVertices();
    void insereVertice(Ponto);
    void desenhaPoligono();
    void pintaPoligono();
    void obtemLimites(Ponto &Min, Ponto &Max);
    void LePoligono(const char *nome);
};
#endif