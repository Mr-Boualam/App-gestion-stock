#include "Produit.h"
#include <iostream>
#include <string.h>

using namespace std;

Produit::Produit()
{
	Reference = new char[1];
	strcpy(Reference,"");
	Designation = new char[1];
	strcpy(Designation,"");
	Prix=0;
	Quantite=0;
}

void Produit::afficher()
{
	cout<<"Reference :"<<Reference<<endl;
	cout<<"Designation :"<<Designation<<endl;
	cout<<"Prix :"<<Prix<<endl;
	cout<<"Quantite :"<<Quantite<<endl;
}

Produit::Produit(char * Reference,char * Designation,float Prix,int Quantite)
{
	this->Reference = new char[strlen(Reference)+1];
	strcpy(this->Reference,Reference);
	this->Designation = new char[strlen(Designation)+1];
	strcpy(this->Designation,Designation);
	this->Prix=Prix;
	this->Quantite=Quantite;
}

Produit::Produit(Produit &P)
{
	this->Reference = new char[strlen(P.Reference)+1];
	strcpy(this->Reference,P.Reference);
	this->Designation = new char[strlen(P.Designation)+1];
	strcpy(this->Designation,P.Designation);
	this->Prix=P.Prix;
	this->Quantite=P.Quantite;
}

//les set :

void Produit::setReference(const char * Reference)
{
	this->Reference = new char[strlen(Reference)+1];
	strcpy(this->Reference,Reference);
}

void Produit::setDesignation(const char * Designation)
{
	this->Designation = new char[strlen(Designation)+1];
	strcpy(this->Designation,Designation);
}
void Produit::setPrix(float Prix)
{
	this->Prix=Prix;
}

void Produit::setQuantite(int Quantite)
{
	this->Quantite=Quantite;
}

//les get:

char * Produit::getReference() const
{
	char * tmp = new char[strlen(Reference)+1];
	strcpy(tmp,Reference);
	return tmp;
}

char * Produit::getDesignation() const
{
	char * tmp = new char[strlen(Designation)+1];
	strcpy(tmp,Designation);
	return tmp;
}

float Produit::getPrix() const
{
	return Prix;
}

int Produit::getQuantite() const
{
	return Quantite;
}

bool Produit::equals(Produit &P)
{
	if(strcmp(P.Reference,Reference) == 0)
	return true;
	return false;
}

void Produit::setProduit(Produit &P)
{
	setReference(P.getReference());
	setDesignation(P.getDesignation());
	setPrix(P.getPrix());
	setQuantite(P.getQuantite());
}














