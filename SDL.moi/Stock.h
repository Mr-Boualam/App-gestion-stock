#ifndef STOCK_H
#define STOCK_H
#include "Produit.h"

class Stock
{
	private:
		Produit TP[10];
		int nbp;
	public:
		Stock();
		void afficher();
		Stock(Stock &s);
		bool ajouterProduit(Produit &P);
		int getNBrProduit();
		int getProduitPosition(const char * Reference);
		bool supprimerProduit(const char * Reference);
		bool modifierProduit(char * reference,Produit P);
		float montantStock();
	    Produit & getProduit(int Poss);
	    Produit & getProduitPos(const char * Reference);
	    void sauvegarderStockCSV();
	    void importFromCSV();
	    
};

#endif


