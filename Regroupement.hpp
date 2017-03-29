#pragma once

#include <iostream>
#include <vector>
#include "EnsemblePompage.hpp"

using namespace std;

class Regroupement {
	private:
		vector<EnsemblePompage> regroupement;
		int distanceRegroupement;

	public:
		Regroupement();
		vector<EnsemblePompage> getRegroupement();
		int getDistanceRegroupement();
		void calculerDistanceRegroupement();
		void remplirRegroupement(int* tabVolume, int nblieux, int capaciteVehicule, int** matriceDistance);
};

