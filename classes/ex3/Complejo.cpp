#include "Complejo.h"
#include <iostream>
#include <math.h>

using namespace std;

/* Default Constructor
 */
Complejo::Complejo() {
    _real = 0.;
	_imag = 0.;
}

/* Parameter constructor
 */
Complejo::Complejo(float r, float i){
    _real = r;
	_imag = i;
}

///* Constructor por copia
// */
//Complejo::Complejo(Complejo& a){
//	_real = a._real;
//	_imag = a._imag;
//}

Complejo::~Complejo(){
    cout << "Calling destructor!" << endl;
}

float Complejo::norma() {
    return sqrt( _real * _real + _imag * _imag );
}

float Complejo::angulo() {
    return atan2( _imag, _real );
}

Complejo Complejo::operator +(Complejo& a){
    Complejo c;
    c._real = _real + a._real;
    c._imag = _imag + a._imag;
    //cout << "Using overloading plus operator for complejos" << endl;
    return c;
}

Complejo Complejo::operator -(Complejo& a){
	Complejo c;
	c._real = _real - a._real;
	return c;
}

bool Complejo::operator <(Complejo& a){
	return this->angulo() < a.angulo();
}

bool Complejo::operator ==(Complejo& a){
	return this->_real == a._real && this->_imag && a._imag;
}

