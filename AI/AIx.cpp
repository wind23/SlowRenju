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
//#include <time.h>
#include <cmath>
#include "..\Headers\game.h"
#ifndef ANDROID
#include <fstream>
#else
#include<android/log.h>
#endif
static int WIDE;
static int DEPTH;
static int RATIO1;
static int RATIO2;
#define COUNT 0
#define RAT1 1
#define RAT2 1
#define CHA3
#define DEPTHA 1
//#define FILEOUT
//#define RTVCF

using namespace std;

#ifdef ANDROID
#define TAG "SlowRenju" // 这个是自定义的LOG的标识 
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型 
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型 
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型 
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型 
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型 
#endif

static U64 CurrentZobrist;
static int depthL;
long long int countx;
static long long int countr,countw;
static long long int TIme;
bool gvstop=0;
static int curdepth;
static int maxdepth;
int rootmove[N][N]={0};
int rootsplit=0;
int compendchanged=0;
eval abval;
static int coverdir[32][2]={{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1},{-2,-2},{-2,-1},{-2,0},{-2,1},{-2,2},{-1,-2},{-1,2},{0,-2},{0,2},{1,-2},{1,2},{2,-2},{2,-1},{2,0},{2,1},{2,2},{-3,-3},{-3,0},{-3,3},{0,-3},{0,3},{3,-3},{3,0},{3,3}};
#ifdef ANDROID
char curstatus[10000] = "-1;-1;;0;0";
string cald;
#endif
//static double resource[20]={15.,3375.,759375.,170859375.,38445559375.,8649755859375.,1946195068359375.,437893890380859375.};

#ifdef FILEOUT
ofstream fout;
#endif

#ifdef TESTHASH
extern int countn;
#endif

void InitHash();
void SetHash(U64 key, int value, int depth, int priority, int hashf, int bestmove);
eval LookupHash(U64 key,int depth, int alpha, int beta);
extern U64 zobrist[2][N*N+1];

void backup(int *dst);
void load(int *src);

static int hashcount=0;
static int hashhit=0;

static int nbestval[400][3];

int rootbonus(int x, int y, int c)
{
	int is_corner = 0;
	int half_corner = 0;
	for (int i = 0; i < S; i++)
	{
		for (int j = 0; j < S; j++)
		{
			if (board[i][j])
			{
				int cur_height = min(i, j);
				cur_height = min(cur_height, S - 1 - i);
				cur_height = min(cur_height, S - 1 - j);
				if (cur_height <= 2)
				{
					if (cur_height == 2)
					{
						half_corner += 1;
						if (half_corner >= 2)
						{
							is_corner = 1;
							break;
						}
					}
					else
					{
						is_corner = 1;
						break;
					}
				}
			}
		}
		if (is_corner)
		{
			break;
		}
	}
	if (is_corner)
	{
		double bonus = 0;
		double height_score[4] = { 4, 3, 2, 1 };
		int height = min(x, y);
		height = min(height, S - 1 - x);
		height = min(height, S - 1 - y);
		if (height <= 3)
		{
			bonus += height_score[height];
		}
		double countalllist[9] = { 0,0,0,1,2,3,4,5,6 };
		int countall = 0;
		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				if (i >= 0 && j >= 0 && i < S&&j < S)
				{
					if (board[i][j] != 0)
					{
						countall += 1;
					}
				}
				else
				{
					countall += 1;
				}
			}
		}
		bonus += countalllist[countall] * 0.7;
		return int(round(bonus*1.0));
	}
	else
	{
		double bonus = 0;
		double height_score[4] = { 8, 4, 2, 1 };
		int height = min(x, y);
		height = min(height, S - 1 - x);
		height = min(height, S - 1 - y);
		if (height <= 3)
		{
			bonus += height_score[height];
		}
		return int(bonus);
	}
}

