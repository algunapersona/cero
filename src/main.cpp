#include "base.h"
#include "plano.h"
#include "longitud.h"
#include "espacio.h"

using namespace std;

int main(int argc, char *argv[]) {

	Base *base = new Espacio();

	base->execute(argc, argv);

	delete base;

    return 0;
}
