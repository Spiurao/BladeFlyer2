/* ALAPETITE Florent
   MAGNIN Antoine */

#include "EnsemblePompage.hpp"
#include "Regroupement.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glpk.h> /* Nous allons utiliser la bibliothèque de fonctions de GLPK */

#include <time.h>
#include <sys/time.h>
#include <sys/resource.h> /* Bibliothèques utilisées pour mesurer le temps CPU */

/* structures et fonctions de mesure du temps CPU */

struct timeval start_utime, stop_utime;

void crono_start()
{
	struct rusage rusage;
	
	getrusage(RUSAGE_SELF, &rusage);
	start_utime = rusage.ru_utime;
}

void crono_stop()
{
	struct rusage rusage;
	
	getrusage(RUSAGE_SELF, &rusage);
	stop_utime = rusage.ru_utime;
}

double crono_ms()
{
	return (stop_utime.tv_sec - start_utime.tv_sec) * 1000 +
    (stop_utime.tv_usec - start_utime.tv_usec) / 1000 ;
}

/* Structure contenant les données du problème */

typedef struct {
	int nblieux; /* Nombre de lieux (incluant le dépôt) */
	int capacite; /* Capacité du véhicule de livraison */
	int *volumePointPompage; /* Demande de chaque lieu (la case 0 est inutilisée car le dépôt n'a aucune demande à voir satisfaire) */
	int **C; /* distancier (les lignes et colonnes 0 correspondent au dépôt) */
} donnees;

/* lecture des donnees */

void lecture_data(char *f, donnees *p)
{
	int i,j;
	FILE *file;
	
	int val;
	file = fopen(f,"rt");

	if (file == NULL){
		throw string("Fichier inconnu");
	}
	else {
		/* Lecture du nombre de villes */
		
		fscanf(file,"%d",&val);
		p->nblieux = val;

		/* Allocation mémoire pour la demande de chaque ville, et le distancier */
		
		p->volumePointPompage = (int *) malloc (val * sizeof(int));
		p->C = (int **) malloc (val * sizeof(int *));
		for(i = 0;i < val;i++) p->C[i] = (int *) malloc (val * sizeof(int));
		
		/* Lecture de la capacité */
		
		fscanf(file,"%d",&val);
		p->capacite = val;
		
		/* Lecture des demandes des clients */
		
		for(i = 1;i < p->nblieux;i++)
		{
			fscanf(file,"%d",&val);
			p->volumePointPompage[i] = val;
		}
		
		/* Lecture du distancier */

		for(i = 0; i < p->nblieux; i++)
			for(j = 0; j < p->nblieux; j++)
			{
				fscanf(file,"%d",&val);
				p->C[i][j] = val;
			}
			
		fclose(file);
	}
}

/* Fonction de libération mémoire des données */

void free_data(donnees *p)
{
	int i;
	for(i = 0;i < p->nblieux;i++) free(p->C[i]);
	free(p->C);
	free(p->volumePointPompage);	
}


