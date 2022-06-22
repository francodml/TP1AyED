//Programa de Ranking

#include <iomanip>
#include <fstream>
#include <iostream>

#define MAX_TEMAS 30

using namespace std;

struct rEncuesta {
    char Sexo;
    ushort Edad;
    ushort Temas[3];
};

struct sRank {
    ushort IdTema;
    ushort Rank;
    ushort MMayor30;
    bool Mujeres;
};

typedef sRank tvrRank[(MAX_TEMAS + 1)];

void InicializarTabla(tvrRank &vrRanking){
    for (int i = 1; i <= MAX_TEMAS; i++)
    {
        sRank r;
        r.IdTema = i;
        r.Rank = 0;
        r.MMayor30 = 0;
        r.Mujeres = false;
        vrRanking[i] = r;
    }
}

int main(){
    //ifstream Encuesta("encuesta.txt");
    tvrRank vrRanking;
    InicializarTabla(vrRanking);
    cout << "ass" << endl;
    cout << vrRanking[30].IdTema << endl;

    return 0;
}
