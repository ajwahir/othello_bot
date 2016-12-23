/*
* @file botTemplate.cpp
* @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
* @date 2010-02-04
* Template for users to create their own bots
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>
#include <algorithm>
#include <string.h>
#include <time.h>

using namespace std;
using namespace Desdemona;

#define CUTOFF  0.98


class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up a cache of "best moves" to
         * spawning a background processing thread. 
         */
        MyBot( Turn turn );
        int count=0;
        double V=0;
        // int i=0;
        list<Move>::iterator req_move;
        double p=10000000000;
        int flag_max=1;
        double alpha=-p;
        double beta=p;
        int kp=2;
        int kply=kp;
        clock_t tStart = clock();
        time_t now =time(0);

        // cout << "Number of sec since January 1,1970:" << now << endl;
        MyBot();
        double e(const OthelloBoard& intermediate_board);
        double alphabeta( const OthelloBoard& intermediate_board, double alpha, double beta); 
        /**
         * Play something 
         */
        virtual Move play( const OthelloBoard& board );
    private:   
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}




double MyBot::e(const OthelloBoard& e_board)
{
    double edges=0,mobility=0,coin_pairity=0,cornerh=0;
 //MOBILITY 
    Turn invertTurn = turn;
    // cout<<"INvert turn One "<<invertTurn<<endl;
        // cout<<" turn "<<turn<<endl;  

    switch( invertTurn )
        {
            case RED:
                invertTurn=BLACK;
                // cout<<"Two 1 "<<endl;
                break;

            case BLACK:
                invertTurn=RED;
                // cout<<"One "<<endl;
                break;
            default:
                invertTurn= EMPTY;
                break;

            
        }
    // cout<<"INvert turn Two "<<invertTurn<<endl;  
    // cout<<" turn "<<turn<<endl;  
    list<Move> moves_max = e_board.getValidMoves( turn );
    // intermediate_board.print();
    int max_player_moves=moves_max.size();
    list<Move> moves_min = e_board.getValidMoves( invertTurn );
    list<Move>::iterator it_min = moves_min.begin();
    // cout<<moves_min;

    int min_player_moves=moves_min.size();
    // cout<<"minimum Player "<<min_player_moves<<endl;
    // cout<<"maxxx Player "<<max_player_moves<<endl;
    
    
    //potential corners (pseudo-expand node)
    int potentialCorners = 0;
    for(int i = 0; i < min_player_moves; i++,it_min++){
        if((*it_min).y == 0 && (*it_min).x == 0)
            potentialCorners++;
        else if((*it_min).y == 7 && (*it_min).x == 7) 
            potentialCorners++;
        else if((*it_min).y == 7&& (*it_min).x == 0) 
            potentialCorners++;
        else if((*it_min).y == 0 && (*it_min).x == 7) 
            potentialCorners++;
    }
    

   


    if ( max_player_moves + min_player_moves != 0)
    mobility =100 * (max_player_moves - min_player_moves) / (max_player_moves + min_player_moves);
    else
    mobility = 0;

//COIN PAIRITY
    double max_coins,min_coins;
    if(turn==1)
    {
        max_coins=e_board.getBlackCount();
        min_coins=e_board.getRedCount();
    }
    else if(turn==2)
    {
        max_coins=e_board.getRedCount();
        min_coins=e_board.getBlackCount();
    }


    coin_pairity = 100*((max_coins - min_coins)/(max_coins+min_coins));

    //Corner 
    int max_player_corners =0,min_player_corners=0;
    if(e_board.get(0,0)==turn)
        max_player_corners+=1;
    else if(e_board.get(0,0)==invertTurn) 
        min_player_moves+=1;
    if(e_board.get(0,7)==turn)
        max_player_corners+=1;
    else if(e_board.get(0,7)==invertTurn) 
        min_player_corners+=1;
    if(e_board.get(7,0)==turn)
        max_player_corners+=1;
    else if(e_board.get(7,0)==invertTurn) 
        min_player_corners+=1;
    if(e_board.get(7,7)==turn)
        max_player_corners+=1;
    else if(e_board.get(7,7)==invertTurn) 
        min_player_corners+=1;

    // cout<<"max player corners "<<max_player_corners<<endl;
    // cout<<"min player corners "<<min_player_corners<<endl;

    if ( max_player_corners+min_player_corners != 0)
        cornerh =100 * (max_player_corners- min_player_corners) / (max_player_corners + min_player_corners);
    else
        cornerh = 0;

    // EDGES
    int max_edge=0,min_edge=0;
    for(int i=0;i<8;i++)
    {
        if(e_board.get(0,i)==turn)
            max_edge+=1;
        else if(e_board.get(0,i)==invertTurn)
            min_edge+=1;
        if(e_board.get(7,i)==turn)
            max_edge+=1;
        else if(e_board.get(7,i)==invertTurn)
            min_edge+=1;
         if(e_board.get(i,0)==turn)
            max_edge+=1;
        else if(e_board.get(i,0)==invertTurn)
            min_edge+=1;
         if(e_board.get(i,7)==turn)
            max_edge+=1;
        else if(e_board.get(i,7)==invertTurn)
            min_edge+=1;
    }   

    // cout<<"edges soemthign : "<<edges<<endl;
    if((max_edge+min_edge)==0)
    edges=0;
    else
    edges = 100.0*(max_edge-min_edge)/(max_edge + min_edge);





    //weights
    int mobW = 15*(100-(max_coins+min_coins));
    int cornerW=7000;
    int edgew=100;
    int pair=3*(max_coins+min_coins);
    int potentialCW = -10000;

    // e_board.print();
    
    // cout<<"mobility :"<<mobility<<endl;
    // cout<<"coin pairity :"<<coin_pairity<<endl;
    // cout<<"edges : "<<edges<<endl;
    // cout<<"corner : "<<cornerh<<endl;
    // cout<<"Returning : "<<mobW*mobility+pair*coin_pairity+cornerW*cornerh+edgew*edges<<endl;
    
    return mobW*mobility+pair*coin_pairity+cornerW*cornerh+edgew*edges+potentialCW*potentialCorners;
}

