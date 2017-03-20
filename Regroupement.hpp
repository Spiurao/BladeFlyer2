#pragma once

#include <iostream>
#include <vector>
#include "EnsemblePompage.hpp"

using namespace std;

class Regroupement {
	private:
		vector<EnsemblePompage> regroupement;

	public:
		Regroupement();
		void remplirRegroupement(int [] tabVolume, int capaciteVehicule);
		int calculerDistanceMin(EnsemblePompage e);
};