int main(int argc, char *argv[])
{		
	/* Déclarations des variables (à compléter) */
	if (argc < 2){
		cout<<"Mettre les données du problème en argument"<<endl;
		return 0;
	}

	donnees p; 
	double temps;
		
	/* Chargement des données à partir d'un fichier */
	try{
		lecture_data(argv[1],&p);
	

		
		/* Lancement de la résolution... */

		crono_start(); // .. et donc du chronomètre

		/* .... */


		//Déclaration du problème
		glp_prob *prob;

		//Données

	    Regroupement r = Regroupement();
	    r.remplirRegroupement(p.volumePointPompage, p.nblieux, p.capacite, p.C);

	    vector<EnsemblePompage> ep = r.getRegroupement();

		int nbVar = ep.size();
		int nbContr = p.nblieux -1;

		//ia, ja, ar
		vector<int> ia;
		vector<int> ja;
		vector<double> ar;

		//Variables
		int i;
		int j;
		bool fini;
		double z;
		double x[nbVar];


		//Tableau des noms
		char nomVar[nbVar][10];
		char nomContr[nbContr][13];


		//Création du problème
		prob = glp_create_prob();
		glp_set_prob_name(prob, "Partionnement d'ensemble : choix des tournees");
		glp_set_obj_dir(prob, GLP_MIN);


		//Déclaration des contraintes

		glp_add_rows(prob, nbContr);

		for(i = 1; i <= nbContr; i++)
		{
			sprintf(nomContr[i], "Contrainte %d", i);

			glp_set_row_name(prob, i, nomContr[i]);

			glp_set_row_bnds(prob, i, GLP_FX, 1.0, 1.0);
		}


		//Déclaration des variables

		glp_add_cols(prob, nbVar); //On met à suivre les variables x puis les variables y

		for(i = 1; i <= nbVar; i++)
		{
			sprintf(nomVar[i],"x%d",i);

			glp_set_col_name(prob, i , nomVar[i]);

			glp_set_col_bnds(prob, i, GLP_DB, 0.0, 1.0); //Variables entre 0 et 1
			glp_set_col_kind(prob, i, GLP_BV); //Variables binaires
		}

		//Fonction objectif
		for(i = 1; i<= nbVar; i++)
			glp_set_obj_coef(prob,i,ep[i-1].getDistanceEnsemble());

		//Matrice creuse

		//ia : le numero de la contrainte
		//ja : le numero de la variable
		//ar : le coefficient de la variable
		ia.push_back(0);
		ja.push_back(0);
		ar.push_back(0);

		//Pour chaque contrainte
		vector <int> pointsdeau;
		for(i=0; i<nbVar; i++)
		{
		    pointsdeau = ep[i].getEnsemblePointsPompage();
			
			for(int j = 0; j < pointsdeau.size(); j++)
			{			
				ia.push_back(pointsdeau[j]); //Le numero de la contrainte
				ja.push_back(i+1); //Le numero de la variable			
				ar.push_back(1.0);
			}	
		}


		glp_load_matrix(prob,ia.size()-1,ia.data(),ja.data(),ar.data());


		//Pour le debug

		glp_write_lp(prob,NULL,"Debug.lp");


		//Résolution

		glp_simplex(prob,NULL);
		glp_intopt(prob,NULL);

		z = glp_mip_obj_val(prob); // on récupère la solution de la fonction objectif

		//On récupère les variables solutions
		for(int i = 0; i <  nbVar; i++)
			x[i] = glp_mip_col_val(prob,i+1);


		//Affichage

		printf("\n\nz = %lf\n",z);

		
		for(int i = 0; i < nbVar; i++) //Affichage des x
		{
			if((int)(x[i] + 0.5) == 1)
			{
				printf("%s = %d  :  ",nomVar[i+1],(int)(x[i] + 0.5)); /* un cast est ajouté, x[i] pourrait être égal à 0.99999... */

				printf("[%d", ep[i].getEnsemblePointsPompage()[0]);
				for(int j = 1; j < ep[i].getEnsemblePointsPompage().size(); j++)
				{
					printf(", %d", ep[i].getEnsemblePointsPompage()[j]);
				}

				printf("] de longueur %d\n", ep[i].getDistanceEnsemble());
			}
		}

		puts("");

		//Libération de la mémoire
		glp_delete_prob(prob);


		/* ... */



		/* Problème résolu, arrêt du chrono */
		
		crono_stop();
		temps = crono_ms()/1000,0;
		
		/* Affichage des résultats (à compléter) */
		
		printf("Temps : %f\n",temps);	
		
		/* libération mémoire (à compléter en fonction des allocations) */

		free_data(&p);
	}

	catch(string c){
		std::cerr << c <<endl; 
	}
		
	/* J'adore qu'un plan se déroule sans accroc! */
	return 0;
}
