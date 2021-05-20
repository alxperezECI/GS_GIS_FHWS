#include <iostream>
#include <math.h>


using namespace std;

int main(int argc, char** argv){

    float real, imag, mag;
    real = imag = mag = 0.;

    cout << "Hello world!" << endl;
    cout << "System sends me " << argc << " arguments." << endl;
    cout << "There are array of characters!!" << endl;

    for(int i=0; i < argc; i++){
            cout << argv[i] << endl;
    }

    //	From text to number
    cout << "Now they are converted to numbers:" << endl;

    if(argc == 3){
            real = atof(argv[1]);
            imag = atof(argv[2]);
            cout << "The sum of the two arguments is: " << real + imag << endl;
    }

    mag = sqrt( real * real + imag * imag );
    cout << "If it is a complex: " << real << " + " << imag << "j,  its norm is " << mag << endl;

    return 0;
}
