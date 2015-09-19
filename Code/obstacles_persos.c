#include "obstacles_persos.h"


/*****FONCTIONS DE CREATIONS ET DE LIBERATION DE STRUCTURE*******/

/*
Fonction qui alloue la mémoire et ajoute des attribut à un personnage.
*/
Personnage * createPersonnages(int id, float x, float y, float xfin, float yfin, int width, int height, int texture, int jumpPower)
{
  Personnage * p = calloc(1,sizeof(Personnage));
  Obstacle * obs = createObstacle(id, 2,texture, x, y, 0, width, height);
  p->obs = obs;
  p->xfin=xfin;
  p->yfin=yfin;
  p->vx=0;
  p->vy=0;
  p->jumpPower=jumpPower;
  p->jumpDuration=0;
  p->groundDistance=0;
  return p;
}

/*
Fonction qui alloue la mémoire et ajoute des attribut à un obstacle.
*/
Obstacle * createObstacle(int id, int type, int texture, float x, float y, float z, int width, int height){
  Obstacle * o = calloc(1,sizeof(Obstacle));
  o->id=id;
  o->type=type;
  o->texture = texture;
  o->x=x;
  o->y=y;
  o->z=z;
  o->width=width;
  o->height=height;
  return o;
}

/*
Fonction qui initialise les personnage d'un niveau ainsi que leurs obstacles objectifs en fonction du fichier texte Persos.txt du niveau
La fonction lit tous les caractère du fichier et reconnait les attributs d'un personnage car ils se trouvent entre le caractère '=' et ';'

Les obstacles sont placés dans le tableau obstacles[]
Les personnages sont placés dans le tableau persos[]

Les objectifs sont mis dans un fichier temporaire pour qu'ils se trouvent après les obstacles personnage dans le tableau obstacles. 
Ainsi les tous les personnages passent devant tous les obstacles dans la scène dessinée.

*/
int initializePersoFromFile(Personnage *p_tab[], Obstacle * obstacles[], char nom_niveau[]){
    char nom_fichier[50];
    strcpy(nom_fichier,nom_niveau);
    strcat(nom_fichier, "Persos.txt");

    FILE* fichier = NULL;
    int caractereActuel = 0;
    int test = 0,nb_perso=0;
    int perso_caract[9];
    int i;
    Personnage * perso;
    Obstacle * objectif;
    Obstacle * tabObjectiftmp[NB_PERSONNAGES];// Pour que les objectifs ne soient pas dessinés devant les personnages, il faut que les obstacles personnages soient avant dans le tableau obstacles, le tableau tabObjectiftmp[] sert à trier les obstacles personnages et objectifs
    fichier = fopen(nom_fichier, "r");
    if(fichier){

      for (i = 0; i < NB_PERSONNAGES; ++i)
      {
        p_tab[i]=NULL;
        tabObjectiftmp[i]=NULL;
      }

      if (fichier != NULL)
      {
            do
          { 
            caractereActuel = fgetc(fichier);
            if(caractereActuel=='=') test = 1;
            
            if (test==1) 
            {
              int i=0;   
              while (caractereActuel!=';') 
              { 
                fscanf(fichier, "%d", &perso_caract[i]);
                i++;
                caractereActuel = fgetc(fichier);
              }
              caractereActuel = fgetc(fichier);
              perso = createPersonnages(perso_caract[0], perso_caract[1], perso_caract[2], perso_caract[3], perso_caract[4], perso_caract[5],perso_caract[6],perso_caract[7],perso_caract[8]);
            
              p_tab[nb_perso]=perso;
              obstacles[nb_perso]=perso->obs;
            
              
              objectif=createObstacle(perso_caract[0], 3, 5, perso->xfin, perso->yfin, 0, perso_caract[5],perso_caract[6]);
              tabObjectiftmp[nb_perso]=objectif;
              nb_perso++;
              test = 0;
            }
   

          } while (caractereActuel != EOF);

          fclose(fichier);
          int j;
          for(j=0; j<NB_PERSONNAGES; j++){
            if(tabObjectiftmp[j]){
              obstacles[j+NB_PERSONNAGES]=tabObjectiftmp[j];
            }
          }

          return 0;
      }
    }

    return 1;
}
/*
Fonction qui initialise les obstacles à partir du fichier Obstacle.txt et les place dans le tableau obstacles[]
La fonction lit tous les caractère du fichier et reconnait les attributs d'un personnage car ils se trouvent entre le caractère '=' et ';'

Si un obstacle a 7 attributs, c'est un obstacle statique.
S'il a 9 attributs c'est qu'il est mobile. Ces informations de mouvements (positions initiales et finales) ainsi que son identifiant sont placé dans le tableau caractObsQuiBougent[]
*/
int initializeObstaclesFromFile(Obstacle *obstacles[],char nom_niveau[], int caractObsQuiBougent[]){
    
    char nom_fichier[50];
    strcpy(nom_fichier,nom_niveau);
    strcat(nom_fichier, "Obstacles.txt");
    
    FILE* fichier = NULL;
    int caractereActuel = 0;
    int test = 0,nb_ob=NB_PERSONNAGES*2;
    int ob_caract[10];
    Obstacle * o;

    fichier = fopen(nom_fichier, "r");

    if (fichier != NULL)
    { 
          do
        { 
          caractereActuel = fgetc(fichier);
          if(caractereActuel=='=') test = 1;/*La fonction récupère les attribut entre un '='' et un';'*/
          
          if (test==1) 
          {
            int i= 0;              
            while (caractereActuel!=';') 
            { 
              fscanf(fichier, "%d", &ob_caract[i]);
              i++;
              caractereActuel = fgetc(fichier);
            }

            caractereActuel = fgetc(fichier);
            o = createObstacle(ob_caract[0], ob_caract[1], ob_caract[2], ob_caract[3], ob_caract[4], ob_caract[5], ob_caract[6], ob_caract[7]);
            obstacles[nb_ob]=o;
            nb_ob++;
            test = 0;
            if (i>8)// si un objet à 9 argument c'est qu'il est mobile on stock alors ses positions de départ et d'arrivée et son identifiant.
            {
              int j=0;
              while(caractObsQuiBougent[j]!=-1 && i< NB_OBSTACLES*5){
                j++;
              }
              caractObsQuiBougent[j]= o->id;
              caractObsQuiBougent[j+1]= 1; // sens dans lequel se déplace l'objet : 1=allé du début à la fin, 2=retour;
              caractObsQuiBougent[j+2]= o->x;//xdeb
              caractObsQuiBougent[j+3]= o->y;//ydeb
              caractObsQuiBougent[j+4]= ob_caract[8]; //xFin
              caractObsQuiBougent[j+5]= ob_caract[9]; //yFin
            }
          }

        } while (caractereActuel != EOF);
        fclose(fichier);

        return 0;
    }

    return 1;
}

