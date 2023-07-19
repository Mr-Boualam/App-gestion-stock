#include <iostream>
#include "Produit.h"
#include <string.h>
#include "Stock.h"
#include <fstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <sstream>
#include <string>

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//test des class:
/*void saisirProduit()
{
	Produit P;
	cout<<"Produit P:"<<endl;
	P.afficher();
	Produit P1("R1","D1",15,12);
	cout<<"Produit P1:"<<endl;
	P1.afficher();
	cout<<"Copie Produit P1 en Produit P2:"<<endl;
	Produit P2(P1);
	P2.afficher();
	cout<<"les set :"<<endl;
	Produit P3;
	P3.setReference("r4");
    P3.setDesignation("d4");
	P3.setPrix(45);
	P3.setQuantite(78);
	P3.afficher();
	cout<<"les get :"<<endl;
	cout<<P2.getReference()<<endl;
	cout<<P2.getDesignation()<<endl;
	cout<<P2.getPrix()<<endl;
	cout<<P2.getQuantite()<<endl;
	cout<<"equls P1 et P3 :"<<endl;
	if(P1.equals(P3))
	cout<<"meme produit"<<endl;
	else 
	cout<<"produit different"<<endl;
	
	
}

void saisirStock()
{
	Stock S;
	Produit P1("r1","d1",11,11);
	Produit P2("r2","d2",22,22);
	Produit P3("r3","d3",33,33);
	Produit P4("r4","d4",44,44);
	S.ajouterProduit(P1);
	S.ajouterProduit(P2);
	S.ajouterProduit(P3);
	S.ajouterProduit(P4);
	S.afficher();
	Stock S1(S);
	S1.afficher();
	cout<<S.getNBrProduit()<<endl;
	cout<<S.getProduitPosition("r2")<<endl;
	S.supprimerProduit("r4");
	S.afficher();
	S.modifierProduit("r1",P4);
	S.afficher();
	cout<<S.montantStock()<<endl;
	Produit P(S.getProduit(1));
	P.afficher();
	cout<<"**"<<endl;
	Produit P5(S.getProduitPos("r4"));
	P5.afficher();
	
}*/
Stock s;
const int SCREEN_WIDTH=600;
const int SCREEN_HEIGHT=600;
const int FONT_SIZE=24;

SDL_Surface* screen = NULL;
SDL_Surface* textSurface = NULL;
TTF_Font* font = NULL;
SDL_Surface* previousFrame = NULL;
SDL_Rect previousFrameRect;

bool returnToPreviousFrame = false;

//les constantes de mouse:
//de Open:
const int INTERVAL_MINO_X=238;
const int INTERVAL_MAXO_X=362;
const int INTERVAL_MINO_Y=385;
const int INTERVAL_MAXO_Y=467;
//Ajouter un produit:
const int INTERVAL_MINAP_X=69;
const int INTERVAL_MAXAP_X=343;
const int INTERVAL_MINAP_Y=64;
const int INTERVAL_MAXAP_Y=149;
//Recherche:
const int INTERVAL_MINR_X=69;
const int INTERVAL_MAXR_X=300;
const int INTERVAL_MINR_Y=177;
const int INTERVAL_MAXR_Y=263;
//Supprimer:
const int INTERVAL_MINS_X=69;
const int INTERVAL_MAXS_X=408;
const int INTERVAL_MINS_Y=294;
const int INTERVAL_MAXS_Y=377;
//Afficher:
const int INTERVAL_MINA_X=69;
const int INTERVAL_MAXA_X=259;
const int INTERVAL_MINA_Y=406;
const int INTERVAL_MAXA_Y=487;
//Afficher-Quiter:
const int INTERVAL_MINAQ_X=268;
const int INTERVAL_MAXAQ_X=346;
const int INTERVAL_MINAQ_Y=544;
const int INTERVAL_MAXAQ_Y=588;

//fonctio qui est responsable de l'initialisation de la bibliothèque SDL, de la création d'une fenêtre,
//de la définition de la légende de la fenêtre et de l'initialisation de l'extension SDL_ttf.
int initializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 0;
    }
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if (screen == NULL)
    {
        fprintf(stderr, "Erreur lors de la cr?ation de la fen?tre: %s\n", SDL_GetError());
        return 0;
    }
    SDL_WM_SetCaption("Affichage de texte avec SDL 1.2", NULL);
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_ttf: %s\n", TTF_GetError());
        return 0;
    }
    return 1;
}

//représentant le chemin d'accès à un fichier de police, et charge la police à l'aide de SDL_ttf.
int loadFont(const char* fontPath)
{
    font = TTF_OpenFont(fontPath, FONT_SIZE);
    if (font == NULL)
    {
        fprintf(stderr, "Erreur lors du chargement de la police de caract?res: %s\n", TTF_GetError());
        return 0;
    }
    return 1;
}

//fonction qui rende un texte :
SDL_Surface * renderText(const char * text,Uint8 r,Uint8 g,Uint8 b)
{
	SDL_Color color = {r,g,b};
	textSurface = TTF_RenderText_Solid(font,text,color);
	if(textSurface == NULL)
	{
		fprintf(stderr, "Erreur lors du rendu du texte: %s\n", TTF_GetError());
		return NULL;
	}
	return textSurface;
}

