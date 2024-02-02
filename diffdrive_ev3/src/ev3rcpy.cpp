#include "diffdrive_ev3/ev3rcpy.hpp"

Ev3::Ev3(const char* IpEv3) {
    char connchar[100];
   	Py_Initialize();
    PyObject *mainModule = PyImport_AddModule("__main__");
	PyRun_SimpleString("import rpyc");
    strcpy(connchar,"conn = rpyc.classic.connect('");
    strcat(connchar,IpEv3);
    strcat(connchar,"')");
	PyRun_SimpleString(connchar);
	PyRun_SimpleString("ev3dev2_motor = conn.modules['ev3dev2.motor']");
    PyRun_SimpleString("ev3dev2_sensor = conn.modules['ev3dev2.sensor']");
	PyRun_SimpleString("motorD = ev3dev2_motor.LargeMotor(ev3dev2_motor.OUTPUT_A)");
	PyRun_SimpleString("motorG = ev3dev2_motor.LargeMotor(ev3dev2_motor.OUTPUT_B)");
    this->motorD = PyObject_GetAttrString(mainModule, "motorD");
    this->motorG = PyObject_GetAttrString(mainModule, "motorG");
    this->posG = PyObject_GetAttrString(motorG,"position");
}
Ev3::~Ev3() {
   Py_Finalize();
}
void Ev3::Grun(int speed) {
    PyObject *sp = PyLong_FromLong(speed);
    PyObject_SetAttrString(this->motorG,"duty_cycle_sp",sp);
    PyObject_CallMethod(this->motorG, "run_direct", "");
}
void Ev3::Drun(int speed) {
    PyObject *sp = PyLong_FromLong(speed);
    PyObject_SetAttrString(this->motorD,"duty_cycle_sp",sp);
    PyObject_CallMethod(this->motorD, "run_direct", "");
}
void Ev3::Dstop() {
    PyObject_CallMethod(this->motorD, "stop", "");
}
void Ev3::Gstop() {
    PyObject_CallMethod(this->motorG, "stop", "");
}
long Ev3::Dpos() {
    this->posD = PyObject_GetAttrString(motorD,"position");
    return PyLong_AsLong(this->posD);
}
long Ev3::Gpos() {
    this->posG = PyObject_GetAttrString(motorG,"position");
    return PyLong_AsLong(this->posG);
}
