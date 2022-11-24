/***********************************************************************************************************/
#include <ctime>
#include <fstream>

#ifdef WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <sys/time.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

using namespace std;

#include "InstanciaBZ.h"
#include "Temporizador.h"

Bezier *Curvas;
InstanciaBZ *Personagens;
Poligono *envelopes;

Temporizador T, T2;
Ponto Min, Max;
Poligono Jogador, Mastro, MeiaSeta, PontosDeControle;

int **nextStates;
int *curvasPlayer, *tCurvasPlayer;
bool* trocaDeCurva;

double AccumDeltaT = 0, nFrames = 0, TempoTotal = 0;
float angulo = 0.0;
int indexPlayer, nInstancias = 0, sizePlayer, z=0;
unsigned int nCurvas;
bool desenha = false, movimento = true;
/************************************************************************************************************/
void animate(){
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;
    if (AccumDeltaT > 1.0 / 30){
        AccumDeltaT = 0;
        angulo += 15;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0){
        TempoTotal = 0;
        nFrames = 0;
    }
}
/***********************************************************************************************************/
void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(Min.x, Max.x, Min.y, Max.y, -10, +10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
/***********************************************************************************************************/
void DesenhaEixos(){
    Ponto Meio;
    Meio.x = (Max.x + Min.x) / 2;
    Meio.y = (Max.y + Min.y) / 2;
    Meio.z = (Max.z + Min.z) / 2;
    glBegin(GL_LINES);
    glVertex2f(Min.x, Meio.y);
    glVertex2f(Max.x, Meio.y);
    glVertex2f(Meio.x, Min.y);
    glVertex2f(Meio.x, Max.y);
    glEnd();
}
/***********************************************************************************************************/
void DesenhaSeta(){
    glPushMatrix();
    MeiaSeta.desenhaPoligono();
    glScaled(1, -1, 1);
    MeiaSeta.desenhaPoligono();
    glPopMatrix();
}
/***********************************************************************************************************/
void DesenhaApontador(){
    glPushMatrix();
    glTranslated(-0.7, 0, 0);
    DesenhaSeta();
    glPopMatrix();
}
/***********************************************************************************************************/
void DesenhaHelice(){
    glPushMatrix();
    for (int i = 0; i < 4; i++){
        glRotatef(90, 0, 0, 1);
        DesenhaApontador();
    }
    glPopMatrix();
}
/***********************************************************************************************************/
void DesenhaHelicesGirando(){
    glPushMatrix();
    glRotatef(angulo, 0, 0, 1);
    DesenhaHelice();
    glPopMatrix();
}
/***********************************************************************************************************/
void DesenhaJogador(){
    glPushMatrix();
        defineCor(Green);
        glRotatef(90, 0, 0, 1);
        glLineWidth(2);
        Jogador.desenhaPoligono();
        Jogador.pintaPoligono();
    glPopMatrix();
}
/***********************************************************************************************************/
void DesenhaInimigo(){
    glPushMatrix();
        defineCor(Red);
        glRotatef(90, 0, 0, 1);
        glLineWidth(2);
        Jogador.desenhaPoligono();
        Jogador.pintaPoligono();
        glTranslatef(0,0.8,0);
        glLineWidth(1);
        defineCor(Yellow);
        DesenhaHelicesGirando();
    glPopMatrix();
}
/***********************************************************************************************************/
void CriaInstancias(){
    nInstancias = 11;
    Personagens = new InstanciaBZ[nInstancias];
    Personagens[0] = InstanciaBZ(&Curvas[0]);
    Personagens[0].modelo = DesenhaJogador;
    Personagens[0].poly = &Jogador;

    for(int k=1;k<11;k++){
        Personagens[k] = InstanciaBZ(&Curvas[k]);
        Personagens[k].modelo = DesenhaInimigo;
        Personagens[k].poly = &Jogador;
    }
    envelopes = new Poligono[nInstancias];

    nextStates = (int**) malloc(nInstancias * sizeof(int*));
    for(int i = 0; i < nInstancias; i++){
        nextStates[i] = (int*) malloc(2 * sizeof(int));
    }
    trocaDeCurva = new bool[nInstancias];
    for(int i = 0; i < nInstancias; i++){
        trocaDeCurva[i] = true;
    }
}
/***********************************************************************************************************/
void CarregaModelos(){
    MeiaSeta.LePoligono("MeiaSeta.txt");
    Jogador.LePoligono("Mastro.txt");
    PontosDeControle.LePoligono("PontosDeControleTesteMartins.txt");
}
/***********************************************************************************************************/
void CarregaCurvas(int**& PontosCurvas, const char *nome){
    ifstream input;
    char file[255] = {"resources/"};
    strcat(file, nome);
    input.open(file, ios::in);
    if (!input){
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    input >> nCurvas;
    
    PontosCurvas = (int**) malloc(nCurvas * sizeof(int*));
    for(int i = 0; i < nCurvas; i++){
        PontosCurvas[i] = (int*) malloc(3 * sizeof(int));
    }

    for (int i=0; i < nCurvas; i++){
        int first,second,third;
        input >> first >> second >> third;
        if(!input){
            break; 
        }   
        PontosCurvas[i][0] = first;
        PontosCurvas[i][1] = second;
        PontosCurvas[i][2] = third;
    }
    cout << "Curva lida com sucesso!" << endl;
}
/***********************************************************************************************************/
void CriaCurvas(){
    int** PontosCurvas;
    CarregaCurvas(PontosCurvas, "CurvasTesteMartins.txt");
    Curvas = new Bezier[nCurvas];

    for(int i = 0; i < nCurvas; i++){
        Ponto start = PontosDeControle.getVertice(PontosCurvas[i][0]) * 2;
        Ponto middle = PontosDeControle.getVertice(PontosCurvas[i][1]) * 2;
        Ponto end = PontosDeControle.getVertice(PontosCurvas[i][2]) * 2;
        Curvas[i] = Bezier(start, middle, end);
    }
}
/***********************************************************************************************************/
void init(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    CarregaModelos();
    CriaCurvas();
    CriaInstancias();
    float d = 15;
    Min = Ponto(-d, -d);
    Max = Ponto(d, d);
}
/***********************************************************************************************************/
void AtualizaCurva(int i){
    Bezier* tempCurva = Personagens[i].Curva;
    Ponto pontoComum = tempCurva->Calcula(Personagens[i].direcao);
    int* possiveisCurvas = new int[nCurvas]; 
    int* inicioOuFim = new int[nCurvas]; 
    int matchCounter = 0; 

    for(int index = 0; index < nCurvas; index++){
        Bezier temp = Curvas[index];
        Ponto start = temp.getPC(0);
        Ponto end = temp.getPC(2);
        if(pontoComum == start){
            possiveisCurvas[matchCounter] = index;
            inicioOuFim[matchCounter] = 0;
            matchCounter++;
        }
        else if(pontoComum == end){
            possiveisCurvas[matchCounter] = index;
            inicioOuFim[matchCounter] = 1;
            matchCounter++;
        }
    }

    srand(time(NULL));
    int randIndex = rand() % matchCounter;
    int curvaIndex = possiveisCurvas[randIndex]; 
    int nextT = inicioOuFim[randIndex]; 
    nextStates[i][0] = curvaIndex;
    nextStates[i][1] = nextT;

    if(i == 0){
        curvasPlayer = possiveisCurvas;
        tCurvasPlayer = inicioOuFim;
        indexPlayer = randIndex;
        sizePlayer = matchCounter;
    }
}
/***********************************************************************************************************/
void AtualizaPersonagem(float tempoDecorrido, int index){
    if(trocaDeCurva[index] 
        && ((Personagens[index].direcao == 1 && Personagens[index].tAtual > 0.5) 
        || (Personagens[index].direcao == 0 && Personagens[index].tAtual < 0.5))){ 
            trocaDeCurva[index] = false;
            AtualizaCurva(index);
    }
    else if(Personagens[index].direcao == Personagens[index].tAtual){ 
        Personagens[index].Curva = &Curvas[nextStates[index][0]];
        Personagens[index].tAtual = nextStates[index][1];
        Personagens[index].direcao = nextStates[index][1] ^ 1; 
        trocaDeCurva[index] = true;
    }
    if(movimento || index != 0){
        Personagens[index].AtualizaPosicao(tempoDecorrido);
    } 
}
/***********************************************************************************************************/
void updateBB(int index){
    Ponto posicao = Personagens[index].ObtemPosicao();
    Poligono* poly = Personagens[index].poly;
    Poligono tempEnvelope;
    
    for(int i = 0; i < poly->getNVertices(); i++){
        Ponto aux = poly->getVertice(i) * 0.5;
        aux.rotacionaZ(Personagens[index].Rotacao + 90);
        tempEnvelope.insereVertice(aux + posicao);
    }
    tempEnvelope.desenhaPoligono();
    envelopes[index] = tempEnvelope;
}
/***********************************************************************************************************/
void DesenhaPersonagens(float tempoDecorrido){
    for (int i = 0; i < nInstancias; i++){
        AtualizaPersonagem(tempoDecorrido, i);
        Personagens[i].desenha();
        updateBB(i);
    }
}
/***********************************************************************************************************/
void DesenhaCurvas(){
    for (int i = 0; i < nCurvas; i++){   
        if(trocaDeCurva[0] || i != nextStates[0][0]){
            defineCor(LightBlue);
            glLineWidth(1);
            Curvas[i].Traca();
        }else{
            defineCor(Blue);
            glLineWidth(2);
            Curvas[i].Traca();
        }
    }
}
/***********************************************************************************************************/
void TestaColisao(){
    Ponto v1 = operator-(envelopes[0].getVertice(1), envelopes[0].getVertice(0));
    Ponto v2 = operator-(envelopes[0].getVertice(2), envelopes[0].getVertice(1));
    Ponto v3 = operator-(envelopes[0].getVertice(0), envelopes[0].getVertice(2));
    for (int i = 1; i < nInstancias; i++){
        for(int j = 0; j < 3;j++){
            Ponto va = operator-(envelopes[i].getVertice(j), envelopes[0].getVertice(0));
            if ((v1.x * va.y) - (v1.y * va.x) < 0){
                va = operator-(envelopes[i].getVertice(j), envelopes[0].getVertice(1));
                if ((v2.x * va.y) - (v2.y * va.x) < 0){
                    va = operator-(envelopes[i].getVertice(j), envelopes[0].getVertice(2));
                    if ((v3.x * va.y) - (v3.y * va.x) < 0){
                        if(z==1)
                            exit(0);
                        z++;
                    }
                }
            }
            Ponto v4 = operator-(envelopes[i].getVertice(1), envelopes[i].getVertice(0));
            Ponto v5 = operator-(envelopes[i].getVertice(2), envelopes[i].getVertice(1));
            Ponto v6 = operator-(envelopes[i].getVertice(0), envelopes[i].getVertice(2));
            Ponto vb = operator-(envelopes[0].getVertice(j), envelopes[i].getVertice(0));
            if ((v4.x * vb.y) - (v4.y * vb.x) < 0){
                vb = operator-(envelopes[0].getVertice(j), envelopes[i].getVertice(1));
                if ((v5.x * vb.y) - (v5.y * vb.x) < 0){
                    vb = operator-(envelopes[0].getVertice(j), envelopes[i].getVertice(2));
                    if ((v6.x * vb.y) - (v6.y * vb.x) < 0){
                        exit(0);
                    }
                }
            }
        }
    }
}
/***********************************************************************************************************/
void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(1);
    glColor3f(1, 1, 1); 

    DesenhaCurvas();
    DesenhaPersonagens(T2.getDeltaT());
    TestaColisao();

    glutSwapBuffers();
}
/***********************************************************************************************************/
void ContaTempo(double tempo){
    Temporizador T;
    unsigned long cont = 0;
    while (true){
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0){
            break;
        }
    }
}
/***********************************************************************************************************/
void keyboard(unsigned char key, int x, int y){
    switch (key){
    case 27:     
        exit(0); 
        break;
    case 't':
        ContaTempo(3);
        break;
    case ' ':
        desenha = !desenha;
        movimento = !movimento;
        break;
    default:
        break;
    }
}
/***********************************************************************************************************/
void arrow_keys(int a_keys, int x, int y){
    switch (a_keys){
    case GLUT_KEY_LEFT:
        if((Personagens[0].direcao == 1 && Personagens[0].tAtual > 0.5) 
        || (Personagens[0].direcao == 0 && Personagens[0].tAtual < 0.5)){ 
            
            indexPlayer--;
            if (indexPlayer < 0) indexPlayer = sizePlayer-1;
            nextStates[0][0] = curvasPlayer[indexPlayer];
            nextStates[0][1] = tCurvasPlayer[indexPlayer];
        }
        break;
    case GLUT_KEY_RIGHT:
        if((Personagens[0].direcao == 1 && Personagens[0].tAtual > 0.5) 
        || (Personagens[0].direcao == 0 && Personagens[0].tAtual < 0.5)){ 
            
            indexPlayer++;
            indexPlayer %= sizePlayer;
            nextStates[0][0] = curvasPlayer[indexPlayer];
            nextStates[0][1] = tCurvasPlayer[indexPlayer];
        }
        break;
    case GLUT_KEY_UP:
        glutFullScreen();
        break;
    case GLUT_KEY_DOWN: 
        if((Personagens[0].direcao == 1 && Personagens[0].tAtual > 0.5) 
        || (Personagens[0].direcao == 0 && Personagens[0].tAtual < 0.5)){
            trocaDeCurva[0] = !trocaDeCurva[0];
        }
        Personagens[0].direcao ^= 1;
        movimento = true;
        break;
    default:
        break;
    }
}
/***********************************************************************************************************/
int main(int argc, char **argv){
    cout << "\nOpenGL - Animacao com Bezier\n" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(650, 500);
    glutCreateWindow("SpaceMaze 2 - The returnal");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);

    glutMainLoop();
    return 0;
}
/***********************************************************************************************************/