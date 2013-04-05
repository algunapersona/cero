#include "espacio.h"
#include <cstring>

#define TAM_DATOS 16

typedef struct bloque_t {
	char datos[TAM_DATOS];
} __attribute__((packed)) bloque_t;

char *lugarDisponible(const bloque_t &bloque) {

	int tam;
	int offset = 0;
	char *resultado = NULL;
	int tmp;

	memcpy(&tam, &bloque, sizeof(int));

	while (offset < TAM_DATOS && tam < TAM_DATOS && tam != 0) {

		offset += sizeof(int) + tam;

		tmp = reinterpret_cast<int>(&bloque);
		tmp += offset;
		memcpy(&tam, reinterpret_cast<char *>(tmp), sizeof(int));
	}

	if (offset == 0) {
		resultado = const_cast<char *>(bloque.datos);
	} else if (offset < TAM_DATOS) {
		resultado = const_cast<char *>(bloque.datos) + offset;
	}

	return resultado;
}

bool bloqueSirve(const int len, const bloque_t &bloque) {

	char *offset = NULL;
	bool resultado = false;

	offset = lugarDisponible(bloque);

	if (offset == NULL) {
	} else if ((offset - bloque.datos + sizeof(int) + len) <= (sizeof(bloque_t) * .98)) {
		resultado = true;
	}

	return resultado;
}

void obtenerBloque(const string &file, const int len, int &numBloque, bloque_t &bloque) {

	ifstream ifs;
	int tamLeido;
	int numBloqueAnalizado = 0;

	numBloque = -1;
	memset(&bloque, 0, sizeof(bloque_t));

	ifs.open(file.c_str(), ios::in | ios::binary);

	if (ifs.is_open()) {

		tamLeido = ifs.readsome(reinterpret_cast<char *>(&bloque), sizeof(bloque_t));

		while (tamLeido == sizeof(bloque_t) && numBloque == -1) {

			++numBloqueAnalizado;

			if (bloqueSirve(len, bloque)) {
				numBloque =	numBloqueAnalizado;
			} else {
				tamLeido = ifs.readsome(reinterpret_cast<char *>(&bloque), sizeof(bloque_t));
			}
		}

		ifs.close();
	}
}

string cargarArchivo(string file) {

	ifstream ifs;
	string datos;

	ifs.open(file.c_str(), ios::in | ios::binary);

	if (ifs.is_open()) {

		//copia el archivo
		const int BUFSIZE = 2;
		char buffer[BUFSIZE];
		long size;

		size = ifs.readsome(buffer, BUFSIZE);

		while (size) {
			datos.append(buffer, size);
			size = ifs.readsome(buffer, BUFSIZE);
		}

	}

	return datos;
}

void agregarRegistro(const string datos, const int numBloque, bloque_t &bloque) {

	char *offset = NULL;


	if (numBloque == -1) {
		memset(&bloque, 0, sizeof(bloque_t));
	}

	offset = lugarDisponible(bloque);

	if (offset != NULL) {
		int tam = datos.length();
		memcpy(offset, &tam, sizeof(int));
		memcpy(offset + sizeof(int), datos.data(), datos.length());
	}
}

void escribirBloque(const string &file, const int numBloque, const bloque_t &bloque) {

	ofstream ofs;

	if (numBloque == -1) {
		ofs.open(file.c_str(), ios::app | ios::in | ios::binary);
	} else {
		ofs.open(file.c_str(), ios::out | ios::in | ios::binary);
	}

	if (ofs.is_open()) {

		if (numBloque == -1) {
		} else {
			ofs.seekp((numBloque - 1) * sizeof(bloque_t), ios::beg);
		}

		ofs.write(reinterpret_cast<const char *>(&bloque), sizeof(bloque_t));

		ofs.close();

	} else {
		//archivo no creado
	}
}

Espacio::Espacio() {
}

