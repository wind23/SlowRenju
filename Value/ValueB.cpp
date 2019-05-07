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
#include "..\Headers\game.h"

using namespace std;

int value1b(int x1,int y1,int c)
{
	int A1l=0,B2l=0,A2l=0,B3l=0,A4l=0,A3l=0,B4l=0,A5l=0,A6l=0;
	int x,y;
	bmove++;

	int affensive=0;
	int defensive=0;

	x=x1;
	y=y1;

	extern int shapeM[2][N][N][4];
	int tc=(c==1)?0:1;
	unsigned int shape=(shapeM[tc][x][y][0])|(shapeM[tc][x][y][1]<<4)|(shapeM[tc][x][y][2]<<8)|(shapeM[tc][x][y][3]<<12);
	for(int i=0;i<4;i++)
	{
		int temp=(shape>>(16+4*i))&15;
		switch(temp)
		{
		case L1:A1l++;break;
		case L2S:
			B2l++;break;
		case L3:
		case L3B:
			A3l++;break;
		case L4S:B4l+=(shape>>(4*i))&15;break;
		case L5:A5l++;break;
		case L3S:B3l++;break;
		case L2:
		case L2B:
		case L2BB:
			A2l++;break;
		case L4:A4l++;B4l++;break;
		case L6:A6l++;break;
		}
	}
	affensive+=A1l;
	affensive+=B2l;
	affensive+=A2l*5;
	affensive+=B3l*10;
	affensive+=A3l*12;
	affensive+=B4l*16;
	affensive+=(A3l>=2?1:0)*100;
	affensive+=((B4l&&A3l)?1:0)*3000;
	affensive+=(B4l>=2?1:0)*4000;
	affensive+=A4l*6000;
	affensive+=A5l*1000000;
	if(fflag&&(!A5l)&&c==1)
	{
		if(B4l>=2)
		{
			affensive=-100000;
		}
		else if(A6l)
		{
			affensive=-100000;
		}
		else if (A3l >= 2)
		{
			line4v l4v;
			if (l4v.foulr(x1, y1))
			{
				affensive = -100000;
			}
		}
	}

	shape=(shapeM[1-tc][x][y][0])|(shapeM[1-tc][x][y][1]<<4)|(shapeM[1-tc][x][y][2]<<8)|(shapeM[1-tc][x][y][3]<<12);

	A1l=0,B2l=0,A3l=0,B4l=0,A5l=0,B3l=0,A2l=0,A4l=0,A6l=0;
	for(int i=0;i<4;i++)
	{
		int temp=(shape>>(16+4*i))&15;
		switch(temp)
		{
			//case 2:A1l++;break;
		//case 3:B2l++;break;
		case L3:
		case L3B:
			A3l++;break;
		case L4S:B4l+=(shape>>(4*i))&15;break;
		case L5:A5l++;break;
		case L3S:B3l++;break;
		case L2:
		case L2B:
		case L2BB:
			A2l++;break;
		case L4:A4l++;B4l++;break;
		case L6:A6l++;break;
		}
	}
	//defensive+=A1l;
	//defensive+=B2l;
	defensive+=A2l;
	defensive+=B3l;
	defensive+=A3l*6;
	defensive+=B4l*11;
	defensive+=(A3l>=2?1:0)*15;
	defensive+=((B4l&&A3l)?1:0)*1500;
	defensive+=(B4l>=2?1:0)*2000;
	defensive+=A4l*3000;
	//defensive+=0;
	defensive+=A5l*50000;

	bmove--;
	board[x][y]=0;

	return 5*affensive+5*defensive;
}
