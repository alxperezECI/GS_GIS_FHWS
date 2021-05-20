
#ifndef _COMPLEJO_H
#define _COMPLEJO_H

class Complejo {
public: 
    
	Complejo();
	Complejo(float r, float i);
    //Complejo(Complejo& a);
	~Complejo();

	Complejo operator+(Complejo& a);
	Complejo operator-(Complejo& a);
    bool operator<(Complejo& a);
	bool operator==(Complejo& a);

	float getReal(){ return _real;}
	void setReal(float r){ _real = r;}
	float getImag(){ return _imag;}
	void setImag(float i){ _imag = i;}

	float norma();
	float angulo();
    
private: 
    float _real;
    float _imag;
};

#endif //_COMPLEJO_H
