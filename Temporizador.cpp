/***********************************************************************************************************/
#include "Temporizador.h"
/***********************************************************************************************************/
Temporizador::Temporizador(){
#ifdef _WIN32
    start_time = GetTickCount();
#else
    gettimeofday(&start_time, NULL);
#endif

}
/***********************************************************************************************************/
double Temporizador::getDeltaT(){
    double dt;
#ifdef _WIN32
    DWORD end_time;
    end_time = GetTickCount();
    dt = (float) (end_time - start_time) / 1000.0;
#else
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    dt = (float)(end_time.tv_sec  - start_time.tv_sec) + 1.0e-6*(end_time.tv_usec - start_time.tv_usec);
#endif
    start_time = end_time;
    return dt;
}
/***********************************************************************************************************/