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

#ifndef _CLASS
#define _CLASS
//#define ANDROID
#ifdef TEST
#define BLACK "¡ñ"
#define ZERO "©à"
#define WHITEN "¡ó"
#define BLACKN "¡ô"
#define WHITE "¡ð"
#endif
//#define USEDB
//#define TESTHASH
//#define FORSEEMK
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <time.h>
#ifndef ANDROID
#include <Windows.h>
#endif
using namespace std;

#ifndef ANDROID
//#define FOUL
#define N 20
//#define N 15
#else
#define N 15
#endif
#define P(X,Y) ((X)+(Y)*(S))

#ifndef min
#define min(X,Y) ((X)<(Y)?(X):(Y))
#endif
#ifndef max
#define max(X,Y) ((X)>(Y)?(X):(Y))
#endif

class line4;
class line4v;
class threat_line;
extern int S;
extern int moven;
extern int fflag;
extern int nosix;
extern long long int timee;
extern volatile long long int times;
extern long long int timel;
extern volatile long long int ts;
extern volatile int compend;
extern volatile int comphalfend;
extern bool gvstop;
extern double para[];
extern int boardSize;
extern const char version[];
extern volatile int thinklock;
extern int staticboard;
extern int computevcf;
extern long long int timestart;
extern long long int countx;
extern int movei;
extern int moven;
extern int nbest;
#ifdef ANDROID
extern int thinking;
extern int thinktime;
extern char curstatus[];
#endif

#ifndef ANDROID
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif

extern long long int nodelimit;

#define hashfEMPTY 0
#define hashfEXACT 1 
#define hashfALPHA 2 
#define hashfBETA 3 
#define valUNKNOWN (-32767-1)

#define DSHAPESIZE 92
#define LASTEVAL (para)
#define NEXTEVAL (para+DSHAPESIZE)
#define ATTACKVALUE (para+DSHAPESIZE*2)
#define DEFENDVALUE (para+DSHAPESIZE*3)
#define DRIFT (para[DSHAPESIZE*4])
#define DGN (para[DSHAPESIZE*4+1])
#define ATDOWN3 (para[DSHAPESIZE*4+2])
#define ATDOWN4 (para[DSHAPESIZE*4+3])
#define LASTWEIGHT (para[DSHAPESIZE*4+4])
#define READCONFIG (para[DSHAPESIZE*4+5])
#define EXTENDRATIO (para[DSHAPESIZE*4+6])

#define WIN 15000
//#define WINF 10000
#define INF 20000
#define NEXT5 (0x1000)
#define LAST5 (0x200)
#define NEXT4 (0x10)
#define NEXT43 (0x1)

#define GA(x) ((x)&0xff)
#define GB(x) (((x)&0xff00)>>8)
#define GC(x) (((x)&0xff0000)>>16)

#define L0 0
#define L1S 1
#define L1 2
#define L2S 3
#define L2BB 4
#define L2B 5
#define L2 6
#define L3S 7
#define L3B 8
#define L3 9
#define L4S 10
#define L4 11
#define L5 12
#define L6 13

typedef pair<short,short> eval;

typedef struct tagHASHE
{ 
	U64 key;  
	short value; 
	unsigned char hashf;
	unsigned char depth;
	unsigned short priority;
	short best;
}
HASHE;

extern int board[N][N];
extern int bmove;
void init();
int value1b(int x1,int y1,int c);
void revaluew();
void ValueWideCompute();
//int value1bWide(int x1,int y1,int c);
int moveValue1bWide(int x1,int y1,int c);
double evalValue1bWide0(int x1,int y1,int c);
double evalValue1bWide1(int x1,int y1,int c);
int attack1bWide(int x1,int y1,int c);
void ComputeShape1b(int x1,int y1,int d);
void ComputeValue1b(int x1,int y1);
double value(int c, int opo);
int VCFd_hash(int begin, int c, int depth);
int AIs();
int rootsearch(int pDepth, int pWide, int pRat1, int pRat2);
eval alphabeta(int wide,double depth,int alpha,int beta,int opo,int root,int downf,int _static);
int getmi(int x, int y, int c);
#ifdef FOUL
void convert();
#endif


class line
{
public:
	int x[N+4];	
	line(){}
	line(int a[]);
	//line(game& b,int p,int d);
	void line_set(int p,int d);
	unsigned int shape(int p);
	int A6(int p);
	int A5(int p);
	int A4(int p);
	int B4(int p);
	int B4p(int p);
	int A3(int p);
	int A3pb(int p);
#ifdef USEDB
	threat_line threat(int p);
#endif
};

class line4v
{

private:
public:
	int x1[N][N],x2[N][N],x3[2*N-1][N],x4[2*N-1][N];
	line4v();
	int A6(int x,int y);
	int A5(int x,int y);
	int A5test(int x,int y);
	int A4(int x,int y);
	int B4(int x,int y);
	int B4p(int x,int y);
	int overline(int x,int y);
	int double4(int x,int y);
	int A3r(int x,int y);
	int double3r(int x,int y);
	int foulr(int x,int y);
	int B4p(int c);
};

#ifdef FOUL

class manual
{
private:
	int state;
public:
	unsigned view[15];
	manual();
	~manual();
	int output();
	int output(int c);
	void ethis();
	void e();
	void l();
	void r();
	void ud();
	void lr();
	void lrl();
	void lrr();
	void lrud();
	void fileout();
	void fileout(int x,int y,std::ofstream& fout);
	void vectorout(int x,int y,bool sign);
	static void init();
	int d2out(int px,int py);
	void mmove();
};

class manualStep
{
public:
	int a[16];
	char b;
};

#endif

#endif