#ifndef BOTLOADER_H
#define BOTLOADER_H


#include "Othello.h"
#include "OthelloPlayer.h"

using namespace Desdemona;

void loadInit();
OthelloPlayer* loadBot( string botPath, Turn turn );

#endif /* #ifndef BOTLOADER_H */
