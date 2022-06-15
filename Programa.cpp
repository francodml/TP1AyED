/*
    Programa: TO1V1 K1092 G-Meneguzzi Franco.cpp
    Grupo: Felipe Simón Ruiz Díaz, Franco Demián Meneguzzi, Carlos Peña, Nicolás Samir Mustafa, Milagros Cuevas, Sara Sophia Fernandez Videla, Lucas Gabriel Aponte, José Brandy Zambrano
    Fecha: Julio 2022
    Comentario: Programa que lee un archivo de texto "VentasFerreteria.Txt" y emite listados en formato de tabla a un archivo de texto
    Versión: 0
    Curso: K1092MiTN
*/

#include <iomanip>
#include <fstream>
#include <iostream>
#include <format>
using namespace std;

typedef char str20[21];

struct sFecha {
    char dia[3],
        mes[3],
        anio[5];
};

struct sVenta {
	unsigned short codVen,
           cant;
	str20  descrip; // cadena de caracteres al estilo Leng. C
	float  preUni;
    sFecha fecha;
};

bool LeerVenta(ifstream &Vtas, sVenta & rVen) {
    if (!Vtas)
        return false;

    Vtas >> rVen.codVen;
    if (!rVen.codVen)
        return false;
    Vtas >> rVen.cant;
    Vtas.ignore();
    Vtas.get(rVen.descrip, 21);
    Vtas >> rVen.preUni;
    Vtas >> rVen.fecha.dia;
    Vtas >> rVen.fecha.mes;
    Vtas >> rVen.fecha.anio;

    return true;
}

void ProcesarVentas(ifstream &VentasIFS, sVenta vrVentas[], unsigned short &nTotalVentas )
{
    while(VentasIFS){
        if(!LeerVenta(VentasIFS, vrVentas[nTotalVentas]))
            break;
        nTotalVentas++;
    }
}

//Ordenar ventas por codigo de vendedor
void OrdenarVentas( sVenta vrVentas[], unsigned short nTotalVentas )
{
    sVenta aux;
    for(int i=0; i<nTotalVentas-1; i++)
    {
        for(int j=i+1; j<nTotalVentas; j++)
        {
            if(vrVentas[i].codVen > vrVentas[j].codVen)
            {
                aux = vrVentas[i];
                vrVentas[i] = vrVentas[j];
                vrVentas[j] = aux;
            }
        }
    }
}

void EmitirVenta(sVenta Venta){
    cout << setw(5)  << Venta.cant << " ";
    cout << setw(21) << Venta.descrip << " ";
    cout << setw(7)  << '$' << Venta.preUni  << " ";
    cout << setw(8)  << '$' << Venta.preUni*Venta.cant << " ";
    cout << setw(5)  << Venta.fecha.dia << "/";
    cout << setw(2)  << Venta.fecha.mes << "/";
    cout << setw(4)  << Venta.fecha.anio << endl;
}

void EmitirColumnas(){
    //TODO: Reescribir emisión de columnas de alguna forma que tenga sentido -- setw nos está fallando
}

int main(){
    const short MAX_REGS = 800;
    const short MAX_VEND = 100;
    float TotalGralVentas;
    unsigned short VendedorVtaMayor, nTotalVentas, vendedorActual;
    sVenta vrVentas[MAX_REGS];

    ifstream VentasAF("VentasFerreteria.txt");

    ProcesarVentas(VentasAF, vrVentas, nTotalVentas);

    cout << vrVentas[0].descrip << "sexo";

    cout << "Listado de ventas" << endl;

    OrdenarVentas(vrVentas, nTotalVentas);
       
    for ( int i = 0; i < nTotalVentas; i++)
    {
        if (vrVentas[i].codVen != vendedorActual)
        {
            vendedorActual = vrVentas[i].codVen;
            VendedorVtaMayor = i;
            cout << "Cod. Vendedor: " << vendedorActual << endl;
            EmitirColumnas();
        }
        EmitirVenta(vrVentas[i]);
    }

    //ofstream SalidaAF("SalidaFerreteria.txt");

    //ExtraerVentas(VentasAF, MAX_REGS, MAX_VENS)


    VentasAF.close();
    //SalidaAF.close();

    return 0;
}