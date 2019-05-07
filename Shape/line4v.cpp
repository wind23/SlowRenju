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
#define GA(x) ((x)&0xff)
#define GB(x) (((x)&0xff00)>>8)

int line4v::foulr(int x,int y)
{
	if(fflag==0)
	{
		return 0;
	}

	int sign=0;
	if(x1[x][y]==0)
	{
		sign=1;
		x1[x][y]=1;
		x2[y][x]=1;
		x3[x+y][y]=1;
		x4[S-1-y+x][S-1-y]=1;
	}
	if(x1[x][y]==-1)
	{
		return 0;
	}
	if(A5(x,y))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;

		return 0;
	}
	if(double4(x,y))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;
		return 2;
	}
	if(double3r(x,y))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;
		return 1;
	}
	if(overline(x,y))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;
		return 3;
	}
	x1[x][y]-=sign;
	x2[y][x]-=sign;
	x3[x+y][y]-=sign;
	x4[S-1-y+x][S-1-y]-=sign;

	return 0;
}

int line4v::B4p(int x,int y)
{
	line l1(x1[x]),l2(x2[y]),l3(x3[x+y]),l4(x4[S-1-y+x]);
	int p1=y,p2=x,p3=y,p4=S-1-y;
	int count=0;
	int count1,count2,count3,count4;
	count1=l1.B4p(p1);
	count2=l2.B4p(p2);
	count3=l3.B4p(p3);
	count4=l4.B4p(p4);
	if((count1|count2|count3|count4)>=(1<<16))
	{
		if(count1>=(1<<16))
		{ 
			return (1<<24)|((count1&255)*S+x);
		}
		else if(count2>=(1<<16))
		{
			return (1<<24)|(y*S+(count2&255));
		}
		else if(count3>=(1<<16))
		{
			return (1<<24)|((count3&255)*(S-1)+x+y);
		}
		else
		{
			return (1<<24)|(((S-1)-(count4&255))*(S+1)+x-y);
		}
	}
	switch((!!(count1))+((!!(count2))<<1)+((!!(count3))<<2)+((!!(count4))<<3))
	{
	case 0: return 0;break;
	case 1: 
		return (1<<16)|((count1&255)*S+x);
		break;
	case 2:
		return (1<<16)|(y*S+(count2&255));
		break;
	case 4:
		return (1<<16)|((count3&255)*(S-1)+x+y);
		break;
	case 8:
		return (1<<16)|(((S-1)-(count4&255))*(S+1)+x-y);
		break;
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
		return (1<<24)|((count1&255)*S+x);
		break;
	case 6:
	case 10:
	case 14:
		return (1<<24)|(y*S+(count2&255));
		break;
	case 12:
		return (1<<24)|((count3&255)*(S-1)+x+y);
		break;
	}
	return -1;
}

int line4v::B4p(int c)
{
	int i;
	int j;
	int count=0;

	for(i=0;i<S;i++)
	{
		for(j=0;j<S;j++)
		{
			if(x1[i][j]==c)
			{
				int temp=B4p(i,j);
				if(temp>=(1<<24))
				{
					return temp;
				}
				if(temp)
				{
					if(count&&count!=temp)
					{
						return (1<<24)|temp;
					}
					else
					{
						count=temp;
					}
				}
			}
		}
	}
	return count;
}

int line4v::A4(int x,int y)
{
	line l1(x1[x]),l2(x2[y]),l3(x3[x+y]),l4(x4[S-1-y+x]);
	int p1=y,p2=x,p3=y,p4=S-1-y;
	if(l1.A4(p1))
	{
		return 1;
	}
	if(l2.A4(p2))
	{
		return 1;
	}
	if(l3.A4(p3))
	{
		return 1;
	}
	if(l4.A4(p4))
	{
		return 1;
	}
	return 0;
}

line4v::line4v()
{
	if(&board==NULL)
	{
		return;
	}
	int i,j;

    for(i=0;i<S;i++)
    {
        for(j=0;j<S;j++)
        {
            x1[i][j]=board[i][j];
            x2[i][j]=board[j][i];
        }
    }
    
    for(i=0;i<S;i++)
    {
        for(j=0;j<=i;j++)
        {
            x3[i][j]=board[i-j][j];
            x4[i][j]=board[i-j][S-1-j];
        }
		for(;j<S;j++)
		{
			x3[i][j]=1024;
			x4[i][j]=1024;
		}
    }
    
    for(i=S;i<2*S-1;i++)
    {
		for(j=0;j<=i-S;j++)
		{
			x3[i][j]=1024;
			x4[i][j]=1024;
		}
        for(j=i-S+1;j<S;j++)
        {
            x3[i][j]=board[i-j][j];
            x4[i][j]=board[i-j][S-1-j];
        }
    }
}

int line4v::A5(int x,int y)
{
	if(x1[x][y]==0)
	{
		return 0;
	}
	line l1(x1[x]),l2(x2[y]),l3(x3[x+y]),l4(x4[S-1-y+x]);
	int p1=y,p2=x,p3=y,p4=S-1-y;
	if(l1.A5(p1))
	{
		return 1;
	}
	if(l2.A5(p2))
	{
		return 1;
	}
	if(l3.A5(p3))
	{
		return 1;
	}
	if(l4.A5(p4))
	{
		return 1;
	}
	return 0;
}

