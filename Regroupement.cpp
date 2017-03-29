#include "Regroupement.hpp"

using namespace std;;

Regroupement::Regroupement(){
	regroupement = vector<EnsemblePompage>();
	distanceRegroupement = 0;
}

vector<EnsemblePompage> Regroupement::getRegroupement(){
	return regroupement;
}

int Regroupement::getDistanceRegroupement(){
	return distanceRegroupement;
}

void Regroupement::calculerDistanceRegroupement(){
	int resultat = 0;
	for(EnsemblePompage e:regroupement)
		resultat+=e.getDistanceEnsemble();

	distanceRegroupement = resultat;
}

void Regroupement::remplirRegroupement(int* tabVolume, int nblieux, int capaciteVehicule, int** matriceDistance){
    int pile[nblieux+1];
    int position = 0;
    int quantiteEau = 0;
    EnsemblePompage ep;
    vector<int> lieux = vector<int>();
    //La base est la position 0 est n'est pas dans les parties de l'ensemble
    pile[0] = 0;

    bool fini = false;

    while(!fini){

            if (pile[position]<nblieux){
                pile[position+1] = pile[position]+1;
                ++position;
            }
            else{
                pile[position-1]++;
                --position;
            }

            if (position == 0)
                fini = true;

            for(int i = 1; i<=position; ++i)
            	quantiteEau += tabVolume[pile[i]];

            if(quantiteEau <= capaciteVehicule){
            	
            	for(int i = 1; i < position; ++i)
            		lieux.push_back(pile[i]);

            	ep = EnsemblePompage(lieux);
                ep.trouverPermutationMin(matriceDistance);
            	regroupement.push_back(ep);
            	lieux = vector<int>();

            }

        quantiteEau = 0;
    }
	
}