//fonction qui est responsable du nettoyage et de la libération des ressources utilisées par SDL et SDL_ttf.
void cleanup()
{
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

//fonction qui représentant la surface contenant le texte à dessiner, et deux paramètres int x et y, 
//représentant les coordonnées où le texte sera dessiné à l'écran.
void drawText(SDL_Surface* textSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_BlitSurface(textSurface, NULL, screen, &destRect);
    SDL_Flip(screen);
}

//fonction qui designe les coordonnes.
void coordonnes(int mouseX,int mouseY)
{
	char coordinates[50];
	sprintf(coordinates, "x=%d, y=%d", mouseX, mouseY);
	SDL_Surface* coordinatesSurface = renderText(coordinates, 255, 0, 0);
	if (coordinatesSurface != NULL)
	{
		drawText(coordinatesSurface, mouseX, mouseY);
		SDL_FreeSurface(coordinatesSurface);
		SDL_Delay(3000);
	}
}










//Ajouter produit :
void AjouterP()
{
	

	
	
	
	
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen = SDL_SetVideoMode(600, 600, 32, SDL_SWSURFACE);
	TTF_Init();
	SDL_EnableUNICODE(1);
	loadFont("arial.ttf");
	SDL_Surface * texteSurface = renderText("Bonjour, SDL 1.2",255,255,255);
	bool isRunning = false;
    SDL_Event event;
	SDL_Surface * background = NULL;
 	background = IMG_Load("3.png");
	SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);
    SDL_Color textColor = { 255, 0, 0 };
	SDL_Rect textField1 = { 350, 107, 200, 30 };
	SDL_Rect textField2 = { 350, 207, 200, 30 };
	SDL_Rect textField3 = { 350, 307, 200, 30 };
	SDL_Rect textField4 = { 350, 407, 200, 30 };
	SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Champ 1 :", textColor);
	SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "Champ 2 :", textColor);
	SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, "Champ 3 :", textColor);
	SDL_Surface* textSurface4 = TTF_RenderText_Solid(font, "Champ 4 :", textColor);
	std::string inputText1, inputText2, inputText3, inputText4;
	bool activeField1 = true, activeField2 = false, activeField3 = false, activeField4 = false;
	//position de button ajouter:
	const int INTERVAL_MIN_X = 182;
	const int INTERVAL_MAX_X = 395;
	const int INTERVAL_MIN_Y = 506;
	const int INTERVAL_MAX_Y = 567;

   while (!isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = true;
			}	
			else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    //coordonnes(mouseX,mouseY);
                    
                    if (mouseX >= INTERVAL_MIN_X && mouseX <= INTERVAL_MAX_X && mouseY >= INTERVAL_MIN_Y && mouseY <= INTERVAL_MAX_Y)
                    {
						// ajouter le produit p dans stock s:
                    	float prix = atof(inputText3.c_str());
                    	int quantite = atoi(inputText4.c_str());            	
                    	char* ref = new char[inputText1.length() + 1];
					    strcpy(ref, inputText1.c_str());					
					    char* designation = new char[inputText2.length() + 1];
					    strcpy(designation, inputText2.c_str());					    
                    	Produit p;
                    	p.setDesignation(designation);
                    	p.setReference(ref);
                    	p.setPrix(prix);
                    	p.setQuantite(quantite);
                    	s.ajouterProduit(p);
                    	return;
                    }
				}
            }
			
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = true;
				}
				else if (event.key.keysym.sym == SDLK_TAB) {
					activeField1 = !activeField1;
					activeField2 = !activeField2;
					activeField3 = !activeField3;
					activeField4 = !activeField4;
				}
				else if (event.key.keysym.sym == SDLK_RETURN) {
					if (activeField1) {
						activeField1 = false;
						activeField2 = true;
					}
					else if (activeField2) {
						activeField2 = false;
						activeField3 = true;
					}
					else if (activeField3) {
						activeField3 = false;
						activeField4 = true;
					}
					else if (activeField4) {
						activeField4 = false;
					}
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE) {
				    if (activeField1 && !inputText1.empty()) {
				    	
				    	 inputText1 = inputText1.substr(0, inputText1.length() - 1);
					        SDL_FillRect(screen, &textField1, SDL_MapRGB(screen->format, 173,179,184));			       
				    }
				    else if (activeField2 && !inputText2.empty()) {
				        inputText2 = inputText2.substr(0, inputText2.length() - 1);
					        SDL_FillRect(screen, &textField2, SDL_MapRGB(screen->format, 173,179,184));
				    }
				    else if (activeField3 && !inputText3.empty()) {
				        inputText3 = inputText3.substr(0, inputText3.length() - 1);
					        SDL_FillRect(screen, &textField3, SDL_MapRGB(screen->format, 173,179,184));
				    }
				    else if (activeField4 && !inputText4.empty()) {
				        inputText4 = inputText4.substr(0, inputText4.length() - 1);
					        SDL_FillRect(screen, &textField4, SDL_MapRGB(screen->format, 173,179,184));
				    }
				}
				else {
					if (activeField1) {
						char inputChar = event.key.keysym.unicode;
						if (isprint(inputChar) || inputChar == ' ') {
							inputText1 += inputChar;
						}
					}
					else if (activeField2) {
						char inputChar = event.key.keysym.unicode;
						if (isprint(inputChar) || inputChar == ' ') {
							inputText2 += inputChar;
						}
					}
					else if (activeField3) {
						char inputChar = event.key.keysym.unicode;
						if (isdigit(inputChar) || inputChar == ' ') {
							inputText3 += inputChar;
						}
					}
					else if (activeField4) {
						char inputChar = event.key.keysym.unicode;
						if (isdigit(inputChar) || inputChar == ' ') {
							inputText4 += inputChar;
						}
					}
				}
			}
		}
		//rendu des surfaces de texte sur l'écran:
		if (activeField1) {
			SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputText1.c_str(), textColor);
			SDL_BlitSurface(inputSurface, NULL, screen, &textField1);
			SDL_FreeSurface(inputSurface);
		}
		else if (activeField2) {
			SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputText2.c_str(), textColor);
			SDL_BlitSurface(inputSurface, NULL, screen, &textField2);
			SDL_FreeSurface(inputSurface);
		}
		else if (activeField3) {
			SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputText3.c_str(), textColor);
			SDL_BlitSurface(inputSurface, NULL, screen, &textField3);
			SDL_FreeSurface(inputSurface);
		}
		else if (activeField4) {
			SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputText4.c_str(), textColor);
			SDL_BlitSurface(inputSurface, NULL, screen, &textField4);
			SDL_FreeSurface(inputSurface);
		}
		SDL_Flip(screen);
	}
    
    SDL_FreeSurface(textSurface1);
	SDL_FreeSurface(textSurface2);
	SDL_FreeSurface(textSurface3);
	SDL_FreeSurface(textSurface4);

	TTF_CloseFont(font);
	TTF_Quit();

	SDL_Quit();
}