int line4v::double3r(int x,int y)
{
	if(A3r(x,y)>=2)
	{
		return 1;
	}
	return 0;
}

int line4v::double4(int x,int y)
{
	if(B4(x,y)>=2)
	{
		return 1;
	}
	return 0;
}

int line4v::overline(int x,int y)
{
	if(A6(x,y))
	{
		return 1;
	}
	return 0;
}

int line4v::A3r(int x,int y)
{
	line l1(x1[x]),l2(x2[y]),l3(x3[x+y]),l4(x4[S-1-y+x]);
	int p1=y,p2=x,p3=y,p4=S-1-y;
	int count=0;
	int ll1=l1.A3(p1);
	int ll2=l2.A3(p2);
	int ll3=l3.A3(p3);
	int ll4=l4.A3(p4);

	if(ll1)
	{
		if(ll1<65536)
		{
			int r=GA(ll1);
	//		cout << r << '\n' ;
			if(x1[x][y]==1&&(foulr(x,r)||A5test(x,r)))
			{
				count--;
			}
			count++;
		}
		else
		{
			int r1=GA(ll1);
			int r2=GB(ll1);
			if(x1[x][y]==1&&(foulr(x,r1)||A5test(x,r1))&&(foulr(x,r2)||A5test(x,r2)))
			{
				count--;
			}
			count++;
		}
	}
	if(ll2)
	{
		if(ll2<65536)
		{
			int r=GA(ll2);
	//		cout << r << '\n' ;
			if(x1[x][y]==1&&(foulr(r,y)||A5test(r,y)))
			{
				count--;
			}
			count++;
		}
		else
		{
			int r1=GA(ll2);
			int r2=GB(ll2);
			if(x1[x][y]==1&&(foulr(r1,y)||A5test(r1,y))&&(foulr(r2,y)||A5test(r2,y)))
			{
				count--;
			}
			count++;
		}
	}
//	cout << "count=" << count << '\n' ;
	if(ll3)
	{
		if(ll3<65536)
		{
			int r=GA(ll3);
	//		cout << r << '\n' ;
			if(x1[x][y]==1&&(foulr(x+y-r,r)||A5test(x+y-r,r)))
			{
				count--;
			}
			count++;
		}
		else
		{
			int r1=GA(ll3);
			int r2=GB(ll3);
			if(x1[x][y]==1&&(foulr(x+y-r1,r1)||A5test(x+y-r1,r1))&&(foulr(x+y-r2,r2)||A5test(x+y-r2,r2)))
			{
				count--;
			}
			count++;
		}
	}
	if(ll4)
	{
		if(ll4<65536)
		{
			int r=GA(ll4);
			if(x1[x][y]==1&&(foulr(S-1+x-y-r,S-1-r)||A5test(S-1+x-y-r,S-1-r)))
			{
				count--;
			}
			count++;
		}
		else
		{
			int r1=GA(ll4);
			int r2=GB(ll4);
			if(x1[x][y]==1&&(foulr(S-1+x-y-r1,S-1-r1)||A5test(S-1+x-y-r1,S-1-r1))&&(foulr(S-1+x-y-r2,S-1-r2)||A5test(S-1+x-y-r2,S-1-r2)))
			{
				count--;
			}
			count++;
		}
	}
	return count;
}

int line4v::B4(int x,int y)
{
	line l1(x1[x]),l2(x2[y]),l3(x3[x+y]),l4(x4[S-1-y+x]);
	int p1=y,p2=x,p3=y,p4=S-1-y;
	int count=0;
	count+=l1.B4(p1);
	count+=l2.B4(p2);
	count+=l3.B4(p3);
	count+=l4.B4(p4);
	return count;
}

int line4v::A5test(int x,int y)
{
	int sign=0;
	if(x1[x][y]==0)
	{
		sign=1;
		x1[x][y]=1;
		x2[y][x]=1;
		x3[x+y][y]=1;
		x4[S-1-y+x][S-1-y]=1;
	}
	line l1(x1[x]),l2(x2[y]),l3(x3[x+y]),l4(x4[S-1-y+x]);
	int p1=y,p2=x,p3=y,p4=S-1-y;
	if(l1.A5(p1))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;
		return 1;
	}
	if(l2.A5(p2))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;
		return 1;
	}
	if(l3.A5(p3))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;
		return 1;
	}
	if(l4.A5(p4))
	{
		x1[x][y]-=sign;
		x2[y][x]-=sign;
		x3[x+y][y]-=sign;
		x4[S-1-y+x][S-1-y]-=sign;
		return 1;
	}
	x1[x][y]-=sign;
	x2[y][x]-=sign;
	x3[x+y][y]-=sign;
	x4[S-1-y+x][S-1-y]-=sign;
	return 0;
}

int line4v::A6(int x,int y)
{
	line l1(x1[x]),l2(x2[y]),l3(x3[x+y]),l4(x4[S-1-y+x]);
	int p1=y,p2=x,p3=y,p4=S-1-y;
	if(l1.A6(p1))
	{
		return 1;
	}
	if(l2.A6(p2))
	{
		return 1;
	}
	if(l3.A6(p3))
	{
		return 1;
	}
	if(l4.A6(p4))
	{
		return 1;
	}
	return 0;
}