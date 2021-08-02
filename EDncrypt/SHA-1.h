/*SHA-1 散列算法*/

#ifndef SHAH
#define SHAH

#include <iostream>
#include <bitset.h>
#include <string>
#include <iomanip>
#include "RSA.h"
//#include "toolFunc.h"

using namespace std;

typedef bitset<32> word;	//SHN-1运算的最小单位是32bit

/*工具函数：32位循环左移*/
word cycLeft(unsigned int i,word w){ 
	return (w<<i)|(w>>32-i);
}

word N[4] = {0x5A827999,0x6ED9EBA1,0x8F1BBCDC,0xCA62C1D6};	//参与模2^23加常数

long long msg_byte;	//原始摘要信息字节数

/*工具函数：模2^32加*/
word modAdd(word a ,word b){
	word c;		//结果
	int jw = 0;	//进位
	for(int i=0; i<32; ++i){
		c[i] = (a[i]^b[i])^jw;
		jw = (a[i]&&b[i])|(a[i]&&jw)|(b[i]&&jw);
	}
	return c;
}

/*函数：f0~f79*/
word f(int ff, word B, word C, word D){
	if((ff>=0&&ff<20))
		return ((B&C)|((~B)&D));
	else if(ff>=20&&ff<40|(ff>=60&&ff<80))
		return (B^C^D);
	else if(ff>=40&&ff<60)
		return ((B&C)|(B&D)|(C&D));
}

/*函数：对每个512bit分组的操作*/
void shaSetF(word w[],word H[]){ //
	for(int r=16; r<80; ++r)	//初始化剩下64个word
		w[r] = cycLeft(1, w[r-3]^w[r-8]^w[r-14]^w[r-16]);
	word A = H[0];		//ABCDE存初始H01234，在该分组运算过程中不变
	word B = H[1];
	word C = H[2];
	word D = H[3];
	word E = H[4];
	for (unsigned int r=0; r<80; r++){
		word tempa = modAdd(w[r],N[r/20]);
		tempa = modAdd(E,tempa);
		cout<<tempa.to_ulong()<<endl;
		tempa = modAdd(tempa,f(r,B,C,D));
		cout<<tempa.to_ulong()<<endl;
		tempa = modAdd(tempa,cycLeft(5,A));
		cout<<tempa.to_ulong();
		cout<<" "<<r<<endl;
		E = D;
		D = C;
		C = cycLeft(30,B);
		B = A;
		A = tempa;
	}
	H[0] = modAdd(H[0],A);
	H[1] = modAdd(H[1],B);
	H[2] = modAdd(H[2],C);
	H[3] = modAdd(H[3],D);
	H[4] = modAdd(H[4],E);
}

/*填充函数,并将填充后结果存入为w[0]-w[16]*/
bool shaFill(const char* s,word w[]){ //因为原字符串长度不定，无法确定初始化bitset长度，所以用string
	unsigned int sbL = strlen(s)*8 ; //原字符串长度
	bitset<512> teps;
	for(int i=0; i<sbL/8; i++){ //将字符串按asci码存入二进制teps
			bitset<8> asc(00000000);
			asc |= s[i];
			//cout<<asc<<endl;
			for(int j=0; j<8; j++)
				teps[504-i*8+j] = asc[j];
	}
	teps[511-sbL] = 1;	//第一位补1，剩下位补0 
	//cout<<teps<<endl;
	if(sbL >= 448) (teps>>63)<<63;	//大于448时分两组，该组只填充100……
	else{					//填充分组小于448在该组填充有效字节数
		bitset<512> bL(msg_byte*8);//初始化时优先低位
		teps |= bL;	//填充后的512bit
		//cout<<bL<<endl;
	}
	for(int i=0; i<16; ++i){
		for(int j=0; j<32; ++j)
			w[i][31-j] = teps[511-i*32-j];
		cout<<w[i]<<" "<<i<<endl;
	}
	cout<<teps<<endl;
	return true;
}

/*初始化每512bit，存入w[0]-W[16]*/
void shaW(const char* s,word w[]){
	bitset<32> temp(0x0000);
	for(int i=0; i<16; ++i){
		w[i] = 0x00000000;
		for(int j=0; j<4; ++j){
			w[i] <<= 8;
			temp = unsigned int(s[i*4+j]);
			//cout<<temp<<" ";
			w[i] |= temp;
		}
		cout<<w[i]<<" "<<i<<endl;
	}
}

/*SHA-1接口函数*/
string hashSha_1(string m){
	word H[5] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0};
	string result;
	word* w = new word[80]();
	msg_byte = strlen(m.c_str());
	unsigned int last_setL = msg_byte % 64;
	unsigned int n_set = msg_byte/64;
	if(n_set>=0)
		for(int c=0; c<n_set; ++c)//对每个512bit分组进行运算
		{
			shaW(m.c_str()+c*64,w);
			shaSetF(w,H);
			for(int i=0; i<80; ++i)
				w[i].reset();
		}
	//处理最后一个分组
	shaFill(m.c_str()+msg_byte-last_setL,w);
	shaSetF(w,H);
	if(last_setL>=56) {
		for(int i=0; i<80; ++i)
			w[i] = 0x00000000;
		bitset<64> bL(msg_byte*8);
		for(int i=0; i<32; ++i)	//低32位->w[31]
			w[15][i] = bL[i];
		for(int i=32; i<64; ++i)	//高32位->w[30]
			w[16][i-32] = bL[i];
		for(int i=0; i<80; ++i)
			cout<<w[i]<<" "<<endl;

		shaSetF(w,H);
	}
	delete []w;
	BigInt h0(H[0].to_ulong());
	BigInt h1(H[1].to_ulong());
	BigInt h2(H[2].to_ulong());
	BigInt h3(H[3].to_ulong());
	BigInt h4(H[4].to_ulong());

	return h0.values+h1.values+h2.values+h3.values+h4.values;
}

#endif 

/*void main(){
	word w[80] = {0x00000000};
	string ss = "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvwxy12345678abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvwxy12345678abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvwxy12345678abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvwxy12345678";
	//msg_byte = ss.length();
	sha_1(ss.c_str(),w);
	cout<<hex<<H[0].to_ulong()<<endl
		<<H[1].to_ulong()<<endl
		<<H[2].to_ulong()<<endl
		<<H[3].to_ulong()<<endl
		<<H[4].to_ulong()<<endl;
	system("pause");
}*/







