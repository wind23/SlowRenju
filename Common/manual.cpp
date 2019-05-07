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
#include <iomanip>
#include "..\Headers\game.h"
#define E 0
#define L 1
#define R 2
#define LR 3
#define UD 4
#define LRL 5
#define LRR 6
#define LRUD 7

using namespace std;

#ifdef FOUL

static FILE *fin=NULL;
static int boardm[N][N]={0};
static int bmovem=0;
//static game boardm;
std::vector<manualStep> manuall;
static manual md;

manual::manual()
{
	int i;

	state=E;
	for(i=0;i<15;i++)
	{
		view[i]=0;
	}
}

void manual::ethis()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(boardm[i][j]+4)%4;
		}
	}
	state=E;
}

void manual::e()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[i][j]+4)%4;
		}
	}
	state=E;
}

void manual::l()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[j][14-i]+4)%4;
		}
	}
	state=L;
}

void manual::r()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[14-j][i]+4)%4;
		}
	}
	state=R;
}

void manual::ud()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[14-i][14-j]+4)%4;
		}
	}
	state=UD;
}

void manual::lr()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[i][14-j]+4)%4;
		}
	}
	state=LR;
}

void manual::lrl()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[14-j][14-i]+4)%4;
		}
	}
	state=LRL;
}

void manual::lrr()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[j][i]+4)%4;
		}
	}
	state=LRR;
}

void manual::lrud()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		view[i]=0;
		for(j=14;j>=0;j--)
		{
			view[i]*=4;
			view[i]+=(board[14-i][j]+4)%4;
		}
	}
	state=LRUD;
}

manual::~manual()
{
}

int manual::output()
{
	int m(int m0,int state);
	
	for(int i=0;i<(int)manuall.size();i++)
	{
		if(manuall[i].a[0]==0xffffffff)
		{
			break;
		}
		bool comp=1;
		for(int j=0;j<15;j++)
		{
			if(manuall[i].a[j]!=view[j])
			{
				comp=0;
				break;
			}
		}
		if(comp)
		{
			int rt=m(manuall[i].a[15],state);
			if(rt==-1)
			{
				continue;
			}
			else
			{
				return rt;
			}
		}
	}

	return 225;
}

int manual::output(int c)
{
	int m(int m0,int state);
	int value(unsigned char cc, int c);
	vector<int> rt;
	vector<int> rt2;
	int valuee=-1;
	int valuef=-1000000000;
	

	{
		for(int i=0;i<(int)manuall.size();i++)
		{
			if(manuall[i].a[0]==0xffffffff)
			{
				break;
			}
			bool comp=1;
			for(int j=0;j<15;j++)
			{
				if(manuall[i].a[j]!=view[j])
				{
					comp=0;
					break;
				}
			}
			if(comp)
			{
				int rtt=m(manuall[i].a[15],state);
				int valuet=value(manuall[i].b,c);
				if(rtt==-1)
				{
					continue;
				}
				if(valuet>valuee)
				{
					valuee=valuet;
					rt.clear();
					rt.push_back(rtt);
				}
				else if(valuet==valuee)
				{
					rt.push_back(rtt);
				}
			}
		}
		if(rt.size()>0&&rt.size()<=4)
		{
			srand(unsigned(time(NULL)));
			return rt[rand()%rt.size()];
		}
		return 225;
	}
}

void manual::fileout()
{
	ofstream fout("manual.txt");
	fout << '{' ;
	for(int i=0;i<15;i++)
	{
		fout << "0x" ;
		fout << setfill('0') << setw(8) << hex << view[i] << ',' ;
		if(i==7)
		{
			fout << '\n' ;
		}
	}
	fout << "*+}," ;
}

void manual::fileout(int x,int y,ofstream& fout)
{
	fout << '\n' << "\t{" ;
	for(int i=0;i<15;i++)
	{
		fout << "0x" ;
		fout << setfill('0') << setw(8) << hex << view[i] << ',' ;
		if(i==7)
		{
			fout << '\n' << '\t' ;
		}
	}
	fout << dec << y << "*15+" << x << "}," ;
	fout << '\n' ;
}

int m(int m0,int state)
{
	int x,y;
	int x0=m0%15,y0=m0/15;
	switch(state)
	{
	case E: x=x0,y=y0; break;
	case L: x=y0,y=14-x0; break;
	case R: x=14-y0,y=x0; break;
	case UD: x=14-x0,y=14-y0; break;
	case LR: x=x0,y=14-y0; break;
	case LRL: x=14-y0,y=14-x0; break;
	case LRR: x=y0,y=x0; break;
	case LRUD: x=14-x0,y=y0; break;
	}

	if(x<0||x>=15||y<0||y>=15)
	{
		return -1;
	}
	else
	{
		return y*15+x;
	}
}
void manual::init()
{
	char baseFile[1024];
	GetModuleFileName(NULL, baseFile, 1000);
	for(int i=strlen(baseFile);i>=0;i--)
	{
		if(baseFile[i]!='\\')
		{
			baseFile[i]='\0';
		}
		else
		{
			break;
		}
	}
	strcat(baseFile, "M.lib");
	if((fin=fopen(baseFile,"rb"))==NULL)
	{
		cout << "MESSAGE ÆåÆ×¶ÁÈ¡Ê§°Ü£¡\n" << endl ;
		//MessageBox(NULL,"ÆåÆ×¶ÁÈ¡Ê§°Ü£¡","ÌáÊ¾ÐÅÏ¢",MB_ICONEXCLAMATION);
		return;
	}
	unsigned char ak[20];
	fread(ak,1,20,fin);

	convert();
	fclose(fin);
}

