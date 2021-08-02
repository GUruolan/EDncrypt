#ifndef MD5H
#define MD5H
#include <cmath>
#include <Windows.h>
#include <bitset.h>
#include <iostream>
#include <string.h>
#include "RSA.h"

using namespace std;

/*������ʵ��32λ����ѭ������xλ*/
void cycLeft(unsigned int &s, unsigned short k){
	if( k>32 ) k %=32;
	s = (s << k ) | (s >> (32 - k));
}

/***************4����㺯��*******************/
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

/*������MD5ѭ������,r:����fghi*/
void accLoop (unsigned short r, unsigned int *state, void* m){
	unsigned int *p1, *p2, *p3, *p4, temp;		
	typedef unsigned int(*clac)(unsigned int x, unsigned int y, unsigned int z);
	clac fp_arr[4] = {logiFuncF, logiFuncG, logiFuncH, logiFuncI};	//����ָ��

	const unsigned int cl_arr[4][4]={ { 7, 12, 17, 22 },
									{ 5, 9, 14, 20 },
									{ 4, 11, 16, 23 },
									{ 6, 10, 15, 21 }     };//ѭ������λ����
	const unsigned int mN[4][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 },
        { 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 },
        { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 }
    };//���������
	const unsigned int *pm = static_cast<unsigned int *>(m);		//��mǿ������ת��Ϊuint
	for (int i=0; i<16; ++i){
		//ָ��任,�൱�ڰ�stateѭ������
		p1 = &(state[3]) - ((3+i)%4);
		p2 = &(state[3]) - ((2+i)%4);
		p3 = &(state[3]) - ((1+i)%4);
		p4 = &(state[3]) - ((0+i)%4);
		unsigned int ti = 4294967296 * abs(sin(((r)*16)+i+1));
		unsigned int v1 = fp_arr[r](*p2, *p3, *p4);
		temp = *p1 + fp_arr[r](*p2, *p3, *p4) + pm[mN[r][i]] + 
			(unsigned int)(4294967296 * abs(sin(((r)*16)+i+1)));	//ti
 		cycLeft(temp, cl_arr[r][i%4]);	//��ѭ������λ����ѭ������
			*p1 = *p2 + temp;
	}

}

/**MD5ǩ������**/
string hashMD5(const char* ms){
	int mLen = strlen(ms);
	if (mLen <= 0) return 0;
	///���
	unsigned int filLen = (448 - (mLen * 8)%512) / 8;	//��һ������ֽ���
	unsigned int L = mLen + filLen +8;		//ȫ�������ֽ�����512/8==64�ı�����
	unsigned char *buff = new unsigned char[L];	//�����������ݻ�����
	CopyMemory(buff, ms, mLen);	//���������ݸ��Ƶ�������
	unsigned long long mbLen = mLen * 8;	//ԭʼ��Ϣbit��
	buff[mLen] = 0x80;	//ԭʼ��Ϣ��ĵ�һλ���1
	ZeroMemory(&buff[mLen]+1, filLen - 1);	//ʣ��λ��0
	
	CopyMemory(&(buff[L-8]), &mbLen, 8);		//������8�ֽ�
	for(int i=0; i<L; i++)
		cout<<int(buff[i])<<" "<<i<<endl;
	///������
	unsigned int setNum = L / 64;		//�ܹ�����
	unsigned int A = 0x67452301, B = 0x0EFCDAB89, C = 0x98BADCFE, D = 0x10325476;//������С������
	unsigned int state[4] = {A,B,C,D};

	for (int i =0 ; i<setNum; ++i){ //��ÿһ512bit����
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