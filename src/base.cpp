#include "base.h"

Base::Base() {
}

Base::~Base() {
}

long Base::tamanoArchivo(string file) {

	long pos = 0;
	ifstream ifs;

	ifs.open(file.c_str(), ios::in);

	if (ifs.is_open()) {

		ifs.seekg(0, ios::end);
		pos = ifs.tellg();
		ifs.close();

	} else {
		//archivo no creado
	}

	return pos;
}
