// g++ GrafoDirigido.cpp -std=c++11 && ./a.out 

#include <iostream>
#include <fstream>
#include <string>

#include <queue>

using namespace std;


#include "OrdenacionTopologica.h"

GrafoDirigido::Arco::Arco(int elOrigen, int elDestino, float elPeso)
	: origen{ elOrigen }, destino{ elDestino }, peso{ elPeso } {

}

void GrafoDirigido::mostrar() const {

	cout << "Listas de adyacencia: " << endl;
	for (int i = 1; i < vertices.size(); i++) {
		cout << "  vertice " << i << ":" << endl;
		for (const auto & arco : vertices[i].adyacentes)
			cout << "    " << arco.origen << "-->" << arco.destino << " (peso = " << arco.peso << ")" << endl;
		cout << endl;
	}

	cout << "Listas de incidencia: " << endl;
	for (int i = 1; i < vertices.size(); i++) {
		cout << "  vertice " << i << ":" << endl;
		for (const auto & arco : vertices[i].incidentes)
			cout << "    " << arco.origen << "-->" << arco.destino << " (peso = " << arco.peso << ")" << endl;
		cout << endl;
	}

}

GrafoDirigido::GrafoDirigido(const char * nombreFichero) throw(string) {

	ifstream flujoEntrada(nombreFichero);

	if (!flujoEntrada.is_open())
		throw string("ERROR: Imposible abrir el fichero ") + nombreFichero;

	char tipoLinea;
	int cantidadVertices = -1, origen, destino;
	float peso;

	while (!flujoEntrada.eof()) {
		flujoEntrada >> tipoLinea;
		if (!flujoEntrada.eof()) {
			if (tipoLinea == 'n') {
				flujoEntrada >> cantidadVertices;
				vertices.resize(cantidadVertices + 1); // Numeramos desde 1 dejando la posicion 0 sin usar
			}
			else if (tipoLinea == 'a') {
				if (cantidadVertices == -1) {
					flujoEntrada.close();
					throw string("ERROR: falta linea n antes de linea a en ") + nombreFichero;
				}
				flujoEntrada >> origen >> destino >> peso;
				vertices[origen].adyacentes.push_back(Arco(origen, destino, peso));
				vertices[destino].incidentes.push_back(Arco(origen, destino, peso));
			}
		}
	}

	flujoEntrada.close();

}

void GrafoDirigido::mostrarOrdenTopologico() const throw(string) {

	//declaramos un vector paralelo para tener el contador de los incidentes
	vector<int> numeroIncidentes(vertices.size());
	queue<int> colaVertices;

	for (int i = 1; i < numeroIncidentes.size(); i++) 
	{ 
		numeroIncidentes[i] = vertices[i].incidentes.size();
		
		if (numeroIncidentes[i] == 0)
			colaVertices.push(i);
	}

	int contador = 1;

	while (!(colaVertices.empty())) 
	{
		int front = colaVertices.front();
		cout << front << "- ";
		colaVertices.pop();
		contador++;

		//el for recorre todos los arcos que salen del vertice que esta en la salida de la cola.
		for (auto elem : vertices[front].adyacentes)
		{
			//restamos 1 a los incidentes de cada nodo destino de front
			numeroIncidentes[elem.destino]--;
			
			//si alguno de esos nodos destino no tiene incidentes lo encolamos
			if (numeroIncidentes[elem.destino] == 0)
				colaVertices.push(elem.destino);
		}
	}


	if (contador != vertices.size())
		throw ("El grafo es c�clico. ");

}


int main() {

	try {
		GrafoDirigido miGrafo("grafoEjemplo2.gr");
		miGrafo.mostrar();
		miGrafo.mostrarOrdenTopologico();
	}
	catch (string error) {
		cerr << error << endl;
	}

}