void convert()
{
	int finish=0;
	bool sign=true;
	int xtemp;
	int ytemp;
	for(;;)
	{
		unsigned char a[2]={0,0};
		int x,y;
		fread(a,1,2,fin);
		if(a[0]==0&&a[1]==0)
		{
			continue;
		}
		x=a[0]/16;
		y=15-(a[0]%16);
		switch(a[1]%16)
		{
		case 0:
			sign=false;
			break;
		case 1:
			sign=true;
			break;
		}
		switch(a[1]/16)
		{
		case 0:
			md.ethis();
			md.vectorout(x,y,sign);
			xtemp=x;
			ytemp=y;
			if(bmovem%2)
			{
				boardm[x][y]=-1;
			}
			else
			{
				boardm[x][y]=1;
			}
			bmovem++;
			convert();
			boardm[xtemp][ytemp]=0;
			bmovem--;
			finish=1;
			break;
		case 4:
			md.ethis();
			md.vectorout(x,y,sign);
			finish=1;
			break;
		case 8:
			md.ethis();
			md.vectorout(x,y,sign);
			xtemp=x;
			ytemp=y;
			if(bmovem%2)
			{
				boardm[x][y]=-1;
			}
			else
			{
				boardm[x][y]=1;
			}
			bmovem++;
			convert();
			boardm[xtemp][ytemp]=0;
			bmovem--;
			break;
		case 12:
			md.ethis();
			md.vectorout(x,y,sign);
			break;
		}
		
		if(finish)
		{
			break;
		}
	}
}

void manual::vectorout(int x,int y,bool sign)
{
	manualStep ms;
	unsigned char a[2];
	if(!sign)
	{
		for(int i=0;i<15;i++)
		{
			ms.a[i]=view[i];
		}
		ms.a[15]=y*15+x;
		ms.b=0;
	}
	else
	{
		for(int i=0;i<15;i++)
		{
			ms.a[i]=view[i];
		}
		ms.a[15]=y*15+x;
		fread(a,1,2,fin);
		fread(a,1,2,fin);
		ms.b=a[0];
	}
	manuall.push_back(ms);
}

int value(unsigned char ch, int c)
{
	int cc=0;
	if(ch=='*')
	{
		{
			ch='E';
		}
	}
	switch(ch)
	{
	case '!':
		return 1000;
	case '?':
		return -1;
	case '@':
		{
			ch='2';
		}
		break;
	case '#':
		{
			ch='1';
		}
		break;
	case '$':
		{
			ch='3';
		}
		break;
	case 'A':
	case 'h':
	case 'a':
		cc++;
	case 'B':
	case 'b':
		cc++;
	case 'C':
	case 'c':
		cc++;
	case 'D':
	case 'd':
		cc++;
	case 'E':
	case 'e':
		cc++;
	case 'F':
	case 'f':
		cc++;
	case 'G':
	case 'g':
		cc++;
	}
	if(cc>0)
	{
		if(c==-1)
		{
			cc=8-cc;
		}
		{
			return 2*cc-3;
		}
	}
	if(ch>='0'&&ch<='9')
	{
		return 20-(ch-'0');
	}
	return 0;
}

int manual::d2out(int px,int py)
{
	int c=1;
	for(int i=0;i<(int)manuall.size();i++)
	{
		if(manuall[i].a[0]==0xffffffff)
		{
			break;
		}
		bool comp=1;
		for(int j=0;j<15;j++)
		{
			if(manuall[i].a[j]!=view[j])
			{
				comp=0;
				break;
			}
		}
		if(comp)
		{
			int rtt=m(manuall[i].a[15],state);
			int valuet=value(manuall[i].b,c);
			if(rtt==-1)
			{
				continue;
			}
			if(rtt==px+py*15)
			{
				return valuet;
			}
		}
	}
	return -100000000;
}

void manual::mmove()
{
	while(true)
	{
		if(view[0]==0)
		{
			for(int i=0;i<14;i++)
			{
				view[i]=view[i+1];
			}
			view[14]=0;
		}
		else
		{
			break;
		}
	}
	while(true)
	{
		int dv;
			dv=1;
		for(int k=0;k<15;k++)
		{
			if(view[k]%4>0)
			{
				dv=0;
				break;
			}
		}
		if(dv)
		{
			for(int i=0;i<15;i++)
			{
				view[i]/=(unsigned)4;
			}
		}
		else
		{
			break;
		}
	}
	return;
}

#endif