//Recherche produit :
void Recherche()
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(600, 600, 32, SDL_SWSURFACE);
    TTF_Init();
    SDL_EnableUNICODE(1);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 50);
    SDL_Color textColor = { 255, 0, 0 };
    SDL_Rect textField1 = { 90, 110, 200, 30 };
    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Champ 1 :", textColor);
    SDL_Event event;
    bool quit = false;
    std::string inputText1;
    bool activeField1 = true;
    SDL_Surface* background = NULL;
    background = IMG_Load("4.png");
    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);
    //position de button quitter:
	const int INTERVAL_MIN_X = 182;
	const int INTERVAL_MAX_X = 395;
	const int INTERVAL_MIN_Y = 506;
	const int INTERVAL_MAX_Y = 567;
	//position de button recherche:
    const int INTERVAL_MIN_Xs = 358;
	const int INTERVAL_MAX_Xs = 560;
	const int INTERVAL_MIN_Ys = 106;
	const int INTERVAL_MAX_Ys = 178;
    std::string referenceInput;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
			        //coordonnes(mouseX,mouseY);
                    if (mouseX >= INTERVAL_MIN_X && mouseX <= INTERVAL_MAX_X && mouseY >= INTERVAL_MIN_Y && mouseY <= INTERVAL_MAX_Y)
                    {
                        return;
                    }
                    else if (mouseX >= INTERVAL_MIN_Xs && mouseX <= INTERVAL_MAX_Xs && mouseY >= INTERVAL_MIN_Ys && mouseY <= INTERVAL_MAX_Ys)
                    {
                        referenceInput = inputText1;
                        bool produitExiste = false; 								
                                produitExiste = true; 
                                const char * reference = new char[50];
								reference = referenceInput.c_str();                              
								int p = s.getProduitPosition(reference);
								if(p != -1){
								Produit produit = s.getProduitPos(reference);
								char* designation = produit.getDesignation();
                                int quantite = produit.getQuantite();
                                float prix = produit.getPrix();
                                char* price = new char[50];
                                char* quantity = new char[50];
                                sprintf(price, "%0.2f", prix);
                                sprintf(quantity, "%d", quantite);
                                SDL_Rect textreference = { 90, 250, 300, 50 };
                                SDL_Rect textdesignation = { 180, 250, 300, 50 };
                                SDL_Rect textprix = { 270, 250, 300, 50 };
                                SDL_Rect textquantite = { 450, 250, 300, 50 };
                                SDL_Surface* inputSurface1 = TTF_RenderText_Solid(font, reference, textColor);
                                SDL_BlitSurface(inputSurface1, NULL, screen, &textreference);
                                SDL_FreeSurface(inputSurface1);
                                SDL_Surface* inputSurface2 = TTF_RenderText_Solid(font, designation, textColor);
                                SDL_BlitSurface(inputSurface2, NULL, screen, &textdesignation);
                                SDL_FreeSurface(inputSurface2);
                                SDL_Surface* inputSurface3 = TTF_RenderText_Solid(font, price, textColor);
                                SDL_BlitSurface(inputSurface3, NULL, screen, &textprix);
                                SDL_FreeSurface(inputSurface3);
                                SDL_Surface* inputSurface4 = TTF_RenderText_Solid(font, quantity, textColor);
                                SDL_BlitSurface(inputSurface4, NULL, screen, &textquantite);
                                SDL_FreeSurface(inputSurface4);
                                break; 
								}
								else{
									SDL_Rect textMessage = { 100, 250, 300, 50 };
		                            SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Produit n'exist pas ", textColor);
		                            SDL_BlitSurface(messageSurface, NULL, screen, &textMessage);
		                            SDL_FreeSurface(messageSurface);
								}       
                        }                       
                        inputText1 = "";
                    }                
                }          
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                else if (event.key.keysym.sym == SDLK_TAB) {
                    activeField1 = !activeField1;
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (activeField1) {
                        activeField1 = false;
                    }
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					    if (activeField1 && !inputText1.empty()) {					    						       
					        inputText1 = inputText1.substr(0, inputText1.length() - 1);
					        SDL_FillRect(screen, &textField1, SDL_MapRGB(screen->format, 124,129,135));         
					    }
					}
                else {
                    if (activeField1) {
                        char inputChar = event.key.keysym.unicode;
                        if (isprint(inputChar) || inputChar == ' ') {
                            inputText1 += inputChar;
                        }
                    }
                }
            }
        }
        if (activeField1) {
            SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputText1.c_str(), textColor);
            SDL_BlitSurface(inputSurface, NULL, screen, &textField1);
            SDL_FreeSurface(inputSurface);
        }
        SDL_Flip(screen);
    }   
}

