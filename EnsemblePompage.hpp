#pragma once

#include <set>
#include <iostream>

using namespace std;

class EnsemblePompage {
	private:
		set<int> ensemblePointsPompage;
		int distanceEnsemble;

	public:
		EnsemblePompage();
		void ajouterPoint(int point, int distance);

};
