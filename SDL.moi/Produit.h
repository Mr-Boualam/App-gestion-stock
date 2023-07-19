#ifndef PRODUIT_H
#define PRODUIT_H

class Produit
{
	private:
		char * Reference;
		char * Designation;
		float Prix;
		int Quantite;
	public:
		Produit();
		void afficher();
	    Produit(char * Reference,char * Designation,float Prix,int Quantite);
	    Produit(Produit &P);
	    //les set :
	    void setReference( const char * Reference);
	    void setDesignation(const char * Designation);
	    void setPrix(float Prix);
	    void setQuantite(int Quantite);
	    void setProduit(Produit &P);
	    //les get :
	    char * getReference() const;
	    char * getDesignation() const;
	    float getPrix() const;
	    int getQuantite() const;
	    bool equals(Produit &P);
	    
	    
	    
};

#endif
