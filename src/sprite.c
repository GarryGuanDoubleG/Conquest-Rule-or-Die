#include "sprite.h"

Sprite			*spriteList = NULL;
int				sprite_count = 0;
Sprite			*Sprite_Mouse;
static int		SPRITE_MAX = 0;

struct
{
	Uint32 state;
	Uint32 shown;

	Uint32 frame_horizontal;
	Uint32 frame_vertical;

	Uint16  x, y;
}Mouse2;

/**
* @brief intializes the sprite for the mouse
*/
void InitMouse2()
{
  if(!SDL_ShowCursor(false)){
	  printf("Could not hide mouse");
  }

  Sprite_Mouse = sprite_load("images/mouse.png", 16,16, NULL,NULL);
  if(Sprite_Mouse == NULL)fprintf(stdout,"mouse didn't load: %s\n", SDL_GetError());
  Mouse2.state = 0;
  Mouse2.shown = 0;
  Mouse2.frame_horizontal = Mouse2.frame_vertical = 0;

}
/**
* @brief retrieves the mouse x and y position
* @return x and y position in a 2d vector
*/
Vec2d get_mouse_pos()
{
	Vec2d mouse_pos = {Mouse2.x, Mouse2.y};
	return mouse_pos;
}

/**
* @brief Draw the mouse onto the screen
*/
void DrawMouse2()
{
  int mx,my;
  SDL_GetMouseState(&mx,&my);
  if(Sprite_Mouse != NULL)
	  sprite_draw(Sprite_Mouse,Mouse2.frame_horizontal, Mouse2.frame_vertical,__gt_graphics_renderer,graphics_get_player_cam().x + mx, graphics_get_player_cam().y + my);
  else
	  printf("Sprite_Mouse did not load properly");

  Mouse2.frame_horizontal = (Mouse2.frame_horizontal + 1)%16;
  if(Mouse2.frame_horizontal == 0)
	  Mouse2.frame_vertical = (Mouse2.frame_vertical + 1)%3;

  Mouse2.x = mx;
  Mouse2.y = my;
}

/**
* @brief allocates memory for a maximum 
* @param int maximum number of sprites to have allocated during game
*/
void sprite_initialize_system(int max_sprites)
{
	int i;
	if( max_sprites <= 0 )
	{
		fprintf(stdout, "The maximum number of sprites cannot be 0");
		return;
	}
	SPRITE_MAX = max_sprites;
	sprite_count = 0;
	spriteList = (Sprite *)malloc(sizeof(Sprite) * SPRITE_MAX);
	memset(spriteList, 0, sizeof(Sprite) * SPRITE_MAX);
	for(i = 0;i < SPRITE_MAX;i++)spriteList[i].image= NULL;

	atexit(sprite_close_system);
}

/**
* @brief deallocates the memory set during initialization
*/

void sprite_close_system()
{
	int i;
	Sprite  **target = NULL;
	Sprite *ptr = NULL;
	for(i = 0; i < SPRITE_MAX; i++){
		if( spriteList[i].refCount > 0){
			//target = spriteList[i];
			ptr = &spriteList[i];
			target = &ptr;
			sprite_free(target);
		}
	}	
}

/**
* @brief loads a sprite from a filepath and initializses its sizes
* @param filepath to find and load sprite, img width and height for size of clip of sprite to load
  frame width and height for size of sprite in game
  @return pointer to loaded sprite
*/

Sprite *sprite_load(char *filename, int img_width, int img_height, int frameW, int frameH)
{
	SDL_Texture * newTexture = NULL;
	SDL_Surface * loadedSurface = NULL;
	Sprite* new_sprite;
	int i;
	if(!spriteList){
		fprintf(stdout, "Sprite List is null");
	}
	if(!filename){
		fprintf(stdout,"Filename in Sprite is Null\n");
		return NULL;
	}

	for(i = 0; i < SPRITE_MAX; i++){
		if(!strcmp(spriteList[i].filename, filename)){
			fprintf(stdout,"Sprite has already been loaded");
			return NULL;
		}
	}
	loadedSurface = IMG_Load(filename);
	if(!loadedSurface){
		fprintf(stdout,"%s was not loaded in sprite_load\n",filename);
		return NULL;
	}
	//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

	newTexture = SDL_CreateTextureFromSurface(__gt_graphics_renderer, loadedSurface);
	if(!newTexture){
		printf("Unable to create New Texture from Sprite Surface");
		return NULL;
	}
	//Loop through sprite list to find unallocated memory location
	for(i = 0; i < SPRITE_MAX; i++){
		if((spriteList + i)->image == NULL){
			spriteList[i].refCount = 1;
			//Get image dimensions
			spriteList[i].imageW = img_width;
			spriteList[i].imageH = img_height;
			//global count
			sprite_count++;
			spriteList[i].image = newTexture;
			strncpy(spriteList[i].filename,filename,20);
			spriteList[i].fpl = 16;//frames per line. May have to change later

			spriteList[i].frameW = frameW > 0 ? frameW : img_width;
			spriteList[i].frameH = frameH > 0 ? frameH : img_height;
			if(sprite_count +1 >= SPRITE_MAX){
				printf("Uh oh, max sprites reached!");
				exit(1);
			}
			SDL_FreeSurface(loadedSurface);
			return &spriteList[i];
		}		
	}


}
/**
* @brief frees the image loaded in the sprite and deallocates the memory for it
*/
void sprite_free(Sprite ** sprite)
{
	Sprite * target = *sprite;
	if(!sprite) return;
	//if(!*sprite) return;
  
	target->refCount--;

	if(target->refCount == 0)
	{
		strcpy(target->filename,"\0");
			/*just to be anal retentive, check to see if the image is already freed*/
		if(target->image != NULL)
			SDL_DestroyTexture(target->image);
		memset(target, 0, sizeof(Sprite));

		target->image = NULL;
	}
	target = NULL;
}

/**
* @brief draws the sprite onto the screen
* @param Sprite pointer to draw, frame_horizontal frame # to play, frame-vertical - which frame to play, renderer of game, and x and y offset to draw
*/

void sprite_draw(Sprite *sprite, int frame_horizontal, int frame_vertical, SDL_Renderer *renderer, int drawX, int drawY)
{
	//Set rendering space and render to screen
	SDL_Rect camera = graphics_get_player_cam();
	SDL_Rect src = { frame_horizontal * sprite->imageW, 
					 frame_vertical * sprite->imageH, 
					 sprite->imageW, sprite->imageH};
	SDL_Rect dest = { drawX, drawY, sprite->frameW, sprite->frameH};

	if(rect_collide(camera, dest))
	{
		dest.x -= camera.x;
		dest.y -= camera.y;
		SDL_RenderCopy(renderer,sprite->image, &src, &dest);
	}
}

/*eol@eof*/