/*
Fonction qui libère un tableau d'obstacle
*/
void  freeObstacles(Obstacle *obstacles[]){
  int i;
  for (i = 0; i < NB_OBSTACLES; ++i)
  {
    if(obstacles[i])
      free(obstacles[i]);
      obstacles[i]=NULL;
  }
}

/*
Fonction qui libère un tableau de personnage
*/
void  freePersos(Personnage *persos[]){
  int i;
  for (i = 0; i < NB_PERSONNAGES; ++i)
  {
    if (persos[i])
      free(persos[i]);
      persos[i]=NULL;
  }
}


/****FONCTIONS DE COLLISIONS***/

/*
Fonction qui retourne un entier selon la collision (dessus, dessous,...) entre une personnage et un obstacle
  retourne 0 s'il n'y a pas de colllision 
  retourne 1 si le perso est à gauche de l'objet
  retourne 2 si le perso est à droite de l'objet 
  retourne 3 si le perso est sur l'objet 
  retourne 4 si le perso est sous l'objet
 */
int detectionCollisionPersoObs(Personnage *p, Obstacle *obs){
  int i;
  int Collision=0;
 if (p)
 {
    float pMaxLeft = p->obs->x;
    float pMaxRight = p->obs->x + p->obs->width;
    float pMaxTop = p->obs->y + p->obs->height;
    float pMaxBot = p->obs->y;

    
    for (i = 0; i < NB_OBSTACLES; ++i)
    {

          /* On ne test pas les collisions d'un personnage avec lui même, ou avec un obstacle de type 0 */
          if (obs && p)
          {
            

                float oMaxleft = obs->x;
                float oMaxRight = obs->x+obs->width;
                float oMaxTop = obs->y + obs->height;
                float oMaxBot = obs->y;


                /* Test Droite */
                if( (pMaxRight >= oMaxleft) &&(pMaxRight <= oMaxRight) &&
                    (pMaxTop > oMaxBot) && (pMaxBot < oMaxTop) ) 
                {      
                    Collision=1;
                }

                /* Test Gauche */
                if( (pMaxLeft >= oMaxleft) &&(pMaxLeft <= oMaxRight) &&
                    (pMaxTop > oMaxBot) && (pMaxBot < oMaxTop) ) 
                {      
                    Collision=2;   
                }

                /* Test Bas */
                if( (pMaxRight-2 > oMaxleft) &&(pMaxLeft < oMaxRight-2) &&
                    (pMaxBot >= oMaxBot) && (pMaxBot <= oMaxTop) )
                    /* Les -2 sont la pour éviter le bug ou le perso saute deux fois si il est au bord d'un obstacle */ 
                {           
                   Collision=3;     
                    
                }
                /* Test Haut */
                if( (pMaxRight-2 > oMaxleft) &&(pMaxLeft < oMaxRight-2) &&
                    (pMaxTop >= oMaxBot) && (pMaxTop <= oMaxTop) )
                {           
                    Collision = 4; 
                }
          }
    }

  }
  return Collision;
}


