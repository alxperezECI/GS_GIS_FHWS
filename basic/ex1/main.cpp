#include <iostream>
#include <math.h>
using namespace std;

/*  Here is defined an structure to handle complex numbers
 */
struct complejo{
    float real;
    float imag;
};

float norm(complejo& a){
    return sqrt( a.real * a.real + a.imag * a.imag );
}

float angle(complejo& a){
    return atan2( a.imag, a.real );
}

int main(int argc, char** argv){

    float mag, ang;

    cout << "Hello world!" << endl;
    cout << "System sends me " << argc << " arguments." << endl;
    cout << "There are array of characters!!" << endl;

    for(int i=0; i < argc; i++){
            cout << argv[i] << endl;
    }

    //	From text to number
    cout << "Now they are converted to numbers:" << endl;

    complejo a;
    if(argc == 3){
            a.real = atof(argv[1]);
            a.imag = atof(argv[2]);
            cout << "The sum of the two arguments is: " << a.real + a.imag << endl;
    }

    mag = norm( a );
    ang = angle ( a );
    cout << "Complex number: " << a.real << " + " << a. imag << "j, the magnitude is: " ;
    cout << mag << " and angle: " << ang << " rad." << endl;


    return 0;
}
