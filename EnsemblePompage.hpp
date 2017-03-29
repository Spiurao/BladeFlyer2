#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

using namespace std;

class EnsemblePompage {
	private:
		vector<int> ensemblePointsPompage;
		int distanceEnsemble;

	public:
		EnsemblePompage();
		EnsemblePompage(vector<int> points);
		vector<int> getEnsemblePointsPompage();
		int getDistanceEnsemble();
		void ajouterPoint(int point);
		void trouverPermutationMin(int** matriceDistance);
		int distanceEnsemblePompage(int** matriceDistance);
};