//Supprimer produit :
/*
void Supprimer()
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(600, 600, 32, SDL_SWSURFACE);
    TTF_Init();
    SDL_EnableUNICODE(1);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 50);
    SDL_Color textColor = { 255, 0, 0 };
    SDL_Rect textField1 = { 90, 110, 200, 30 };
    SDL_Rect textMessage = { 125, 250, 300, 50 };
    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Champ 1 :", textColor);
    SDL_Event event;
    bool quit = false;
    std::string inputText1;
    bool activeField1 = true;
    SDL_Surface* background = NULL;
    background = IMG_Load("5.png");
    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);
    
    
    // Copie de l'image sur la fenêtre aux coordonnées x-y
    
    
    
    //position de button quitter:
	const int INTERVAL_MIN_X = 182;
	const int INTERVAL_MAX_X = 395;
	const int INTERVAL_MIN_Y = 506;
	const int INTERVAL_MAX_Y = 567;
	//position de button recherche:
    const int INTERVAL_MIN_Xs = 358;
	const int INTERVAL_MAX_Xs = 560;
	const int INTERVAL_MIN_Ys = 106;
	const int INTERVAL_MAX_Ys = 178;
	//Position button annuler
/*	const int INTERVAL_MIN_Xs = 358;
	const int INTERVAL_MAX_Xs = 560;
	const int INTERVAL_MIN_Ys = 106;
	const int INTERVAL_MAX_Ys = 178;
	//Position button valider
	const int INTERVAL_MIN_Xv = 168;
	const int INTERVAL_MAX_Xv = 309;
	const int INTERVAL_MIN_Yv = 241;
	const int INTERVAL_MAX_Yv = 283;
    std::string referenceInput;
    SDL_Surface* zone = NULL;
    SDL_Surface* savedImage = NULL;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
			        //coordonnes(mouseX,mouseY);
                    if (mouseX >= INTERVAL_MIN_X && mouseX <= INTERVAL_MAX_X && mouseY >= INTERVAL_MIN_Y && mouseY <= INTERVAL_MAX_Y)
                    {
                        return;
                    }
                    else if (mouseX >= INTERVAL_MIN_Xs && mouseX <= INTERVAL_MAX_Xs && mouseY >= INTERVAL_MIN_Ys && mouseY <= INTERVAL_MAX_Ys)
                    {
						                    	                
						SDL_Rect imageRect;
					    imageRect.x = 60;
					    imageRect.y = 70;
					    
					    zone = IMG_Load("confirmation.png");
					
					    // Save the first image
					    savedImage = SDL_CreateRGBSurface(0, screen->w, screen->h, 32, 0, 0, 0, 0);
					    SDL_BlitSurface(screen, NULL, savedImage, NULL);
					
					    // Display the second image
					    SDL_BlitSurface(zone, NULL, screen, &imageRect);
					    SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
					
					   // SDL_Delay(5000); // Delay for 5 seconds
				       
                    }
					else if(mouseX >= INTERVAL_MIN_Xv && mouseX <= INTERVAL_MAX_Xv && mouseY >= INTERVAL_MIN_Yv && mouseY <= INTERVAL_MAX_Yv){
                    		
							// Restore the first image
						    SDL_BlitSurface(savedImage, NULL, screen, NULL);
						    SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
						
						    SDL_FreeSurface(savedImage);
						    SDL_FreeSurface(zone);
							
							referenceInput = inputText1;
	                        bool produitExiste = false; 								
	                        produitExiste = true; 
	                        const char * reference = new char[50];
							reference = referenceInput.c_str();                              
							bool p = s.supprimerProduit(reference);
							if(p){
								inputText1 = inputText1.substr(0, inputText1.length() - 1);
				        		SDL_FillRect(screen, &textMessage, SDL_MapRGB(screen->format, 124,129,135));
	                            SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Produit Supprimé", textColor);
	                            SDL_BlitSurface(messageSurface, NULL, screen, &textMessage);
	                            SDL_FreeSurface(messageSurface);
	                      
							}
							else{
								SDL_Rect textMessage = { 100, 250, 300, 50 };
	                            SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Produit n'éxist pas ", textColor);
	                            SDL_BlitSurface(messageSurface, NULL, screen, &textMessage);
	                            SDL_FreeSurface(messageSurface);
							}
							
							
							
							
						}                     
                        inputText1 = "";
                }                
            }          
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                else if (event.key.keysym.sym == SDLK_TAB) {
                    activeField1 = !activeField1;
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (activeField1) {
                        activeField1 = false;
                    }
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					    if (activeField1 && !inputText1.empty()) {					    						       
					        inputText1 = inputText1.substr(0, inputText1.length() - 1);
					        SDL_FillRect(screen, &textField1, SDL_MapRGB(screen->format, 124,129,135));         
					    }
					}
                else {
                    if (activeField1) {
                        char inputChar = event.key.keysym.unicode;
                        if (isprint(inputChar) || inputChar == ' ') {
                            inputText1 += inputChar;
                        }
                    }
                }
            }
        }
        if (activeField1) {
            SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputText1.c_str(), textColor);
            SDL_BlitSurface(inputSurface, NULL, screen, &textField1);
            SDL_FreeSurface(inputSurface);
        }
        SDL_Flip(screen);
    }   
}
*/
void confirmationSupp(){
	const int INTERVAL_MIN_Xv = 168;
    const int INTERVAL_MAX_Xv = 309;
    const int INTERVAL_MIN_Yv = 241;
    const int INTERVAL_MAX_Yv = 283;
    
    const int INTERVAL_MIN_Xn = 308;
    const int INTERVAL_MAX_Xn = 452;
    const int INTERVAL_MIN_Yn = 242;
    const int INTERVAL_MAX_Yn = 284;
    
	SDL_Event event;
    bool quit = false;
	while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                }
        	}
        }
	}
}
int GoConfimer(string referenceInput){
	
	SDL_Color textColor = { 255, 0, 0 };
    SDL_Rect textField1 = { 90, 110, 200, 30 };
    SDL_Rect textMessage = { 125, 250, 300, 50 };
    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Champ 1 :", textColor);
    SDL_Event event;
    bool quit = false;
    std::string inputText1;
	SDL_Surface* background = NULL;
    background = IMG_Load("5.png");
	

	const int INTERVAL_MIN_Xv = 168;
    const int INTERVAL_MAX_Xv = 309;
    const int INTERVAL_MIN_Yv = 241;
    const int INTERVAL_MAX_Yv = 283;
    
    const int INTERVAL_MIN_Xn = 308;
    const int INTERVAL_MAX_Xn = 452;
    const int INTERVAL_MIN_Yn = 242;
    const int INTERVAL_MAX_Yn = 284;
	while(!quit){
		while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                	int mouseX = event.button.x;
                    int mouseY = event.button.y;
					if (mouseX >= INTERVAL_MIN_Xn && mouseX <= INTERVAL_MAX_Xn && mouseY >= INTERVAL_MIN_Yn && mouseY <= INTERVAL_MAX_Yn){
				                   	
					
				        return 0;
				        //coordonnes(mouseX,mouseY);
					}
				    else if (mouseX >= INTERVAL_MIN_Xv && mouseX <= INTERVAL_MAX_Xv && mouseY >= INTERVAL_MIN_Yv && mouseY <= INTERVAL_MAX_Yv) {
				        // Restore the saved image
				      
				       
				        bool produitExiste = false;
						
				        // Call your supprimerProduit() function with the appropriate implementation
				        produitExiste = true;
				        const char* reference = referenceInput.c_str();
				        bool p = s.supprimerProduit(reference);
				
				        
				        if (p) {
							    /*inputText1 = inputText1.substr(0, inputText1.length() - 1);
							    SDL_FillRect(screen, &textMessage, SDL_MapRGB(screen->format, 124, 129, 135));
							    SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Produit Supprimé", textColor);
							    SDL_BlitSurface(background, NULL, screen, NULL);  // Clear the screen by blitting the background
							    SDL_BlitSurface(messageSurface, NULL, screen, &textMessage);
							    SDL_FreeSurface(messageSurface);*/
							    inputText1 = "";
								return 1;
							}
							else {
							   /* SDL_Rect textMessage = { 100, 250, 300, 50 };
							    SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Produit n'existe pas", textColor);
							    SDL_BlitSurface(background, NULL, screen, NULL);  // Clear the screen by blitting the background
							    SDL_BlitSurface(messageSurface, NULL, screen, &textMessage);
							    SDL_FreeSurface(messageSurface);*/
							    inputText1 = "";
								return 2;
							}
							                        
							            
				        
				        
				        
	    			}
	    		}
			}
		}
	}
}
void Supprimer()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(600, 600, 32, SDL_SWSURFACE);
    TTF_Init();
    SDL_EnableUNICODE(1);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 50);
    SDL_Color textColor = { 255, 0, 0 };
    SDL_Rect textField1 = { 90, 110, 200, 30 };
    SDL_Rect textMessage = { 125, 250, 300, 50 };
    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Champ 1 :", textColor);
    SDL_Event event;
    bool quit = false;
    std::string inputText1;
    bool activeField1 = true;
    SDL_Surface* background = NULL;
    background = IMG_Load("5.png");
    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);

    const int INTERVAL_MIN_X = 182;
    const int INTERVAL_MAX_X = 395;
    const int INTERVAL_MIN_Y = 506;
    const int INTERVAL_MAX_Y = 567;

    const int INTERVAL_MIN_Xs = 358;
    const int INTERVAL_MAX_Xs = 560;
    const int INTERVAL_MIN_Ys = 106;
    const int INTERVAL_MAX_Ys = 178;

   
    std::string referenceInput;
    SDL_Surface* zone = NULL;
    SDL_Surface* savedImage = NULL;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
					
                    if (mouseX >= INTERVAL_MIN_X && mouseX <= INTERVAL_MAX_X && mouseY >= INTERVAL_MIN_Y && mouseY <= INTERVAL_MAX_Y) {
                        //coordonnes(mouseX,mouseY);
						return;
                    }
                    else if (mouseX >= INTERVAL_MIN_Xs && mouseX <= INTERVAL_MAX_Xs && mouseY >= INTERVAL_MIN_Ys && mouseY <= INTERVAL_MAX_Ys) {
                      // coordonnes(mouseX,mouseY);
					    SDL_Rect imageRect;
                        imageRect.x = 60;
                        imageRect.y = 70;
						
                        zone = IMG_Load("7.png");

                        // Save the current screen image
                        savedImage = SDL_CreateRGBSurface(0, screen->w, screen->h, 32, 0, 0, 0, 0);
                        SDL_BlitSurface(screen, NULL, savedImage, NULL);
                        
                        // Display the confirmation image
                        SDL_BlitSurface(zone, NULL, screen, &imageRect);
                        SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
                        
                        
						int action = GoConfimer(inputText1);
						
						// Restore the saved image
				        SDL_BlitSurface(savedImage, NULL, screen, NULL);
				        SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
				
				        SDL_FreeSurface(savedImage);
				        SDL_FreeSurface(zone);
                        if(action == 1 ){
							inputText1 = inputText1.substr(0, inputText1.length() - 1);
							    SDL_FillRect(screen, &textMessage, SDL_MapRGB(screen->format, 124, 129, 135));
							    SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Produit Supprimé", textColor);
							    SDL_BlitSurface(background, NULL, screen, NULL);  // Clear the screen by blitting the background
							    SDL_BlitSurface(messageSurface, NULL, screen, &textMessage);
							    SDL_FreeSurface(messageSurface);
						}else if(action == 2){
							    SDL_Rect textMessage = { 100, 250, 300, 50 };
							    SDL_Surface* messageSurface = TTF_RenderText_Solid(font, "Produit n'existe pas", textColor);
							    SDL_BlitSurface(background, NULL, screen, NULL);  // Clear the screen by blitting the background
							    SDL_BlitSurface(messageSurface, NULL, screen, &textMessage);
							    SDL_FreeSurface(messageSurface);
						}
                    }
                    
                    
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                else if (event.key.keysym.sym == SDLK_TAB) {
                    activeField1 = !activeField1;
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (activeField1) {
                        activeField1 = false;
                    }
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (activeField1 && !inputText1.empty()) {
                        inputText1 = inputText1.substr(0, inputText1.length() - 1);
                        SDL_FillRect(screen, &textField1, SDL_MapRGB(screen->format, 124, 129, 135));
                    }
                }
                else {
                    if (activeField1) {
                        char inputChar = event.key.keysym.unicode;
                        if (isprint(inputChar) || inputChar == ' ') {
                            inputText1 += inputChar;
                        }
                    }
                }
            }
        }
        if (activeField1) {
            SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputText1.c_str(), textColor);
            SDL_BlitSurface(inputSurface, NULL, screen, &textField1);
            SDL_FreeSurface(inputSurface);
        }
        SDL_Flip(screen);
    }
}
//afficher produit :
void afficherproduit(SDL_Surface* screen)
{
	int x=0;
    for(int i=0;i<s.getNBrProduit();i++)
    {
    Produit produit(s.getProduit(i));
	char * reference = produit.getReference();
	char * designation = produit.getDesignation();
	int quantite = produit.getQuantite();
	float prix = produit.getPrix();
	char * price=new char [50];
	char * quantity=new char [50];
	sprintf(price, "%0.2f", prix);
	sprintf(quantity, "%d", quantite);
    SDL_Rect textreference = { 70, 94+x, 200, 30 };
	SDL_Rect textdesignation = { 195,  94+x, 200, 30 };
	SDL_Rect textprix = { 320, 94+x, 200, 30 };
	SDL_Rect textquantite = { 445, 94+x, 200, 30 };
	TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
	SDL_Color textColor = { 255, 0, 0 };
	SDL_Surface* inputSurface1 = TTF_RenderText_Solid(font, reference, textColor);
	SDL_BlitSurface(inputSurface1, NULL, screen, &textreference);
	SDL_FreeSurface(inputSurface1);
	SDL_Surface* inputSurface2 = TTF_RenderText_Solid(font, designation, textColor);
	SDL_BlitSurface(inputSurface2, NULL, screen, &textdesignation);
	SDL_FreeSurface(inputSurface2);
	SDL_Surface* inputSurface3 = TTF_RenderText_Solid(font, price, textColor);
	SDL_BlitSurface(inputSurface3, NULL, screen, &textprix);
	SDL_FreeSurface(inputSurface3);
	SDL_Surface* inputSurface4 = TTF_RenderText_Solid(font, quantity, textColor);
	SDL_BlitSurface(inputSurface4, NULL, screen, &textquantite);
	SDL_FreeSurface(inputSurface4);
	x=x+42;
	}
}

