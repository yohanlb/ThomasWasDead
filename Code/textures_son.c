#include "textures_son.h"


/****FONCTIONS DE GESTION DES IMAGES ET DES TEXTURES****/

/*
Fonction qui charge les images en fonction du nom_niveau
*/
void load_images(SDL_Surface* images[], char nom_image[]){
  int i;
  char nom_image_tmp[50];
  char nom_a_garder[50];
  char num[10];
  strcpy(nom_image_tmp,nom_image);
  strcat(nom_image_tmp, "images/"); // nom_image_tmp = "images/1/" 
  strcpy(nom_a_garder,nom_image_tmp);
  for (i = 0; i < NB_TEXTURES; ++i)
  {
    sprintf(num, "%d.jpg", i);

    strcat(nom_image_tmp,num);// nom_image_tmp = "images/1/i.jpg" 

    SDL_Surface* img = IMG_Load(nom_image_tmp);
    if(img == NULL) {
        /* si on trouve pas de jpg, on essaye avec un png */
        strcpy(nom_image_tmp,nom_a_garder);
        sprintf(num, "%d.png", i);
        strcat(nom_image_tmp,num);
        img = IMG_Load(nom_image_tmp);        

    }
    if(img == NULL) {
            /* si on a trouvé ni jpg ni png => Null */
            images[i]=NULL;
    }
    else{
      images[i]=img;
    }

    strcpy(nom_image_tmp,nom_a_garder);
    /* nom_image_tmp = "images/1/" */
  }
}

/*
Fonction qui libère les images d'un tableau d'images.
*/
void FreeImages(SDL_Surface* images[]){
  int i;
  for ( i = 0; i < NB_TEXTURES; ++i)
  {
    if(images[i]){
      SDL_FreeSurface(images[i]);
    }
  }
}

/*Fonction qui à partir d'un tableau d'image crée un tableau de textures.*/
void transformImageEnTexture(SDL_Surface* images[], GLuint textures[]){
    int i;
    for( i= 0; i< NB_TEXTURES; i++){
      if(images[i]){
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        GLenum format=0;
        switch(images[i]->format->BytesPerPixel) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                /* GL_BGR  ou GL_RGB  */
                format = GL_RGB;
                break;
            case 4:
                /* GL_BGRA ou GL_RGBA*/
                format = GL_RGBA;
                break;
            default:
                fprintf(stderr, "Format des pixels de l'image non pris en charge\n");
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[i]->w, images[i]->h, 0, format, GL_UNSIGNED_BYTE, images[i]->pixels);
        
        glBindTexture(GL_TEXTURE_2D, 0);
      }
  }
}

/*
Cette fonction initialise un tableau d'image et réutilise les fonctions précédentes
Elle load les images, les transfoment en un tableau de texture, puis les libèrent.
*/
void initialiseTextures(GLuint textures[],char nom_niveau[]){
    
    SDL_Surface* images[NB_TEXTURES];
    load_images(images, nom_niveau);
    glGenTextures(NB_TEXTURES+1, textures);
    transformImageEnTexture(images, textures);
    FreeImages(images);
}

  
/****FONCTIONS DE GESTION DES BRUITAGES ET DE LA MUSIQUE****/

/*
Fonction qui initialise et lance la musique en fonction du niveau
*/
void InitialiseMusique(char  nom_niveau[], Mix_Music *musique){
  char nom_fichier[50];
  strcpy(nom_fichier,nom_niveau);
  strcat(nom_fichier, "musique/musique.wav");
  

  musique = Mix_LoadMUS(nom_fichier); //Chargement de la musique
  Mix_PlayMusic(musique, -1);// Lancement de la musique

}
/*
Fonction qui initialise les bruitages en fonction du numéro de niveau
*/
void InitiliseBruitages(char  nom_niveau[],Mix_Chunk * bruit[]){
  char nom_fichier_saut[50];
  strcpy(nom_fichier_saut,nom_niveau);
  strcat(nom_fichier_saut, "musique/saut.wav");
  char nom_fichier_ennemis[50];
  strcpy(nom_fichier_ennemis,nom_niveau);
  strcat(nom_fichier_ennemis, "musique/ennemis.wav");
  char nom_fichier_success[50];
  strcpy(nom_fichier_success,nom_niveau);
  strcat(nom_fichier_success, "musique/success.wav");

  Mix_AllocateChannels(NB_BRUITAGES);
    bruit[0] = Mix_LoadWAV(nom_fichier_saut); //Charger un wav dans un pointeur
    bruit[1] = Mix_LoadWAV(nom_fichier_ennemis);
    bruit[2] = Mix_LoadWAV(nom_fichier_success);

}
/*
Fonction qui lance un bruitage
*/
void JoueBruit(Mix_Chunk * bruit){
    int a=Mix_Playing(1);
      if(a==0){
      Mix_PlayChannel(1, bruit, 0);
    }
}
/*
Fonction qui libère l'espace allouée pour les bruitages.
*/
void freeBruit(Mix_Chunk * bruit[]){

  int i;
  for (i = 0; i < NB_BRUITAGES; ++i)
  {
    if (bruit[i])
    {
      Mix_FreeChunk(bruit[i]);
    }
  }
   
}
