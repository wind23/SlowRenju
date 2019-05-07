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
	
#include "..\Headers\game.h"
#include <iostream>
#include <assert.h>
#include <vector>
using namespace std;

static int boardM[N][N]={0};
int shapeM[2][N][N][4]={0};
int valueM[2][N][N]={0};
static int attackM[2][N][N]={0};
static vector<int> pretrd3;
static vector<int> trd3;

static int cmpmem[N][N]={0};

void revaluew()
{
	for(int i=0;i<S;i++)
	{
		for(int j=0;j<S;j++)
		{
			switch(board[i][j])
			{
			case 0:
				boardM[i][j] = 1; break;
			case 1:
				boardM[i][j] = -1; break;
			case -1:
				boardM[i][j] = 0; break;
			}
		}
	}
	ValueWideCompute();
	//memset(&boardM[0][0],0,N*N*sizeof(int)/sizeof(char));
	//memset(&valueM[0][0][0],0,2*N*N*sizeof(int)/sizeof(char));
	//memset(&shapeM[0][0][0][0],0,2*N*N*4*sizeof(int)/sizeof(char));
	//memset(&attackM[0][0][0],0,2*N*N*sizeof(int)/sizeof(char));
}

void backup(int *dst)
{
	memcpy(dst, &boardM[0][0], sizeof(int)*N*N);
	memcpy(dst+N*N, &shapeM[0][0][0][0], sizeof(int)*2*N*N*4);
	memcpy(dst+N*N+2*N*N*4, &valueM[0][0][0], sizeof(int)*2*N*N);
	memcpy(dst+N*N+2*N*N*4+2*N*N, &attackM[0][0][0], sizeof(int)*2*N*N);
}

void load(int *src)
{
	memcpy(&boardM[0][0], src, sizeof(int)*N*N);
	memcpy(&shapeM[0][0][0][0], src+N*N, sizeof(int)*2*N*N*4);
	memcpy(&valueM[0][0][0], src+N*N+2*N*N*4, sizeof(int)*2*N*N);
	memcpy(&attackM[0][0][0], src+N*N+2*N*N*4+2*N*N, sizeof(int)*2*N*N);
}

void ValueWideCompute()
{
	int ar=(nosix||fflag)?5:4;
	char Comp[N][N][4]={0};
	for(int i=0;i<S;i++)
	{
		if(memcmp(boardM[i],board[i],sizeof(int)*S))
		{
			for(int j=0;j<S;j++)
			{
				if(boardM[i][j]!=board[i][j])
				{
					(*(int*)Comp[i][j])=0x01010101;
					int f,ii,jj;
					ii=i,jj=j+1;
					f=0;
					for(;jj<S&&jj<=j+ar;jj++)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][0]=1;
					}
					ii=i,jj=j-1;
					f=0;
					for(;jj>=0&&jj>=j-ar;jj--)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][0]=1;
					}
					ii=i+1,jj=j;
					f=0;
					for(;ii<S&&ii<=i+ar;ii++)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][1]=1;
					}
					ii=i-1,jj=j;
					f=0;
					for(;ii>=0&&ii>=i-ar;ii--)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][1]=1;
					}
					ii=i+1,jj=j+1;
					f=0;
					for(;(jj<S&&ii<S&&jj<=j+ar&&ii<=i+ar);jj++,ii++)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][3]=1;
					}
					ii=i-1,jj=j-1;
					f=0;
					for(;jj>=0&&ii>=0&&jj>=j-ar&&ii>=i-ar;jj--,ii--)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][3]=1;
					}
					ii=i-1,jj=j+1;
					f=0;
					for(;jj<S&&ii>=0&&jj<=j+ar&&ii>=i-ar;jj++,ii--)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][2]=1;
					}
					ii=i+1,jj=j-1;
					f=0;
					for(;jj>=0&&ii<S&&jj>=j-ar&&ii<=i+ar;jj--,ii++)
					{
						if(f==0)
						{
							f=board[ii][jj];
						}
						else
						{
							if(board[ii][jj]&&board[ii][jj]!=f)
							{
								break;
							}
						}
						Comp[ii][jj][2]=1;
					}
				}
			}
		}
	}
	if (fflag)
	{
		pretrd3 = trd3;
		trd3.clear();
		for (int i = 0; i < pretrd3.size(); i++)
		{
			int xcur = pretrd3[i] % N;
			int ycur = pretrd3[i] / N;
			Comp[xcur][ycur][0] = 1;
			Comp[xcur][ycur][1] = 1;
			Comp[xcur][ycur][2] = 1;
			Comp[xcur][ycur][3] = 1;
		}
	}
	for(int i=0;i<S;i++)
	{
		if(memcmp(Comp[i][0],cmpmem[i],sizeof(int)*S))
		{
			for(int j=0;j<S;j++)
			{
				boardM[i][j]=board[i][j];
				if((*(int*)&Comp[i][j][0])&&board[i][j]==0)
				{
					if(Comp[i][j][0])
					{
						ComputeShape1b(i,j,0);
					}
					if(Comp[i][j][1])
					{
						ComputeShape1b(i,j,1);
					}
					if(Comp[i][j][2])
					{
						ComputeShape1b(i,j,2);
					}
					if(Comp[i][j][3])
					{
						ComputeShape1b(i,j,3);
					}
					ComputeValue1b(i,j);
				}
			}
		}
	}
}

int moveValue1bWide(int x1,int y1,int c)
{
	int c1=(c==1)?0:1;
	return ATTACKVALUE[valueM[c1][x1][y1]]+DEFENDVALUE[valueM[1-c1][x1][y1]];
}

double evalValue1bWide0(int x1,int y1,int c)
{
	int c1=(c==1)?0:1;
	return NEXTEVAL[valueM[c1][x1][y1]];
}