/*
 Fonction qui met à jour les attributs du Personnage en fonction de ses collisions avec les obstacles
 */
void CollisionsPersonnage(Personnage *p, Obstacle *obs[]){
  int i;
 if (p)
 {

    p->touchRight=0;
    p->touchLeft=0;
    p->touchTop=0;
    p->touchGround=0;
    
    p->groundDistance = 1000000;
    float distance = 0;

    float pMaxLeft = p->obs->x;
    float pMaxRight = p->obs->x + p->obs->width;
    float pMaxTop = p->obs->y + p->obs->height;
    float pMaxBot = p->obs->y;

    
    for (i = 0; i < NB_OBSTACLES; ++i)
    {

          /* On ne test pas les collisions d'un personnage avec lui même, ou avec un obstacle de type 0 ou un objectif (type 3) */
          if (obs[i] && p->obs->id != obs[i]->id && obs[i]->type!=0 && obs[i]->type!=3)
          {
            

                float oMaxleft = obs[i]->x;
                float oMaxRight = obs[i]->x+obs[i]->width;
                float oMaxTop = obs[i]->y + obs[i]->height;
                float oMaxBot = obs[i]->y;


                /* Test Droite */
                if( (pMaxRight >= oMaxleft) &&(pMaxRight <= oMaxRight) &&
                    (pMaxTop > oMaxBot+1) && (pMaxBot < oMaxTop-1) ) 
                {      
                    p->touchRight=1;
                }

                /* Test Gauche */
                if( (pMaxLeft >= oMaxleft) &&(pMaxLeft <= oMaxRight) &&
                    (pMaxTop > oMaxBot+1) && (pMaxBot < oMaxTop-1) ) 
                {      
                    p->touchLeft=1;   
                }

                /* Test Bas */
                if( (pMaxRight-2 > oMaxleft) &&(pMaxLeft < oMaxRight-2) &&
                    (pMaxBot >= oMaxBot) && (pMaxBot <= oMaxTop) )
                    /* Les -2 sont la pour éviter le bug ou le perso saute deux fois si il est au bord d'un obstacle */ 
                {           
                    p->touchGround=1;
                    p->jumpDuration = 0;
                    p->groundDistance = 0;
                    
                    
                }

                /* Test Haut */
                if( (pMaxRight-2 > oMaxleft) &&(pMaxLeft < oMaxRight-2) &&
                    (pMaxTop >= oMaxBot) && (pMaxTop <= oMaxTop) )
                {           
                    p->touchTop=1;
                }

                /* Sinon, si on est au dessus de l'objet obs[i]... */
                else if( (pMaxRight > oMaxleft) && (pMaxLeft < oMaxRight) && (pMaxBot > oMaxTop) )
                {  /* On calcule la distance entre obs[i] et le personnage */
                    distance = pMaxBot - oMaxTop;
                    if (distance < p->groundDistance)
                    {
                      /* Si la distance est plus petite que les précédentes, on la garde dans groundDistance*/
                      p->groundDistance = distance;
                    }

                } 

          }
      
    }

    if(p->touchGround == 0)
    { /* Si on ne touche pas le sol, alors l'objet est en chute*/
      p->jumpDuration ++;
    }


  }
}