//Afficher :
void Afficher()
{
	const int INTERVAL_MINS_X=38;
	const int INTERVAL_MAXS_X=209;
	const int INTERVAL_MINS_Y=545;
	const int INTERVAL_MAXS_Y=597;
	
	
	const int INTERVAL_MINL_X=355;
	const int INTERVAL_MAXL_X=548;
	const int INTERVAL_MINL_Y=532;
	const int INTERVAL_MAXL_Y=577;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen = SDL_SetVideoMode(600, 600, 32, SDL_SWSURFACE);
	TTF_Init();
	SDL_EnableUNICODE(1);
	loadFont("arial.ttf");
	SDL_Color textColor = { 255, 0, 0 };
	
	SDL_Surface * texteSurface = renderText("Bonjour, SDL 1.2",255,255,255);
	SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Champ 1 :", textColor);
    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "Champ 2 :", textColor);
    SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, "Champ 3 :", textColor);
    SDL_Surface* textSurface4 = TTF_RenderText_Solid(font, "Champ 4 :", textColor);
	bool isRunning = true;
    SDL_Event event;
	SDL_Surface * background = NULL;
	background = IMG_Load("6.jpg");
	SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);
    while(isRunning)
    {
    	while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
            	if (event.button.button == SDL_BUTTON_LEFT)
            	{
            		int mouseX = event.button.x;
                    int mouseY = event.button.y;
					//coordonnes(mouseX,mouseY);
					if (mouseX >= INTERVAL_MINAQ_X && mouseX <= INTERVAL_MAXAQ_X && mouseY >= INTERVAL_MINAQ_Y && mouseY <= INTERVAL_MAXAQ_Y)
                    {
					    return;
						if (returnToPreviousFrame)
                        {
                            returnToPreviousFrame = false;  
                            SDL_BlitSurface(previousFrame, &previousFrameRect, screen, NULL);
                            SDL_Flip(screen);
                        }
                    }
					else if(mouseX >= INTERVAL_MINS_X && mouseX <= INTERVAL_MAXS_X && mouseY >= INTERVAL_MINS_Y && mouseY <= INTERVAL_MAXS_Y){
                    	//coordonnes(mouseX,mouseY);
                    	s.sauvegarderStockCSV();
					}
					
					else if(mouseX >= INTERVAL_MINL_X && mouseX <= INTERVAL_MAXL_X && mouseY >= INTERVAL_MINL_Y && mouseY <= INTERVAL_MAXL_Y){
						s.importFromCSV();
					}
				}
			}
        }
        SDL_BlitSurface(background, NULL, screen, NULL);
        afficherproduit(screen);
        SDL_Flip(screen);
	}
	 SDL_FreeSurface(textSurface1);
    SDL_FreeSurface(textSurface2);
    SDL_FreeSurface(textSurface3);
    SDL_FreeSurface(textSurface4);
    TTF_CloseFont(font);
    TTF_Quit();
	SDL_Quit();
	cleanup();
}

