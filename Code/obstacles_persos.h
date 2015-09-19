#include <SDL/SDL.h>
#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>


# define NB_PERSONNAGES 3
# define NB_OBSTACLES (NB_PERSONNAGES*2 + 60)

# define NB_NIVEAUX 4


static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


/***STRUCTURES UTILISEES***/

typedef struct Obstacle Obstacle;
struct Obstacle
{
  int id;
  int type; /* 0 = objet traversable, 1 = obstacle, 2 = personnage, 3 = objectif, 4 = ennemis, 5 porte vers un autre niveau*/
  int texture; /*num de la texture que l'on veut appliquer : 1==>1.jpg ou 0 si on n'en met pas*/
  float x, y, z;
  int width, height;
};

typedef struct Personnage Personnage;
struct Personnage
{
  Obstacle * obs;
  float vx, vy;
  float xfin, yfin;
  int jumpPower;
  int touchGround, touchTop, touchLeft, touchRight;
  int jumpDuration;
  float groundDistance;
};


/*****FONCTIONS DE CREATIONS ET DE LIBERATION DE STRUCTURE*******/
Personnage * createPersonnages(int id, float x, float y, float xfin, float yfin, int width, int height, int texture, int jumpPower);
Obstacle * createObstacle(int id, int type, int texture, float x, float y, float z, int width, int height);
int initializePersoFromFile(Personnage *p_tab[], Obstacle * obstacles[], char nom_niveau[]);
int initializeObstaclesFromFile(Obstacle *obstacles[], char nom_niveau[] ,int caractObsQuiBougent[]);
void  freeObstacles(Obstacle *obstacles[]);
void  freePersos(Personnage *persos[]);

/****FONCTIONS DE COLLISIONS***/
int detectionCollisionPersoObs(Personnage *p, Obstacle *obs);
void CollisionsPersonnage(Personnage *p, Obstacle *obs[]);



/*****FONCTIONS LIEES AUX DEPLACEMENT DES PERSONNAGES*****/
void updateVitesses(Personnage *p,char nom_niveau[]);
void movePersonnage(Personnage *p,int * jumpbool);
int jump(Personnage *p);
void switchPerso(int *persoSelected, Personnage * persos[]);

/*****FONCTIONS DE CAMERA*******/
void moveCameraSuivrePerso(Personnage *p,int windowWidth,int windowHeight, float* centerX, float* centerY, Obstacle *obstacles[]);
void zoomOut( int keyZoomOut, float *echelle, char nom_niveau[]);

/****FONCTIONS POUR DEPLACER UN OBSTACLE ET LES PERSONNAGE QU'IL PEUT POUSSER****/
void persoPoussePerso(int collisions,Personnage * p, Personnage * persoDejaTouche, Personnage * persos[],Obstacle * o);
int deplaceObstacle(Obstacle * o,float xFin, float yFin, Personnage * persos[], Obstacle * obstacles[]);
void deplaceObstacleContinue(Obstacle * o, Personnage * persos[], Obstacle * obstacles[], int caractObsQuiBougent[]);
