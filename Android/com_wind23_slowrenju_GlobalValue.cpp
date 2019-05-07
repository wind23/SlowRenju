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
//#include <AssetsManager.h>
using namespace std;
#include "../Headers/game.h"

#ifdef ANDROID
#include "com_wind23_slowrenju_GlobalValue.h"


int thinking = 0;
int thinktime = 0;
extern int board[N][N];
extern int bmove;
bool inited = 0;

/*
 * Class:     com_wind23_slowrenju_GlobalValue
 * Method:    getPos
 * Signature: ([[III)I
 */

JNIEXPORT jint JNICALL Java_com_wind23_slowrenju_GlobalValue_getPos
  (JNIEnv * env, jclass p2, jintArray p3, jint p4, jint p5)
{
	jint *carr = env->GetIntArrayElements(p3, NULL);
	bmove=0;
	for(int i=0;i<15;i++)
	{
		for(int j=0;j<15;j++)
		{
			board[i][j]=carr[i*15+j];
			if(board[i][j])
			{
				bmove++;
			}
		}
	}
	moven=bmove;
	if ((fflag && !p4) || (!fflag&&p4))
	{
		inited = 0;
	}
	fflag=p4;
	nosix=0;
	compend=0;
	comphalfend=0;
	env->ReleaseIntArrayElements(p3, carr, 0);
	extern int nodelimit;
	if(p5>0) {
		nodelimit=20000*(1<<(p5-1));
	}
	else if(p5==0) {
		nodelimit=-1;
	}
	else if(p5==-1) {
		nodelimit=0;
	}
	if (!inited)
	{
		void InitHash();
		InitHash();
		revaluew();
		inited = 1;
	}
	ts = clock();
	timee = 2147483647;
	timel = 2147483647;
	thinking = 1;
	int ret = rootsearch(24, 60, 1, 1);
	thinking = 0;
	return ret;
}

/*
 * Class:     com_wind23_slowrenju_GlobalValue
 * Method:    testWin
 * Signature: ([[II)I
 */
JNIEXPORT jint JNICALL Java_com_wind23_slowrenju_GlobalValue_testWin
  (JNIEnv * env, jclass p2, jintArray p3, jint p4, jint p5)
{
	jint *carr;
	carr = env->GetIntArrayElements(p3, NULL);
	bmove=0;
	for(int i=0;i<15;i++)
	{
		for(int j=0;j<15;j++)
		{
			board[i][j]=carr[i*15+j];
			if(board[i][j])
			{
				bmove++;
			}
		}
	}
	if ((fflag && !p4) || (!fflag&&p4))
	{
		inited = 0;
	}
	fflag=p4;
	nosix=0;
	if(bmove>=S*S)
	{
		return 2;
	}
	int x=p5%15;
	int y=p5/15;
	line4v lin4v=line4v();
	if(board[x][y]==1)
	{
		if(lin4v.A5(x,y))
		{
			env->ReleaseIntArrayElements(p3, carr, 0);
			return 1;
		}
		else if(lin4v.foulr(x,y))
		{
			env->ReleaseIntArrayElements(p3, carr, 0);
			return -1;
		}
	}
	else if(board[x][y]==-1)
	{
		if(lin4v.A5(x,y))
		{
			env->ReleaseIntArrayElements(p3, carr, 0);
			return -1;
		}
	}
	env->ReleaseIntArrayElements(p3, carr, 0);
	return 0;
}

/*
 * Class:     com_wind23_slowrenju_GlobalValue
 * Method:    stopThink
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_wind23_slowrenju_GlobalValue_stopThink
  (JNIEnv *, jclass)
{
	compend=1;
	return 0;
}

/*
* Class:     com_wind23_slowrenju_GlobalValue
* Method:    thinkingStatus
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_com_wind23_slowrenju_GlobalValue_thinkingStatus
(JNIEnv * env, jclass)
{
	char rets[10000] = "";
	sprintf(rets, "%d;%d;%d;%s", countx, nodelimit, (int)(thinking ? clock() - ts : thinktime), curstatus);
	return env->NewStringUTF(rets);
}
#endif