/*****FONCTIONS LIEES AUX DEPLACEMENT DES PERSONNAGES*****/
/*
Fonction qui met à jour la vitesse d'un personnage.
*/
void updateVitesses(Personnage *p, char nom_niveau[]){
  float k = 0.90; /* Constante de ralentissement (frottements du sol ou de l'air) */
  float g = 3.3; /* Constante de gravité */
  if(strcmp(nom_niveau,"3/")==0){//Parcequ'il y a moins de gravité dans l'espace
    g = 0.8;
  }
  /* Rallentissements lattéraux */
  if (p)
  {

    p->vx = p->vx * k;

    /* Gravité */ 
    if (p->jumpDuration > 0)
    {
      p->vy -= p->jumpDuration * g ;  
    }
  }

}

/*
Fonction qui met à jour la position d'un perso en fonction de sa vitesse et de ses colisions 
*/
void movePersonnage(Personnage *p, int * jumpbool){
  if(p)
  {
    if ( (p->touchRight==1) && (p->vx > 0) ) /* Si ca bloque à droite, on met la vitesse à zero */
    {
      p->vx=0;
    }
    if ( (p->touchLeft==1) && (p->vx < 0) ) /* Si ca bloque à gauche, on met la vitesse à zero */
    {
      p->vx=0;
    }
    if ( (p->touchGround==1) && (p->vy < 0) ) /* Si ca bloque en bas... */
    {
      p->vy=0;
    }
    if ( (p->touchTop==1) && (p->vy > 0) ) /* Si ca bloque en haut... */
    {
      p->vy=0;
    }
    /* Si besoin, on amortis la chute pour pas que le perso rentre dans le sol */

    if (p->groundDistance*100 < -p->vy )
    { 
      p->vy = -p->groundDistance*100;
      
    }
    if(p->groundDistance==0){
      *jumpbool=0;
    }

    p->obs->x += p->vx/100;
    p->obs->y += p->vy/100;
  }
}

  /* Si le personnage peut sauter, lance le timer (jumpDuration) et augmente la vitesse selon +y */
  /* Retourne 1 si le peronnage saute */
  int jump(Personnage *p){
    if (p && p->jumpDuration == 0 && !p->touchTop)
    { 
      p->vy += p->jumpPower;
      return 1;
    }
    return 0;
  }

/*
Fonction qui change le personnage sélectionné. 
*/
void switchPerso(int *persoSelected, Personnage * persos[]){
  int compteur=0;
  do{
    (*persoSelected) ++;    
    if ( *persoSelected >= NB_PERSONNAGES)
    {
      (*persoSelected) = 0;
    }
    compteur++;
  }
  while ( persos[*persoSelected]==NULL && compteur<=NB_PERSONNAGES);
  
}

/*****FONCTIONS DE CAMERA*******/


/*
Fonction qui centre la fenêtre sur un personnage et qui rend l'effet de paralaxe 
*/
void moveCameraSuivrePerso(Personnage *p,int windowWidth,int windowHeight, float* centerX, float* centerY, Obstacle *obstacles[]){
  float  Xrapport, Yrapport;
  float distanceCentreX, distanceCentreY, diffX, diffY;
  int i;

  if(p)
  {
    diffX = (*centerX);
    diffY = (*centerY);

    distanceCentreX=p->obs->x-*centerX;//distance entre le personnage et le centre de la fenêtre
    distanceCentreY=p->obs->y-*centerY;
    
    Xrapport=0.05*windowWidth;// distance du centre au perso à partir duquel la caméra bouge
    Yrapport=0.05*windowHeight;

   
    if(distanceCentreX>Xrapport){
        //influe sur la vitesse de déplacement de la caméra
        (*centerX) += +1 * (distanceCentreX - Xrapport) / 10;
    }
    else if (distanceCentreX<(-Xrapport)){
      (*centerX) -= -1 * (distanceCentreX + Xrapport) / 10;
    }

    if(distanceCentreY>Yrapport){
        (*centerY) += +1 * (distanceCentreY - Yrapport) / 10;
    }
    else if (distanceCentreY<(-Yrapport)){
        (*centerY) -= -1 * (distanceCentreY + Yrapport) / 1;
    }


    /* effet de parlaxe sur l'arriere plan */
    diffX = diffX - (*centerX);
    diffY = diffY - (*centerY);
    for (i = 0; i < NB_OBSTACLES; ++i)
    {
      if (obstacles[i] )
      {

        if (obstacles[i]->z != 0)
        {
          obstacles[i]->x -= ( obstacles[i]->z) * diffX * 0.05;
        }
        

      }
    }



    glTranslatef(-(*centerX),-(*centerY),0);


  }  
}

