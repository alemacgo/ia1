#include "IDAestrella.h"

using namespace std;

extern int num_candidatos;
extern int num_votantes;

extern int num_generados;
extern int num_expandidos;
extern int num_cambios;

bool todos;

int IDFS(int g, int limite, Perfil* p, list<candidato>* metas, list<Cambio*>* visitados, Perfil* inicial) {

    int f = g + p->h();
    if (f > limite) {
        return f;
    }

    candidato ganador = p->calcular_ganador_dodgson();

    if (ganador != NO_GANADOR) {
        num_cambios = g;
        metas->push_back(ganador);

        return f;
    }

	//cout << visitados->next() << endl;
	//inicial->print(cout);
	
    int preferencias = p->obtener_num_preferencias();
    int nuevo_limite;

    /* Para cada preferencia, todos sus posibles cambios
     * elementales */

    num_expandidos++;
	for (int i=0; i < preferencias; i++){
		for (unsigned char j = 0; j + 1 < num_candidatos; j++){

			p->swap_N(p->obtener(j,i), p->obtener(j+1,i));
			int busqueda = p->aplicar_cambio_elemental(j, i);
			
			/*
			num_generados++;
			

			nuevo_limite = IDFS(g + 1, limite, p, metas, visitados, inicial);
			
			p->aplicar_cambio_elemental(j, busqueda);
			p->swap_N(p->obtener(j+1,i), p->obtener(j,i));

			if (!todos && !metas->empty()) {
				return nuevo_limite;
			}
			
			continue;
			*/
			
			/* Comparar con los nodos visitados ancestros */
			Perfil* s = new Perfil(*inicial);
			
			bool iguales = false;
			if (s->compare(*p) == 0){
				iguales = true;
			} else {
				//cout << "Hola mundo " << s->compare(*p) << endl;
				list<Cambio*>::iterator it;

				for ( it=visitados->begin() ; it != visitados->end() && !iguales; it++ ){
					s->aplicar_cambio_elemental((*it)->fila, (*it)->columna);
					iguales = (s->compare(*p)==0);
				}
			}
			delete s;
			
			if (!iguales){
				num_generados++;
				
				Cambio* nuevo_cambio = new Cambio();
				nuevo_cambio->fila = j;
				nuevo_cambio->columna = i;
				visitados->push_back(nuevo_cambio);

				nuevo_limite = IDFS(g + 1, limite, p, metas, visitados, inicial);
			
				delete visitados->back();
				visitados->pop_back();
			}
			
			p->aplicar_cambio_elemental(j, busqueda);
			p->swap_N(p->obtener(j+1,i), p->obtener(j,i));

			if (!todos && !metas->empty()) {
				return nuevo_limite;
			}
		}
	}
	
	return nuevo_limite;
}

list<candidato> IDAestrella(Perfil *perfil_inicial, bool all){
	todos = all;
	
    list<candidato>* metas = new list<candidato>;

    perfil_inicial->crear_N();
    perfil_inicial->obtener_N();

    int limite_f = perfil_inicial->h();
    list<Cambio*>* visitados = new list<Cambio*>;


	Perfil *p = new Perfil(*perfil_inicial);
	
    while (metas->empty()) {
        limite_f = IDFS(0, limite_f, p, metas, visitados, perfil_inicial);
    }

    //cout << limite_f << endl;
    return *metas;
}
