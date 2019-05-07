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
	
#include <stdio.h>
#include <math.h>
#include <ctime>
#include "..\Headers\game.h"
#ifndef ANDROID
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <process.h>
#include <Windows.h>
using namespace std;

//#define TEST

static int movep[N*N];

void findmove(PVOID ptr);

int __Moves();
mutex mtx;
void readConfig();
void Thread(PVOID ptr);
void InitHash();
#ifdef FOUL
#endif

#ifndef TEST

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			para[i-1] = atof(argv[i]);
		}
	}
	else {
		readConfig();
	}
	srand(unsigned(1232356));
#ifdef FOUL
	manual::init();
#endif
	printf_s("MESSAGE SlowRenju %s is ready.\n", version);
	fflush(stdout);
	//game board;
	string s1;
	char s[1000];
	char com[1000];
	int self=-1;
	while(true)
	{
		cin.getline(s,999);
		sscanf_s(s,"%s",com,998);
		string cs(com);
		for(int i=0;i<(int)cs.length();i++)
		{
			if(cs[i]>='a'&&cs[i]<='z')
			{
				cs[i]+='A'-'a';
			}
		}

		if(thinklock==0)
		{
			if(cs=="START")
			{
				int size;
				sscanf_s(s,"%s %d",com,998,&size);
				self=-1;
				moven=0;
				nosix=0;
	#ifdef FOUL
				fflag=1;
	#endif
				if(size<=N&&size>=5)
				{
					//size=15;
					boardSize=size;
					InitHash();
					printf_s("OK\n") ;
					fflush(stdout);
				}
				else
				{
					printf("ERROR Size error.\n") ;
					fflush(stdout);
				}
			}
			else if(cs=="TURN")
			{
				int tx,ty;
				sscanf_s(s,"%s %d,%d",com,998,&tx,&ty);
				movep[moven]=tx+ty*N;
				moven++;
				_beginthread(findmove,0,NULL);
			}
			else if (cs == "SRNBEST")
			{
				int rnbest;
				sscanf_s(s, "%s %d", com, 998, &rnbest);
				nbest = rnbest;
			}
			else if(cs=="BEGIN")
			{
				self=1;
				_beginthread(findmove,0,NULL);
			}
			else if(cs=="BOARD")
			{
				int moveflist[225];
				int sfn=0,opn=0;
				while(true)
				{
					cin.getline(s,999);
					if(s[0]=='D'||s[0]=='d')
					{
						break;
					}
					else
					{
						int a,b,c;
						sscanf_s(s,"%d,%d,%d",&a,&b,&c);			
						if(c==1)
						{
							sfn++;
							moveflist[moven]=(1<<16)|(a+b*N);
						}
						else
						{
							opn++;
							moveflist[moven]=(2<<16)|(a+b*N);
						}
						moven++;
					}
				}
				if(sfn==opn)
				{
					self=1;
					int six=0,oix=0;
					for(int i=0;i<moven;i++)
					{
						if(moveflist[i]>>16==1)
						{
							movep[six*2]=moveflist[i]&0xffff;
							six++;
						}
						else
						{
							movep[oix*2+1]=moveflist[i]&0xffff;
							oix++;
						}
					}
				}
				else if(sfn==opn-1)
				{
					self=-1;
					int six=0,oix=0;
					for(int i=0;i<moven;i++)
					{
						if(moveflist[i]>>16==1)
						{
							movep[six*2+1]=moveflist[i]&0xffff;
							six++;
						}
						else
						{
							movep[oix*2]=moveflist[i]&0xffff;
							oix++;
						}
					}
				}
				else
				{
					printf_s("ERROR Board error.\n");
					fflush(stdout);
				}
				_beginthread(findmove,0,NULL);
			}
			else if(cs=="END")
			{
				break;
			}
			else if(cs=="RESTART")
			{
				self=-1;
				moven=0;
				InitHash();
				printf_s("OK\n");
				fflush(stdout);
			}
			else if(cs=="INFO")
			{
				char key[980];
				long long int value;
				sscanf_s(s,"%s %s %lld",com,998,key,978,&value);
				if(!strcmp(key,"timeout_turn"))
				{
					timee=value;
					if(timee==0)
					{
						timee=200;
					}
				}
				else if(!strcmp(key,"timeout_match"))
				{
					timel=value;
					if(timel==0)
					{
						timel=99999999;
					}
				}
				else if(!strcmp(key,"max_memory"))
				{
				}
				else if(!strcmp(key,"time_left"))
				{
					timel=value;
				}
				else if(!strcmp(key,"game_type"))
				{
				}
				else if(!strcmp(key,"rule"))
				{
	#ifdef FOUL
					value=4;
	#endif
					value=value&5;
					switch(value)
					{
					case 0:nosix=0;fflag=0;break;
					case 1:nosix=1;fflag=0;break;
					case 4:nosix=0;fflag=1;break;
					}
					//nosix=value%2;
				}
				else if(!strcmp(key,"evaluate"))
				{
				}
				else if(!strcmp(key,"folder"))
				{
				}
				else if(!strcmp(key,"static")) //self
				{
					staticboard=value%2;
				}
				else if(!strcmp(key,"max_node"))
				{
					nodelimit=value;
				}
				else if(!strcmp(key,"compute_vcf"))
				{
					computevcf=value;
				}
			}
			else if(cs=="ABOUT")
			{
				printf("name=\"SlowRenju\", version=\"%s\", author=\"Tianyi Hao\", country=\"China\", email=\"haotianyi0@126.com\", www=\"https://www.wind23.com/\"\n", version);
				fflush(stdout);
			}
			else if(cs=="TAKEBACK")
			{
				moven--;
				printf_s("OK\n");
				fflush(stdout);
			}
			else if(cs=="RECTSTART")
			{
				int size1,size2;
				sscanf_s(s,"%s %d,%d",com,998,&size1,&size2);
				if(size1==size2&&(size1<=N&&size1>=5))
				{
					self=-1;
					moven=0;
					nosix=0;
					boardSize=size1;
					InitHash();
					printf_s("OK\n");
					fflush(stdout);
				}
				else
				{
					printf_s("ERROR Size error.\n");
					fflush(stdout);
				}
			}
		}
		else
		{
			if(cs=="YXSTOP")
			{
				mtx.lock();
				compend=1;
				mtx.unlock();
			}
		}
	}
	return 0;
}

