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
#include <unordered_map>
#include <algorithm>

#include <string>
#include "..\Headers\game.h"
#define OUTT 0
//#define RTVCF
#define VCFM 5
#ifndef LOWORD
#define LOWORD(X) (X&0xffff)
#endif
#ifndef HIWORD
#define HIWORD(X) ((X&0xffff0000)>>16)
#endif

using namespace std;

static bool finishflag;
static const int vec[16][2]={{-2,-2},{-1,-1},{2,2},{1,1},{-2,2},{-1,1},{2,-2},{1,-1},
	{2,0},{1,0},{0,2},{0,1},{-2,0},{-1,0},{0,-2},{0,-1}};

struct str_hash{
	size_t operator()(const wstring& str) const
	{
		unsigned long __h = 0; 
		for(size_t i=0; i<str.size(); i++)
		{
			__h=5*__h+str[i];
		}
		return str.size()*size_t(__h);
	}
};
static unordered_map<wstring, int, str_hash, equal_to<wstring>, allocator<pair<wstring,int>>> hm;
wstring Reorder(wstring s);
static wstring CurrentLine;
extern bool gvstop;

line4v line_4;

int VCFd_hash(int begin, int c, int depth)
{
	extern int computevcf;
	if(!computevcf)
	{
		return S*S+1;
	}
	if(compend)
	{
		gvstop=1;
	}
	if(gvstop)
	{
		return S*S+1;
	}
	if(begin)
	{
#ifndef RTVCF
		if(depth==8)
		{
			depth=min(depth,VCFM);
		}
		else
		{
			depth=min(depth,VCFM-1);
		}
#endif
	}
	if(depth==0)
	{
		finishflag=0;
		return S*S+1;
	}
	if(begin)
	{
		hm.clear();
		int VCFtemp=VCFd_hash(1, c, depth-1);
		if(VCFtemp<S*S)
		{
			return VCFtemp;
		}
		if(finishflag)
		{
			return S*S;
		}
		finishflag=1;
		//cout << "Depth " << depth << '\n' ;

		CurrentLine=L"";
	}

	int x,y;
	int x_temp,y_temp;
	if(begin)
	{
		line_4=line4v();
	}

	unordered_map<wstring, int, str_hash, equal_to<wstring>, allocator<pair<wstring,int>>>::iterator iter;
	iter=hm.find(Reorder(CurrentLine));
	if(iter!=hm.end())
	{
	//	cout << "found" << "\n" ;
		if(LOWORD((*iter).second)<=S*S)
		{
			return (*iter).second;
		}
		else
		{
			if(HIWORD((*iter).second)==depth)
			{
				return S*S+1;
			}
		}
	}

	int temp001=line_4.B4p(c);
	if(temp001)
	{
		hm[Reorder(CurrentLine)]=(temp001&65535)|(depth<<16);
		return temp001&65535;
	}
	temp001=line_4.B4p(-c);
	if(temp001&&temp001<(1<<24))
	{
		x_temp=(temp001&65535)%S;
		y_temp=(temp001&65535)/S;
		bmove++;
		line_4.x1[x_temp][y_temp]=c;
		line_4.x2[y_temp][x_temp]=c;
		line_4.x3[x_temp+y_temp][y_temp]=c;
		line_4.x4[S-1-y_temp+x_temp][S-1-y_temp]=c;

		if(line_4.A4(x_temp,y_temp))
		{
			if(!(line_4.foulr(x_temp,y_temp)))
			{
				line_4.x1[x_temp][y_temp]=0;
				line_4.x2[y_temp][x_temp]=0;
				line_4.x3[x_temp+y_temp][y_temp]=0;
				line_4.x4[S-1-y_temp+x_temp][S-1-y_temp]=0;
				bmove--;
				hm[Reorder(CurrentLine)]=(S*y_temp+x_temp)|(depth<<16);
				return S*y_temp+x_temp;
			}
		}
		int temp002=line_4.B4p(x_temp,y_temp);
		if(temp002)
		{
			if(!(line_4.foulr(x_temp,y_temp)))
			{
				int xx1=(temp002&65535)%S;
				int yy1=(temp002&65535)/S;

				line_4.x1[xx1][yy1]=-c;
				line_4.x2[yy1][xx1]=-c;
				line_4.x3[xx1+yy1][yy1]=-c;
				line_4.x4[S-1-yy1+xx1][S-1-yy1]=-c;
				bmove++;
				if(line_4.foulr(xx1,yy1))
				{
					line_4.x1[xx1][yy1]=0;
					line_4.x2[yy1][xx1]=0;
					line_4.x3[xx1+yy1][yy1]=0;
					line_4.x4[S-1-yy1+xx1][S-1-yy1]=0;
					bmove--;
					line_4.x1[x_temp][y_temp]=0;
					line_4.x2[y_temp][x_temp]=0;
					line_4.x3[x_temp+y_temp][y_temp]=0;
					line_4.x4[S-1-y_temp+x_temp][S-1-y_temp]=0;
					bmove--;
					hm[Reorder(CurrentLine)]=(S*y_temp+x_temp)|(depth<<16);
					return S*y_temp+x_temp;
				}
				else 
				{
					CurrentLine+=(wchar_t)(y_temp*S+x_temp+S);
					CurrentLine+=(wchar_t)(yy1*S+xx1+S);
					if(VCFd_hash(0, c, depth-1)<S*S)
					{	
						CurrentLine.erase(CurrentLine.length()-2,2);
						line_4.x1[xx1][yy1]=0;
						line_4.x2[yy1][xx1]=0;
						line_4.x3[xx1+yy1][yy1]=0;
						line_4.x4[S-1-yy1+xx1][S-1-yy1]=0;
						bmove--;
						line_4.x1[x_temp][y_temp]=0;
						line_4.x2[y_temp][x_temp]=0;
						line_4.x3[x_temp+y_temp][y_temp]=0;
						line_4.x4[S-1-y_temp+x_temp][S-1-y_temp]=0;
						bmove--;
						hm[Reorder(CurrentLine)]=(S*y_temp+x_temp)|(depth<<16);
						return S*y_temp+x_temp;
					}
					CurrentLine.erase(CurrentLine.length()-2,2);
				}
				line_4.x1[xx1][yy1]=0;
				line_4.x2[yy1][xx1]=0;
				line_4.x3[xx1+yy1][yy1]=0;
				line_4.x4[S-1-yy1+xx1][S-1-yy1]=0;
				bmove--;
			}
		}
		line_4.x1[x_temp][y_temp]=0;
		line_4.x2[y_temp][x_temp]=0;
		line_4.x3[x_temp+y_temp][y_temp]=0;
		line_4.x4[S-1-y_temp+x_temp][S-1-y_temp]=0;
		bmove--;
	}

	if(temp001==0)
	{
		for(x=0;x<S;x++)
		{
			for(y=0;y<S;y++)
			{
				int comflag=0;
				if(line_4.x1[x][y]==0)
				{
					for(int v=0;v<16;v++)
					{
						int xv=x+vec[v][0];
						int yv=y+vec[v][1];
						if(xv>=0&&xv<S&&yv>=0&&yv<S)
						{
							if(line_4.x1[xv][yv]==c)
							{
								comflag=1;
								break;
							}
						}
					}
				}
				if(comflag)
				{
					line_4.x1[x][y]=c;
					line_4.x2[y][x]=c;
					line_4.x3[x+y][y]=c;
					line_4.x4[S-1-y+x][S-1-y]=c;
					bmove++;
					if(line_4.A4(x,y))
					{
						if(!(line_4.foulr(x,y)))
						{
							line_4.x1[x][y]=0;
							line_4.x2[y][x]=0;
							line_4.x3[x+y][y]=0;
							line_4.x4[S-1-y+x][S-1-y]=0;
							bmove--;
							hm[Reorder(CurrentLine)]=(S*y+x)|(depth<<16);
							return S*y+x;
						}
					}
					line_4.x1[x][y]=0;
					line_4.x2[y][x]=0;
					line_4.x3[x+y][y]=0;
					line_4.x4[S-1-y+x][S-1-y]=0;
					bmove--;
				}
			}
		}

		for(x=0;x<S;x++)
		{
			for(y=0;y<S;y++)
			{
				int comflag=0;
				if(line_4.x1[x][y]==0)
				{
					for(int v=0;v<16;v++)
					{
						int xv=x+vec[v][0];
						int yv=y+vec[v][1];
						if(xv>=0&&xv<S&&yv>=0&&yv<S)
						{
							if(line_4.x1[xv][yv]==c)
							{
								comflag=1;
								break;
							}
						}
					}
				}
				if(comflag)
				{
					line_4.x1[x][y]=c;
					line_4.x2[y][x]=c;
					line_4.x3[x+y][y]=c;
					line_4.x4[S-1-y+x][S-1-y]=c;
					bmove++;

					int temp002=line_4.B4p(x,y);
					if(temp002&&!(line_4.foulr(x,y)))
					{
						int xx1=(temp002&65535)%S;
						int yy1=(temp002&65535)/S;

						line_4.x1[xx1][yy1]=-c;
						line_4.x2[yy1][xx1]=-c;
						line_4.x3[xx1+yy1][yy1]=-c;
						line_4.x4[S-1-yy1+xx1][S-1-yy1]=-c;
						bmove++;
						if(line_4.foulr(xx1,yy1))
						{	
							line_4.x1[xx1][yy1]=0;
							line_4.x2[yy1][xx1]=0;
							line_4.x3[xx1+yy1][yy1]=0;
							line_4.x4[S-1-yy1+xx1][S-1-yy1]=0;
							bmove--;
							line_4.x1[x][y]=0;
							line_4.x2[y][x]=0;
							line_4.x3[x+y][y]=0;
							line_4.x4[S-1-y+x][S-1-y]=0;
							bmove--;
							hm[Reorder(CurrentLine)]=(S*y+x)|(depth<<16);
							return S*y+x;
						}
						else 
						{
							CurrentLine+=(char)(y*S+x+S);
							CurrentLine+=(char)(yy1*S+xx1+S);
							if(VCFd_hash(0, c, depth-1)<S*S)
							{	
								CurrentLine.erase(CurrentLine.length()-2,2);			
								line_4.x1[xx1][yy1]=0;
								line_4.x2[yy1][xx1]=0;
								line_4.x3[xx1+yy1][yy1]=0;
								line_4.x4[S-1-yy1+xx1][S-1-yy1]=0;
								bmove--;
								line_4.x1[x][y]=0;
								line_4.x2[y][x]=0;
								line_4.x3[x+y][y]=0;
								line_4.x4[S-1-y+x][S-1-y]=0;
								bmove--;
								hm[Reorder(CurrentLine)]=(S*y+x)|(depth<<16);
								return S*y+x;
							}
							CurrentLine.erase(CurrentLine.length()-2,2);
						}
						line_4.x1[xx1][yy1]=0;
						line_4.x2[yy1][xx1]=0;
						line_4.x3[xx1+yy1][yy1]=0;
						line_4.x4[S-1-yy1+xx1][S-1-yy1]=0;
						bmove--;
					}
					line_4.x1[x][y]=0;
					line_4.x2[y][x]=0;
					line_4.x3[x+y][y]=0;
					line_4.x4[S-1-y+x][S-1-y]=0;
					bmove--;
				}
			}
		}
	}
	if(finishflag)
	{
		hm[Reorder(CurrentLine)]=(S*S)|(depth<<16);
		return S*S;
	}
	else
	{
		hm[Reorder(CurrentLine)]=(S*S+1)|(depth<<16);
		return S*S+1;
	}
}

wstring Reorder(wstring s)
{
	wchar_t *s1,*s2;
	int l=s.length();
	s1=new wchar_t[(l+1)/2];
	s2=new wchar_t[l/2];
	for(int i=0;i<(l+1)/2;i++)
	{
		s1[i]=s[2*i];
	}
	for(int i=0;i<l/2;i++)
	{
		s2[i]=s[2*i+1];
	}
	sort(s1,s1+(s.length()+1)/2);
	sort(s2,s2+(s.length())/2);
	for(int i=0;i<(l+1)/2;i++)
	{
		s[2*i]=s1[i];
	}
	for(int i=0;i<l/2;i++)
	{
		s[2*i+1]=s2[i];
	}
	delete[] s1;
	delete[] s2;
	return s;
}
