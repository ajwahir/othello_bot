/**	
 * Basic primitives
 */

#include <iostream>
using namespace std;

#include "Othello.h"
using namespace Desdemona;

bool Move::operator==( const Move other ) const
{
    return ( this->x == other.x ) && ( this->y == other.y );
}

istream& operator>>( istream& inp, Move& move )
{
    char x;
    int y;
    inp >> x >> y; 
    move.x = x - 'a';
    move.y = y;

    return inp;
}

ostream& operator<<( ostream& out, const Move& move )
{
    return out << 'a' + move.x << move.y << " ";
}


