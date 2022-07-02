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
#include <locale.h>

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
	float  preUni;
    float  preTotal;
	unsigned short codVen,
           cant;
    sFecha fecha;
	str20  descrip; // cadena de caracteres al estilo Leng. C
};

struct rTotales {
    short id;
    int itemsTotalVendido;
    int importeTotalVendido;
    int importeVentaMayor;
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
        rVen.preTotal = rVen.preUni * rVen.cant;
    } catch (...) {
        cout << "Error en lectura de archivo" << endl;
        return false;
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

//Intercambio genérico - No utiliza tipos definidos.

template <class T>

void Intercambiar( T &v1, T &v2, bool &sorted)
{
    T aux;
    aux = v1;
    v1 = v2;
    v2 = aux;
    sorted = false;
}

template <class T>

//Ordenado por Burbuja genérico. *Comparar indica la función de comparación.

void OrdxBurGenerico(T array[], ushort n, bool (*Comparar)(T, T))
{
    for (ushort i = 0; i < n - 1; i++)
    {
        bool sorted = true;
        for (ushort j = 0; j < n - i - 1; j++)
            if (Comparar(array[j], array[j + 1]))
                Intercambiar(array[j], array[j + 1], sorted);

        if (sorted)
            break;
    }
}

#pragma region Comparadores
    bool ComparVentas(rVenta v1, rVenta v2) {
        return v1.codVen > v2.codVen;
    }

    bool ComparTotalesItems(rTotales v1, rTotales v2) {
        return v1.itemsTotalVendido < v2.itemsTotalVendido;
    }

    bool ComparTotalesImportes(rTotales v1, rTotales v2) {
        return v1.importeTotalVendido < v2.importeTotalVendido;
    }
#pragma endregion


void OrdxBur(rVenta vrVentas[], ushort cVtas) {
    OrdxBurGenerico<rVenta>(vrVentas, cVtas, *ComparVentas);
}

void OrdxBur(rTotales vrTotales[], ushort cTotales) {
    OrdxBurGenerico<rTotales>(vrTotales, cTotales, *ComparTotalesItems);
}

void OrdxBur(ushort cTotales, rTotales vrTotales[]) {
    OrdxBurGenerico<rTotales>(vrTotales, cTotales, *ComparTotalesImportes);
}

void EmitirVenta(ostream &sld, rVenta Venta, ushort i){
    sld << setw(5) << i << " "
         << setw(4) << Venta.fecha.dia << "/" << setw(2) << Venta.fecha.mes << "/" << setw(4) << Venta.fecha.anio << " "
         << setw(6)  << Venta.cant << " "
         << setw(20) << Venta.descrip << " "
         << setw(3) << "$" << setw(8) << Venta.preUni  << " "
         << setw(3) << "$" << setw(9) << Venta.preUni*Venta.cant  << " "
         << endl;
}

void EmitirColumnas(ostream &sld){
    sld << setw(5) << "#Item" << " "
        << right << setw(13) << "Dia/Mes/Anio" << " "
        << setw(5) << "Cant" << " "
        << left << setw(21) << "Descripcion" << " "
        << right << setw(10) << "Precio" << " "
        << right << setw(12) << "Total" << " "
        << right << setw(12) << "Total Ven." << " "
        << endl;
}

void AcumularTotales(rTotales vrTotales[], ushort codVen, int totalItems, int totalImporte, ushort &cTotales) {
    rTotales rTotalesAux;
    rTotalesAux.id = codVen;
    rTotalesAux.itemsTotalVendido = totalItems;
    rTotalesAux.importeTotalVendido = totalImporte;
    vrTotales[cTotales] = rTotalesAux;
    cTotales++;
}

void ListadoVentasAgrupVen(ostream &sld, rVenta vrVentas[], ushort cVtas, rTotales vrTotales[], ushort &cTotales)
{
    ushort vendedorActual, nVentasVendedor, codVenVentMayor;
    float totalGeneral, vtaMayor = 0;
    double totalImporteCurVen;
    int totalItemsCurVen;
    sld << "Listado (1) ordenado por Codigo de Vendedor con repeticion" << endl;

    OrdxBur(vrVentas, cVtas);
    for ( int i = 0; i < cVtas; i++)
    {
        if (!vrVentas[i].codVen)
            continue;
        if (vrVentas[i].codVen != vendedorActual)
        {
            nVentasVendedor = 0;
            totalImporteCurVen = 0;
            totalItemsCurVen = 0;
            vendedorActual = vrVentas[i].codVen;
            sld << Replicate('-',83) << endl;
            sld << "Cod. Vendedor: " << vendedorActual << endl;
            EmitirColumnas(sld);
        }

        nVentasVendedor++;
        totalImporteCurVen += vrVentas[i].preTotal;
        totalItemsCurVen += vrVentas[i].cant;

        EmitirVenta(sld, vrVentas[i], nVentasVendedor);

        if ((vrVentas[i].preTotal) > vtaMayor)
        {
            vtaMayor = vrVentas[i].preTotal;
            codVenVentMayor = vendedorActual;
        }

        if (vrVentas[i+1].codVen != vendedorActual)
        {
            sld << setw(74) << "$" << setw(9) << totalImporteCurVen << endl;
            totalGeneral += totalImporteCurVen;
            AcumularTotales(vrTotales, vendedorActual, totalItemsCurVen, totalImporteCurVen, cTotales);
        }
    }
    sld << "Total General: $" << totalGeneral << endl;
    sld << "Codigo de vendedor con mayor importe: " << codVenVentMayor << endl; //Falla cuando quiere? Por alguan razón y no sé por qué. El debugger se rompe a veces
}

void ListadoCantTotalxVend(ostream &sld ,rTotales vrTot[], ushort cTotales) {
    sld << "Listado (2) ordenado decreciente por Cantidad Total de cada Codigo de Vendedor" << endl;
    OrdxBur(vrTot, cTotales);
    sld << "Cod. Ven.  Cant. Total" << endl;
    for (int i = 0; i < cTotales; i++)
    {
        sld << right << setw(9) << vrTot[i].id << "  " << vrTot[i].itemsTotalVendido << endl;
    }
}

void ListadoImporteTotalxVend(ostream &sld, rTotales vrTot[], ushort cTotales) {
    sld << "Listado (3) ordenado decreciente por Importe Total de cada Codigo de Vendedor" << endl;
    OrdxBur(cTotales, vrTot);
    sld << "Cod. Ven.  Importe Total" << endl;
    for (int i = 0; i < cTotales; i++)
    {
        sld << right << setw(9) << vrTot[i].id << "  " << vrTot[i].importeTotalVendido << endl;
    }
}

int main(){
    float TotalGralVentas;
    ushort VendedorVtaMayor, cVtas = 0, cTotales = 0;
    rVenta vrVentas[MAX_REGS];
    rTotales vrTotales[MAX_VEND];

    ifstream VentasAF("VentasFerreteria.txt");
    ofstream SalidaAF("Sld.txt");

    if (!VentasAF)
    {
        cout << "No se pudo abrir el archivo de ventas" << endl;
        return 1;
    }
    if (!SalidaAF)
    {
        cout << "No se pudo abrir el archivo de salida" << endl;
        return 1;
    }

    ProcesarVentas(VentasAF, vrVentas, cVtas);

    ListadoVentasAgrupVen(SalidaAF, vrVentas, cVtas, vrTotales, cTotales);
    ListadoCantTotalxVend(SalidaAF, vrTotales, cTotales);
    ListadoImporteTotalxVend(SalidaAF, vrTotales, cTotales);


    VentasAF.close();
    SalidaAF.close();

    return 0;
}
