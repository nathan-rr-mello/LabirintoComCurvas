/***********************************************************************************************************/
#include "Ponto.h"

long int ContadorInt=0;
/***********************************************************************************************************/
Ponto::Ponto (){
    x=y=z=0;
}
/***********************************************************************************************************/
Ponto::Ponto(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}
/***********************************************************************************************************/
void Ponto::rotacionaZ(float angulo){
    float xr, yr;
    double anguloRad = angulo * 3.14159265359/180.0;
    xr = x*cos(anguloRad) - y*sin(anguloRad);
    yr = x*sin(anguloRad) + y*cos(anguloRad);
    x = xr;
    y = yr;
}
/***********************************************************************************************************/
Ponto ObtemMaximo (Ponto P1, Ponto P2){
    Ponto Max;
    Max.x = (P2.x > P1.x) ? P2.x : P1.x;
    Max.y = (P2.y > P1.y) ? P2.y : P1.y;
    Max.z = (P2.z > P1.x) ? P2.z : P1.z;
    return Max;
}
/***********************************************************************************************************/
Ponto ObtemMinimo (Ponto P1, Ponto P2){
    Ponto Min;
    Min.x = (P2.x < P1.x) ? P2.x : P1.x;
    Min.y = (P2.y < P1.y) ? P2.y : P1.y;
    Min.z = (P2.z < P1.x) ? P2.z : P1.z;
    return Min;
}
/***********************************************************************************************************/
bool operator==(Ponto P1, Ponto P2){
    if (P1.x != P2.x) return false;
    if (P1.y != P2.y) return false;
    if (P1.z != P2.z) return false;
    return true;
}
/***********************************************************************************************************/
Ponto operator+(Ponto P1, Ponto P2){
    Ponto temp;
    temp = P1;
    temp.x += P2.x;
    temp.y += P2.y;
    temp.z += P2.z;
    return temp;
}
/***********************************************************************************************************/
Ponto operator- (Ponto P1, Ponto P2){
    Ponto temp;
    temp = P1;
    temp.x -= P2.x;
    temp.y -= P2.y;
    temp.z -= P2.z;
    return temp;
}
/***********************************************************************************************************/
Ponto operator* (Ponto P1, float k){
    Ponto temp;
    temp.x = P1.x * k;
    temp.y = P1.y * k;
    temp.z = P1.z * k;
    return temp;
}
/***********************************************************************************************************/
double calculaDistancia(Ponto P, Ponto Q){
    float dx, dy, dz;
    dx = P.x - Q.x;
    dy = P.y - Q.y;
    dz = P.z - Q.z;
    return sqrt(dx*dx+dy*dy+dz*dz);
}
/***********************************************************************************************************/