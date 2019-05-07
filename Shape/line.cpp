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
#define COMB(X, Y) (((X)<<8)|((Y)-2))
#define COMC(X, Y, Z) COMB(COMB((X), (Y)), (Z))
#define COMD(X, Y, Z, W) COMB(COMC((X), (Y), (Z)), (W))
//#define COME(X, Y, Z, W, V) COMB(COMD((X), (Y), (Z), (W)), (V))
using namespace std;

extern int nosix;
extern int fflag;

line::line(int a[])
{
	x[0]=1024;
	x[1]=1024;
	for(int i=0;i<S;i++)
	{
		x[i+2]=a[i];
	}
	x[S+2]=1024;
	x[S+3]=1024;
}

void line::line_set(int p,int d)
{
	x[0]=1024;
	x[1]=1024;
	x[S+2]=1024;
	x[S+3]=1024;
	switch(d)
	{
	case 0:
		for(int i=0;i<S;i++)
		{
			x[i+2]=board[p][i];
		}
		break;
	case 1:
		for(int i=0;i<S;i++)
		{
			x[i+2]=board[i][p];
		}
		break;
	case 2:
		if(p<S)
		{
			int i;
			for(i=0;i<=p;i++)
			{
				x[i+2]=board[p-i][i];
			}
			for(;i<S;i++)
			{
				x[i+2]=1024;
			}
		}
		else
		{
			int i;
			for(i=0;i<=p-S;i++)
			{
				x[i+2]=1024;
			}
			for(;i<S;i++)
			{
				x[i+2]=board[p-i][i];
			}
		}
		break;
	case 3:
		if(p<S)
		{
			int i;
			for(i=0;i<=p;i++)
			{
				x[i+2]=board[p-i][S-1-i];
			}
			for(;i<S;i++)
			{
				x[i+2]=1024;
			}
		}
		else
		{
			int i;
			for(i=0;i<=p-S;i++)
			{
				x[i+2]=1024;
			}
			for(;i<S;i++)
			{
				x[i+2]=board[p-i][S-1-i];
			}
		}
		break;
	}
}

unsigned int line::shape(int p)
{
	extern const unsigned char ShapeList[2][3969];
	int ssp=0;
	int si=0,sj=0;
	p+=2;
	switch(x[p])
	{
	case 1:
		{
			switch(x[p+1])
			{
			case 0:break;
			case 1:ssp|=16;break;
			default:goto lab1;
			}
			switch(x[p+2])
			{
			case 0:break;
			case 1:ssp|=8;break;
			default:sj=1;goto lab1;
			}
			switch(x[p+3])
			{
			case 0:break;
			case 1:ssp|=4;break;
			default:sj=2;goto lab1;
			}
			switch(x[p+4])
			{
			case 0:break;
			case 1:ssp|=2;break;
			default:sj=3;goto lab1;
			}
			switch(x[p+5])
			{
			case 0:break;
			case 1:ssp|=1;break;
			default:sj=4;goto lab1;
			}
			sj=5;
	lab1:
			switch(x[p-1])
			{
			case 0:break;
			case 1:ssp|=32;break;
			default:goto lab2;
			}
			switch(x[p-2])
			{
			case 0:break;
			case 1:ssp|=64;break;
			default:si=1;goto lab2;
			}
			switch(x[p-3])
			{
			case 0:break;
			case 1:ssp|=128;break;
			default:si=2;goto lab2;
			}
			switch(x[p-4])
			{
			case 0:break;
			case 1:ssp|=256;break;
			default:si=3;goto lab2;
			}
			switch(x[p-5])
			{
			case 0:break;
			case 1:ssp|=512;break;
			default:si=4;goto lab2;
			}
			si=5;
	lab2:
			ssp>>=(5-sj);
			int trt=ShapeList[fflag||nosix][(1<<si)*((1<<sj)+62)-63+ssp];
			return ((trt&0xf0)<<12)|(trt&0xf);
		}
	case -1:
		{
			switch(x[p+1])
			{
			case 0:break;
			case -1:ssp|=16;break;
			default:goto lab3;
			}
			switch(x[p+2])
			{
			case 0:break;
			case -1:ssp|=8;break;
			default:sj=1;goto lab3;
			}
			switch(x[p+3])
			{
			case 0:break;
			case -1:ssp|=4;break;
			default:sj=2;goto lab3;
			}
			switch(x[p+4])
			{
			case 0:break;
			case -1:ssp|=2;break;
			default:sj=3;goto lab3;
			}
			switch(x[p+5])
			{
			case 0:break;
			case -1:ssp|=1;break;
			default:sj=4;goto lab3;
			}
			sj=5;
	lab3:
			switch(x[p-1])
			{
			case 0:break;
			case -1:ssp|=32;break;
			default:goto lab4;
			}
			switch(x[p-2])
			{
			case 0:break;
			case -1:ssp|=64;break;
			default:si=1;goto lab4;
			}
			switch(x[p-3])
			{
			case 0:break;
			case -1:ssp|=128;break;
			default:si=2;goto lab4;
			}
			switch(x[p-4])
			{
			case 0:break;
			case -1:ssp|=256;break;
			default:si=3;goto lab4;
			}
			switch(x[p-5])
			{
			case 0:break;
			case -1:ssp|=512;break;
			default:si=4;goto lab4;
			}
			si=5;
	lab4:
			ssp>>=(5-sj);
			int trt=ShapeList[nosix][(1<<si)*((1<<sj)+62)-63+ssp]; 
			return ((trt&0xf0)<<12)|(trt&0xf);
		}
	default:
		return 0;
	}
}