int rootsearch(int pDepth, int pWide, int pRat1, int pRat2)
{
	hashcount = 0;
	hashhit = 0;
#ifdef FILEOUT
	fout.open("E:\\output3.txt");
#endif
	gvstop = 0;
	compendchanged = 0;
	times = clock() + min(timee, timel / 15) - 100;
#ifndef ANDROID
	printf_s("MESSAGE Expected time=%lldms\n", min(timee, timel / 15) - 100);
#endif
	fflush(stdout);
	countx = 0;
#ifdef ANDROID
	sprintf(curstatus, "%d;%d;%s;%d;%d", -1, -1, "", 0, 0);
#endif
	TIme = clock();

#ifdef FOUL
	int move_temp;
	if (fflag)
	{
		if (bmove >= 3)
		{
			{
				int c;
				if (bmove % 2)
				{
					c = -1;
				}
				else
				{
					c = 1;
				}
				manual m;
				m.e();
				move_temp = m.output(c);
				if (move_temp < 225 || move_temp >= 65536)
				{
					goto end;
				}

				m.l();
				move_temp = m.output(c);
				if (move_temp != 225 || move_temp >= 65536)
				{
					goto end;
				}

				m.r();
				move_temp = m.output(c);
				if (move_temp < 225 || move_temp >= 65536)
				{
					goto end;
				}

				m.lr();
				move_temp = m.output(c);
				if (move_temp < 225 || move_temp >= 65536)
				{
					goto end;
				}

				m.ud();
				move_temp = m.output(c);
				if (move_temp < 225 || move_temp >= 65536)
				{
					goto end;
				}

				m.lrl();
				move_temp = m.output(c);
				if (move_temp < 225 || move_temp >= 65536)
				{
					goto end;
				}

				m.lrr();
				move_temp = m.output(c);
				if (move_temp < 225 || move_temp >= 65536)
				{
					goto end;
				}

				m.lrud();
				move_temp = m.output(c);
				if (move_temp < 225 || move_temp >= 65536)
				{
					goto end;
				}
			}
		}
	}

#endif



	//InitHash();
	DEPTH = pDepth;
	WIDE = pWide;
	RATIO1 = pRat1;
	RATIO2 = pRat2;
	CurrentZobrist = 0;
	for (int i = 0;i < S;i++)
	{
		for (int j = 0;j < S;j++)
		{
			if (board[i][j] == 1)
			{
				CurrentZobrist ^= zobrist[0][i + j*S];
			}
			else if (board[i][j] == -1)
			{
				CurrentZobrist ^= zobrist[1][i + j*S];
			}
		}
	}
	if (bmove <= 0)
	{
		return AIs();
	}
	if (bmove == 1)
	{
		if (N == 15 && board[7][7] == 1)
		{
			return (rand() % 2) ? (7 * N + 6) : (8 * N + 6);
		}
	}

	int vcf = VCFd_hash(1, bmove % 2 ? -1 : 1, 8);
	if (vcf < S*S)
	{
#ifndef ANDROID
		printf_s("MESSAGE Here it is!\n");
#endif
		fflush(stdout);
		return vcf;
	}

	int x0 = bmove % 2 ? -1 : 1;

	int pll;
	int vctt = 0;
	if (VCFd_hash(1, -x0, 7) < S*S)
	{
		vctt = 1;
	}
	for (int i = 0;i < S;i++)
	{
		int bkf = 0;
		for (int j = 0;j < S;j++)
		{
			if (board[i][j] == 0)
			{
				pll = i + j*S;
				bkf = 1;
				break;
			}
		}
		if (bkf)
		{
			break;
		}
	}
	curdepth = 0;
	maxdepth = 0;
	int maxdepthstatic = 0;
	int depthstatic = 0;

	ValueWideCompute();
	memset(&rootmove[0][0], 0, sizeof(int)*N*N);
	rootsplit = 0;
	for (int mov = 0;mov < S*S;mov++)
	{
		int value_temp = 0;
		int x_temp = mov%S;
		int y_temp = mov / S;

		if (board[x_temp][y_temp])
		{
			continue;
		}

		if (vctt)
		{
			board[x_temp][y_temp] = x0;
			bmove++;
			if (VCFd_hash(1, bmove % 2 ? -1 : 1, 7) < S*S)
			{
				board[x_temp][y_temp] = 0;
				bmove--;
				continue;
			}
			board[x_temp][y_temp] = 0;
			bmove--;
		}
		rootmove[x_temp][y_temp] = 1;
		rootsplit++;
	}

	abval = eval(0, -1);
	eval abval2 = eval(0, 0);
	eval abval3 = eval(0, 0);
	if (nbest > 0)
	{
		for (int i = 0;i < nbest;i++)
		{
			nbestval[i][0] = -1;
			nbestval[i][1] = -INF;
			nbestval[i][2] = -1;
		}
	}

	for(depthL=0;depthL<=DEPTH;depthL+=DEPTHA)
	{		
#ifdef FILEOUT
		fout << "depth " << depthL+1 << "\n{\n" ;
#endif
#ifdef ANDROID
		cald = "";
		sprintf(curstatus, "%d;%d;%s;%d;%d", abval.second, abval.second, cald.c_str(), depthL + 1, abval.first);
#endif
		abval3=abval2;
		abval2=abval;
		abval=alphabeta(pWide,depthL+1,-INF,INF,1,1,0,0);
		if(abval.first<=abval2.first-5||abval2.second!=abval.second||abval3.second!=abval2.second)
		{
			times=min(timee,timel/7)-100;
		}
		else
		{
			times=min(timee,timel/15)-100;
		}
#ifndef ANDROID
		if(clock()-ts>=times/3)
		{
			if (!nbest)
			{
				comphalfend = 1;
			}
			else
			{
				comphalfend = 0;
			}
		}
		else
		{
			comphalfend=0;
		}
#endif
#ifndef ANDROID
		if(depthL<=0||countx>=(depthL+1)*2)
		{
			printf_s("MESSAGE %2d;(%02d,%02d) %7d ---------- %8d\n",depthL+1,abval.second%S,abval.second/S,abval.first,clock()-TIme);
		}
		fflush(stdout);
#endif
#ifdef FILEOUT
		fout << "}\n" ;
#endif
		if(gvstop)
		{
			break;
		}
		maxdepthstatic=max(maxdepth,maxdepthstatic);
		depthstatic=max(depthL,depthstatic);
		if((nbest==0&&abval.first>=WIN)||abval.first<=-WIN||comphalfend)
		{
			depthL+=DEPTHA;
			break;
		}
	}
#ifdef FILEOUT
	fout.close();
#endif
	gvstop=false;
#ifndef ANDROID
	if (nbest > 0)
	{
		char outstr[8000] = "SRNBESTRESULT ";
		for (int i = 0;i < nbest;i++)
		{
			int valx = nbestval[i][0] % S;
			int valy = nbestval[i][0] / S;
			char curout[20];
			sprintf_s(curout, 20, "%d,%d,%d,%d;", valx, valy, nbestval[i][1], nbestval[i][2] + 1);
			strcat_s(outstr, 8000, curout);
		}
		printf_s("%s\n", outstr);
	}
	printf_s("MESSAGE Nodes=%lld, ",countx);
#endif
#ifdef TESTHASH
	countn+=countx;
#endif
	long long int TTime=clock()-TIme;
#ifndef ANDROID
	printf_s("Time=%dms, ",TTime);
#endif
	if(TTime>0)
	{
#ifndef ANDROID
		printf_s("speed=%lld, ",(long long int)countx*1000/TTime);
#endif
	}
	else
	{
#ifndef ANDROID
		printf_s("speed=0, ");
#endif
	}
	
#ifndef ANDROID
	printf_s("depth=%d-%d, Evaluation=%d\n",depthstatic+1,maxdepthstatic,abval.first);
	fflush(stdout);
#else
	thinktime = TTime;
	sprintf(curstatus, "%d;%d;%s;%d;%d", -1, -1, "", depthL + 1, abval.first);
#endif
	

	if(abval.second==-1)
	{
#ifndef ANDROID
		printf_s("MESSAGE Er...\n");
#endif
		fflush(stdout);
		return AIs();
	}
	else
	{
		return abval.second;
	}

#ifdef FOUL
end:
	return (move_temp%15)+(move_temp/15)*N;
#endif
}

