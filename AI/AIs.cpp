/*
	(C) 2012-2019  Tianyi Hao
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	*/


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "..\Headers\game.h"

using namespace std;

int AIs()
{
	if(bmove==0)
	{
		return (S/2)*S+(S/2);
	}
	
	int move_temp[N*N];
	int x_temp;
	int y_temp;
	int mov;
	int casen=0;
	int value=0;

	ValueWideCompute();
	for(mov=0;mov<S*S;mov++)
	{
		int value_temp=0;

		x_temp=mov%S;
		y_temp=mov/S;

		if(board[x_temp][y_temp])
		{
			continue;
		}

		int x0=(bmove+1)%2;
		if(x0==0)
		{
			x0=-1;
		}
		else
		{
			x0=1;
		}

		value_temp=value1b(x_temp,y_temp,x0);

		if(value_temp>=value)
		{
			if(value_temp>value)
			{
				casen=0;
			}
			value=value_temp;
			move_temp[casen]=mov;
			casen++;
		}		
	}

	//srand(unsigned(time(NULL)));
	int case_=rand()%casen;

	return move_temp[case_];
}