int line::A6(int p) 
{
	if(!fflag)
	{
		return 0;
	}
	p+=2;
	if(x[p]!=1)
	{
		return 0;
	}
	int xmin=2<p-5?p-5:2;
	int xmax=S-4<p?S-4:p;
	for(int i=xmin;i<=xmax;i++)
	{
		int num=x[i]+x[i+1]+x[i+2]+x[i+3]+x[i+4]+x[i+5];
		if(num==6)
		{
			return 1;
		}
	}
	return 0;
}

int line::A5(int p)
{
	p+=2;
	int x0=x[p];
	if(x0==0)
	{
		return 0;
	}
	int xmin=2<p-4?p-4:2;
	int xmax=S-3<p?S-3:p;
	for(int i=xmin;i<=xmax;i++)
	{
		int num=x[i]+x[i+1]+x[i+2]+x[i+3]+x[i+4];
		if(num==5*x0)
		{
			if(!(nosix||(fflag&&x0==1)))
			{
				if(x0==1)
				{
					if(1)   //XXXXX
					{
						return 1;
					}
				}
				else                //OOOOO
				{
					return 1;
				}
			}
			else
			{
				if(x[i-1]!=x0&&x[i+5]!=x0)   //XXXXX
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int line::A4(int p)
{
	p+=2;
	int x0=x[p];
	if(x0==0)
	{
		return 0;
	}
	int xmin=2<p-3?p-3:2;
	int xmax=S-2<p?S-2:p;
	for(int i=xmin;i<=xmax;i++)
	{
		int num=x[i]+x[i+1]+x[i+2]+x[i+3];
		if(num==4*x0)
		{
			if(!(nosix||(fflag&&x0==1)))
			{
				if(x0==1)         //XXXX
				{
					if((x[i-1]==0)&&(x[i+4]==0))
					{
						return 1;
						//return COMC(1,i-1,i+4);
					}
				}
				else                //OOOO
				{
					if((x[i-1]==0)&&(x[i+4]==0))
					{
						return 1;
						//return COMC(1,i-1,i+4);
					}
				}
			}
			else
			{
				if((x[i-1]==0)&&(x[i+4]==0)&&x[i-2]!=x0&&x[i+5]!=x0)
				{
					return 1;
					//return COMC(1,i-1,i+4);
				}
			}
		}
	}
	return 0;
}

int line::B4(int p)
{
	p+=2;
	int x0=x[p];
	if(x0==0)
	{
		return 0;
	}
	int xmin=2<p-4?p-4:2;
	int xmax=S-3<p?S-3:p;
	for(int i=xmin;i<=xmax;i++)
	{
		int num=x[i]+x[i+1]+x[i+2]+x[i+3]+x[i+4];
		if(num==4*x0)
		{
			int shape=(x[i]<<4)+(x[i+1]<<3)+(x[i+2]<<2)+(x[i+3]<<1)+x[i+4];
			if(!(nosix||(fflag&&x0==1)))
			{
				if(x0==1)
				{
					switch(shape)
					{
					case 0x1e:                        //XXXX_
						if(1)           
						{
							return 1;
							//return COMB(1,i+4);
						}
						break;
					case 0x1d:                       //XXX_X
						if(1)
						{
							if(x[i+5]==0)
							{
								if(i<=S-7)
								{
									if(x[i+6]==1&&x[i+7]==1&&x[i+8]==1)
									{
										if(p==i+4)
										{
											return 2;
										}
										//return COMC(1,i+3,i+5);
									}
								}
							}
							return 1;
							//return COMB(1,i+3);
						}
						break;
					case 0x1b:                      //XX_XX
						if(1)
						{
							if(x[i+5]==0)
							{
								if(i<=S-6)
								{
									if(x[i+6]==1&&x[i+7]==1)
									{
										if(p==i+4||p==i+3)
										{
											return 2;
										}
										//return COMC(1,i+2,i+5);
									}
								}
							}
							return 1;
							return COMB(1,i+2);
						}
						break;
					case 0x17:                     //X_XXX
						if(1)
						{
							if(x[i+5]==0)
							{
								if(i<=S-5)
								{
									if(x[i+6]==1)
									{
										if(p==i+4||p==i+3||p==i+2)
										{
											return 2;
										}
										//return COMC(1,i+1,i+5);
									}
								}
							}
							return 1;
							//return COMB(1,i+1);
						}
						break;
					case 0xf:                       //_XXXX
						if(1)
						{
							return 1;
							//return COMB(1,i);
						}
						break;
					}
				}
				else
				{
					shape=-shape;
					switch(shape)
					{
					case 0x1e:                  //OOOO_
						return 1;
						//return COMB(1,i+4);
						break;
					case 0x1d:                 //OOO_O
						if(i<=S-7)
						{
							if(x[i+5]==0&&x[i+6]==-1&&x[i+7]==-1&&x[i+8]==-1)
							{
								if(p==i+4)
								{
									return 2;
								}
								//return COMC(1,i+3,i+5);
							}
						}
						return 1;
						//return COMB(1,i+3);
						break;
					case 0x1b:                //OO_OO
						if(i<=S-6)
						{
							if(x[i+5]==0&&x[i+6]==-1&&x[i+7]==-1)
							{
								if(p==i+4||p==i+3)
								{
									return 2;
								}
								//return COMC(1,i+2,i+5);
							}
						}
						return 1;
						//return COMB(1,i+2);
						break;
					case 0x17:                 //O_OOO
						if(i<=S-5)
						{
							if(x[i+5]==0&&x[i+6]==-1)
							{
								if(p==i+4||p==i+3||p==i+2)
								{
									return 2;
								}
								//return COMC(1,i+1,i+5);
							}
						}
						return 1;
						//return COMB(1,i+1);
						break;
					case 0xf:                  //_OOOO
						return 1;
						//return COMB(1,i);
						break;
					}
				}
			}
			else
			{
				switch(shape)
				{
				case 0x1e:                        //XXXX_
				case -0x1e:
					if(x[i-1]!=x0&&x[i+5]!=x0)           
					{
						return 1;
						//return COMB(1,i+4);
					}
					break;
				case 0x1d:                       //XXX_X
				case -0x1d:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						if(x[i+5]==0)
						{
							if(i<=8)
							{
								if(x[i+6]==x0&&x[i+7]==x0&&x[i+8]==x0&&x[i+9]!=x0)
								{
									if(p==i+4)
									{
										return 2;
									}
									//return COMC(1,i+3,i+5);
								}
							}
						}
						return 1;
						//return COMB(1,i+3);
					}
					break;
				case 0x1b:                      //XX_XX
				case -0x1b:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						if(x[i+5]==0)
						{
							if(i<=9)
							{
								if(x[i+6]==x0&&x[i+7]==x0&&x[i+8]!=x0)
								{
									if(p==i+4||p==i+3)
									{
										return 2;
									}
									//return COMC(1,i+2,i+5);
								}
							}
						}
						return 1;
						return COMB(1,i+2);
					}
					break;
				case 0x17:                     //X_XXX
				case -0x17:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						if(x[i+5]==0)
						{
							if(i<=10)
							{
								if(x[i+6]==x0&&x[i+7]!=x0)
								{
									if(p==i+4||p==i+3||p==i+2)
									{
										return 2;
									}
									//return COMC(1,i+1,i+5);
								}
							}
						}
						return 1;
						//return COMB(1,i+1);
					}
					break;
				case 0xf:                       //_XXXX
				case -0xf:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						return 1;
						//return COMB(1,i);
					}
					break;
				}
			}
		}
	}
	return 0;
}

