/*
#include <Bebop.h>
 * streamWindow.cpp
 *
 *  Created on: 2017. 7. 12.
 *      Author: lee
 */

#include<iostream>
#include<SDL.h>

#include "streamWindow.h"
#include "Bebop.h"

using namespace std;

static int SCREEN_WIDTH = 1280;
static int SCREEN_HEIGHT = 720;

static SDL_Texture* texture;
static SDL_Renderer* renderer;
static SDL_Window* window;

int init(){
	if( SDL_Init(SDL_INIT_VIDEO) ==-1){
			std::cout << "Error : cannot initialize sdl : " << SDL_GetError() << std::endl;
			return false;
		}

		window = SDL_CreateWindow("Bebop Controller", 			//
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH,		// width
									SCREEN_HEIGHT,	// height
									SDL_WINDOW_SHOWN);
		if(window == NULL){
			std::cout << "Error : cannot create window : " << SDL_GetError() << std::endl;
			return false;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
		if(renderer == NULL){
			std::cout << "Error : cannot create renderer : " << SDL_GetError() << std::endl;
			return false;
		}

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
		if(texture == NULL){
			std::cout << "Error : cannot create texture : " << SDL_GetError() << std::endl;
			return false;
		}
		return true;
}

void presentTexture(const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch){
	if(SDL_UpdateYUVTexture(texture, NULL, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch) != 0){
		std::cout << "Error : cannot update texture : " << SDL_GetError() << std::endl;
	}
	if(SDL_RenderClear(renderer) != 0){
		std::cout << "Error : cannot clear renderer : " << SDL_GetError() << std::endl;
	}
	if(SDL_RenderCopy(renderer, texture, NULL, NULL) != 0){
		std::cout << "Error : cannot copy texture : " << SDL_GetError() << std::endl;
	}
	SDL_RenderPresent(renderer);
}

void eventHandler(SDL_Event event){
	switch(event.type){
	case SDL_KEYUP :
	      switch(event.key.keysym.sym){
	      case SDLK_RIGHT:
	    				  mNavigationCommand = Stop;
	    					  cout << "Right" << endl;
	    				  break;
	    				  case SDLK_LEFT:
	    				  mNavigationCommand = Stop;
	    					  cout << "Left" << endl;
	    				  break;
	    				  case SDLK_UP:
	    				  mNavigationCommand = Stop;
	    					  cout << "up" << endl;
	    				  break;
	    				  case SDLK_DOWN:
	    				  mNavigationCommand = Stop;
	    					  cout << "Down" << endl;
	    				  break;
	    				  case SDLK_q:
	    				  mNavigationCommand = Stop;
	    					  cout << "stop" << endl;
	    				  break;
	    				  case SDLK_l:
	    					  land();
	    					  mNavigationCommand = Land;
	    					  cout << "Land" << endl;
	    					  break;
	    				  case SDLK_SPACE:
	    					  takeoff();
	    					  mNavigationCommand = Stop;
	    					  cout << "Land" << endl;
	    					  break;
	    				  case SDLK_ESCAPE:
	    					  emergency();
	    					  cout << "Emergency" << endl;
	    					  break;
	    				  case SDLK_w:
	    				  mNavigationCommand = Stop;
	    					  cout << "UP" << endl;
	    					  break;
	    				  case SDLK_s:
	    				  mNavigationCommand = Stop;
	    					  cout << "DOWN" << endl;
	    					  break;
	    				  case SDLK_d:
	    				  mNavigationCommand = Stop;
	    					  cout << "Clockwise" << endl;
	    					  break;
	    				  case SDLK_a:
	    				  mNavigationCommand = Stop;
	    					  cout << "CounterClockwise" << endl;
	    					  break;

	    				  break;
	      }
   break;

   case SDL_KEYDOWN :
		      switch(event.key.keysym.sym){
				  case SDLK_RIGHT:
				  mNavigationCommand = Right;
					  cout << "Right" << endl;
				  break;
				  case SDLK_LEFT:
				  mNavigationCommand = Left;
					  cout << "Left" << endl;
				  break;
				  case SDLK_UP:
				  mNavigationCommand = Forward;
					  cout << "up" << endl;
				  break;
				  case SDLK_DOWN:
				  mNavigationCommand = Backward;
					  cout << "Down" << endl;
				  break;
				  case SDLK_q:
				  mNavigationCommand = Stop;
					  cout << "stop" << endl;
				  break;
				  case SDLK_l:
					  land();
					  mNavigationCommand = Land;
					  cout << "Land" << endl;
					  break;
				  case SDLK_SPACE:
					  takeoff();
					  mNavigationCommand = Stop;
					  cout << "Land" << endl;
					  break;
				  case SDLK_ESCAPE:
					  emergency();
					  cout << "Emergency" << endl;
					  break;
				  case SDLK_w:
					  mNavigationCommand = Up;
					  cout << "UP" << endl;
					  break;
				  case SDLK_s:
					  mNavigationCommand = Down;
					  cout << "DOWN" << endl;
					  break;
				  case SDLK_d:
					  mNavigationCommand = Clockwise;
					  cout << "Clockwise" << endl;
					  break;
				  case SDLK_a:
					  mNavigationCommand = CounterClockwise;
					  cout << "CounterClockwise" << endl;
					  break;

				  break;
		      }
    break;
    default :
	  	  break;

	}
}




