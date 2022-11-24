/***********************************************************************************************************/
#include <iostream>
#include <fstream>
using namespace std;

#include "Poligono.h"
/***********************************************************************************************************/
Poligono::Poligono(){}
/***********************************************************************************************************/
void Poligono::insereVertice(Ponto p){
    Vertices.push_back(p);
}
/***********************************************************************************************************/
Ponto Poligono::getVertice(int i){
    return Vertices[i];
}
/***********************************************************************************************************/
void Poligono::pintaPoligono(){
    glBegin(GL_POLYGON);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
/***********************************************************************************************************/
void Poligono::desenhaPoligono(){
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
/***********************************************************************************************************/
unsigned long Poligono::getNVertices(){
    return Vertices.size();
}
/***********************************************************************************************************/
void Poligono::obtemLimites(Ponto &Min, Ponto &Max){
    Max = Min = Vertices[0];
    
    for (int i=0; i<Vertices.size(); i++){
        Min = ObtemMinimo (Vertices[i], Min);
        Max = ObtemMaximo (Vertices[i], Max);
    }
}
/***********************************************************************************************************/
void Poligono::LePoligono(const char *nome){
    ifstream input;   
    char file[255] = {"resources/"};
    strcat(file, nome);
    input.open(file, ios::in);
    if (!input){
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    unsigned int qtdVertices;
    input >> qtdVertices;
    for (int i=0; i< qtdVertices; i++){
        double x,y;
        input >> x >> y;
        if(!input)
            break;
        insereVertice(Ponto(x,y));
    }
    cout << "Poligono lido com sucesso!" << endl;
}
/***********************************************************************************************************/
Poligono Poligono::createEnvelope(){
    Poligono envelope;
    Ponto min, max;
    this->obtemLimites(min, max);

    envelope.insereVertice(min);
    envelope.insereVertice(Ponto(min.x, max.y));
    envelope.insereVertice(max);
    envelope.insereVertice(Ponto(max.x, min.y));

    envelope.desenhaPoligono();
    return envelope;
}
/***********************************************************************************************************/