double MyBot::alphabeta( const OthelloBoard& intermediate_board, double alpha, double beta)
{ // cout<<"here"<<endl;
    Turn current_turn;
    // intermediate_board.print();
    // cout<<"Time difference : "<<(((float)(tStart-clock()))/CLOCKS_PER_SEC)<<endl;
    if((((float)(clock()-tStart))/CLOCKS_PER_SEC) > 2*CUTOFF)
    {
        // timeout = true;
        cout<<"time out cutoff "<<endl;
        return e(intermediate_board);
        
    }
    
    count+=1;
    if(count%2!=0)
    {
        current_turn=other(turn);
        flag_max=0;
    }
    else
    {
        current_turn=turn;
        flag_max=1;
    }
    // cout<<"current time : "<<clock()<<endl; 
    // cout<<"starting_time taken : "<<tStart<<endl;
    // cout<<"Time difference : "<<(((float)(tStart-clock()))/CLOCKS_PER_SEC)<<endl;
    // if((((float)(clock()-tStart))/CLOCKS_PER_SEC) > 1)
    // {
    //     // timeout = true;
    //     cout<<"time out cutoff "<<endl;
    //     V=e(intermediate_board);
    //     break;
    // }
    
    if(count==kply)
    {
        // cout<<"inside kply"<<endl;
        V=e(intermediate_board);
        // intermediate_board.print();
        // count=0;
        // cout<<"count in v : "<<count<<endl;
        count--;
        // return V;
        // cout<<"cont decreased in V "<<endl;
      
        // cout<<"here"<<endl;      
        // kply--;
        // cout<<"V "<<V<<endl;
    }
    else
    {   
        
       
        // cout<<"count :"<<count<<endl;

        list<Move> move_loop = intermediate_board.getValidMoves( current_turn );
        list<Move>::iterator it_loop = move_loop.begin();
        
        // cout<<"count : "<<count<<endl;
        // cout<<"move loop size "<<move_loop.size()<<endl;

        if(move_loop.size()==0)
        {   
            // cout<<" count here : "<<count<<" move loop size here : "<<move_loop.size()<<endl;
            return e(intermediate_board);
        }

        for( int i=0;i<move_loop.size();i++)
        {   
            // cout<<" I : "<<i<<" count here : "<<count<<" move loop size here : "<<move_loop.size()<<endl;
             

            if(current_turn==turn)
            {
                OthelloBoard alpha_inter = intermediate_board;
                // alpha_inter.print(); 
                // cout<<"alpha turn : "<<current_turn<<endl;

                alpha_inter.makeMove(current_turn,*it_loop);
                // alpha_inter.print();
                // double prev_alpha = alpha;
                 double temp_alpha = alphabeta(alpha_inter,alpha,beta);
                 // cout<<"temp_alpha :"<<temp_alpha<<endl;
                // alpha=max(prev_alpha,temp_alpha);

                alpha=max(alpha,temp_alpha);
                // cout<< "alpha :"<<alpha<<endl;
                if(count == 0){
                    if(alpha == temp_alpha)
                        req_move = it_loop;
                }
                it_loop++ ; //to be checked later
                // cout<<"check "<<endl;
                // if(alpha>=beta)
                // {   
                //     // cout<<"cutoff in max "<<endl;
                //     // cout<<"beta in max "<<beta<<endl;
                //     count--;
                //     // count--;
                //     return beta;
                // }
                if(i==move_loop.size()-1)
                {   
                    // cout<<"alpha in max "<<alpha<<endl;
                    // cout<<"move loop end reached : in max"<<endl;
                    count--;
                    // count--;
                    return alpha;
                }

            }
            else if(current_turn==other(turn))
            {
                OthelloBoard beta_inter = intermediate_board;
                // cout<<"other of turn : "<<other(turn)<<endl;
                // cout<<"beta turn : "<<current_turn<<endl;

                beta_inter.makeMove(current_turn,*it_loop);
                double temp_beta= alphabeta(beta_inter,alpha,beta);
                // cout<<"temp beta : "<<temp_beta<<endl;
                beta=min(beta,temp_beta);
                it_loop++;   //to be checked later
                // if(alpha>=beta)
                // {   
                //     // cout<<"alpha in min "<<alpha<<endl;
                //      // cout<<"cutoff in min "<<endl;
                //     count--;
                //     // count--;
                //     return alpha;
                // }
                if(i==move_loop.size()-1)
                {   
                    // cout<<"beta in min "<<beta<<endl;
                    // cout<<"move loop end reached : in min"<<endl;

                    count--;
                    // count--;
                    return beta;
                }

            }
        }
        
        // cout<<"end of for loop ////"<<endl;
    }
    // cout<<"V the final :"<<V<<endl;
    return V;
}

