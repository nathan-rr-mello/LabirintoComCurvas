#ifndef ControlaTempo_hpp
#define ControlaTempo_hpp

#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

using namespace std;

class Temporizador{
#ifdef WIN32
    DWORD start_time;
#else
    struct timeval start_time;
#endif
public:
    Temporizador();
    double getDeltaT(); 
};
#endif 