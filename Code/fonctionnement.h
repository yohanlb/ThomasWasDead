#include "obstacles_persos.h"
#include "textures_son.h"


/***FONCTIONS SDL***/
void reshape(int winWidth, int winHeight, float echelle);
void setVideoMode(int winWidth, int winHeight);


/****FONCTIONS DE SAUVEGARDE ET DE REINITIALISATION DE LA PROGRESSION***/
int LireProgression();
void  AugmenterProgression(char nom_niveau[]);
void RestartProgression();


/*****FONCTIONS DE TRI*****/
void EchangeObstacle(Obstacle * obstacles[], Personnage * persos[] ,int a, int b);
void triBulle(Obstacle * obstacles[],Personnage * persos[]);

/****FONCTIONS DE MISE EN PAUSE ET CHANGEMENT DE NIVEAU*****/
void mettreEnPause(int pause, Obstacle * o, GLuint textures[]);
int objectifAtteint(Personnage *persos[], Obstacle *obstacles[]);
void dessinePortes(Obstacle *obstacles[], GLuint textures[], int*pause);
void ChangerNiveau(char nom_a_lancer[],char nom_niveau[],Personnage *persos[], Obstacle *obstacles[], int caractObsQuiBougent[], GLuint textures[],Mix_Music *musique, int * pause, float *centerX, float *centerY, int * persoSelected);
void lancerNiveauDepuisPorte(char nom_niveau[],Personnage *persos[], Obstacle *obstacles[], int caractObsQuiBougent[], GLuint textures[],Mix_Music *musique, Mix_Chunk * bruit[], int*pause, float *centerX, float *centerY, int *persoSelected);


/*****FONCTIONS DE DESSIN*****/
void dessinRepere();
void drawObstacle(Obstacle *o);
void drawObstacleTexture(GLuint texture ,Obstacle *o, char nom_niveau[], int nbLoop);
void drawCurseurSelection(Personnage *p, int nbLoop);
void drawMiniaturePersos(Personnage *persos[], int persoSelected, int nbLoop, float centerX, float centerY, int winWidth, int winHeight);


/****FONCTION VIDÉO*******/
void launchVideoIntro(Mix_Music *musique, char nom_fichier[]);  