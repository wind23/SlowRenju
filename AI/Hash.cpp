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
#include <cstdlib>
#include <ctime>
#include "..\Headers\game.h"
using namespace std;

U64 zobrist[2][N*N+1];


HASHE hash_table[1<<23][2];

U64 rand64()
{ 
	return rand() ^ ((U64)rand() << 15) ^ ((U64)rand() << 30) ^ ((U64)rand() << 45) ^ ((U64)rand() << 60); 
}

void InitHash()
{
#ifdef TESTHASH
	//srand(unsigned(1232356));
#else
	//srand(unsigned(1232356));
#endif
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<N*N+1;j++)
		{
			zobrist[i][j]=rand64();
		}
	}
	memset(hash_table[0],0,(1<<24)*sizeof(HASHE));
}


void SetHash(U64 key, int value, int depth, int priority, int hashf, int bestmove)
{
	int off=key&0x7fffff;
	HASHE *phashe=&hash_table[off][0];
	//if(phashe->hashf!=hashfEMPTY && phashe->priority>priority && !(phashe->hashf!=hashfEXACT && hashf==hashfEXACT))
	if(phashe->hashf!=hashfEMPTY && phashe->priority>priority)
	{
		phashe++;
	}
	phashe->key=key;
	phashe->value=value;
	phashe->best=bestmove;
	phashe->hashf=hashf;
	phashe->depth=depth;
	phashe->priority=priority;
}

bool bwindow=false;
short windowa=0,windowb=0;

eval LookupHash(U64 key,int depth, int alpha, int beta)
{
	bwindow=false;
	int off=key&0x7fffff;
	HASHE *phashe = &hash_table[off][0];
	if(phashe->key == key)
	{
		if(phashe->depth>=depth)
		{
			if(phashe->hashf == hashfEXACT)
			{
				return eval(phashe->value,phashe->best);
			}
			if(phashe->hashf == hashfALPHA)
			{
				if(phashe->value <= alpha)
				{
					return eval(phashe->value,-1);
				}
				else
				{
					bwindow=true;
					windowa=alpha;
					windowb=min(beta,phashe->value+1);
				}
			}
			if(phashe->hashf == hashfBETA && phashe->value >= beta)
			{
				if(phashe->value >= beta)
				{
					return eval(phashe->value,-1);
				}
				else
				{
					bwindow=true;
					windowa=max(alpha,phashe->value-1);
					windowb=beta;
				}
			}
		}
		return eval(valUNKNOWN, phashe->best);
	}
	else
	{
		phashe++;
		if(phashe->key == key)
		{
			if(phashe->depth>=depth)
			{
				if(phashe->hashf == hashfEXACT)
				{
					return eval(phashe->value,phashe->best);
				}
				if(phashe->hashf == hashfALPHA)
				{
					if(phashe->value <= alpha)
					{
						return eval(phashe->value,-1);
					}
					else
					{
						bwindow=true;
						windowa=alpha;
						windowb=min(beta,phashe->value+1);
					}
				}
				if(phashe->hashf == hashfBETA && phashe->value >= beta)
				{
					if(phashe->value >= beta)
					{
						return eval(phashe->value,-1);
					}
					else
					{
						bwindow=true;
						windowa=max(alpha,phashe->value-1);
						windowb=beta;
					}
				}
			}
			return eval(valUNKNOWN, phashe->best);
		}
	}
	return eval(valUNKNOWN,-1);
}