#else

int main()
{
	S=20;
	game board;
	readConfig();
	board.revaluew();
	board.board[3][3]=1;
	board.board[2][5]=-1;
	board.board[2][7]=1;
	board.board[4][7]=-1;
	board.board[5][8]=1;
	board.board[6][7]=-1;
	board.board[5][7]=1;
	board.board[5][6]=-1;
	board.board[4][5]=1;
	board.board[3][6]=-1;
	board.board[4][6]=1;
	board.board[6][5]=-1;
	board.board[3][8]=1;
	board.board[6][8]=-1;
	board.board[6][6]=1;
	board.board[4][8]=-1;
	board.board[4][4]=1;
	board.board[1][4]=-1;
	board.board[0][3]=1;
	board.board[5][5]=-1;
	board.board[7][4]=1;
	board.board[4][3]=-1;
	board.board[5][4]=1;
	board.board[6][4]=-1;
	board.board[5][2]=1;
	board.board[6][3]=-1;
	board.board[6][2]=1;
	board.board[5][3]=-1;
	board.board[4][2]=1;
	board.board[7][2]=-1;
	board.board[8][3]=1;
	board.board[5][9]=-1;
	board.board[2][4]=1;
	board.board[1][5]=-1;
	board.board[1][3]=1;
	board.board[3][5]=-1;
	board.board[4][10]=1;
	board.board[7][8]=-1;
	board.board[5][10]=1;
	board.board[8][9]=-1;
	board.board[9][10]=1;
	board.board[3][7]=-1;
	board.board[2][6]=1;
	board.board[6][10]=-1;
	board.board[7][11]=1;
	board.bmove=45;
	board.ValueWideCompute();
	int a=board.value(1,0);
	return 0;
}

#endif

void makemove(int x,int y)
{
	int rt=x+y*N;
	movep[moven]=rt;
	moven++;	
	thinklock=0;
	printf_s("%d,%d\n",rt%N,rt/N);
	fflush(stdout);
}

