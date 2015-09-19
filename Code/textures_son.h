#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

# define NB_TEXTURES 250
# define NB_BRUITAGES 3

/****FONCTIONS DE GESTION DES IMAGES ET DES TEXTURES****/
void load_images(SDL_Surface* images[], char nom_image[]);
void FreeImages(SDL_Surface* images[]);
void transformImageEnTexture(SDL_Surface* image[], GLuint textures[]);
void initialiseTextures(GLuint textures[],char nom_niveau[]);




/****FONCTIONS DE GESTION DES BRUITAGES ET DE LA MUSIQUE****/
void InitialiseMusique(char  nom_niveau[], Mix_Music *musique);
void InitiliseBruitages(char  nom_niveau[],Mix_Chunk * bruit[]); 
void freeBruit(Mix_Chunk * bruit[]);
void JoueBruit(Mix_Chunk * bruit);