/*
 Fonction qui change l'echelle du jeu en fonction du booleen keyZoomOut
 */
void zoomOut( int keyZoomOut, float *echelle, char nom_niveau[]){
  float zoomMin = 150.0;
  float zoomMax = 250.0;

  if (strcmp(nom_niveau, "5/") == 0)
  {
    zoomMin =  250.0;
    zoomMax =  500.0;
  }

  if (keyZoomOut && *echelle < zoomMax)
  {
    *echelle += 10;
  }
  if (!keyZoomOut && *echelle > zoomMin)
  {
    *echelle -= 10;
  }

}



/****FONCTIONS POUR DEPLACER UN OBSTACLE ET LES PERSONNAGE QU'IL PEUT POUSSER****/


/*
Cette fonction regarde si un personnage poussé lors du déplacement d'un objet ne pousse pas un autre personnage, 
si oui, il faut le pousser à son tour
*/
void persoPoussePerso(int collisions,Personnage * p, Personnage * persoDejaTouche, Personnage * persos[],Obstacle * o){
  int i;
  for ( i = 0; i < NB_PERSONNAGES; ++i)
  {
    if(persos[i] && p && p->obs->id!=persos[i]->obs->id){//on ne cherche pas les collisions du personnage avec lui même
      int collisions2;
      collisions2=detectionCollisionPersoObs(persos[i], p->obs);
      if(collisions2!=0 && collisions==collisions2){
        float diff;
        diff=persos[i]->obs->y- p->obs->y- p->obs->height;
        if (collisions == 1)//Collision avec la gauche d'un perso
          { if (persoDejaTouche== NULL)
            {
              persos[i]->obs->x = o->x - persos[i]->obs->width- p->obs->width;
            }
            else if(persoDejaTouche->obs->id!=persos[i]->obs->id){
              persos[i]->obs->x = o->x - persos[i]->obs->width- p->obs->width-persoDejaTouche->obs->width;
            }
          }
          else if(collisions==2){//Collision avec la droite d'un perso
            if (persoDejaTouche== NULL)
            {
              persos[i]->obs->x= o->x+o->width +p->obs->width;
            }
            else if(persoDejaTouche->obs->id!=persos[i]->obs->id){
              persos[i]->obs->x= o->x+o->width + p->obs->width + persoDejaTouche->obs->width;
            }
          }
          else if(collisions2==3){//Collision avec le bas d'un perso
              persos[i]->obs->y= p->obs->y+p->obs->height;
          }
        persoPoussePerso(collisions2,persos[i],p,persos,o);
        }
        else if(collisions==0 && fabs(persos[i]->obs->y- p->obs->y- p->obs->height)<0.2 ){//Le perso est sur l'objet et que l'objet descend, le personnage ne touche pas l'objet et ne peut plus sauter
          Personnage *p_tmp;
          p_tmp= createPersonnages(persos[i]->obs->id, persos[i]->obs->x, p->obs->y+p->obs->height, persos[i]->xfin,persos[i]->yfin,persos[i]->obs->width, persos[i]->obs->height, persos[i]->obs->texture,persos[i]->jumpPower);
          int a;
          a=detectionCollisionPersoObs(p_tmp, p->obs);
          if(a!=0){
             persos[i]->obs->y= p->obs->y+ p->obs->height;
          }
          if(p_tmp)
          free(p_tmp);
          p_tmp=NULL;
        persoPoussePerso(collisions2,persos[i],p,persos,o);// Si le personnage poussé pousse le dernier personnage on relance la fonction mais persoDejaTouche n'est plus NULL
      }  
    }
  }  
}


