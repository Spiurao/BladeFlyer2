#include "EnsemblePompage.hpp"

EnsemblePompage::EnsemblePompage(){
	ensemblePointsPompage = set<int>();
}

void EnsemblePompage::ajouterPoint(int point, int distance){
	ensemblePointsPompage.insert(point);
	distanceEnsemble+=distance;
}