int line::B4p(int p)
{
	p+=2;
	int x0=x[p];
	if(x0==0)
	{
		return 0;
	}
	int xmin=2<p-4?p-4:2;
	int xmax=S-3<p?S-3:p;
	for(int i=xmin;i<=xmax;i++)
	{
		int num=x[i]+x[i+1]+x[i+2]+x[i+3]+x[i+4];
		if(num==4*x0)
		{
			int shape=(x[i]<<4)+(x[i+1]<<3)+(x[i+2]<<2)+(x[i+3]<<1)+x[i+4];
			if(!(nosix||(fflag&&x0==1)))
			{
				if(x0==1)
				{
					switch(shape)
					{
					case 0x1e:                        //XXXX_
						if(x[i-1]==0)
						{
							return COMC(1,i-1,i+4);
						}
						else
						{
							return COMB(1,i+4);
						}
						break;
					case 0x1d:                       //XXX_X
						if(1)
						{
							if(1)
							{
								if(i<=S-7)
								{
									if(x[i+5]==0&&x[i+6]==1&&x[i+7]==1&&x[i+8]==1)
									{
										if(p==i+4)
										{
											return COMC(1,i+3,i+5);
										}
									}
								}
							}
							return COMB(1,i+3);
						}
						break;
					case 0x1b:                      //XX_XX
						if(1)
						{
							if(1)
							{
								if(i<=S-6)
								{
									if(x[i+5]==0&&x[i+6]==1&&x[i+7]==1)
									{
										if(p==i+4||p==i+3)
										{
											return COMC(1,i+2,i+5);
										}
									}
								}
							}
							return COMB(1,i+2);
						}
						break;
					case 0x17:                     //X_XXX
						if(1)
						{
							if(1)
							{
								if(i<=S-5)
								{
									if(x[i+5]==0&&x[i+6]==1)
									{
										if(p==i+4||p==i+3||p==i+2)
										{
											return COMC(1,i+1,i+5);
										}
									}
								}
							}
							return COMB(1,i+1);
						}
						break;
					case 0xf:                       //_XXXX
						if(1)
						{
							if(x[i+5]==0)
							{
								return COMC(1,i,i+5);
							}
							else
							{
								return COMB(1,i);
							}
						}
						break;
					}
				}
				else
				{
					shape=-shape;
					switch(shape)
					{
					case 0x1e:                  //OOOO_
						if(x[i-1]==0)
						{
							return COMC(1,i-1,i+4);
						}
						else
						{
							return COMB(1,i+4);
						}
						break;
					case 0x1d:                 //OOO_O
						if(i<=S-7)
						{
							if(x[i+5]==0&&x[i+6]==-1&&x[i+7]==-1&&x[i+8]==-1)
							{
								if(p==i+4)
								{
									return COMC(1,i+3,i+5);
								}
							}
						}
						return COMB(1,i+3);
						break;
					case 0x1b:                //OO_OO
						if(i<=S-6)
						{
							if(x[i+5]==0&&x[i+6]==-1&&x[i+7]==-1)
							{
								if(p==i+4||p==i+3)
								{
									return COMC(1,i+2,i+5);
								}
							}
						}
						return COMB(1,i+2);
						break;
					case 0x17:                 //O_OOO
						if(i<=S-5)
						{
							if(x[i+5]==0&&x[i+6]==-1)
							{
								if(p==i+4||p==i+3||p==i+2)
								{
									return COMC(1,i+1,i+5);
								}
							}
						}
						return COMB(1,i+1);
						break;
					case 0xf:                  //_OOOO
						if(x[i+5]==0)
						{
							return COMC(1,i,i+5);
						}
						else
						{
							return COMB(1,i);
						}
						break;
					}
				}
			}
			else
			{
				switch(shape)
				{
				case 0x1e:                        //XXXX_
				case -0x1e:
					if(x[i-1]!=x0&&x[i+5]!=x0)
						{
							if(x[i-1]==0&&x[i-2]!=x0)
							{
								return COMC(1,i-1,i+4);
							}
							else
							{
								return COMB(1,i+4);
							}
						}
					break;
				case 0x1d:                       //XXX_X
				case -0x1d:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						if(1)
						{
							if(i<=8)
							{
								if(x[i+5]==0&&x[i+6]==x0&&x[i+7]==x0&&x[i+8]==x0&&x[i+9]!=x0)
								{
									if(p==i+4)
									{
										return COMC(1,i+3,i+5);
									}
								}
							}
						}
						return COMB(1,i+3);
					}
					break;
				case 0x1b:                      //XX_XX
				case -0x1b:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						if(1)
						{
							if(i<=9)
							{
								if(x[i+5]==0&&x[i+6]==x0&&x[i+7]==x0&&x[i+8]!=x0)
								{
									if(p==i+4||p==i+3)
									{
										return COMC(1,i+2,i+5);
									}
								}
							}
						}
						return COMB(1,i+2);
					}
					break;
				case 0x17:                     //X_XXX
				case -0x17:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						if(1)
						{
							if(i<=10)
							{
								if(x[i+5]==0&&x[i+6]==x0&&x[i+7]!=x0)
								{
									if(p==i+4||p==i+3||p==i+2)
									{
										return COMC(1,i+1,i+5);
									}
								}
							}
						}
						return COMB(1,i+1);
					}
					break;
				case 0xf:                       //_XXXX
				case -0xf:
					if(x[i-1]!=x0&&x[i+5]!=x0)
					{
						if(x[i+5]==0&&x[i+6]!=x0)
						{
							return COMC(1,i+5,i);
						}
						else
						{
							return COMB(1,i);
						}
					}
					break;
				}
			}
		}
	}
	return 0;
}

