/*
    Programa: TO1V1 K1092 G-Meneguzzi Franco.cpp

    Grupo N°3: Felipe Simón Ruiz Díaz
    Franco Demián Meneguzzi
    Carlos Peña, Nicolás Samir Mustafa
    Milagros Cuevas
    Sara Sophia Fernandez Videla
    Lucas Gabriel Aponte
    José Brandy Zambrano

    Fecha: Julio 2022
    Comentario: Programa que lee un archivo de texto "VentasFerreteria.Txt" y emite listados en formato de tabla a un archivo de texto
    Versión: 0
    Curso: K1092MiTN
*/

#include <iomanip>
#include <fstream>
#include <iostream>

using namespace std;

typedef char str20[21];

typedef unsigned short ushort;

const short MAX_REGS = 800;
const short MAX_VEND = 100;

struct sFecha {
    char dia[3],
        mes[3],
        anio[5];
};

struct rVenta {
	unsigned short codVen,
           cant;
	str20  descrip; // cadena de caracteres al estilo Leng. C
	float  preUni;
    sFecha fecha;
};

struct rTotales {
    short id;
    int itemsTotalVendido;
    int importeTotalVendido;
};

string Replicate(char c, ushort n) {
    string str;

    for (short i = 0; i < n; i++)
        str += c;
    return str;
}

bool LeerVenta(ifstream &Vtas, rVenta & rVen) {
    if (!Vtas)
        return false;

    Vtas >> rVen.codVen;
    if (!rVen.codVen)
        return false;
    try {
        Vtas >> rVen.cant;
        Vtas.ignore();
        Vtas.get(rVen.descrip, 21);
        Vtas >> rVen.preUni;
        Vtas >> rVen.fecha.dia;
        Vtas >> rVen.fecha.mes;
        Vtas >> rVen.fecha.anio;
    } catch (...) {
        return false;
        cout << "Error en lectura de archivo" << endl;
    }

    return true;
}

void ProcesarVentas(ifstream &VentasIFS, rVenta vrVentas[], unsigned short &cVtas )
{
    while(VentasIFS){
        if(!LeerVenta(VentasIFS, vrVentas[cVtas]))
            break;
        cVtas++;
    }
}

void Intercambiar(rVenta &v1, rVenta &v2, bool &sorted)
{
    rVenta aux;
    aux = v1;
    v1 = v2;
    v2 = aux;
    sorted = false;
}

template <class T>

//Ordenado por Burbuja genérico. asc indica si el ordenamiento es ascendente o descendente. *cmp indica la función de comparación.

void OrdxBurGenerico(T array[], ushort n, bool (*cmp)(T, T))
{
    for (ushort i = 0; i < n - 1; i++)
    {
        bool sorted = true;
        for (ushort j = 0; j < n - i - 1; j++)
            if (cmp(array[j], array[j + 1]))
                Intercambiar(array[j], array[j + 1], sorted);

        if (sorted)
            break;
    }
}

void OrdxBur(rVenta vrVentas[], ushort cVtas) {
    OrdxBurGenerico<rVenta>(vrVentas, cVtas, [](rVenta v1, rVenta v2) {
        return v1.codVen > v2.codVen;
    });
}

void EmitirVenta(rVenta Venta, ushort i){
    cout << setw(5) << i << " ";
    cout << setw(3) << Venta.fecha.dia << "/" << setw(2) << Venta.fecha.mes << "/" << setw(4) << Venta.fecha.anio << " ";
    cout << setw(6)  << Venta.cant << " ";
    cout << setw(20) << Venta.descrip << " ";
    cout << setw(3) << "$" << setw(8) << Venta.preUni  << " ";
    cout << setw(3) << "$" << setw(9) << Venta.preUni*Venta.cant  << " ";
    cout << endl;
}

void EmitirColumnas(){
    cout << setw(5) << "#Item" << " "
        << right << setw(4) << "Dia" << "/" << setw(2) << "Mes" << "/" << setw(4) << "Año" << " "
        << setw(5) << "Cant" << " "
        << left << setw(21) << "Descripción" << " "
        << right << setw(11) << "Precio" << " "
        << right << setw(12) << "Total" << " "
        << right << setw(12) << "Total Ven." << " "
        << endl;
}

void ListadoVentasAgrupVen(rVenta vrVentas[], ushort cVtas, rTotales vrTotales[])
{
    ushort vendedorActual, nVentasVendedor;
    float totalVendedor;
    cout << "Listado de ventas" << endl;

    OrdxBur(vrVentas, cVtas);

    for ( int i = 0; i < cVtas; i++)
    {
        if (!vrVentas[i].codVen)
            continue;
        if (vrVentas[i].codVen != vendedorActual)
        {
            nVentasVendedor = 0;
            totalVendedor = 0;
            vendedorActual = vrVentas[i].codVen;
            cout << Replicate('-',80) << endl;
            cout << "Cod. Vendedor: " << vendedorActual << endl;
            EmitirColumnas();
        }
        nVentasVendedor++;
        totalVendedor += vrVentas[i].preUni * vrVentas[i].cant;
        EmitirVenta(vrVentas[i], nVentasVendedor);
        if (vrVentas[i+1].codVen != vendedorActual)
        {
            cout << setw(74) << "$" << setw(9) << totalVendedor << endl;
            rTotales rTotalesAux;
            rTotalesAux.id = vendedorActual;
            rTotalesAux.itemsTotalVendido = nVentasVendedor;
            rTotalesAux.importeTotalVendido = totalVendedor;
            vrTotales[vendedorActual] = rTotalesAux;
        }
    }
}

int main(){
    float TotalGralVentas;
    ushort VendedorVtaMayor, nTotalVentas;
    rVenta vrVentas[MAX_REGS];
    rTotales vrTotales[MAX_VEND];

    ifstream VentasAF("VentasFerreteria.txt");

    ProcesarVentas(VentasAF, vrVentas, nTotalVentas);

    ListadoVentasAgrupVen(vrVentas, nTotalVentas, vrTotales);

    //ofstream SalidaAF("SalidaFerreteria.txt");

    //ExtraerVentas(VentasAF, MAX_REGS, MAX_VENS)


    VentasAF.close();
    //SalidaAF.close();

    return 0;
}
