#include "plano.h"

Plano::Plano() {
}

void Plano::execute(int argc, char *argv[]) {

    const string ACTION_C = "-c";
    const string ACTION_R = "-r";
    const string ACTION_L = "-l";
    const string ACTION_S = "-s";
    const string ACTION_E = "-e";

    const int TAM_BUFFER = 1024;

    string file = argv[1];
    string action = argv[2];

    //Manejo de files de Texto Plano

	if (argc >= 3 && action == ACTION_C) {

		// ./ejecutable [NOMBRE_file] -c
		//Crea el file de texto en la ruta [NOMBRE_file].

		ofstream fs(file.c_str(), ios::trunc);
		fs.close();

	} else if (argc >= 3 && action == ACTION_R) {

		// ./ejecutable [NOMBRE_file] -r "[TEXTO]
		//Registra el [TEXTO] en una línea del file de texto con ruta
		//[NOMBRE_file]. Si no está creado devuelve error.

		string texto;
		texto = argv[3];

		ofstream fs(file.c_str(), ios::out | ios::app);
		fs << texto << endl;
		fs.close();

	} else if (argc >= 3 && action == ACTION_L) {

		// ./ejecutable [NOMBRE_file] -l
		//Muestra por pantalla las líneas de texto contenidas en el file de texto con
		//ruta [NOMBRE_file]. Si no está creado devuelve error.

		char texto[TAM_BUFFER];
		ifstream fs(file.c_str(), ios::in);

		fs.getline(texto, sizeof texto);

		while (!fs.eof()) {
			cout << texto << endl;
			fs.getline(texto, sizeof texto);
		}

		fs.close();

	} else if (argc >= 4 && action == ACTION_S) {

		// ./ejecutable [NOMBRE_file] -s "[TEXTO]"
		//Busca el [TEXTO] en las líneas del file de texto con ruta
		//[NOMBRE_file]. Muestra por pantalla las líneas que contienen el
		//texto. Si no está creado devuelve error.

		string buscar = argv[3];
		char texto[TAM_BUFFER];
		ifstream fs(file.c_str(), ios::in);
		unsigned found;

		fs.getline(texto, sizeof texto);

		while (!fs.eof()) {

			found = string(texto).find(buscar);

			if (found != std::string::npos) {
				cout << texto << endl;
			}

			fs.getline(texto, sizeof texto);
		}

		fs.close();

	} else if (argc >= 3 && action == ACTION_E) {

		// ./ejecutable [NOMBRE_file] -e
		//Elimina el file de texto con ruta [NOMBRE_file].

		if (remove(file.c_str()) != 0) {
			cout << "error al borrar el archivo" << endl;
		}
	}
}

Plano::~Plano() {
}