int line::A3(int p)
{
	p+=2;
	int x0=x[p];
	if(x0==0)
	{
		return 0;
	}
	else
	{
		int xmin=2<p-3?p-3:2;
		int xmax=S-2<p?S-2:p;
		for(int i=xmin;i<=xmax;i++)
		{
			int num1=x[i]+x[i+1]+x[i+2]+x[i+3];
			int num2=x[i]*x[i+1]*x[i+2]*x[i+3];
			if(num1==3*x0&&num2==0)
			{
				int shape=(x[i]<<3)+(x[i+1]<<2)+(x[i+2]<<1)+x[i+3];
				if(!(nosix||(fflag&&x0==1)))
				{
					if(x0==1)
					{
						switch(shape)
						{
						case 0xe:                      //XXX_
							if(x[i-1]==0&&x[i-2]!=1&&x[i+4]!=1)
							{
								if((x[i-2]==0)&&(x[i+4]==0))
								{
									return COMC(1,i-1,i+3);
								}
								if(x[i-2]==0)
								{
									return COMB(1,i-1);
								}
								if(x[i+4]==0)
								{
									return COMB(1,i+3);
								}
							}
							break;
						case 0xd:                    //XX_X
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMB(1,i+2);
							}
							break;
						case 0xb:                     //X_XX
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMB(1,i+1);
							}
							break;
						}
					}
					else
					{
						shape=-shape;
						switch(shape)
						{
						case 0xe:                      //OOO_
							if(x[i-1]==0&&x[i-2]!=-1&&x[i+4]!=-1)
							{
								if((x[i-2]==0)&&(x[i+4]==0))
								{
									return COMC(1,i-1,i+3);
								}
								if(x[i-2]==0)
								{
									return COMB(1,i-1);
								}
								if(x[i+4]==0)
								{
									return COMB(1,i+3);
								}
							}
							break;
						case 0xd:                     //OO_O
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMB(1,i+2);
							}
							break;
						case 0xb:                    //O_OO
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMB(1,i+1);
							}
							break;
						}
					}
				}
				else
				{
					switch(shape)
					{
					case 0xe:                      //XXX_
					case -0xe:
						if(x[i-1]==0&&x[i-2]!=x0&&x[i+4]!=x0)
						{
							if((x[i-2]==0&&x[i-3]!=x0)&&(x[i+4]==0&&x[i+5]!=x0))
							{
								return COMC(1,i-1,i+3);
							}
							if(x[i-2]==0&&x[i-3]!=x0)
							{
								return COMB(1,i-1);
							}
							if(x[i+4]==0&&x[i+5]!=x0)
							{
								return COMB(1,i+3);
							}
						}
						break;
					case 0xd:                    //XX_X
					case -0xd:
						if(x[i-1]==0&&x[i+4]==0&&x[i-2]!=x0&&x[i+5]!=x0)
						{
							return COMB(1,i+2);
						}
						break;
					case 0xb:                     //X_XX
					case -0xb:
						if(x[i-1]==0&&x[i+4]==0&&x[i-2]!=x0&&x[i+5]!=x0)
						{
							return COMB(1,i+1);
						}
						break;
					}
				}
			}
		}
	}
	return 0;
}