void findmove(PVOID ptr)
{
	thinklock=1;
	//timestart=clock();
	times=timestart+min(timee,timel/15)-100;
	if(READCONFIG)
	{
		readConfig();
	}
	if(moven==0)
	{
		makemove(boardSize/2,boardSize/2);
		return;
	}
	int a=N,b=-1,c=N,d=-1;
	if((!staticboard))
	{
		for(int i=0;i<moven;i++)
		{
			int ps=movep[i];
			int ps1=ps%N,ps2=ps/N;
			if(ps1<a)
			{
				a=ps1;
			}
			if(ps1>b)
			{
				b=ps1;
			}
			if(ps2<c)
			{
				c=ps2;
			}
			if(ps2>d)
			{
				d=ps2;
			}
		}
		a=(a<4?0:a-4);
		b=(b>boardSize-5?boardSize-1:b+4);
		c=(c<4?0:c-4);
		d=(d>boardSize-5?boardSize-1:d+4);
		int ctt=0;
		while(b-a!=d-c)
		{
			ctt++;
			if(b-a>d-c)
			{
				if(ctt%2)
				{
					if(c>0)
					{
						c--;
					}
					else
					{
						d++;
					}
				}
				else
				{
					if(d<boardSize-1)
					{
						d++;
					}
					else
					{
						c--;
					}
				}
			}
			else
			{
				if(ctt%2)
				{
					if(a>0)
					{
						a--;
					}
					else
					{
						b++;
					}
				}
				else
				{
					if(b<boardSize-1)
					{
						b++;
					}
					else
					{
						a--;
					}
				}
			}
		}
		S=b-a+1;
	}
	else
	{
		a=0;
		b=boardSize-1;
		c=0;
		d=boardSize-1;
		S=boardSize;
	}

	init();
	for(int i=0;i<moven;i++)
	{
		int ps=movep[i];
#ifndef FORSEEMK
		board[ps%N-a][ps/N-c]=i%2?-1:1;
#else
		if(moven%2==0)
		{
			board.board[ps%N-a][ps/N-c]=i%2?1:-1;
		}
		else
		{
			board.board[ps%N-a][ps/N-c]=i%2?-1:1;
		}
#endif
	}
	bmove=moven;
	int trt=__Moves();
	makemove(trt%S+a,trt/S+c);
}

int __Moves()	
{
	compend=0;
	//printf("MESSAGE change 0 compend=0\n");
	//fflush(stdout);
	comphalfend=0;
	ts=clock();
	HANDLE hThread=(HANDLE)_beginthread(Thread,0,NULL);
	int ret=rootsearch(24,60,1,1);
	//printf("MESSAGE xoxo compend=%d\n", compend);
	//fflush(stdout);
	mtx.lock();
	//printf("MESSAGE lock1 compend=%d\n", compend);
	//fflush(stdout);
	if(compend==2)
	{
		mtx.unlock();
		//printf("MESSAGE unlock1 in compend=%d\n", compend);
		//fflush(stdout);
		return ret;
	}
	compend=1;
	//printf("MESSAGE change 1 compend=1\n");
	//fflush(stdout);
	mtx.unlock();
	//printf("MESSAGE unlock1 compend=%d\n", compend);
	//fflush(stdout);
	while(true)
	{
		Sleep(1);
		if(compend==2)
		{
			break;
		}
	}
	return ret;
}

void Thread(PVOID ptr)					
{
	while(true)
	{
		Sleep(1);
		if(compend||clock()-ts>=times)
		{
			break;
		}
		if(clock()-ts>=times/3)
		{
			comphalfend=1;
		}
	}
	mtx.lock();
	//printf("MESSAGE lock2 compend=%d\n",compend);
	//fflush(stdout);
	compend=2;
	//printf("MESSAGE change 2 compend=2\n");
	//fflush(stdout);
	mtx.unlock();
	//printf("MESSAGE unlock2 compend=%d\n", compend);
	//fflush(stdout);
	_endthread();
}

void readConfig()
{
	char configFile[1024];
	GetModuleFileName(NULL, configFile, 1000);
	for(int i=strlen(configFile);i>=0;i--)
	{
		if(configFile[i]!='\\')
		{
			configFile[i]='\0';
		}
		else
		{
			break;
		}
	}
	strcat(configFile, "srconfig.txt");
	ifstream fin(configFile);
	int paraindex=0;
	if(fin)
	{
		string line;
		while(getline(fin,line))
		{
			if(line[0]=='#'||line[0]=='\0')
			{
				continue;
			}
			istringstream iss(line);
			while(iss >> para[paraindex])
			{
				paraindex++;
			};
		}
		para[156]+=65536.;
		para[157]+=65536.;
		fin.close();
	}
}
#endif