void Open()
{
    loadFont("arial.ttf");
    SDL_Surface* texteSurface = renderText("Bonjour, SDL 1.2", 255, 255, 255);
    bool isRunning = true;
    SDL_Event event;
    SDL_Surface* background = NULL;
    background = IMG_Load("2.png");
    SDL_Surface* button = NULL;
    button = IMG_Load("2.png");

    // Define the position and dimensions of the buttons
    SDL_Rect addButtonRect;
    addButtonRect.x = INTERVAL_MINAP_X;
    addButtonRect.y = INTERVAL_MINAP_Y;
    addButtonRect.w = INTERVAL_MAXAP_X - INTERVAL_MINAP_X;
    addButtonRect.h = INTERVAL_MAXAP_Y - INTERVAL_MINAP_Y;

    SDL_Rect searchButtonRect;
    searchButtonRect.x = INTERVAL_MINR_X;
    searchButtonRect.y = INTERVAL_MINR_Y;
    searchButtonRect.w = INTERVAL_MAXR_X - INTERVAL_MINR_X;
    searchButtonRect.h = INTERVAL_MAXR_Y - INTERVAL_MINR_Y;

    SDL_Rect deleteButtonRect;
    deleteButtonRect.x = INTERVAL_MINS_X;
    deleteButtonRect.y = INTERVAL_MINS_Y;
    deleteButtonRect.w = INTERVAL_MAXS_X - INTERVAL_MINS_X;
    deleteButtonRect.h = INTERVAL_MAXS_Y - INTERVAL_MINS_Y;


    SDL_Rect afficherButtonRect;
     afficherButtonRect.x = INTERVAL_MINA_X;
     afficherButtonRect.y = INTERVAL_MINA_Y;
     afficherButtonRect.w = INTERVAL_MAXA_X - INTERVAL_MINA_X;
     afficherButtonRect.h = INTERVAL_MAXA_Y - INTERVAL_MINA_Y;
    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);

    bool isAddButtonHovered = false;
    bool isSearchButtonHovered = false;
     bool isdeleteButtonHovered = false;
      bool isafficherButtonHovered = false;
      
    // Create yellow overlay surfaces for each button
    SDL_Surface* addButtonYellowOverlay = SDL_CreateRGBSurface(SDL_SWSURFACE, addButtonRect.w, addButtonRect.h, 32, 0, 0, 0, 0);
    Uint32 addButtonYellowColor = SDL_MapRGBA(addButtonYellowOverlay->format, 255, 255, 0, 128);
    SDL_FillRect(addButtonYellowOverlay, NULL, addButtonYellowColor);

    SDL_Surface* searchButtonYellowOverlay = SDL_CreateRGBSurface(SDL_SWSURFACE, searchButtonRect.w, searchButtonRect.h, 32, 0, 0, 0, 0);
    Uint32 searchButtonYellowColor = SDL_MapRGBA(searchButtonYellowOverlay->format, 255, 255, 0, 128);
    SDL_FillRect(searchButtonYellowOverlay, NULL, searchButtonYellowColor);
    
    
      SDL_Surface* deleteButtonYellowOverlay = SDL_CreateRGBSurface(SDL_SWSURFACE, deleteButtonRect.w, deleteButtonRect.h, 32, 0, 0, 0, 0);
    Uint32 deleteButtonYellowColor = SDL_MapRGBA(deleteButtonYellowOverlay->format, 255, 255, 0, 128);
    SDL_FillRect(deleteButtonYellowOverlay, NULL, deleteButtonYellowColor);
    
    
    SDL_Surface* afficherButtonYellowOverlay = SDL_CreateRGBSurface(SDL_SWSURFACE, afficherButtonRect.w, afficherButtonRect.h, 32, 0, 0, 0, 0);
    Uint32 afficherButtonYellowColor = SDL_MapRGBA(afficherButtonYellowOverlay->format, 255, 255, 0, 128);
    SDL_FillRect(afficherButtonYellowOverlay, NULL, afficherButtonYellowColor);

  

    // Create copies of the original background surface
    SDL_Surface* originalBackground = SDL_ConvertSurface(background, background->format, background->flags);

    // Load the font for rendering text
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24); // Adjust the font size as needed

    SDL_Color textColor = { 255, 0, 0 }; // Set the color of the text
    
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
bool isAudioPlaying = false;
bool isAudioPlaying2 = false;
bool isAudioPlaying3 = false;
bool isAudioPlaying4 = false;
Mix_Chunk* soundEffect =NULL;
// Chargement du fichier audio (hors de la boucle principale)
soundEffect = Mix_LoadWAV("1.wav");
if (soundEffect == NULL) {
    printf("Erreur lors du chargement du fichier audio : %s\n", Mix_GetError());
    // Gérer l'erreur de chargement du fichier audio
    // ...
}
    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    // Ajouter produit:
                    if (mouseX >= addButtonRect.x && mouseX <= addButtonRect.x + addButtonRect.w &&
                        mouseY >= addButtonRect.y && mouseY <= addButtonRect.y + addButtonRect.h)
                    {
                        AjouterP();
                        if (returnToPreviousFrame)
                        {
                            returnToPreviousFrame = false;
                            SDL_BlitSurface(previousFrame, &previousFrameRect, screen, NULL);
                            SDL_Flip(screen);
                        }
                    }

                    // Recherche produit:
                    if (mouseX >= searchButtonRect.x && mouseX <= searchButtonRect.x + searchButtonRect.w && mouseY >= searchButtonRect.y && mouseY <= searchButtonRect.y + searchButtonRect.h)
						{
						Recherche();
						if (returnToPreviousFrame)
						{
						returnToPreviousFrame = false;
						SDL_BlitSurface(previousFrame, &previousFrameRect, screen, NULL);
						SDL_Flip(screen);
						}
						}                // Supprimer produit:
                if (mouseX >= INTERVAL_MINS_X && mouseX <= INTERVAL_MAXS_X && mouseY >= INTERVAL_MINS_Y && mouseY <= INTERVAL_MAXS_Y)
                {
                    Supprimer();
                    if (returnToPreviousFrame)
                    {
                        returnToPreviousFrame = false;
                        SDL_BlitSurface(previousFrame, &previousFrameRect, screen, NULL);
                        SDL_Flip(screen);
                    }
                }

                // Afficher:
                if (mouseX >= INTERVAL_MINA_X && mouseX <= INTERVAL_MAXA_X && mouseY >= INTERVAL_MINA_Y && mouseY <= INTERVAL_MAXA_Y)
                {
                    Afficher();
                    if (returnToPreviousFrame)
                    {
                        returnToPreviousFrame = false;
                        SDL_BlitSurface(previousFrame, &previousFrameRect, screen, NULL);
                        SDL_Flip(screen);
                    }
                }
            }
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
        	
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

             // Check if the mouse is over the "Ajouter" button
            if (mouseX >= addButtonRect.x && mouseX <= addButtonRect.x + addButtonRect.w &&
                mouseY >= addButtonRect.y && mouseY <= addButtonRect.y + addButtonRect.h)
            {
                if (!isAddButtonHovered)
                {
                    // Blit the yellow overlay onto the screen
                    SDL_BlitSurface(addButtonYellowOverlay, NULL, screen, &addButtonRect);

                    // Render the text surface
                    texteSurface = TTF_RenderText_Solid(font, "Ajouter un produit", textColor);

                    // Set the hover state to true
                    isAddButtonHovered = true;

                    // Play audio only if it's not already playing
                    if (!isAudioPlaying)
                    {
                        isAudioPlaying = true;
                        Mix_PlayChannel(-1, soundEffect, 0);
                    }
                }
            }
            else
            {
                if (isAddButtonHovered)
                {
                    // Blit the original background onto the screen
                    SDL_BlitSurface(originalBackground, NULL, screen, NULL);

                    // Reset the hover state to false
                    isAddButtonHovered = false;
                }

                // Stop audio playback
                if (isAudioPlaying)
                {
                    Mix_HaltChannel(-1);
                    isAudioPlaying = false;
                }
            }

            // Check if the mouse is over the "Recherche" button
            if (mouseX >= searchButtonRect.x && mouseX <= searchButtonRect.x + searchButtonRect.w &&
                mouseY >= searchButtonRect.y && mouseY <= searchButtonRect.y + searchButtonRect.h)
            {
                if (!isSearchButtonHovered)
                {
                    // Blit the yellow overlay onto the screen
                    SDL_BlitSurface(searchButtonYellowOverlay, NULL, screen, &searchButtonRect);

                    // Render the text surface
                    texteSurface = TTF_RenderText_Solid(font, "Recherche", textColor);

                    // Set the hover state to true
                    isSearchButtonHovered = true;
                    // Play audio only if it's not already playing
                    if (!isAudioPlaying2)
                    {
                        isAudioPlaying2 = true;
                        Mix_PlayChannel(-1, soundEffect, 0);
                    }
                }
            }
            else
            {
                if (isSearchButtonHovered)
                {
                    // Blit the original background onto the screen
                    SDL_BlitSurface(originalBackground, NULL, screen, NULL);

                    // Reset the hover state to false
                    isSearchButtonHovered = false;
                }
                // Stop audio playback
                if (isAudioPlaying2)
                {
                    Mix_HaltChannel(-1);
                    isAudioPlaying2 = false;
                }
            }
            
            
               // Check if the mouse is over the "supprmer" button
            if (mouseX >= deleteButtonRect.x && mouseX <= deleteButtonRect.x + deleteButtonRect.w &&
                mouseY >= deleteButtonRect.y && mouseY <= deleteButtonRect.y + deleteButtonRect.h)
            {
                if (!isdeleteButtonHovered)
                {
                    // Blit the yellow overlay onto the screen
                    SDL_BlitSurface(deleteButtonYellowOverlay, NULL, screen, &deleteButtonRect);

                    // Render the text surface
                    texteSurface = TTF_RenderText_Solid(font, "Supprimer un produit", textColor);

                    // Set the hover state to true
                    isdeleteButtonHovered = true;
                    // Play audio only if it's not already playing
                    if (!isAudioPlaying3)
                    {
                        isAudioPlaying3 = true;
                        Mix_PlayChannel(-1, soundEffect, 0);
                    }
                }
            }
            else
            {
                if (isdeleteButtonHovered)
                {
                    // Blit the original background onto the screen
                    SDL_BlitSurface(originalBackground, NULL, screen, NULL);

                    // Reset the hover state to false
                    isdeleteButtonHovered = false;
                }
                // Stop audio playback
                if (isAudioPlaying3)
                {
                    Mix_HaltChannel(-1);
                    isAudioPlaying3 = false;
                }
            }
            
            
            //afficher hover
        if (mouseX >= afficherButtonRect.x && mouseX <= afficherButtonRect.x + afficherButtonRect.w &&
                mouseY >= afficherButtonRect.y && mouseY <= afficherButtonRect.y + afficherButtonRect.h)
            {
                if (!isafficherButtonHovered)
                {
                    // Blit the yellow overlay onto the screen
                    SDL_BlitSurface(afficherButtonYellowOverlay, NULL, screen, &afficherButtonRect);

                    // Render the text surface
                    texteSurface = TTF_RenderText_Solid(font, "Afficher", textColor);

                    // Set the hover state to true
                    isafficherButtonHovered = true;
                }
                // Play audio only if it's not already playing
                    if (!isAudioPlaying4)
                    {
                        isAudioPlaying4 = true;
                        Mix_PlayChannel(-1, soundEffect, 0);
                    }
            }
            else
            {
                if (isafficherButtonHovered)
                {
                    // Blit the original background onto the screen
                    SDL_BlitSurface(originalBackground, NULL, screen, NULL);

                    // Reset the hover state to false
                    isafficherButtonHovered = false;
                }
                // Stop audio playback
                if (isAudioPlaying4)
                {
                    Mix_HaltChannel(-1);
                    isAudioPlaying4 = false;
                }
            }
            
        

            // Update the screen
            SDL_BlitSurface(background, NULL, screen, NULL); // Blit the background
            if (isAddButtonHovered)
            {
                SDL_BlitSurface(addButtonYellowOverlay, NULL, screen, &addButtonRect); // Blit the yellow overlay
                SDL_Rect textRect;
					textRect.x = addButtonRect.x + (addButtonRect.w - texteSurface->w) / 2; // Center the text horizontally
					textRect.y = addButtonRect.y + (addButtonRect.h - texteSurface->h) / 2; // Center the text vertically
					 SDL_BlitSurface(texteSurface, NULL, screen, &textRect); // Blit the text surface
            }

            if (isSearchButtonHovered)
            {
                SDL_BlitSurface(searchButtonYellowOverlay, NULL, screen, &searchButtonRect); // Blit the yellow overlay
                SDL_Rect textRect;
                textRect.x = searchButtonRect.x + (searchButtonRect.w - texteSurface->w) / 2; // Center the text horizontally
                textRect.y = searchButtonRect.y + (searchButtonRect.h - texteSurface->h) / 2; // Center the text vertically

                SDL_BlitSurface(texteSurface, NULL, screen, &textRect); // Blit the text surface
            }
              if (isdeleteButtonHovered)
            {
                SDL_BlitSurface(deleteButtonYellowOverlay, NULL, screen, &deleteButtonRect); // Blit the yellow overlay
                SDL_Rect textRect;
                textRect.x = deleteButtonRect.x + (deleteButtonRect.w - texteSurface->w) / 2; // Center the text horizontally
                textRect.y = deleteButtonRect.y + (deleteButtonRect.h - texteSurface->h) / 2; // Center the text vertically

                SDL_BlitSurface(texteSurface, NULL, screen, &textRect); // Blit the text surface
            }


       if (isafficherButtonHovered)
            {
                SDL_BlitSurface(afficherButtonYellowOverlay, NULL, screen, &afficherButtonRect); // Blit the yellow overlay
                SDL_Rect textRect;
                textRect.x = afficherButtonRect.x + (afficherButtonRect.w - texteSurface->w) / 2; // Center the text horizontally
                textRect.y = afficherButtonRect.y + (afficherButtonRect.h - texteSurface->h) / 2; // Center the text vertically

                SDL_BlitSurface(texteSurface, NULL, screen, &textRect); // Blit the text surface
            }


            SDL_Flip(screen);
        }
    }
}

		SDL_FreeSurface(background);
		SDL_FreeSurface(button);
		SDL_FreeSurface(previousFrame);
		SDL_FreeSurface(addButtonYellowOverlay); // Free the yellow overlay surface for "Ajouter" button
		SDL_FreeSurface(searchButtonYellowOverlay); // Free the yellow overlay surface for "Recherche" button
		SDL_FreeSurface(originalBackground); // Free the original background surface
		SDL_FreeSurface(texteSurface); // Free the text surface
		TTF_CloseFont(font); // Close the font
		SDL_ShowCursor(SDL_ENABLE);
		SDL_Quit();
		cleanup();
		
}



