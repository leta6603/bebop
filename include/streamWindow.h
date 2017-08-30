/*
 * streamWindow.h
 *
 *  Created on: 2017. 8. 8.
 *      Author: lee
 */

#ifndef STREAMWINDOW_H_
#define STREAMWINDOW_H_

#include <SDL.h>


int init();

void presentTexture(const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch);
void eventHandler(SDL_Event event);
void detachSDLThread();

#endif /* STREAMWINDOW_H_ */
