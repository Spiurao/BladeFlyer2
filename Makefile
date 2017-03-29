all:LancerModelisation

compilationInitiale: 
	g++ *.cpp -std=c++11 -o modeleProjet -lglpk -lm


LancerModelisation: compilationInitiale
	./modeleProjet ${ARGS}

clean:	
	rm Debug.lp
	rm modeleProjet


