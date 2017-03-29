#include "EnsemblePompage.hpp"

EnsemblePompage::EnsemblePompage(){
	ensemblePointsPompage = vector<int>();
	distanceEnsemble = 0;
}

EnsemblePompage::EnsemblePompage(vector<int> points){
	ensemblePointsPompage = points;
}

vector<int> EnsemblePompage::getEnsemblePointsPompage(){
	return ensemblePointsPompage;
}

int EnsemblePompage::getDistanceEnsemble(){
	return distanceEnsemble;
}

void EnsemblePompage::ajouterPoint(int point){
	ensemblePointsPompage.push_back(point);
}

void EnsemblePompage::trouverPermutationMin(int** matriceDistance){
	int distanceMin = distanceEnsemblePompage(matriceDistance);
	vector<int> ensembleMin = ensemblePointsPompage;

	while(next_permutation(ensemblePointsPompage.begin(), ensemblePointsPompage.end())){
		int distance = distanceEnsemblePompage(matriceDistance);
	
		if (distance < distanceMin){
			distanceMin=distance;
			ensembleMin = ensemblePointsPompage;
		}
	
	}

	ensemblePointsPompage = ensembleMin;
	distanceEnsemble = distanceMin;
}

int EnsemblePompage::distanceEnsemblePompage(int** matriceDistance){
	int resultat = matriceDistance[0][ensemblePointsPompage[0]];

	//Si un seul point à visiter, on fait l'aller retour
	if(ensemblePointsPompage.size() == 0)
		resultat *= 2;
	else{
		//Parcours des distances des ensemblePointsPompage 2 à 2
		for(int i = 1; i < ensemblePointsPompage.size(); ++i)
			resultat += matriceDistance[ensemblePointsPompage[i-1]][ensemblePointsPompage[i]];

		//Retour à la base
		resultat += matriceDistance[ensemblePointsPompage [ensemblePointsPompage.size()-1] ]
									[0];
	}

	return resultat;
}