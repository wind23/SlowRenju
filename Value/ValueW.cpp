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
#include <cmath>
#include <cassert>
#include "..\Headers\game.h"

using namespace std;
//#define DRIFT (16.4716694747*2./3.)
//#define SA1 (0.804341136828)
//#define SB2 (0.986594370351)
//#define SA2 (8.7913351859)
//#define SB3 (11.9569723338)
//#define SA3 (28.0448027093)
//#define SB4 (26.5585575175)

void init()
{
	int i,j;
	for(i=0;i<S;i++)
	{
		for(j=0;j<S;j++)
		{
			board[i][j]=0;
		}
	}
	bmove=0;
	revaluew();
}

//static const int bg[8][2]={{1,2},{1,-2},{-2,1},{2,1},{-1,-2},{-1,2},{2,-1},{-2,-1}};
static int dir[9]={3,1,2,0,0,0,2,1,3};

double value(int c, int opo)
{
	double affensive=0;
	double defensive=0;
	int dgn=0;
	int tc=(1-c)/2;

	extern int shapeM[2][N][N][4];
	extern int valueM[2][N][N];
	for(int i=0;i<S;i++)
	{
		for(int j=0;j<S;j++)
		{
			if(board[i][j]==c)
			{
				int cc=1;

				for(int k=0;k<9;k++)
				{
					if(k!=4)
					{
						int ii=i-1+k/3;
						int jj=j-1+k%3;
						if(ii<0||jj<0||ii>=S||jj>=S||board[ii][jj])
						{
							cc++;
						}
						else if(((shapeM[tc][ii][jj][dir[k]]>>16)&15)==0)
						{
								cc++;
						}
					}
				}
				if(cc<=1)
				{
					dgn-=5;
				}
				else if(cc-1>=5)
				{
					dgn-=cc-1-3;
				}
			}
			else if(board[i][j]==-c)
			{
				int cc=1;

				for(int k=0;k<9;k++)
				{
					if(k!=4)
					{
						int ii=i-1+k/3;
						int jj=j-1+k%3;
						if(ii<0||jj<0||ii>=S||jj>=S||board[ii][jj])
						{
							cc++;
						}
						else if(((shapeM[1-tc][ii][jj][dir[k]]>>16)&15)==0)
						{
							cc++;
						}
					}
				}
				if(cc<=1)
				{
					dgn+=5;
				}
				else if(cc-1>=5)
				{
					dgn+=cc-1-3;
				}
			}
			else
			{
				affensive+=evalValue1bWide1(i,j,c);
				defensive+=evalValue1bWide0(i,j,-c);
			}
		}
	}
	double ret = affensive-defensive;
	if(ret>-32768&&ret<32768)
	{
		return ret-DRIFT+dgn*DGN;
	}
	else
	{
		int winv=floor((ret+32768)/65536.);
		if(winv<=-NEXT5/2)
		{
			return -WIN;
		}
		else if(winv>=LAST5/2)
		{
			int flag=0;
			int ci,cj;
			for(int i=0;i<S;i++)
			{
				for(int j=0;j<S;j++)
				{
					if(board[i][j]==0&&evalValue1bWide1(i,j,c)>=LAST5*65536/2)
					{
						ci=i;
						cj=j;
						flag=1;
						break;
					}
				}
				if(flag)
				{
					break;
				}
			}
			if(fflag&moveValue1bWide(ci,cj,-c)<0)
			{
				return WIN;
			}
			board[ci][cj]=-c;
			bmove++;
			//countx++;
			ValueWideCompute();
			ret=value(-c,1-opo);
			bmove--;
			board[ci][cj]=0;
			return -ret;
		}
		else if(winv<=-NEXT4/2)
		{
			return -WIN;
		}
		else if(winv<=-NEXT43)
		{
			int flag=0;
			int ci,cj;
			for(int i=0;i<S;i++)
			{
				for(int j=0;j<S;j++)
				{
					if(board[i][j]==0&&evalValue1bWide0(i,j,-c)>=NEXT43*65536/2)
					{
						ci=i;
						cj=j;
						board[i][j]=-c;
						bmove++;
						line li1,li2,li3,li4;
						li1.line_set(i,0);
						li2.line_set(j,1);
						li3.line_set(i+j,2);
						li4.line_set(i-j+S-1,3);
						int sp=0;
						int num3=0;
						sp=li1.B4p(j);
						if(sp>0)
						{
							num3=1;
						}
						else
						{
							sp=li2.B4p(i);
							if(sp>0)
							{
								num3=2;
							}
							else
							{
								sp=li3.B4p(j);
								if(sp>0)
								{
									num3=3;
								}
								else
								{
									sp=li4.B4p(S-1-j);
									if(sp>0)
									{
										num3=4;
									}
								}
							}
						}
						int r1=GA(sp);
						int blx, bly;
						switch(num3)
						{
						case 1:
							blx=i;
							bly=r1;
							break;
						case 2:
							blx=r1;
							bly=j;
							break;
						case 3:
							blx=i+j-r1;
							bly=r1;
							break;
						case 4:
							blx=S-1+i-j-r1;
							bly=S-1-r1;
							break;
						}
						board[blx][bly]=c;
						bmove++;
						li1.line_set(blx,0);
						li2.line_set(bly,1);
						li3.line_set(blx+bly,2);
						li4.line_set(blx-bly+S-1,3);
						if(!(li1.B4p(bly)||li2.B4p(blx)||li3.B4p(bly)||li4.B4p(S-1-bly)))
						{
							flag=1;
						}
						//countx += 2;
						board[blx][bly]=0;
						bmove--;
						board[i][j]=0;
						bmove--;
						if(flag)
						{
							break;
						}
					}
				}
				if(flag==1)
				{
					break;
				}
			}
			if(flag)
			{
				return -WIN;
			}
			else
			{
				return ret-65536*winv-DRIFT+dgn*DGN;
			}
		}
	}
	assert(0);
	return 0;
}