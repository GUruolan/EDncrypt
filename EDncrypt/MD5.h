#ifndef MD5H
#define MD5H
#include <cmath>
#include <Windows.h>
#include <bitset.h>
#include <iostream>
#include <string.h>
#include "RSA.h"

using namespace std;

/*函数，实现32位整数循环左移x位*/
void cycLeft(unsigned int &s, unsigned short k){
	if( k>32 ) k %=32;
	s = (s << k ) | (s >> (32 - k));
}

/***************4组计算函数*******************/
unsigned int logiFuncF(unsigned int x, unsigned int y, unsigned int z) {
	return ( (x)&(y) ) | ( (~x)&(z) );
}
unsigned int logiFuncG(unsigned int x, unsigned int y, unsigned int z) {
	return ( x&z ) | ( (~z)&y );
}
unsigned int logiFuncH(unsigned int x, unsigned int y, unsigned int z) {
	return x^y^z;
}
unsigned int logiFuncI(unsigned int x, unsigned int y, unsigned int z) {
	return y^(x|(~z));
}

/*函数：MD5循环计算,r:决定fghi*/
void accLoop (unsigned short r, unsigned int *state, void* m){
	unsigned int *p1, *p2, *p3, *p4, temp;		
	typedef unsigned int(*clac)(unsigned int x, unsigned int y, unsigned int z);
	clac fp_arr[4] = {logiFuncF, logiFuncG, logiFuncH, logiFuncI};	//函数指针

	const unsigned int cl_arr[4][4]={ { 7, 12, 17, 22 },
									{ 5, 9, 14, 20 },
									{ 4, 11, 16, 23 },
									{ 6, 10, 15, 21 }     };//循环左移位数表
	const unsigned int mN[4][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 },
        { 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 },
        { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 }
    };//数据坐标表
	const unsigned int *pm = static_cast<unsigned int *>(m);		//将m强制类型转换为uint
	for (int i=0; i<16; ++i){
		//指针变换,相当于把state循环左移
		p1 = &(state[3]) - ((3+i)%4);
		p2 = &(state[3]) - ((2+i)%4);
		p3 = &(state[3]) - ((1+i)%4);
		p4 = &(state[3]) - ((0+i)%4);
		unsigned int ti = 4294967296 * abs(sin(((r)*16)+i+1));
		unsigned int v1 = fp_arr[r](*p2, *p3, *p4);
		temp = *p1 + fp_arr[r](*p2, *p3, *p4) + pm[mN[r][i]] + 
			(unsigned int)(4294967296 * abs(sin(((r)*16)+i+1)));	//ti
 		cycLeft(temp, cl_arr[r][i%4]);	//按循环左移位数表循环左移
			*p1 = *p2 + temp;
	}

}

/**MD5签名函数**/
string hashMD5(const char* ms){
	int mLen = strlen(ms);
	if (mLen <= 0) return 0;
	///填充
	unsigned int filLen = (448 - (mLen * 8)%512) / 8;	//第一步填充字节数
	unsigned int L = mLen + filLen +8;		//全部填充后字节数（512/8==64的倍数）
	unsigned char *buff = new unsigned char[L];	//分配填充后数据缓冲区
	CopyMemory(buff, ms, mLen);	//将填充后数据复制到缓冲区
	unsigned long long mbLen = mLen * 8;	//原始信息bit数
	buff[mLen] = 0x80;	//原始信息后的第一位填充1
	ZeroMemory(&buff[mLen]+1, filLen - 1);	//剩下位补0
	
	CopyMemory(&(buff[L-8]), &mbLen, 8);		//填充最后8字节
	for(int i=0; i<L; i++)
		cout<<int(buff[i])<<" "<<i<<endl;
	///填充结束
	unsigned int setNum = L / 64;		//总共组数
	unsigned int A = 0x67452301, B = 0x0EFCDAB89, C = 0x98BADCFE, D = 0x10325476;//幻数，小端类型
	unsigned int state[4] = {A,B,C,D};

	for (int i =0 ; i<setNum; ++i){ //对每一512bit分组
		for(int r=0; r<4;++r)
			accLoop(r, state, buff+i*64);	
		state[0] = (state[0] += A);
		state[1] = (state[1] += B);
		state[2] = (state[2] += C);
		state[3] = (state[3] += D);

	}
	BigInt h0(state[0]);
	BigInt h1(state[1]);
	BigInt h2(state[2]);
	BigInt h3(state[3]);
	return h0.values+h1.values+h2.values+h3.values;
}


#endif