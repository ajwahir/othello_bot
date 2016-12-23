/**
 * Class for the Othello board 
 */

#include "OthelloPlayer.h"
using namespace Desdemona;

OthelloPlayer::OthelloPlayer( Turn turn ) :
    turn( turn )
{
}

Move OthelloPlayer::play( const OthelloBoard& board )
{
    return Move::empty();
}