/*
Fonction qui déplace un obstacle.
Elle renvoi 0 si le déplacement est en cours. 
Elle renvoi 1 si le perso à atteint sa bonne position.
*/
int deplaceObstacle(Obstacle * o,float xFin, float yFin, Personnage * persos[], Obstacle * obstacles[]){
  float vitesse = 0.1;
  if (o->id == 25 && o->texture == 15){vitesse = 0.03;} /* pour les nuages du lvl 1*/
  if (o->id == 23 && o->texture == 16){vitesse = 10.00;} /* pour le corbeau du lvl 1*/

  /*On vérifie si le perso n'est pas déjà arrivé au bon endroit*/
 if( fabs(o->x - xFin) < 0.05 && fabs(o->y - yFin) < 0.05  ){
 
  return 1;
 }
 else{
    /*On déplace l'obstacle*/
    if(o->x < xFin){
      o->x += vitesse;
    }
    else{
      o->x -= vitesse;
    }
    if(o->y < yFin){
      o->y += vitesse;
    }
    else{
      o->y -= vitesse;
    }

  /*On regarde si un obstacle ne va pas entrainer le déplacement d'autres personnages*/
  int i=0;
    for (i = 0; i < NB_PERSONNAGES; ++i)
    {
      int collisions; 
      if(persos[i] && persos[i]->obs->id != o->id && o->type != 0){ 
         collisions=detectionCollisionPersoObs(persos[i], o);
        if (collisions == 1)//Collision avec la gauche d'un perso
        {
          persos[i]->obs->x = o->x - persos[i]->obs->width;
        }
        else if(collisions==2){//Collision avec la droite d'un perso
          persos[i]->obs->x= o->x+o->width;
        }
        else if(collisions==3 ){//Collision avec le bas d'un perso
          persos[i]->obs->y= o->y+ o->height;
        }
        else if(collisions==0 && fabs(persos[i]->obs->y- o->y- o->height)<0.3){//Le perso est sur l'objet et que l'objet descend, le personnage ne touche pas l'objet et ne peut plus saute
            Personnage *p_tmp;
            p_tmp= createPersonnages(persos[i]->obs->id, persos[i]->obs->x, o->y+o->height, persos[i]->xfin,persos[i]->yfin,persos[i]->obs->width, persos[i]->obs->height, persos[i]->obs->texture,persos[i]->jumpPower);
            int a;
            a=detectionCollisionPersoObs(p_tmp, o);
            if(a!=0){
               persos[i]->obs->y= o->y+ o->height;
            }
            if(p_tmp)
            free(p_tmp);
            p_tmp=NULL;
        }

        if(collisions!=0 || (collisions==0 && persos[i]->obs->y- o->y- o->height<0.3))
       persoPoussePerso(collisions,persos[i],NULL,persos,o);
        
      }

    }

    return 0;
  }
}

/*
Cette fonction déplace les obstacles selon les informations contenuent dans le tableau caractObsQuiBougent[]
*/
void deplaceObstacleContinue(Obstacle * o, Personnage * persos[], Obstacle * obstacles[], int caractObsQuiBougent[]){
  int i;
  for(i=0; i< NB_OBSTACLES*6; i=i+6){
    if(o && caractObsQuiBougent[i]==o->id){//On cherche les caractéristiques de déplacement d'un obstacle grace à son identifiant
      int deplacement=0;
      if (caractObsQuiBougent[i+1]==1)//L'obstacle se déplace vers xFin et yFin
      {
        deplacement=deplaceObstacle(o,caractObsQuiBougent[i+4],caractObsQuiBougent[i+5],persos, obstacles);
        if (deplacement==1)// L'obstacle à fini son parcourt, il doit repartir dans l'autre sens.
        {
          caractObsQuiBougent[i+1]=2;
        }
      }
      else //L'obstacle se dirige vers ses positions de départ
      {
        deplacement=deplaceObstacle(o,caractObsQuiBougent[i+2],caractObsQuiBougent[i+3],persos, obstacles);
        if (deplacement==1)// L'obstacle à fini son parcourt, il doit repartir dans l'autre sens.
        {
          caractObsQuiBougent[i+1]=1;
        }

      }
    }
  } 

}