Move MyBot::play( const OthelloBoard& board )
{
     
     count=-1;
     V=0;
     p=100000000000000;
     flag_max=1;
     alpha=-p;
     beta=p;
     kply=kp;
     int cost=0;
     now=time(0);
     tStart=clock();

    OthelloBoard intermediate_board;
    intermediate_board=board;
    // intermediate_board.print();

    list<Move> moves = board.getValidMoves( turn );
    req_move = moves.begin();
    cost=alphabeta(intermediate_board,alpha,beta);
    // cout<<"END OF FIRST ALPHA : "<<endl;
    // cout<<cost<<endl;
    // intermediate_board.print();
    // list<Move> moves = board.getValidMoves( turn );
    // int randNo = rand() % moves.size();
    // cout<<"Moves size :"<<moves.size()<<endl;
    list<Move>::iterator it = moves.begin();
    // cout<<moves.size();
   


    // unsigned ip = 0;
    // for(ip=0;ip<moves.size();ip++)
    // {  
    //     cout<<"IP "<<ip<<" cost at this IP : "<<cost<<endl;
    //    alpha=-p;
    //    beta=p;
    //    OthelloBoard temp_inter = board; 
    //    temp_inter.makeMove(turn,*it);
    //    // cout<<"i : "<<i<<endl;
    //    // cout<<"alfa beta : "<<alphabeta(temp_inter,alpha,beta)<<endl;
    //    // cout<<"cost : "<<cost<<endl;
    //    double check_cost = alphabeta(temp_inter,alpha,beta);
    //    cout<<"check cost : "<<check_cost<<endl;
    //    if(cost==check_cost)
    //     break;
    //    if(ip==moves.size()-1)
    //     break;
    //    it++;

    // }
    // cout<<"ip : "<<ip<<endl  ;
    // cout<<"MOVE IS OVER : YOUR TURN : "<<endl;
    return *req_move;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}