double evalValue1bWide1(int x1,int y1,int c)
{
	int c1=(c==1)?0:1;
	return LASTEVAL[valueM[c1][x1][y1]];
}

int attack1bWide(int x1,int y1,int c)
{
	int c1=(c==1)?0:1;
	//cout << x1 << ' ' << y1 << ' ' << c << ' ' << valueM[c1][x1][y1] << "\n" ;// system("pause");
	return attackM[c1][x1][y1];
}

void ComputeShape1b(int x1,int y1,int d)
{
	int x,y;
	//bmove++;

	x=x1;
	y=y1;

	board[x][y]=1;

	int lisx,lisp;
	switch(d)
	{
	case 0:lisx=x;lisp=y;break;
	case 1:lisx=y;lisp=x;break;
	case 2:lisx=x+y;lisp=y;break;
	case 3:lisx=S-1-y+x;lisp=S-1-y;break;
	}

	line lin4s;
	lin4s.line_set(lisx,d);
	//line4 lin4(*this,x,y);
	shapeM[0][x][y][d]=lin4s.shape(lisp);
	board[x][y]=-1;
	lin4s.x[lisp+2]=-1;

	shapeM[1][x][y][d]=lin4s.shape(lisp);
	board[x][y]=0;
	//bmove--;
}

int doubleShape[13][13]={
	{1},
	{2,3},
	{4,5,6},
	{7,8,9,10},
	{11,12,13,14,15},
	{16,17,18,19,20,21},
	{22,23,24,25,26,27,28},
	{29,30,31,32,33,34,35,36},
	{37,38,39,40,41,42,43,44,45},
	{46,47,48,49,50,51,52,53,54,55},
	{56,57,58,59,60,61,62,63,64,65,66},
	{67,68,69,70,71,72,73,74,75,76,77,78},
	{79,80,81,82,83,84,85,86,87,88,89,90,91},
};

void ComputeValue1b(int x1,int y1)
{
	int A3l=0,B4l=0,A5l=0,A6l=0;
	int x,y;

	int affensive1=0;
	int affensive2=0;

	x=x1;
	y=y1;

	int at=0;
	int lines[4]={0,0,0,0};
	int maxlines[2]={0,0};
	for(int i=0;i<4;i++)
	{
		int temp=(shapeM[0][x][y][i]>>16)&15;
		lines[i]=temp%L6;
		switch(temp)
		{
		case L3:
		case L3B:
			A3l++;at=max(at,3);break;
		case L4S:{
			int B4p=(shapeM[0][x][y][i])&15;
			B4l+=B4p;
			at=max(at,4);
			if(B4p>=2)
			{
				lines[i]=8;
			}
			break;
			   }
		case L5:A5l++;at=6;break;
		case L4:B4l++;at=max(at,5);break;
		case L6:A6l++;break;
		}
	}
	attackM[0][x][y]=at;
	if(lines[0]<lines[1])
	{
		swap(lines[0],lines[1]);
	}
	if(lines[2]<lines[3])
	{
		swap(lines[2],lines[3]);
	}
	if(lines[1]>=lines[2])
	{
		maxlines[0]=lines[0];
		maxlines[1]=lines[1];
	}
	else if(lines[3]>=lines[0])
	{
		maxlines[0]=lines[2];
		maxlines[1]=lines[3];
	}
	else if(lines[0]>=lines[2])
	{
		maxlines[0]=lines[0];
		maxlines[1]=lines[2];
	}
	else
	{
		maxlines[0]=lines[2];
		maxlines[1]=lines[0];
	}
	valueM[0][x][y]=doubleShape[maxlines[0]][maxlines[1]];
	if(fflag&&(!A5l))
	{
		if(B4l>=2||A6l)
		{
			valueM[0][x][y]=0;
			attackM[0][x][y] = 0;
		}
		else if (A3l >= 2)
		{
			trd3.push_back(x + y * N);
			line4v l4v;
			if (l4v.foulr(x1, y1))
			{
				valueM[0][x1][y1] = 0;
				attackM[0][x1][y1] = 0;
			}
		}
	}

	A3l=0,B4l=0,A5l=0,A6l=0;
	at=0;
	memset(lines,0,sizeof(int)*4);
	memset(maxlines,0,sizeof(int)*2);
	for(int i=0;i<4;i++)
	{
		int temp=(shapeM[1][x][y][i]>>16)&15;
		lines[i]=temp%L6;
		switch(temp)
		{
		case L3:
		case L3B:
			A3l++;at=max(at,3);break;
		case L4S:{
			int B4p=(shapeM[1][x][y][i])&15;
			B4l+=B4p;
			at=max(at,4);
			if(B4p>=2)
			{
				lines[i]=8;
			}
			break;
			   }
		case L5:A5l++;at=6;break;
		case L4:B4l++;at=max(at,5);break;
		case L6:A6l++;break;
		}
	}
	attackM[1][x][y]=at;
	if(lines[0]<lines[1])
	{
		swap(lines[0],lines[1]);
	}
	if(lines[2]<lines[3])
	{
		swap(lines[2],lines[3]);
	}
	if(lines[1]>=lines[2])
	{
		maxlines[0]=lines[0];
		maxlines[1]=lines[1];
	}
	else if(lines[3]>=lines[0])
	{
		maxlines[0]=lines[2];
		maxlines[1]=lines[3];
	}
	else if(lines[0]>=lines[2])
	{
		maxlines[0]=lines[0];
		maxlines[1]=lines[2];
	}
	else
	{
		maxlines[0]=lines[2];
		maxlines[1]=lines[0];
	}
	valueM[1][x][y]=doubleShape[maxlines[0]][maxlines[1]];
	attackM[1][x][y]=at;
}
