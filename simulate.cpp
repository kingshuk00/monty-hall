/******************************************************************************
    This simulates the Monty-Hall problem
    Copyright (C) 2018  Kingshuk

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

******************************************************************************/

#include<iostream>
#include<chrono>
#include<random>
#include<vector>

#include<cstdlib>

#ifdef DEBUG_SIM
int debug= 1;
#else
int debug= 0;
#endif

class Random {
public:
   inline Random()
      : _gen(std::chrono::system_clock::now().time_since_epoch().count())
   {
   }

   inline ~Random()
   {
   }

   inline int get(const int min, const int max)
   {
      std::uniform_int_distribution<int> dist(min, max);
      const int num= dist(_gen);
      return num;
   }

private:
   std::default_random_engine _gen;
};  // class Random

Random _random= Random();

class Game {
public:
   inline Game()
      : _goodie(-1)
   {
   }

   inline ~Game()
   {
   }

   inline void play()
   {
      _goodie= _random.get(0, 2);
      if(debug) {
         std::cout<< "good= "<< _goodie<< ", ";
      }
   }

   inline int openEmptyGate(const int firstChoice)
   {
      if(debug) {
         std::cout<< "choice= "<< firstChoice<< ", ";
      }
      int empty;
      if(_goodie== firstChoice) {
         empty= (_goodie+ _random.get(1, 2))% 3;
      } else {
         empty= 3- (_goodie+ firstChoice);
      }
      if(debug) {
         std::cout<< "empty= "<< empty<< ", ";
      }
      return empty;
   }

   inline bool check(const int choice)
   {
      return _goodie== choice;
   }

private:
   int _goodie;
};  // class Game


int main(int argc, char *argv[])
{
   int num;
   if(argc< 2|| 0== (num= atoi(argv[1]))) {
      num= 100;
   }

   Game game;
   std::vector<int> metric(2, 0);
   for(int i= 0; i< num; ++i) {
      game.play();
      int choice= _random.get(0, 2);
      const int empty= game.openEmptyGate(choice);
      const int change= _random.get(0, 1);
      if(change) {
         choice= 3- (choice+ empty);
      }
      const int isWin= game.check(choice)? 1: 0;
      if(debug) {
         std::cout<< "change= "<< change<< ", result= "<< isWin<< std::endl;
      }
      metric[change]+= isWin;
   }

   std::cout<< "no-change and win= "<< metric[0]<< std::endl;
   std::cout<< "   change and win= "<< metric[1]<< std::endl;

   return 0;
}