void InterfaceP()
{
    initializeSDL();
    loadFont("arial.ttf");
    SDL_Surface* texteSurface = renderText("Bonjour, SDL 1.2", 255, 255, 255);
    bool isRunning = true;
    SDL_Event event;
    SDL_Surface* background = NULL;
    background = IMG_Load("1.png");
    SDL_Surface* pinkButton = NULL;
    pinkButton = IMG_Load("5.png");

    SDL_Rect buttonRect;  // Define the position and dimensions of the button
    buttonRect.x = INTERVAL_MINO_X;
    buttonRect.y = INTERVAL_MINO_Y;
    buttonRect.w = INTERVAL_MAXO_X - INTERVAL_MINO_X;
    buttonRect.h = INTERVAL_MAXO_Y - INTERVAL_MINO_Y;

    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w && mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h)
                    {
                        Open();
                        if (returnToPreviousFrame)
                        {
                            returnToPreviousFrame = false;
                            SDL_BlitSurface(previousFrame, &previousFrameRect, screen, NULL);
                        }
                    }
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;

                if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w && mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h)
                {
                    // Create a semi-transparent pink overlay surface
                    SDL_Surface* pinkOverlay = SDL_CreateRGBSurface(SDL_SWSURFACE, buttonRect.w, buttonRect.h, 32, 0, 0, 0, 0);
                    Uint32 pinkColor = SDL_MapRGBA(pinkOverlay->format, 255, 192, 203, 128);
                    SDL_FillRect(pinkOverlay, NULL, pinkColor);

                    // Blend the pink overlay with the pink button region
                    SDL_BlitSurface(pinkOverlay, NULL, pinkButton, &buttonRect);
                    SDL_SetAlpha(pinkButton, SDL_SRCALPHA, 128);

                    // Blit the modified pink button region onto the screen
                    SDL_BlitSurface(pinkButton, &buttonRect, screen, &buttonRect);

                    // Render and display the button text "Open" at the center of the button region
                    SDL_Surface* buttonTextSurface = renderText("Open", 255, 255, 255);
                    int textX = buttonRect.x + (buttonRect.w - buttonTextSurface->w) / 2;
                    int textY = buttonRect.y + (buttonRect.h - buttonTextSurface->h) / 2;
                    SDL_Rect textRect = { textX, textY, buttonTextSurface->w, buttonTextSurface->h };
                    SDL_BlitSurface(buttonTextSurface, NULL, screen, &textRect);
                    SDL_FreeSurface(buttonTextSurface);

                    // Free the pink overlay surface
                    SDL_FreeSurface(pinkOverlay);
                }
               
               
  				else
                {
                    SDL_BlitSurface(background, NULL, screen, NULL);
                }
            }
        }

        SDL_Flip(screen);
    }

    SDL_FreeSurface(background);
    SDL_FreeSurface(pinkButton);
    SDL_FreeSurface(previousFrame);
    SDL_ShowCursor(SDL_ENABLE);
    SDL_Quit();
    cleanup();
}


int main(int argc, char** argv) 
{
	//saisirProduit();
	//saisirStock();
	Produit p1("r1","d1",20,30);
	Produit p2("r2","d2",50,90);
	Produit p3("r3","d3",70,180);
	s.ajouterProduit(p1);
	s.ajouterProduit(p2);
	s.ajouterProduit(p3);
	InterfaceP();
	

	return 0;
}