int backValueWide[100][N*N+2*N*N*4+2*N*N+2*N*N];

eval alphabeta(int wide,double depth,int alpha,int beta,int opo,int root,int downf,int _static)
{
	if(nodelimit!=0&&countx>=nodelimit) {
		if(!compendchanged)
		{
			compend=1;
			compendchanged=1;
			//printf("MESSAGE change 3 compend=1\n");
			//fflush(stdout);
		}
	}
#ifdef ANDROID
	if(countx>=nodelimit/4) {
		//comphalfend=1;
	}
#endif
	if(compend)
	{
		gvstop=1;
	}
	if(gvstop)
	{
		return eval(0,-1);
	}
	countx++;

#ifdef FOUL
	int valuee[N*N]={-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000};
	int pl[N*N]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
#else
#ifdef ANDROID
	int valuee[N*N]={-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000};
	int pl[N*N]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
#else
	int valuee[N*N]={ -1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000,-1000000000};
	int pl[N*N]={ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
#endif
#endif
	int mis[N*N]={0};
	int pll=-1;
	int casen=0;
	int c=bmove%2?1:-1;

	int current=-INF-1;
	int vv;
	int orialpha = alpha;
	int oribeta = beta;

	int hashf = hashfALPHA;
	int fFoundPv=0;
	eval val(valUNKNOWN, -1);
#ifdef FILEOUT
				for(int kki=0;kki<curdepth+1;kki++)
			{
				fout << '\t' ;
			}
		fout << hex << CurrentZobrist << ' ' << dec << countx << ' ' ;
#endif
	hashcount++;
	if ((val = LookupHash(CurrentZobrist, depth, alpha, beta)).first != valUNKNOWN) { 
		hashhit++;
	#ifdef FILEOUT
	fout << val.first << "\n" ;
#endif
		return val ; 
	}
	extern bool bwindow;
	extern short windowa,windowb;
	if(bwindow&&!root)
	{
		alpha=max(windowa,alpha);
		beta=min(windowb,beta);
	}
	#ifdef FILEOUT
	fout << val.first << "\n" ;
#endif
	//int backValueWide[N*N+2*N*N*4+2*N*N+2*N*N];
	if(depth<=0)
	{
		backup(backValueWide[curdepth]);
		ValueWideCompute();
		vv=-value(c,opo);
#ifdef FILEOUT
			for(int kki=0;kki<curdepth+1;kki++)
			{
				fout << '\t' ;
			}
			fout << '{' << vv << '}' << endl ;
#endif
		if(vv>=WIN)
		{
			vv=INF-curdepth;
		}
		else if(vv<=-WIN)
		{
			vv=-INF+curdepth;
		}
		load(backValueWide[curdepth]);
#ifdef FILEOUT
		/*for(int kki=0;kki<curdepth+1;kki++)
		{
			fout << '\t' ;
		}*/
		//fout << "<" << countx << ">" << "\n" ;
#endif
		if(!compend)
		{
			SetHash(CurrentZobrist, vv, 0, moven*10+0, hashfEXACT, -1);
		}
		return eval(vv,-1);
	}
	else
	{
		int x0=bmove%2?-1:1;
		int vctt=0;
#ifdef RTVCF
		if(depth+6-2*DEPTH>0)
		{
			if(VCFd_hash(1,-x0,depth+6-2*DEPTH)<S*S)
			{
				vctt=1;
			}
		}
#endif
		/*{
			CurrentZobrist^=zobrist[bmove%2][N*N];
			curdepth++;
			bmove++;
			int e1=-alphabeta(min(wide*RATIO1/RATIO2+1,wide),depth-1-2,-(beta), -(alpha),1-opo,0,downf).first;
			curdepth--;
			bmove--;
			CurrentZobrist^=zobrist[bmove%2][N*N];
			countr++;
			if(e1>=beta)
			{
				if(!compend)
				{
					hashf=hashfBETA;
					SetHash(CurrentZobrist, e1, depth, moven*10+depth, hashf, -1);
				}
				return eval(e1,-1);
			}
		}*/

		int mov,x_temp,y_temp;
		backup(backValueWide[curdepth]);
		ValueWideCompute();
		int sglflag=0;
		int hsflag=0;
		//int blf=-1;
		int vbw1[N*N]={0};
		int at[N*N]={0};
		int at1pri=0;
		int at2pri=0;
		int winpri=0;
		int covered[N*N]={0}; //COVER/*
		for(y_temp=0;y_temp<S;y_temp++)
		{
			for(x_temp=0;x_temp<S;x_temp++)
			{
				if(board[x_temp][y_temp])
				{
					for(int i=0;i<32;i++)
					{
						int curx=x_temp+coverdir[i][0];
						int cury=y_temp+coverdir[i][1];
						if(curx>=0&&cury>=0&&curx<S&&cury<S)
						{
							covered[cury*S+curx]=1;
						}
					}
				}
			}
		}	//COVER*/
		for(y_temp=0;y_temp<S;y_temp++)
		{
			for(x_temp=0;x_temp<S;x_temp++)
			{		
				if(board[x_temp][y_temp]||(covered[y_temp*S+x_temp]==0))//COVER
				{
					continue;
				}

				int value_temp=0;
				mov=y_temp*S+x_temp;

				int vbw=moveValue1bWide(x_temp,y_temp,x0);
				int att1=attack1bWide(x_temp,y_temp,x0);
				int att2=attack1bWide(x_temp,y_temp,-x0);
				vbw1[mov]=vbw;
				if(vbw<=0)
				{
					at2pri=max(at2pri,att2);
					continue;
				}
				if(att2==6||att1>=5)
				{
					sglflag++;
					/*if(blf<vbw)
					{
						blf=vbw;
					}*/
				}
				else if(att2==5)
				{
					hsflag=1+mov;
				}
				at[mov]=att1;
				at1pri=max(at1pri,att1);
				at2pri=max(at2pri,att2);
			}
		}
		if(at1pri==6||(at1pri==5&&at2pri<=5))
		{
			winpri=1;
		}
#ifndef CHA3
		hsflag=0;
#endif
		if(!sglflag&&hsflag)
		{ 
			int movs=hsflag-1;
			int xs=movs%S;
			int ys=movs/S;
			int num3=0,sp=0;
			line li1,li2,li3,li4;
			li1.line_set(xs,0);
			li2.line_set(ys,1);
			li3.line_set(xs+ys,2);
			li4.line_set(xs-ys+S-1,3);
			if(li1.x[ys+2+1]==c)
			{
				sp=li1.A3pb(ys+1);
			}
			else if(li1.x[ys+2-1]==c)
			{
				sp=li1.A3pb(ys-1);
			}
			if(sp>0)
			{
				num3=1;
			}
			else
			{
				if(li2.x[xs+2+1]==c)
				{
					sp=li2.A3pb(xs+1);
				}
				else if(li2.x[xs+2-1]==c)
				{
					sp=li2.A3pb(xs-1);
				}
				if(sp>0)
				{
					num3=2;
				}
				else
				{
					if(li3.x[ys+2+1]==c)
					{
						sp=li3.A3pb(ys+1);
					}
					else if(li3.x[ys+2-1]==c)
					{
						sp=li3.A3pb(ys-1);
					}
					if(sp>0)
					{
						num3=3;
					}
					else
					{
						if(li4.x[S-1-ys+2+1]==c)
						{
							sp=li4.A3pb(S-1-ys+1);
						}
						else if(li4.x[S-1-ys+2-1]==c)
						{
							sp=li4.A3pb(S-1-ys-1);
						}
						if(sp>0)
						{
							num3=4;
						}
					}
				}
			}
			switch(num3)
			{
			case 1:
				if(sp<(1<<24))
				{
					int r1=GA(sp);
					int r2=GB(sp);
					vbw1[xs+r1*S]+=10000;
					vbw1[xs+r2*S]+=10000;
				}
				else
				{
					int r1=GA(sp);
					int r2=GB(sp);
					int r3=GC(sp);
					vbw1[xs+r1*S]+=10000;
					vbw1[xs+r2*S]+=10000;
					vbw1[xs+r3*S]+=10000;
				}
				break;
			case 2:
				if(sp<(1<<24))
				{
					int r1=GA(sp);
					int r2=GB(sp);
					vbw1[r1+ys*S]+=10000;
					vbw1[r2+ys*S]+=10000;
				}
				else
				{
					int r1=GA(sp);
					int r2=GB(sp);
					int r3=GC(sp);
					vbw1[r1+ys*S]+=10000;
					vbw1[r2+ys*S]+=10000;
					vbw1[r3+ys*S]+=10000;
				}
				break;
			case 3:
				if(sp<(1<<24))
				{
					int r1=GA(sp);
					int r2=GB(sp);
					vbw1[xs+ys-r1+r1*S]+=10000;
					vbw1[xs+ys-r2+r2*S]+=10000;
				}
				else
				{
					int r1=GA(sp);
					int r2=GB(sp);
					int r3=GC(sp);
					vbw1[xs+ys-r1+r1*S]+=10000;
					vbw1[xs+ys-r2+r2*S]+=10000;
					vbw1[xs+ys-r3+r3*S]+=10000;
				}
				break;
			case 4:
				if(sp<(1<<24))
				{
					int r1=GA(sp);
					int r2=GB(sp);
					vbw1[S-1+xs-ys-r1+(S-1-r1)*S]+=10000;
					vbw1[S-1+xs-ys-r2+(S-1-r2)*S]+=10000;
				}
				else
				{
					int r1=GA(sp);
					int r2=GB(sp);
					int r3=GC(sp);
					vbw1[S-1+xs-ys-r1+(S-1-r1)*S]+=10000;
					vbw1[S-1+xs-ys-r2+(S-1-r2)*S]+=10000;
					vbw1[S-1+xs-ys-r3+(S-1-r3)*S]+=10000;
				}
				break;
			}
		}

		int cover_range=0;
		int count_wide=0;
		for(y_temp=0;y_temp<S;y_temp++)
		{
			for(x_temp=0;x_temp<S;x_temp++)
			{		
				if(board[x_temp][y_temp]||(covered[y_temp*S+x_temp]==0)) //COVER
				{
					continue;
				}
				cover_range+=1;
				int value_temp=0;
				mov=y_temp*S+x_temp;
				int vbw=vbw1[mov];
				if(root&&!rootmove[x_temp][y_temp])
				{
					vbw-=5000;
				}
				if(hsflag)
				{
					vbw-=5000;
					if(at[mov]>=4)
					{
						vbw+=8000;
					}
				}
				if(vbw<=0)
				{
					continue;
				}
	#ifdef RTVCF
				if(vctt&&depth+6-2*DEPTH+2*(depthL-depthL)>0)
				{
					board[x_temp][y_temp]=x0;
					bmove++;
					if(VCFd_hash(1,bmove%2?-1:1,depth+6-2*DEPTH)<S*S)
					{
						board[x_temp][y_temp]=0;
						bmove--;
						continue;
					}
					board[x_temp][y_temp]=0;
					bmove--;
				}
	#endif
				value_temp=vbw-300000000;
				if(1)
				{
					if(mov==val.second)
					{
						int kkk=100;
						if(kkk>-10000000)
						{
							value_temp=kkk;
						}
					}
				}

				if(value_temp<-250000000&&value_temp>=500000-300000000)
				{
					if(!compend)
					{
						SetHash(CurrentZobrist, -(-INF+curdepth), depth, moven*10+depth, hashfEXACT, mov);
					}
					return eval(-(-INF+curdepth),mov);
				} 
				if(value_temp>=WIN)
				{
					return eval(value_temp,mov);
				}
				if(value_temp<=-WIN&&value_temp>=-200000000)
				{
					continue;
				}
				if(value_temp>-200000000||(value_temp>=-300000000&&value_temp<=250000000))
				{
					pl[casen]=mov;
					valuee[casen]=value_temp;
					casen++;
				}
			}
		}

		if(winpri)
		{
			load(backValueWide[curdepth]);
			int maxvalue=-2147483647;
			int maxpl=-1;
			for(int i=0;i<casen;i++)
			{
				if(valuee[i]>maxvalue)
				{
					maxvalue=valuee[i];
					maxpl=pl[i];
				}
			}
			return eval(INF,maxpl);
		}

		if(sglflag&&casen>=1)
		{
			int maxvalue=-2147483647;
			int maxpl=-1;
			for(int i=0;i<casen;i++)
			{
				if(valuee[i]>maxvalue)
				{
					maxvalue=valuee[i];
					maxpl=pl[i];
				}
			}
			valuee[0]=maxvalue;
			pl[0]=maxpl;
			casen=1;
		}
		if(root)
		{
			if(rootsplit<=0)
			{
				gvstop=1;
				return eval(-INF,-1);
			}
			else if(rootsplit==1)
			{
	#ifdef TESTHASH
				countn+=countx;
	#endif
				for(int i=0;i<S;i++)
				{
					for(int j=0;j<S;j++)
					{
						if(rootmove[i][j])
						{
							gvstop=1;
							return eval(abval.first,i+j*S);
						}
					}
				}
				assert(0);
			}
		}
		int vv=0;
		int oricasen=casen;
		casen=min(casen,wide);
		for(int i=0;i<casen;i++)
		{
			int maxvalue=-2147483647;
			int maxpl=-1;
			int maxj=-1;
			for(int j=i;j<oricasen;j++)
			{
				int curpl=pl[j];
				if(valuee[j]>maxvalue)
				{
					maxvalue=valuee[j];
					maxpl=curpl;
					maxj=j;
				}
				else if(valuee[j]==maxvalue)
				{
					int orimi,curmi;
					if(mis[maxpl]==0)
					{
						mis[maxpl]=orimi=getmi(maxpl%S,maxpl/S,-c);
					}
					else
					{
						orimi=mis[maxpl];
					}
					if(mis[curpl]==0)
					{
						mis[curpl]=curmi=getmi(curpl%S,curpl/S,-c);
					}
					else
					{
						curmi=mis[curpl];
					}
					if(curmi>orimi)
					{
						maxvalue=valuee[j];
						maxpl=curpl;
						maxj=j;
					}
				}
			}
			swap(valuee[i], valuee[maxj]);
			swap(pl[i], pl[maxj]);
#ifdef ANDROID
			if (root)
			{
				char plstr[10];
				sprintf(plstr, "%d", pl[i]);
				if (i == 0)
				{
					cald = string(plstr);
				}
				else
				{
					cald += "," + string(plstr);
				}
				sprintf(curstatus, "%d;%d;%s;%d;%d", pll<0?abval.second:pll, pl[i], cald.c_str(), depthL + 1, current>=-INF?current:abval.first);
			}
#endif
			CurrentZobrist^=zobrist[bmove%2][pl[i]];
			int score;
#ifdef FILEOUT
			for(int kki=0;kki<curdepth+1;kki++)
			{
				fout << '\t' ;
			}
			fout << "(" << pl[i]%S << "," << pl[i]/S << ")" << " " << i << "\n" ;
			for(int kki=0;kki<curdepth+1;kki++)
			{
				fout << '\t' ;
			}
			fout << "{\n" ;
#endif
			curdepth++;
			maxdepth=max(curdepth,maxdepth);
			
			board[pl[i]%S][pl[i]/S]=bmove%2?-1:1;
			bmove++;
			downf+=i;
			double depthdown=max(0.,1.-EXTENDRATIO+EXTENDRATIO*log((double)casen)/log((double)WIDE));//TO_RECOVER min(WIDE, cover_range)));
			int net=0;
			if(downf>=15)
			{
				net=downf/15;
				depthdown+=net;
				downf%=15;
			}
			int atdown=0;
			if(at[pl[i]]==4)
			{
				atdown=ATDOWN4;
			}
			else if(at[pl[i]]==3)
			{
				atdown=ATDOWN3; 
			}
			if (root)
			{
				atdown += rootbonus(pl[i] % S, pl[i] / S, bmove % 2 ? 1 : -1);
			}
			//atdown*=opo;
			if(_static)
			{
				depthdown=1.;
				net=0;
			}
			while(true)
			{
				if(fFoundPv)
				{
					score=-atdown-alphabeta(min(wide*RATIO1/RATIO2+1,wide),depth-depthdown,-(alpha+atdown)-1, -(alpha+atdown),1-opo,0,downf,_static).first;
					if(score>alpha && score<beta)
					{
						score=-atdown-alphabeta(min(wide*RATIO1/RATIO2+1,wide),depth-depthdown,-(beta+atdown), -(alpha+atdown),1-opo,0,downf,_static).first;
					}
				}
				else
				{
					score=-atdown-alphabeta(min(wide*RATIO1/RATIO2+1,wide),depth-depthdown,-(beta+atdown), -(alpha+atdown),1-opo,0,downf,_static).first;
				}
				if(score>=WIN)
				{
					break;
				}
				if(score>alpha&&score>current&&net>0)
				{
					depthdown-=net;
					net=0;
				}
				else
				{
					break;
				}
			}
			board[pl[i]%S][pl[i]/S]=0;
			bmove--;
			curdepth--;
#ifdef FILEOUT
			for(int kki=0;kki<curdepth+1;kki++)
			{
				fout << '\t' ;
			}
			fout << "}\n" ;
			for(int kki=0;kki<curdepth+1;kki++)
			{
				fout << '\t' ;
			}
			fout << "[" << score << "," << 1-opo << "]\n" ;
#endif			
			CurrentZobrist^=zobrist[bmove%2][pl[i]];
			countr++;
			if(gvstop)
			{
				if(root)
				{
					if(i==0)
					{
						return abval;
					}
					else if(current<=-WIN)
					{
						return eval(-INF,pl[i]);
					}
					else
					{
						return eval(current,pll);
					}
				}
				else
				{
					return eval(-INF,-1);
				}
			}
			if (root && nbest > 0 && !(compend || gvstop))
			{
				int iindex = 0;
				for (;iindex < nbest;iindex++)
				{
					if (nbestval[iindex][0] == pl[i] || nbestval[iindex][0] == -1||(iindex==nbest-1&&score>nbestval[iindex][1]))
					{
						nbestval[iindex][0] = pl[i];
						int changeflag = score - nbestval[iindex][1];
						nbestval[iindex][1] = score;
						nbestval[iindex][2] = depthL;
						if (changeflag > 0)
						{
							for (;iindex >= 1;iindex--)
							{
								if (nbestval[iindex][1] > nbestval[iindex - 1][1])
								{
									swap(nbestval[iindex][0], nbestval[iindex - 1][0]);
									swap(nbestval[iindex][1], nbestval[iindex - 1][1]);
									swap(nbestval[iindex][2], nbestval[iindex - 1][2]);
								}
								else
								{
									break;
								}
							}
						}
						else if (changeflag < 0)
						{
							for (;iindex <= nbest - 2;iindex++)
							{
								if (nbestval[iindex][1] < nbestval[iindex + 1][1])
								{
									swap(nbestval[iindex][0], nbestval[iindex + 1][0]);
									swap(nbestval[iindex][1], nbestval[iindex + 1][1]);
									swap(nbestval[iindex][2], nbestval[iindex + 1][2]);
								}
								else
								{
									break;
								}
							}
						}
						break;
					}
				}
			}
			if (score > current)
			{	
				current = score;
				if (root&&nbest)
				{
					pll = pl[i];
					hashf = hashfEXACT;
				}
			}
			if(score>alpha)
			{
				if (!(root&&nbest))
				{
					pll = pl[i];
					hashf = hashfEXACT;
				}
				if (root && nbest > 0)
				{
					alpha = max(alpha, nbestval[nbest - 1][1]);
				}
				else
				{
					alpha = score;
				}	
				fFoundPv=1;
			}
			//alpha=max(alpha,score);

			if (root&&nbest)
			{
				if (nbestval[nbest - 1][1] >= WIN)
				{
					break;
				}
			}
			else
			{
				if (score >= WIN)
				{
					break;
				}
				if (alpha >= beta)
				{
					hashf = hashfBETA;
					break;
				}
			}
		}
	}
	int windepth=0;
	if((current>=WIN&&current>orialpha)||(current<=-WIN&&current<oribeta))
	{
		hashf=hashfEXACT;
		windepth=10;
	}
	if(!compend)
	{
		SetHash(CurrentZobrist, current, depth+windepth, moven*10+depth, hashf, pll);
	}
	load(backValueWide[curdepth]);
	return eval(current,pll);
}

int getmi(int x, int y, int c)
{
	int ret=1;
	int ii,jj;
	ii=x+1,jj=y;
	for(;ii<=x+4&&ii<S;ii++)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	ii=x-1,jj=y;
	for(;ii>=x-4&&ii>=0;ii--)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	ii=x,jj=y+1;
	for(;jj<=y+4&&jj<S;jj++)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	ii=x,jj=y-1;
	for(;jj>=y-4&&jj>=0;jj--)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	ii=x+1,jj=y+1;
	for(;ii<=x+4&&ii<S&&jj<S;ii++,jj++)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	ii=x+1,jj=y-1;
	for(;ii<=x+4&&ii<S&&jj>=0;ii++,jj--)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	ii=x-1,jj=y+1;
	for(;ii>=x-4&&ii>=0&&jj<S;ii--,jj++)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	ii=x-1,jj=y-1;
	for(;ii>=x-4&&ii>=0&&jj>=0;ii--,jj--)
	{
		if(board[ii][jj]==-c)
		{
			break;
		}
		else
		{
			ret++;
		}
	}
	return ret;
}
