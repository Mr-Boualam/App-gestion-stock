#include "Stock.h"
#include "Produit.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


using namespace std;

Stock::Stock()
{
	nbp=0;
}

void Stock::afficher()
{
	for(int i=0;i<nbp;i++)
	{
		cout<<"Produit "<<i+1<<":"<<endl;
		TP[i].afficher();
	}
}

bool Stock::ajouterProduit(Produit &P)
{
	if(nbp<10)
	{
		TP[nbp].setProduit(P);
		nbp++;
		return true;
	}
	return false;
}

int Stock::getNBrProduit()
{
	return nbp;
}

Stock::Stock(Stock &s)
{
	for(int i=0;i<s.nbp;i++)
	TP[i].setProduit(s.TP[i]);
    nbp=s.nbp;
}

int Stock::getProduitPosition(const char * Reference)
{
	for(int i=0;i<nbp;i++)
	{
		if(strcmp(Reference,TP[i].getReference())==0)
		return i;
	}
	return -1;
}

bool Stock::supprimerProduit(const char * Reference)
{
	int x=getProduitPosition(Reference);
	if(x != -1){
		for(int i=x;i<nbp-1;i++){
			TP[i]=TP[i+1];	
		}
	    nbp--;
	    return true;
    }else return false;
}

bool Stock::modifierProduit(char * Reference,Produit P)
{
	int x=getProduitPosition(Reference);
	if(x !=-1)
	{
		TP[x].setProduit(P);
		return 1;
	}
	return -1;
}

float Stock::montantStock()
{
	float som=0;
	for(int i=0;i<nbp;i++)
	som+=(TP[i].getPrix()) * (TP[i].getQuantite());
    return som;
}

Produit & Stock::getProduit(int Poss)
{
	
	if(Poss>=0 && Poss<nbp)
	return TP[Poss];
	else 
	{
		static Produit DProduit;
		return DProduit;
	}
}

Produit & Stock::getProduitPos(const char * Reference)
{
	int x=getProduitPosition(Reference);
	if(x!=-1)
	return TP[x];
}
void Stock::sauvegarderStockCSV(){
	
	ofstream fichier("Stock.csv");

    if (fichier.is_open()) {
        // Écrire l'en-tête du fichier CSV
        fichier << "Reference,Designation,Prix,Quantité" << endl;

        // Parcourir le tableau de produits et écrire chaque produit dans le fichier CSV
        for (int i = 0; i < nbp; ++i) {
            fichier << TP[i].getReference() << "," << TP[i].getDesignation() << "," << TP[i].getPrix() << "," << TP[i].getQuantite() << endl;
        }

        // Fermer le fichier
        fichier.close();

        cout << "Le stock a été sauvegardé dans le fichier CSV : " << "Stock.csv" << endl;
    } else {
        cerr << "Impossible d'ouvrir le fichier : " << "Stock.csv" << endl;
    }
}

void Stock::importFromCSV() {
    ifstream fichier("Stock.csv");

    if (fichier.is_open()) {
        string ligne;

        // Ignorer l'en-tête du fichier CSV
        getline(fichier, ligne);

        int index = 0;

        // Lire chaque ligne du fichier CSV
        while (getline(fichier, ligne)) {
            istringstream iss(ligne);
            string token;

            // Lire les valeurs séparées par des virgules dans chaque ligne
            getline(iss, token, ',');
            TP[index].setReference(token.c_str());
            
            getline(iss, token, ',');
            TP[index].setDesignation(token.c_str());

            /*getline(iss, token, ',');
            float prix = std::stof(token.c_str());
            TP[index].setPrix(prix);
            */
            
			getline(iss, token, ',');
            std::stringstream ss(token);
		    float floatValue = 0;
		    ss >> floatValue;
		    TP[index].setPrix(floatValue);
            
            getline(iss, token, ',');
            std::stringstream ss1(token);
		    int intValue = 0;
		    ss1 >> intValue;
		    TP[index].setQuantite(intValue);
		            

            index++;
        }
        nbp=index;

        cout << "Importation du fichier CSV terminée." << endl;
        cout << "Nombre de produits importés : " << index << endl;

        fichier.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier : " << "Stock.csv" << endl;
    }
}
