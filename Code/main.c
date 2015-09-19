#include "fonctionnement.h"

 
int main(int argc, char** argv) {

  unsigned int windowWidth  = 480*2;
  unsigned int windowHeight = 360*2;
  float echelle = 150;
  float centerX=0,centerY=0;
  int i;
 

  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }  
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
      printf("%s", Mix_GetError());
   }

  setVideoMode(windowWidth, windowHeight);
  SDL_WM_SetCaption("Thomas is dead (in an explosion)", NULL);


  char nom_niveau[10]="0/"; 
  GLuint textures[NB_TEXTURES+1];
  initialiseTextures(textures,nom_niveau);

  Personnage *persos[NB_PERSONNAGES];
  Obstacle *obstacles[NB_OBSTACLES];
  int caractObsQuiBougent[NB_OBSTACLES*6];

  int j=0;
  for(j=0;j<NB_PERSONNAGES; j++){
    persos[j]=NULL;
  }
  
  for(j=0;j<NB_OBSTACLES; j++){
    obstacles[j]=NULL;
  }
  Mix_Music *musique=NULL;
  Mix_Chunk * bruit[NB_BRUITAGES];
  for(j=0;j<NB_BRUITAGES; j++){
    bruit[j]=NULL;
  }

  for (j = 0; j < NB_OBSTACLES*6; ++j)
  {
    caractObsQuiBougent[j]=-1;//Cas limites par convention aucun id d'obstacle ni aucune position de début ou de fin de mouvement ne doit être égale à -1
  }
  
  InitiliseBruitages("0/",bruit);
  int nbLoop=0,loop=1;
  int leftDown=0, rightDown=0, jumpDown=0, keyZoomOut = 0;
  int persoSelected=0;
  int obj=0,pause=0; //Pause:0 on joue, pause=1 le jeu est en pause, pause = 2 fin de niveau, pause= 4 game over
  Obstacle * opause;
  opause = createObstacle(10,0,1,-100,-50, 0, 200,200);
  int win=0,loose=0,jumpbool=0;//booléen pour la gestion des bruitages
  
  
  launchVideoIntro(musique, "intro/");


  ChangerNiveau("0/",nom_niveau,persos,obstacles,caractObsQuiBougent,textures,musique,&pause,&centerX,&centerY,&persoSelected);


  while(loop) {
    /* temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    /* dessin */
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
   
    moveCameraSuivrePerso(persos[persoSelected],windowWidth,windowHeight,&centerX,&centerY, obstacles);

        
    obj = objectifAtteint(persos, obstacles);

    
   for (i = NB_OBSTACLES-1; i >= 0; --i){ /* On dessine les obstacles */
      if(obstacles[i]){

        if(obstacles[i]->texture != 0){
          drawObstacleTexture(textures[obstacles[i]->texture], obstacles[i], nom_niveau, nbLoop);
        }
        else{
          drawObstacle(obstacles[i]);  
        }
         
      }
    } 
    drawCurseurSelection(persos[persoSelected], nbLoop);
    
    if (strcmp(nom_niveau, "0/") != 0 && keyZoomOut != 1){
      drawMiniaturePersos(persos, persoSelected, nbLoop, centerX, centerY, windowWidth,  windowHeight);
    }
    if(pause){
      Mix_PauseMusic();
      opause->x = centerX - opause->width/2;  /* Pour centrer l'image pause */
      opause->y = centerY - opause->height/2;
      mettreEnPause(pause, opause, textures);
    }
    else if(obj==0){

      Mix_ResumeMusic();

      zoomOut(keyZoomOut, &echelle, nom_niveau);
      reshape(windowWidth,windowHeight,echelle);
    
      for (i = NB_OBSTACLES-1; i >= 0; --i){ /* Deplacement des obstacles */
        if(obstacles[i]){
          deplaceObstacleContinue(obstacles[i],persos,obstacles, caractObsQuiBougent);
        }
      }

      for (i = 0; i < NB_PERSONNAGES; ++i) /* Mouvements des personnages */
      {
        if(persos[i])
        { 
          movePersonnage(persos[i],&jumpbool);
          updateVitesses(persos[i],nom_niveau);
          CollisionsPersonnage(persos[i], obstacles);
        }

      }
      if (persos[persoSelected] && jumpDown == 1){
        jumpbool = 0;
        if (jump(persos[persoSelected])) {JoueBruit(bruit[0]);}
      }
      if (persos[persoSelected] && leftDown == 1)
      {
        persos[persoSelected]->vx -= 20;
      }
      if (persos[persoSelected] && rightDown ==1)
      {
        persos[persoSelected]->vx += 20;
      }

    }

    lancerNiveauDepuisPorte(nom_niveau,persos,obstacles, caractObsQuiBougent, textures,musique,bruit,&pause, &centerX, &centerY,&persoSelected);

    if(obj==1){ //On regarde si l'objectif est atteint ou si on a perdu
      if(win==0){ /* Moment on l'on gagne*/

        JoueBruit(bruit[2]);
        win=1;
      }
      AugmenterProgression(nom_niveau);
      if(pause==0 || pause ==2){
        pause=2;
        mettreEnPause(pause, opause, textures);
      }
      else{
        if (strcmp(nom_niveau, "4/") == 0){
          launchVideoIntro(musique, "fin/");
        }
        ChangerNiveau("0/",nom_niveau,persos,obstacles,caractObsQuiBougent,textures,musique,&pause,&centerX,&centerY,&persoSelected);
        win=0;
      }

    }
    else if(obj==-1 || loose==1){
      if(loose==0){
        JoueBruit(bruit[1]);
        loose=1;
      }
      if(pause==0 || pause==4){
        pause=4;
        mettreEnPause(pause, opause, textures);
      }
      else
      {
        ChangerNiveau(nom_niveau,nom_niveau,persos,obstacles,caractObsQuiBougent,textures,musique,&pause,&centerX,&centerY,&persoSelected);
        loose=0;
      }
    }


    /*******************************************************************************/    
    
    SDL_GL_SwapBuffers();    

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      switch(e.type) {
        case SDL_MOUSEBUTTONDOWN:
          break;
        case SDL_MOUSEBUTTONUP:
          break;  

        case SDL_VIDEORESIZE:
          windowWidth  = e.resize.w;
          windowHeight = e.resize.h;
          setVideoMode(windowWidth,windowHeight);
          break;

        case SDL_KEYDOWN:
          switch(e.key.keysym.sym){
            case SDLK_ESCAPE : 
              loop = 0;
              break;
            case 'q' :  case SDLK_LEFT :
              leftDown=1;
              break;
            case 'd' :  case SDLK_RIGHT :
              rightDown=1;
              break;
            case 'z' :  case SDLK_UP :  case SDLK_SPACE:
              jumpDown=1;
              break;
            case 's' :  case SDLK_DOWN :
              keyZoomOut = 1;
              break;
            default : 
              break;
          }
          break;

          case SDL_KEYUP:
          switch(e.key.keysym.sym){
            case 'q' :  case SDLK_LEFT :
              leftDown=0;
              break;
            case 'd' :  case SDLK_RIGHT :
              rightDown=0;
              break;
            case 'z' :  case SDLK_UP : case SDLK_SPACE:
              jumpDown=0;
              break;
            case 's' :  case SDLK_DOWN :
              keyZoomOut = 0;
              break;
            case 'e' : case SDLK_TAB :
              if(!pause){
                switchPerso(&persoSelected, persos);
              }              
              break;
            case '\r' :
              pause++;
              pause=pause%2;
              break;
            case 'm' :
              if(pause && pause!=4){
              ChangerNiveau("0/",nom_niveau,persos,obstacles,caractObsQuiBougent,textures,musique,&pause,&centerX,&centerY,&persoSelected);
              }
              break;
            case 'r' :
              if(pause && pause!=4){
              ChangerNiveau(nom_niveau,nom_niveau,persos,obstacles,caractObsQuiBougent,textures,musique,&pause,&centerX,&centerY,&persoSelected);
              }
              break;
            default : 
              break;
          }
          break;
         
        default:
          break;
      }
    }
    
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
    nbLoop++;
  }
  
  //Libérer les texture//
  glDeleteTextures(NB_TEXTURES+1, textures);


  
  freeObstacles(obstacles);
  freePersos(persos);
  if(musique){
    free(musique);
  }
  if (opause)
  {
    free(opause);
  }

  freeBruit(bruit);
  Mix_CloseAudio();// fermeture de l'api
  SDL_Quit();

  
  return EXIT_SUCCESS;
}