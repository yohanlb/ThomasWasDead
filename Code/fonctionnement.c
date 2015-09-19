#include "fonctionnement.h"


/***FONCTIONS SDL***/

/*
Permet de redimensionner la fenetre avec le "width", "height" et "echelle" donnée en parametre.
*/
void reshape(int winWidth, int winHeight, float echelle) {
  glViewport(0, 0, winWidth, winHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float ratio = (winWidth/(float)winHeight);
  if (echelle == -1)
  {
    echelle = 150.;
  }
  gluOrtho2D(-echelle*ratio, echelle*ratio, -echelle, echelle);
}

/*
Définis les parametres de la fenetre.
*/
void setVideoMode(int winWidth, int winHeight) {
  if(NULL == SDL_SetVideoMode(winWidth, winHeight, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  reshape(winWidth,winHeight,-1);
}



/****FONCTIONS DE SAUVEGARDE ET DE REINITIALISATION DE LA PROGRESSION***/


/*
Cette fonction retourne l'entier qui de trouve dans le fichier "0/Progression.txt". 
Cet entier se trouve entre un caractère '=' et ';'
*/
int LireProgression(){
    char * nom_fichier="0/Progression.txt";

    FILE* fichier = NULL;
    int caractereActuel = 0;
    int test = 0;
    int progression=-1;

    fichier = fopen(nom_fichier, "r");

    if (fichier != NULL)
    { 
          do
        { 
          caractereActuel = fgetc(fichier);
          if(caractereActuel=='=') test = 1;
          
          if (test==1) 
          {

          fscanf(fichier, "%d", &progression);

          }
 

        } while (caractereActuel != EOF);
        fclose(fichier);
    }

    return progression;
} 

/*
Cette fonction incrémente l'entier du fichier "0/Progression.txt" 
Si le niveau dans lequel se trouve le joueur est égal à la progression celle -ci est incrémenté.
Sinon elle est inchangée. 
*/
void AugmenterProgression(char nom_niveau[]){
    int a,b;
    a=atoi(nom_niveau);
    b=LireProgression();


    if(a==b){
      a++;
      FILE* fichier = NULL;
      char num[10];
      char message[100]="Nom du niveau à lancer \n=";
      char * nom_fichier="0/Progression.txt";
      sprintf(num, "%d;", a);
      strcat(message, num);

      fichier = fopen(nom_fichier, "w");
      if (fichier != NULL)
      { 
          fputs(message, fichier);
          fclose(fichier);
      }
    }

}

/*
Cette fonction réinitialis la progression.
Elle met '=1;' dans le fichier "0/Progression.txt"
*/
void RestartProgression(){
  
    FILE* fichier = NULL;

    fichier = fopen("0/Progression.txt", "w");
    if (fichier != NULL)
    { 
        fputs("Nom du niveau à lancer \n=1;", fichier);
        fclose(fichier);
    }
}



/*****FONCTIONS DE TRI*****/

/* 
Echange le contenu de obstacles[a] et obstacles[b] 
*/
void EchangeObstacle(Obstacle * obstacles[], Personnage * persos[] ,int a, int b)
{ 
    Obstacle * c=NULL;
    if(obstacles[a] && obstacles[b] && (obstacles[b]->type!=obstacles[a]->type || obstacles[b]->id!=obstacles[a]->id) ){

      int i;
      for (i = 0; i < NB_PERSONNAGES; ++i)//Si un obstacle à echanger est l'obstacle d'un personnage, il faut changer le pointeur
        {
          if(persos[i] && persos[i]->obs->id==obstacles[a]->id && obstacles[a]->type==2){
            persos[i]->obs=obstacles[b];
          }
          else if(persos[i] && persos[i]->obs->id==obstacles[b]->id && obstacles[b]->type==2){
            persos[i]->obs=obstacles[a];
          }
        }
      //c = obstacles[a];
      c=createObstacle(obstacles[a]->id, obstacles[a]->type, obstacles[a]->texture, obstacles[a]->x,obstacles[a]->y, obstacles[a]->z, obstacles[a]->width, obstacles[a]->height);

      //obstacles[a] = obstacles[b];
      obstacles[a]->id=obstacles[b]->id; 
      obstacles[a]->type=obstacles[b]->type; 
      obstacles[a]->texture=obstacles[b]->texture;
      obstacles[a]->x=obstacles[b]->x;
      obstacles[a]->y=obstacles[b]->y;
      obstacles[a]->z=obstacles[b]->z;
      obstacles[a]->width=obstacles[b]->width;
      obstacles[a]->height=obstacles[b]->height;

      //obstacles[b] = c;
      obstacles[b]->id=c->id; 
      obstacles[b]->type=c->type; 
      obstacles[b]->texture=c->texture;
      obstacles[b]->x=c->x;
      obstacles[b]->y=c->y;
      obstacles[b]->z=c->z;
      obstacles[b]->width=c->width;
      obstacles[b]->height=c->height;

      if(c){
        free(c);
        c=NULL;
      }
    }

}

/*
Tri à bullle d'un tableau d'obstacles en fonction de leur z
*/
void triBulle(Obstacle * obstacles[],Personnage * persos[])
{
    int j = 0;
    int b = 1;

    while (b == 1)
    {
        b = 0;
        for (j = 0; j < NB_OBSTACLES - 1; j++)
        {
            if (obstacles[j] && obstacles[j+1] && (obstacles[j]->z > obstacles[j+1]->z))
            {
                EchangeObstacle(obstacles,persos,j , j+1);
                b = 1;
            }
        }
    }
}



/****FONCTIONS DE MISE EN PAUSE ET CHANGEMENT DE NIVEAU*****/

/*
Cette fonction dessine un obstacle avec une texture qui dépend de l'entier pause.
*/
void mettreEnPause(int pause, Obstacle * o, GLuint textures[]){
  if (o){
    if(textures[pause]){
      drawObstacleTexture(textures[pause],o,"0/",0);
    }
    else{
      drawObstacle(o);
    }
  }
}

/*
Fonction qui retourne :
     1 si tous les personnages sont sur leur objectif.
     0 si les objectifs ne sont pas atteints. 
     -1 si on est game over (collision avec un ennemi).
*/
int objectifAtteint(Personnage *persos[], Obstacle *obstacles[]){
  int i,j;
  int a;
  int obj=0;
  for ( i = 0; i < NB_PERSONNAGES; ++i)
  {
    for (j = 0; j < NB_OBSTACLES; ++j)
    {   
      if (persos[i] && obstacles[j])
      { 

        if((persos[i]->obs->id==obstacles[j]->id)&&(persos[i]->obs->type!=obstacles[j]->type)){
            a=detectionCollisionPersoObs(persos[i],obstacles[j]);
            if (a!=0)
            {
              obj++;
            }
        }
        else if(obstacles[j]->type==4){
          a=detectionCollisionPersoObs(persos[i],obstacles[j]);
          if (a!=0)
          {
             obj=-1;
          }
        }
      }
    }
  }
  if(obj==1){
    obj=0;
  }
  else if (obj==-1){

  }
  else{
    obj = obj%2;
  }
  return obj;
}

/*
Cette fonction dessine les textures sur les portes du niveau 0 en fonction de la progression.
*/
void dessinePortes(Obstacle *obstacles[], GLuint textures[], int*pause){
   *pause=0;
    int progression;
    progression=LireProgression();
    int i;
    for ( i = 0; i < NB_OBSTACLES; ++i)
    {
      if(obstacles[i] && obstacles[i]->type==5 && obstacles[i]->id > 100){
        int niveau;
        niveau = obstacles[i]->id % 100;
        if(niveau <= progression){
          obstacles[i]->texture = 3;
        }
        else{
          obstacles[i]->texture = 2;
        }
      }
    }
}

/*
Fonction qui libère l'espace mémoire allouée pour un niveau.
Puis elle alloue l'espace mémoire pour le nouveau niveau
*/
void ChangerNiveau(char nom_a_lancer[],char nom_niveau[],Personnage *persos[], Obstacle *obstacles[], int caractObsQuiBougent[], GLuint textures[], Mix_Music *musique,int*pause, float *centerX, float *centerY, int *persoSelected){
  glDeleteTextures(NB_TEXTURES+1, textures); 
  freeObstacles(obstacles);
  freePersos(persos);
  if(musique){
    Mix_FreeMusic(musique);
  }
  strcpy(nom_niveau,nom_a_lancer);//On change le nom du niveau
  int j;
  
  for(j=0;j<NB_OBSTACLES*6; j++){
    caractObsQuiBougent[j]=-1;
  }
  
  musique=NULL;

  *centerX = 0;
  *centerY = 0;
  *persoSelected=0;
  initialiseTextures(textures,nom_niveau);
  initializePersoFromFile(persos,obstacles,nom_niveau);
  initializeObstaclesFromFile(obstacles,nom_niveau,caractObsQuiBougent);
  InitialiseMusique(nom_niveau,musique);

  triBulle(obstacles,persos);//On tri le tableau d'obstacle en fonction de leur Z pour les dessiner dans l'ordre.

  if(strcmp(nom_niveau,"0/")==0){
   
    dessinePortes(obstacles,textures,pause);
    
  }
  else{
    *pause = 1;
  }
}

/*
Si on est dans le niveau 0 et que le perso touche une porte.
Selon sa progression et l'identifiant de l'obstacle porte, un niveau sera lancé.
*/
void lancerNiveauDepuisPorte(char nom_niveau[],Personnage *persos[], Obstacle *obstacles[], int caractObsQuiBougent[], GLuint textures[],Mix_Music *musique, Mix_Chunk * bruit[],int *pause, float *centerX, float *centerY, int *persoSelected){
  if (strcmp(nom_niveau,"0/")==0){
    int i;
    for (i = 0; i < NB_OBSTACLES; ++i)
    { 
      if(obstacles[i] && persos[0] && obstacles[i]->type == 5){
        int a;

        a=detectionCollisionPersoObs(persos[0], obstacles[i]);
        if(a!=0){
          int num;
          int progression;
          progression=LireProgression();
          num=obstacles[i]->id % 100;
          if (num == 5)
          {
            RestartProgression();
            ChangerNiveau("0/",nom_niveau,persos,obstacles,caractObsQuiBougent,textures,musique,pause, centerX, centerY,persoSelected);
          }
          else if(num <= progression){
            sprintf(nom_niveau, "%d/", num);
            ChangerNiveau(nom_niveau,nom_niveau,persos,obstacles,caractObsQuiBougent,textures,musique,pause, centerX, centerY,persoSelected);
          }
        }
      }
    }
  }
}


/*****FONCTIONS DE DESSIN*****/

/*
Dessine un repere centré en 0 et d'axe de longueur 10.
*/
void dessinRepere() {
  glBegin(GL_LINES);

  // Axe des x
  glColor3ub(255, 0, 0);
  glVertex2f(0, 0); 
  glVertex2f(10, 0);

  // Axe des y
  glColor3ub(0, 255, 0);
  glVertex2f(0, 0);
  glVertex2f(0, 10);

  glEnd();
}

/*
Fonction qui dessine les obstacles qui n'ont pas de texture.
*/
void drawObstacle(Obstacle *o){

  if (o)
  {   
    switch(o->type){
              case 0 :
                glColor3ub(00,00,00);                
              break;
              case 1 :
                glColor3ub(00,00,00); 
                break;
              case 2 :
                 glColor3ub(255,255,0);  
                break;
               case 3 :
                 glColor3ub(255,255,255);  
                break;
              case 4 :
                 glColor3ub(255,0,0);  
                break;
              default :
                glColor3ub(0,0,0); 
                break;
    }
      
    glBegin(GL_QUADS);
      glVertex2f(o->x, o->y); 
      glVertex2f(o->x + o->width, o->y); 
      glVertex2f(o->x + o->width, o->y + o->height); 
      glVertex2f(o->x, o->y + o->height); 
    glEnd();
  }  
}

/*
Dessine les obstacles avec une texture.
*/
void drawObstacleTexture(GLuint texture ,Obstacle *o, char nom_niveau[], int nbLoop){
  if (o)
  {
    /*Pour jouer le gif du volcan, niveau 4*/
    if (strcmp(nom_niveau, "4/") == 0 && o->id == 4)
        {
          /* Change de texture toutes les 6 loops, et récupere les 15 images numérotées de 20 à 35 */
          o->texture = (nbLoop/6)%15 + 20; 
        }


    glEnable(GL_TEXTURE_2D);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
      glColor3f(1, 1, 1);

      glTexCoord2f(0, 1);
      glVertex2f(o->x, o->y); 

      glTexCoord2f(1, 1);
      glVertex2f(o->x + o->width, o->y); 

      glTexCoord2f(1, 0);
      glVertex2f(o->x + o->width, o->y + o->height); 

      glTexCoord2f(0, 0);
      glVertex2f(o->x, o->y + o->height); 

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

  }
  
}

/*
Fonction qui dessine un curseur de sélection au dessus du personnage sélectionné.
*/
void drawCurseurSelection(Personnage *p, int nbLoop){
  if(p){
    float offset = cos((nbLoop % 200)*0.1); /* animation du triangle */
     glBegin(GL_TRIANGLES);
      glColor3ub(255,255,255);   
      glVertex2f(p->obs->x + p->obs->width/2 + 5, p->obs->y + p->obs->height + 10 + offset); 
      glVertex2f(p->obs->x + p->obs->width/2    , p->obs->y + p->obs->height + 5 + offset); 
      glVertex2f(p->obs->x + p->obs->width/2 - 5, p->obs->y + p->obs->height + 10 + offset); 
    glEnd();
  }
}

/*
Fonction qui dessine un curseur de sélection au dessus du personnage sélectionné.
*/
void drawMiniaturePersos(Personnage *persos[], int persoSelected, int nbLoop, float centerX, float centerY, int winWidth, int winHeight){
  int i=0;
  float taille = 0.5;
  float positionY;
  float positionX;
  
  for (i = 0; i < NB_PERSONNAGES; ++i)
  {
    if (persos[i])
    {
      positionY = centerY - 150;
      //positionX = centerX  + ((persos[i]->obs->id)-1) * 10; // en bas au milieu
      positionX = centerX  + (persos[i]->obs->x) / 10; // en bas , en fct de la position du perso

      glBegin(GL_QUADS);
          if(i == persoSelected){   glColor4ub(255,255,255,150); }
          else{glColor4ub(255,255,255,100);}

          glVertex2f(positionX, positionY); // bas gauche
          glVertex2f(positionX, positionY + persos[i]->obs->height * taille); //haut gauche
          glVertex2f(positionX + persos[i]->obs->width * taille, positionY + persos[i]->obs->height * taille); // haut droit
          glVertex2f(positionX + persos[i]->obs->width * taille, positionY); // bas droit
      glEnd();      
    }    
    
   
  }
  
}

/****FONCTION VIDÉO*******/

/*
Affiche la vidéo d'intro au lancement du jeu
Utilise le dossier "intro/"
*/
void launchVideoIntro(Mix_Music *musique, char nom_fichier[]){

    char nom[100];
    int i;
    GLuint texturesIntro[250];
    initialiseTextures(texturesIntro,nom_fichier);
    Obstacle *introFrame = createObstacle(1,0,0,-170,-120, 0, 320,240);
    i=25;
    strcpy(nom,nom_fichier);
    strcat(nom, "musique/musique.wav");
    if(musique){
      Mix_FreeMusic(musique);
    }
    musique = Mix_LoadMUS(nom);
    Mix_PlayMusic(musique,1);
    if (strcmp(nom_fichier, "fin/") == 0){
      SDL_Delay(800); /* delay pour synchroniser musique et vidéo */
    }
    else if (strcmp(nom_fichier, "intro/") == 0){
      SDL_Delay(1800); /* delay pour synchroniser musique et vidéo */
    }

    while(i < 250){
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      drawObstacleTexture(texturesIntro[i], introFrame,"0/",0);
      SDL_Delay(1000/25);
      SDL_GL_SwapBuffers();   
      i++; 
    }
    Mix_PauseMusic();
    if(musique){
      Mix_FreeMusic(musique);
    }
    musique = NULL;

    glDeleteTextures(NB_TEXTURES+1, texturesIntro);
    if(introFrame){
      free(introFrame);
    }

}