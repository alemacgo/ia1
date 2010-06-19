#include "BFS.h"

using namespace std;

extern int num_candidatos;
extern int num_votantes;
extern string final;
extern string candidatos[250];

extern int num_generados;
extern int num_expandidos;
extern int num_cambios;

listasoluciones BFS(Perfil *perfil_inicial, bool all){
	queue<Estado*> *q = new queue<Estado*>();
	listasoluciones metas;
	vector<Estado*> visitados;
	
	Perfil *perfil_padre = NULL;
	Perfil *perfil_actual = NULL;
	Estado *padre_anterior = NULL;

	perfil_inicial->crear_N();

	/* Encolamos el estado inicial */
	Estado *n = new Estado(NULL, 0, 0 , 0);
	q->push(n);

	int iteraciones = 0;

	while (!q->empty()
		  && (metas.empty()
		     || num_cambios == q->front()->obtener_profundidad())) {
		n = q->front();
		q->pop();

        /*
		if (num_cambios != n->obtener_profundidad()){
			cout << n->obtener_profundidad() << endl;
		}*/
		
		num_cambios = n->obtener_profundidad();

		/* Verificamos si el padre del nodo anterior es el mismo de este */
		if (!padre_anterior){
			padre_anterior = n->padre();
			perfil_padre = n->construir_padre(perfil_inicial);

		} else if (!n->es_mi_padre(padre_anterior)) {

			delete perfil_padre;
			perfil_padre = n->construir_padre(perfil_inicial);
			padre_anterior = n->padre();
		}

		perfil_actual = new Perfil(*perfil_padre);
		/* Construimos el perfil actual */
		if (n->tiene_padre()) {
			Estado* s = n;

			while (s) {
				s = s->padre();
			}
			perfil_actual->aplicar_cambio_elemental(n->obtener_fila(),
													n->obtener_columna());
		}

		perfil_actual->obtener_N();

		candidato ganador = perfil_actual->calcular_ganador_dodgson();
		if (ganador != NO_GANADOR){
			/* Hemos llegado a un goal */
			metas.push_back(pair<candidato, Perfil*>(ganador, perfil_actual));
            if (!all)
                break;
		} else if (metas.empty()) {
			/* No expandimos si hemos conseguido una meta, sus hijos
			 * estaran siempre en un nivel mas que estos
			 */
			n->expandir(q, perfil_actual, num_candidatos,
						&visitados, perfil_inicial);
			num_expandidos++;
		}
		
		delete perfil_actual;
	}
	
	for (int i=0;i<visitados.size();i++){
		delete visitados[i];
	}

	num_generados = visitados.size();
	
	return metas;
}