int line::A3pb(int p)
{
	p+=2;
	int x0=x[p];
	if(x0==0)
	{
		return 0;
	}
	else
	{
		int xmin=2<p-3?p-3:2;
		int xmax=S-2<p?S-2:p;
		for(int i=xmin;i<=xmax;i++)
		{
			int num1=x[i]+x[i+1]+x[i+2]+x[i+3];
			int num2=x[i]*x[i+1]*x[i+2]*x[i+3];
			if(num1==3*x0&&num2==0)
			{
				int shape=(x[i]<<3)+(x[i+1]<<2)+(x[i+2]<<1)+x[i+3];
				if(!(nosix||(fflag&&x0==1)))
				{
					if(x0==1)
					{
						switch(shape)
						{
						case 0xe:                      //XXX_
							if(x[i-1]==0&&x[i-2]!=1&&x[i+4]!=1)
							{
								if((x[i-2]==0)&&(x[i+4]==0))
								{
									return COMC(1,i-1,i+3);
								}
								else if(x[i-2]==0)
								{
									return COMD(1,i-1,i-2,i+3);
								}
								if(x[i+4]==0)
								{
									return COMD(1,i+3,i-1,i+4);
								}
							}
							break;
						case 0xd:                    //XX_X
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMD(1,i+2,i-1,i+4);
							}
							break;
						case 0xb:                     //X_XX
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMD(1,i+1,i-1,i+4);
							}
							break;
						}
					}
					else
					{
						shape=-shape;
						switch(shape)
						{
						case 0xe:                      //OOO_
							if(x[i-1]==0&&x[i-2]!=-1&&x[i+4]!=-1)
							{
								if((x[i-2]==0)&&(x[i+4]==0))
								{
									return COMC(1,i-1,i+3);
								}
								else if(x[i-2]==0)
								{
									return COMD(1,i-1,i-2,i+3);
								}
								else if(x[i+4]==0)
								{
									return COMD(1,i+3,i-1,i+4);
								}
							}
							break;
						case 0xd:                     //OO_O
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMD(1,i+2,i-1,i+4);
							}
							break;
						case 0xb:                    //O_OO
							if(x[i-1]==0&&x[i+4]==0)
							{
								return COMD(1,i+1,i-1,i+4);
							}
							break;
						}
					}
				}
				else
				{
					switch(shape)
					{
					case 0xe:                      //XXX_
					case -0xe:
						if(x[i-1]==0&&x[i-2]!=x0&&x[i+4]!=x0)
						{
							if((x[i-2]==0&&x[i-3]!=x0)&&(x[i+4]==0&&x[i+5]!=x0))
							{
								return COMC(1,i-1,i+3);
							}
							if(x[i-2]==0&&x[i-3]!=x0)
							{
								return COMD(1,i-1,i-2,i+3);
							}
							if(x[i+4]==0&&x[i+5]!=x0)
							{
								return COMD(1,i+3,i-1,i+4);
							}
						}
						break;
					case 0xd:                    //XX_X
					case -0xd:
						if(x[i-1]==0&&x[i+4]==0&&x[i-2]!=x0&&x[i+5]!=x0)
						{
							return COMD(1,i+2,i-1,i+4);
						}
						break;
					case 0xb:                     //X_XX
					case -0xb:
						if(x[i-1]==0&&x[i+4]==0&&x[i-2]!=x0&&x[i+5]!=x0)
						{
							return COMD(1,i+1,i-1,i+4);
						}
						break;
					}
				}
			}
		}
	}
	return 0;
}