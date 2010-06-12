#include "dodgson.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <map>

#define NO_ALGORITMO 0
using namespace std;

int num_candidatos;
int num_votantes;

int main(int argc, char* argv[]){
	char algoritmo = NO_ALGORITMO;
	bool all = false;
	bool ok = true;
	string filename, final;

	filename = "";

	/* Verificamos que los parametros se pasen correctamente
	 */
	for (int i=1;i<argc && ok;i++) {
		if(!strcmp(argv[i],"-ida") || !strcmp(argv[i],"-bfs"))
		{
			/* Asignacion del algoritmo */
			if (algoritmo == NO_ALGORITMO){
				algoritmo = (strcmp(argv[i],"-ida") ? 1:2);
			} else {
				ok = false;	// Caso de doble asignacion de algoritmo
			}

		} else if (!strcmp(argv[i],"-all")){

			/* Indica que se deben devolver todos los optimos */
			if (!all){
				all = true;
			} else {
				ok = false;	// Caso de doble asignacion
			}

		} else if (!strcmp(argv[i],"-final")){

			/* Asignacion de archivo de salida */
			if (++i<argc && final.size() == 0){
				final = argv[i];
			} else {
				ok = false; // Doble asignacion o falta de nombre
			}

		} else if (filename.size() == 0){

			/* Asignacion de archivo de entrada */
			filename = argv[i];

			/* Verificacion de existencia */
			struct stat f__stat;
			if(stat(filename.c_str(),&f__stat)){
				ok = false;
			}
		} else {

			/* Error de entrada */
			ok = false;
		}
	}

	ok = ok && filename.size() && algoritmo;

	if (!ok){
		cout << endl
			 << "  Uso de dogson: "
			 << endl << endl
			 << "	dogson {-ida,-bfs} [-all] [-final <output>] input"
			 << endl << endl;
		exit(0);
	}

	/* Leemos el perfil asociado al archivo de entrada */

	ifstream file;
	file.open(filename.c_str());
	
	string siguiente;
	int num_preferencias;

	/* Descartamos la lectura de profile y leemos el numero de candidatos */
	file >> siguiente >> num_candidatos;

	/* vector y map que permiten la traduccion entre candidatos y si posicion
	 */
	vector<string> candidatos;
	map<string,unsigned char> indice_candidatos;

	candidatos.resize(num_candidatos);

	/* Leemos todos los candidatos y hacemos las respectivas
	 * asociaciones de indice
	 */
	for (int i=0; i < num_candidatos; i++){
		file >> candidatos[i];
		indice_candidatos.insert(pair<string,unsigned char>(candidatos[i],i));
	}

	file >> num_preferencias;

	Perfil *p = new Perfil();

	/* Leemos el conjunto de preferencias */

	num_votantes = 0;
	while(num_preferencias){
		int num_preferencia;
		candidato *a = new candidato[num_candidatos];

		file >> num_preferencia;

		for (int k=0; k<num_candidatos; k++){
			file >> siguiente;
			a[k] = indice_candidatos[siguiente];
		}

		Preferencia *pr = new Preferencia(num_preferencia,a);
		p->agregar_preferencia(pr);

		num_preferencias -= 1;

		num_votantes += num_preferencia;
	}

	cout << "votantes " << num_votantes << endl;

	file.close();

    cout << (int)algoritmo << endl;
	switch (algoritmo) {
        case 1:
            BFS(p, all);
            break;
        case 2:
            IDAestrella(p, all);
    }

	return 0;
}