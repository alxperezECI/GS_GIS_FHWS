#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;
	
struct mycomplex{
	float real;
	float imag;
};

float norm(mycomplex& a){
	return sqrt(a.real*a.real+a.imag*a.imag);
}
	
float angle(mycomplex& a){
	return atan2( a.imag, a.real );
}

ostream& operator<<(ostream& stream, mycomplex& a){
	stream << a.real << " + " << a.imag << "j";
	return stream;
}

mycomplex operator+(mycomplex& a, mycomplex& b){
    mycomplex c;
	c.real = a.real +  b.real;
	c.imag = a.imag +  b.imag;
	return c;
}

mycomplex operator-(mycomplex& a, mycomplex& b){
    mycomplex c;
    c.real = a.real - b.real;
    c.imag = a.imag - b.imag;
    return c;
}

bool operator<(mycomplex& a, mycomplex& b){
    //cout << "Usando el comparador" << endl;
    return angle(a) < angle(b);
}

mycomplex comp_prom(vector<mycomplex>& a){
    mycomplex prom;
	prom.real = 0.;
	prom.imag = 0.;

	int m = a.size();
	for(int i=0; i< m; i++)
        prom.real += a.at(i).real;
		
	prom.real /= m;
	prom.imag /= m;
	return prom;
}

void ordernar(vector<mycomplex>& a){
    bool cambios;
    mycomplex temp;
    do{
        cambios = false;
        for(int i = 0; i < a.size()-1; i++){
            if(!(a.at(i) < a.at(i+1))){
                cambios = true;
                temp = a.at(i);
                a.at(i) = a.at(i+1);
                a.at(i+1) = temp;
            }
        }
    }while (cambios);
}



int main(int argc, char** argv){
	
	if(argc != 2){
        cout << "Use this program:" << endl;
        cout << "complex path_to_file" << endl;
        cout << "You should provide the path of the file with data." << endl;
        return 1;
    }

    string path;
    path = argv[1];
    ifstream archivo;
    archivo.open(path);

    cout << "File to be opened: " << path << endl;

    mycomplex uncomp;
    float a,b;
    vector<mycomplex> vcomp;
    vector<float>   vnorms;
    vector<float>   vangles;
 

    if(archivo.is_open()){
        // Reading the file
        while(!archivo.eof()){
            getline(archivo, path);
            a = atof(path.c_str());
            getline(archivo, path);
            b = atof(path.c_str());
            uncomp.real = a;
            uncomp.imag = b;

            // Pushing the complex inside vector
            vcomp.push_back(uncomp);
        }

        // This code to calculates the norms
        int m = vcomp.size();
        for(int i=0; i < m ; i++){
            vnorms.push_back( norm( vcomp.at(i) ) );
            vangles.push_back( angle( vcomp.at(i) ) );
            cout << vnorms.at(i) << ", " << vangles.at(i) << endl;
        }

        //average complex
        uncomp = comp_prom( vcomp );
        cout << "The average is: "<< uncomp.real << " + " << uncomp.imag << "j" << endl;

        // ahora usando el operador sobrecargado
        cout << "The average is: " << uncomp << endl;

        // vector ordering
        ordernar(vcomp);

        vnorms.clear();
        vangles.clear();

        cout << "Norm: \t\tAngle:" << endl;
        for(int i=0; i < m ; i++){
            vnorms.push_back( norm( vcomp.at(i) ) );
            vangles.push_back( angle( vcomp.at(i) ) );
            cout << vnorms.at(i) << ",\t" << vangles.at(i) << endl;
        }


    }else{
        cout << "Plese verify the file." << endl;
    }
	
	return 0;
}