void Espacio::execute(int argc, char *argv[]) {

    const string ACTION_C = "-c";
    const string ACTION_RT = "-rt";
    const string ACTION_RB = "-rb";
    const string ACTION_L = "-l";
    const string ACTION_LN = "-ln";
    const string ACTION_LP = "-lp";
    const string ACTION_S = "-s";
    const string ACTION_E = "-e";

    string file = argv[1];
    string action = argv[2];

    //Manejo de files de Texto Espacio

	if (argc >= 3 && action == ACTION_C) {

		// ./ejecutable [NOMBRE_file] -c
		//Crea el file de texto en la ruta [NOMBRE_file].

		ofstream fs(file.c_str(), ios::trunc);
		fs.close();

	} else if (argc >= 3 && action == ACTION_RT) {

		//./ejecutable [NOMBRE_ARCHIVO] -rt "[TEXTO]
		//Registra el [TEXTO] en un registro variable de un bloque del archivo con
		//ruta [NOMBRE_ARCHIVO]. Si no está creado devuelve error.

		string datos;
		bloque_t bloque;
		int numBloque;

		datos = argv[3];

		obtenerBloque(file, datos.length(), numBloque, bloque);
		agregarRegistro(datos, numBloque, bloque);
		escribirBloque(file, numBloque, bloque);

	} else if (argc >= 3 && action == ACTION_RB) {

		//./ejecutable [NOMBRE_ARCHIVO] -rb [NOMBRE_ARCHIVO_R]
		//Registra el archivo ubicado en la ruta [NOMBRE_ARCHIVO_R] en un
		//registro variable de un bloque del archivo con ruta [NOMBRE_ARCHIVO].
		//Si no está creado devuelve error.

		string org;
		string datos;
		bloque_t bloque;
		int numBloque;
		int len;

		org = argv[3];
		len = tamanoArchivo(org);

		obtenerBloque(file, len, numBloque, bloque);
		datos = cargarArchivo(org);
		agregarRegistro(datos, numBloque, bloque);
		escribirBloque(file, numBloque, bloque);

	} else if (argc >= 3 && action == ACTION_L) {

		//./ejecutable [NOMBRE_ARCHIVO] -l
		//Muestra por pantalla la lista de bloques y registros variables incluidos en el
		//archivo ubicado en la ruta y las posiciones relativas de cada registro.
		//Estructura del listado: Nro Bloque - Nro. Registro en el Bloque - Posición en
		//Bytes del Registro en Bloque.

	} else if (argc >= 3 && action == ACTION_LN) {

		//./ejecutable [NOMBRE_ARCHIVO] -ln [NUMERO_BLOQUE]
		//Muestra por pantalla el listado de registros variables ubicados en el bloque de
		//la posición relativa [NUMERO_BLOQUE], en número de bloques, contenido
		//en el archivo de ruta [NOMBRE_ARCHIVO]. Si no está creado devuelve
		//error.

	} else if (argc >= 3 && action == ACTION_LP) {

		//./ejecutable [NOMBRE_ARCHIVO] -lp [NUMERO_BLOQUE]
		//[POSICION_REGISTRO]
		//Muestra por pantalla los datos del registro variable ubicado en el bloque
		//[NUMERO_BLOQUE] en la posición relativa interna
		//[POSICION_REGISTRO], en Bytes, contenido en el archivo de ruta
		//[NOMBRE_ARCHIVO]. Si no está creado devuelve error. Ejemplo para
		//probar un archivo binario de imagen: ./ejecutable [NOMBRE_ARCHIVO]
		//-lp [NUMERO_BLOQUE] [POSICION_REGISTRO]
		//>[archivo_de_imagen_salida]; [programa_visualizador_de_imagen]
		//[archivo_de_imagen_salida].

	} else if (argc >= 3 && action == ACTION_S) {

		//./ejecutable [NOMBRE_ARCHIVO] -s "[TEXTO]
		//Busca el [TEXTO] en los registros del archivo ubicado con ruta
		//[NOMBRE_ARCHIVO]. Muestra por pantalla los números de bloque, de
		//registro y las posiciones relativas internas de registro, que contengan el texto.
		//Si no está creado devuelve error.

	} else if (argc >= 3 && action == ACTION_E) {

		// ./ejecutable [NOMBRE_file] -e
		//Elimina el file de texto con ruta [NOMBRE_file].

		if (remove(file.c_str()) != 0) {
			cout << "error al borrar el archivo" << endl;
		}
	}
}

Espacio::~Espacio() {
}

