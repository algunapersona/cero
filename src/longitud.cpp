#include "longitud.h"
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <string>

Longitud::Longitud() {
}

Longitud::~Longitud() {
}

void Longitud::execute(int argc, char *argv[]) {

    const string ACTION_C = "-c";
    const string ACTION_RT = "-rt";
    const string ACTION_RB = "-rb";
    const string ACTION_L = "-l";
    const string ACTION_LP = "-lp";
    const string ACTION_LN = "-ln";
    const string ACTION_S = "-s";
    const string ACTION_E = "-e";

    string file = argv[1];
    string action = argv[2];

    //Manejo de Archivos de Registros Variables (estructura: [longitud, dato_valor])

	if (argc >= 3 && action == ACTION_C) {

		// ./ejecutable [NOMBRE_ARCHIVO] -c
		//Crea el archivo de registros variables en la ruta [NOMBRE_ARCHIVO].

		ofstream fs(file.c_str(), ios::trunc);
		fs.close();

	} else if (argc >= 4 && action == ACTION_RT) {

		// ./ejecutable [NOMBRE_ARCHIVO] -rt "[TEXTO]
		//Registra el [TEXTO] en un registro variable del archivo con ruta
		//[NOMBRE_ARCHIVO]. Si no está creado devuelve error.

		string texto;
		texto = argv[3];

		ofstream ofs;

		ofs.open(file.c_str(), ios::out | ios::app | ios::binary);

		if (ofs.is_open()) {

			long longitud;

			longitud = texto.length();

			//copia la longitud
			ofs.write(reinterpret_cast<char *>(&longitud), sizeof(longitud));

			if (ofs.good()) {

				//copia el texto
				ofs << texto;
			}

			ofs.close();

		} else {
			//el archivo no fue creado
		}

	} else if (argc >= 4 && action == ACTION_RB) {

		// ./ejecutable [NOMBRE_ARCHIVO] -rb [NOMBRE_ARCHIVO_R]
		//Registra el archivo ubicado en la ruta [NOMBRE_ARCHIVO_R] en un
		//registro variable del archivo con ruta [NOMBRE_ARCHIVO]. Si no está
		//creado devuelve error.

		string source;
		source = argv[3];
		ifstream ifs;
		ofstream ofs;

		ofs.open(file.c_str(), ios::out | ios::app | ios::binary);

		if (ofs.is_open()) {

			ifs.open(source.c_str(), ios::in);

			if (ifs.is_open()) {

				long longitud;

				//tamano del archivo de entrada
				longitud = tamanoArchivo(source);

				//copia la longitud
				ofs.write(reinterpret_cast<char *>(&longitud), sizeof(long));

				if (ofs.good()) {

					//copia el archivo
					const int BUFSIZE = 2;
					char buffer[BUFSIZE];
					long size;

					size = ifs.readsome(buffer, BUFSIZE);

					while (size) {
						ofs.write(buffer, size);
						size = ifs.readsome(buffer, BUFSIZE);
					}
				}

				ifs.close();
			}

			ofs.close();

		} else {
			//el archivo no fue creado
		}

	} else if (argc >= 3 && action == ACTION_L) {

		// ./ejecutable [NOMBRE_ARCHIVO] -l
		//Muestra por pantalla la lista de registros variables incluidos en el archivo
		//ubicado en la ruta y las posiciones relativas de cada registro. Estructura del
		//listado: Nro. Registro - Posición en Bytes del Registro.

		ifstream ifs;

		ifs.open(file.c_str(), ios::in | ios::binary);

		if (ifs.is_open()) {

			long size, tamano, numreg = 0, suma = 0;

			size = ifs.readsome(reinterpret_cast<char *>(&tamano), sizeof(long));

			while (size == sizeof(long)) {

				++numreg;
				long inicio = suma; //+ sizeof(long);

				/*
				long final = suma + sizeof(long) + tamano - 1;

				cout << "tamano: " << tamano << " (" << hex << tamano << dec
						<< ") inicio: " << inicio << " (" << hex << inicio << dec
						<< ") final: " << final << " (" << hex << final << ")" << endl;
				*/

				cout << setw(10) << numreg << " - " << setw(10) << inicio << endl;

				suma += sizeof(long) + tamano;

				ifs.seekg(suma, ios::beg);

				size = ifs.readsome(reinterpret_cast<char *>(&tamano), sizeof(long));
			}

			ifs.close();
		}

	} else if (argc >= 4 && action == ACTION_LP) {

		// ./ejecutable [NOMBRE_ARCHIVO] -lp [POSICION_REGISTRO]
		//Muestra por pantalla los datos del registro variable ubicado en la posición
		//absoluta [POSICION_REGISTRO], en Bytes, contenido en el archivo de ruta
		//[NOMBRE_ARCHIVO]. Si no está creado devuelve error. Ejemplo para
		//probar un archivo binario de imagen: ./ejecutable [NOMBRE_ARCHIVO]
		//-lp [POSICION_REGISTRO] >[archivo_de_imagen_salida];
		//[programa_visualizador_de_imagen] [archivo_de_imagen_salida].

		long posicion = atol(argv[3]);
		ifstream ifs;

		ifs.open(file.c_str(), ios::in | ios::binary);

		if (ifs.is_open()) {

			long size, tamano, suma = 0;

			ifs.seekg(posicion, ios::beg);

			size = ifs.readsome(reinterpret_cast<char *>(&tamano), sizeof(long));

			if (size == sizeof(long)) {

				long inicio = suma + sizeof(long);
				long final = suma + sizeof(long) + tamano - 1;

				//copia el archivo
				const int BUFSIZE = 2;
				char buffer[BUFSIZE];
				long size, listo, falta;

				falta = final - inicio;
				listo = BUFSIZE < falta ? BUFSIZE : falta;

				size = ifs.readsome(buffer, BUFSIZE);

				while (size && falta > 0 && cout.good()) {
					cout.write(buffer, size);
					falta -= listo;
					listo = BUFSIZE < falta ? BUFSIZE : falta;
					size = ifs.readsome(buffer, BUFSIZE);
				}
			}

			ifs.close();
		}

	} else if (argc >= 4 && action == ACTION_LN) {

		// ./ejecutable [NOMBRE_ARCHIVO] -ln [NUMERO_REGISTRO]
		//Muestra por pantalla los datos del registro variable ubicado en la posición
		//relativa [NUMERO_REGISTRO], en número de registros, contenido en el
		//archivo de ruta [NOMBRE_ARCHIVO]. Si no está creado devuelve error.

		long registro = atol(argv[3]);
		ifstream ifs;

		ifs.open(file.c_str(), ios::in | ios::binary);

		if (ifs.is_open()) {

			long size, tamano, numreg = 0, suma = 0;

			size = ifs.readsome(reinterpret_cast<char *>(&tamano), sizeof(long));

			while (size == sizeof(long)) {

				++numreg;

				long inicio = suma + sizeof(long);
				long final = suma + sizeof(long) + tamano - 1;

				/*

				long final = suma + sizeof(long) + tamano - 1;

				cout << "tamano: " << tamano << " (" << hex << tamano << dec
						<< ") inicio: " << inicio << " (" << hex << inicio << dec
						<< ") final: " << final << " (" << hex << final << ")" << endl;
				*/

				if (numreg == registro) {

					ifs.seekg(inicio, ios::beg);

					//copia el archivo
					const int BUFSIZE = 2;
					char buffer[BUFSIZE];
					long size, listo, falta;

					falta = final - inicio;
					listo = BUFSIZE < falta ? BUFSIZE : falta;

					size = ifs.readsome(buffer, BUFSIZE);

					while (size && falta > 0 && cout.good()) {
						cout.write(buffer, size);
						falta -= listo;
						listo = BUFSIZE < falta ? BUFSIZE : falta;
						size = ifs.readsome(buffer, BUFSIZE);
					}
				}

				//cout << setw(10) << numreg << " - " << setw(10) << inicio << endl;

				suma += sizeof(long) + tamano;

				ifs.seekg(suma, ios::beg);

				size = ifs.readsome(reinterpret_cast<char *>(&tamano), sizeof(long));
			}

			ifs.close();
		}

	} else if (argc >= 4 && action == ACTION_S) {

		// ./ejecutable [NOMBRE_ARCHIVO] -s "[TEXTO]
		//Busca el [TEXTO] en los registros del archivo con ruta
		//[NOMBRE_ARCHIVO]. Muestra por pantalla los números de registro, y las
		//posiciones relativas de los mismos, que contengan el texto. Si no está creado
		//devuelve error.

		string texto = argv[3];
		ifstream ifs;

		ifs.open(file.c_str(), ios::in | ios::binary);

		if (ifs.is_open()) {

			long size, tamano, numreg = 0, suma = 0;

			size = ifs.readsome(reinterpret_cast<char *>(&tamano), sizeof(long));

			while (size > 0) {

				++numreg;

				long inicio = suma + sizeof(long);
				long final = suma + sizeof(long) + tamano - 1;

				ifs.seekg(inicio, ios::beg);

				//copia el archivo
				const int BUFSIZE = 2;
				char buffer[BUFSIZE];
				long listo, falta;

				falta = final - inicio;
				listo = BUFSIZE < falta ? BUFSIZE : falta;
				ostringstream tmp;

				size = ifs.readsome(buffer, BUFSIZE);

				while (size && falta > 0 && cout.good()) {
					tmp.write(buffer, size);
					falta -= listo;
					listo = BUFSIZE < falta ? BUFSIZE : falta;
					size = ifs.readsome(buffer, BUFSIZE);
				}

				string strTmp = tmp.str();

				if (strTmp.find(texto) != string::npos) {
					cout << "Cadena encontrada en el registro: " << setw(9) << numreg << endl;
				}

				suma += sizeof(long) + tamano;

				ifs.seekg(suma, ios::beg);

				size = ifs.readsome(reinterpret_cast<char *>(&tamano), sizeof(long));
			}

			ifs.close();
		}

	} else if (argc >= 3 && action == ACTION_E) {

		// ./ejecutable [NOMBRE_ARCHIVO] -e
		//Elimina el archivo con ruta [NOMBRE_ARCHIVO].

		if (remove(file.c_str()) != 0) {
			cout << "error al borrar el archivo" << endl;
		}
	}
}
