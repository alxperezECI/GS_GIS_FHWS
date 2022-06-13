#include <iostream>
#include "Complejo.h"

using namespace std;

int main()
{
    Complejo a;
    Complejo b = Complejo(5.3, 6.1);
    Complejo c = Complejo(5.3, -6.1);
    c.setReal( 89 );

    if(true){
        Complejo d = b + c;
    }
    
    cout << "Hello World!" << endl;
    cout << "Helo FHWS" << endl;
    cout << b.angulo() << endl;
    cout << c.angulo() << endl;
    cout << b.norma() << endl;
    return 0;
}
