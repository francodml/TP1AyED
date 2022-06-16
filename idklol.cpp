#include <iostream>
#define MAX_ELEM = 100
typedef tVec short[MAX_ELEM];

using namespace std;

void GenVec(tVec v, short &n) {
    cout << "Ingrese cardinalidad del vector";
    short card = 0;
    do {
        cin >> card;
    }
    while( card <= 1 || card >= MAX_ELEM)
}

short Sum(tVec v, short n) {
    int sum = 0;
    for(int i = 0; i <= n-1; i++) {
        sum += v[i];
    }
}

char Menu() {
    cout << "A: Promedio" << endl;
    cout << "B: Maximo" << endl;
    cout << "C: Minimo" << endl;
    cout << "D: Sumatoria" << endl;
    cout << "E: Salir" << endl;
    cout << endl << "Elija una opcion [ ]";
    
}


int main() {

}