/***********************************************************************************************************/
#include "InstanciaBZ.h"
/***********************************************************************************************************/
void InstanciaPonto(Ponto &p, Ponto &out){
    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;
    
    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);
    for(i=0;i<4;i++){
        ponto_novo[i]= matriz_gl[0][i] * p.x+
        matriz_gl[1][i] * p.y+
        matriz_gl[2][i] * p.z+
        matriz_gl[3][i];
    }
    out.x=ponto_novo[0];
    out.y=ponto_novo[1];
    out.z=ponto_novo[2];
}
/***********************************************************************************************************/
Ponto InstanciaPonto(Ponto P){
    Ponto temp;
    InstanciaPonto(P, temp);
    return temp;
}
/***********************************************************************************************************/
InstanciaBZ::InstanciaBZ(){
    Rotacao = 0;
    Posicao = Ponto(0,0,0);
    Escala = Ponto(0.5,0.5,0.5);
    nroDaCurva = 0;
    proxCurva = -1;
    tAtual = 0.0;
    direcao = 1;
    Velocidade = 3.0;

}
/***********************************************************************************************************/
InstanciaBZ::InstanciaBZ(Bezier *C){
    Rotacao = 0;
    Posicao = Ponto(0,0,0);
    Escala = Ponto(0.5,0.5,0.5);
    Curva = C;
    tAtual = 0;
    direcao = 1;
    Velocidade = 3.0;
}
/***********************************************************************************************************/
void InstanciaBZ::desenha(){
    glPushMatrix();
        glTranslatef(Posicao.x, Posicao.y, 0);
        glRotatef(Rotacao, 0, 0, 1);
        glScalef(Escala.x, Escala.y, Escala.z);
        
        (*modelo)();
        
    glPopMatrix();
}
/***********************************************************************************************************/
Ponto InstanciaBZ::ObtemPosicao(){
    glPushMatrix();
        glTranslatef(Posicao.x, Posicao.y, 0);
        glRotatef(Rotacao, 0, 0, 1);
        Ponto PosicaoDoPersonagem;
        Ponto Origem (0,0,0);
        InstanciaPonto(Origem, PosicaoDoPersonagem);
    glPopMatrix();
    return PosicaoDoPersonagem;
}
/***********************************************************************************************************/
void InstanciaBZ::AtualizaPosicao(float tempoDecorrido){
    float desloc = Velocidade * tempoDecorrido;
    float deltaT = desloc/ Curva->ComprimentoTotalDaCurva;

    if(direcao == 1){
        tAtual += deltaT;
    }else{
        tAtual -= deltaT;
    }

    if(tAtual > 1) tAtual = 1;
    if(tAtual < 0) tAtual = 0;

    Ponto tempPosicao = Curva->Calcula(tAtual);
    Rotacao = atan2(Posicao.y - tempPosicao.y, Posicao.x - tempPosicao.x) * (180 / 3.14159265);
    Posicao = tempPosicao;
}
/***********************************************************************************************************/