#ifndef _EV3RCPY_HPP_
#define _EV3RCPY_HPP_
#include <Python.h>

// Entrer l'IP de l'EV3 ici
#define IPEV3 "169.254.145.58"

class Ev3 {
    PyObject *motorD;
    PyObject* motorG;
    PyObject *posD;
    PyObject *posG;
    public:
        Ev3(const char* IpEv3);
        ~Ev3();
        void Drun(int speed);
        void Grun(int speed);
        void Dstop();
        void Gstop();
        long Dpos();
        long Gpos();
};
#endif