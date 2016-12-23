#ifndef LOGGEDOTHELLOGAME_H
#define LOGGEDOTHELLOGAME_H



#include "OthelloGame.h"
#include <string>
#include <fstream>
using namespace std;

namespace Desdemona
{
    class LoggedOthelloGame: public OthelloGame
    {
        public:

            /**
             * Open stream to log file
             */
            LoggedOthelloGame( string logfile, OthelloPlayer& player1, OthelloPlayer& player2 );

            /**
             * Close log file
             */
            ~LoggedOthelloGame();
        protected:
            fstream log;

            /**
             * Log moves to log file
             */
            void postPlayActions( Move& move );
    };
}

#endif /* #ifndef LOGGEDOTHELLOGAME_H */
