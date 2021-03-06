/*	> 基于CBC模式的DES加密算法
	> 输出基于ASIC码的字符串
	> 输入8字节密钥*/
#ifndef DESH
#define DESH
//#include<stdio.h>
#include <iostream>
#include <string>
#include <bitset.h>
#include <fstream>
#include <cmath>
//#include "toolFunc.h"
using namespace std;
using std::bitset;

namespace Ndes{
/*---------------------变量声明------------------------------*/
	bitset<64> key;                // 64位密钥      
 
// 初始置换表
	int IP[] = {58, 50, 42, 34, 26, 18, 10, 2,
				60, 52, 44, 36, 28, 20, 12, 4,
				62, 54, 46, 38, 30, 22, 14, 6,
				64, 56, 48, 40, 32, 24, 16, 8,
				57, 49, 41, 33, 25, 17, 9,  1,
				59, 51, 43, 35, 27, 19, 11, 3,
				61, 53, 45, 37, 29, 21, 13, 5,
				63, 55, 47, 39, 31, 23, 15, 7};
 
// 结尾置换表
	int IP_1[] = {40, 8, 48, 16, 56, 24, 64, 32,
				  39, 7, 47, 15, 55, 23, 63, 31,
				  38, 6, 46, 14, 54, 22, 62, 30,
				  37, 5, 45, 13, 53, 21, 61, 29,
				  36, 4, 44, 12, 52, 20, 60, 28,
				  35, 3, 43, 11, 51, 19, 59, 27,
				  34, 2, 42, 10, 50, 18, 58, 26,
				  33, 1, 41,  9, 49, 17, 57, 25};
 
/*------------------下面是生成密钥所用表-----------------*/
 
// 密钥置换表，将64位密钥变成56位
	int PC_1[] = {57, 49, 41, 33, 25, 17, 9,
				   1, 58, 50, 42, 34, 26, 18,
				  10,  2, 59, 51, 43, 35, 27,
				  19, 11,  3, 60, 52, 44, 36,
				  63, 55, 47, 39, 31, 23, 15,
				   7, 62, 54, 46, 38, 30, 22,
				  14,  6, 61, 53, 45, 37, 29,
				  21, 13,  5, 28, 20, 12,  4}; 
 
// 压缩置换，将56位密钥压缩成48位子密钥
	int PC_2[] = {14, 17, 11, 24,  1,  5,
				   3, 28, 15,  6, 21, 10,
				  23, 19, 12,  4, 26,  8,
				  16,  7, 27, 20, 13,  2,
				  41, 52, 31, 37, 47, 55,
				  30, 40, 51, 45, 33, 48,
				  44, 49, 39, 56, 34, 53,
				  46, 42, 50, 36, 29, 32};
 
// 每轮左移的位数
	int shiftBits[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
 
	/*------------------下面是密码函数 f 所用表-----------------*/
 
// 扩展置换表，将 32位 扩展至 48位
	int E[] = {32,  1,  2,  3,  4,  5,
				4,  5,  6,  7,  8,  9,
				8,  9, 10, 11, 12, 13,
			   12, 13, 14, 15, 16, 17,
			   16, 17, 18, 19, 20, 21,
			   20, 21, 22, 23, 24, 25,
			   24, 25, 26, 27, 28, 29,
			   28, 29, 30, 31, 32,  1};
 
// S盒，每个S盒是4x16的置换表，6位 -> 4位
	int S_BOX[8][4][16] = {
		{  
			{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},  
			{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},  
			{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0}, 
			{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13} 
		},
		{  
			{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},  
			{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5}, 
			{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},  
			{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}  
		}, 
		{  
			{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},  
			{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},  
			{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},  
			{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}  
		}, 
		{  
			{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},  
			{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},  
			{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},  
			{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}  
		},
		{  
			{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},  
			{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},  
			{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},  
			{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}  
		},
		{  
			{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},  
			{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},  
			{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},  
			{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}  
		}, 
		{  
			{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},  
			{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},  
			{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},  
			{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}  
		}, 
		{  
			{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},  
			{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},  
			{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},  
			{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}  
		} 
	};
 
// P置换，32位 -> 32位
	int P[] = {16,  7, 20, 21,
			   29, 12, 28, 17,
				1, 15, 23, 26,
				5, 18, 31, 10,
				2,  8, 24, 14,
			   32, 27,  3,  9,
			   19, 13, 30,  6,
			   22, 11,  4, 25 };
/*函数：将56位密钥前后28位进行循环左移shift位*/
	bitset<28> leftShift(bitset<28> k,int shift)
	{
		bitset<28> tmp = k;
		for (int i=27; i>=0; --i){
			if (i<shift) k[i] = tmp[i-shift+28];
			else k[i] = tmp[i-shift];
		}
		return k;
	}

/*函数：生成16个48位子密钥*/
	void generKeys(bitset<64> key,bitset<48>* sub_key)
	{
		bitset<56> real_key;	//56位密钥
		bitset<28> left;		//左28位
		bitset<28> right;		//右28位
		bitset<48> cps_key;
		
		//初始密钥根据密钥置换表PC-1,去掉（1，9，17...）位，原（6，14，22..）->(1,2,3...)等，将64位变成56位密钥
		for (int i=0; i<56; ++i)
			real_key[55-i] = key[64 -PC_1[i]];

		//生成子密钥
		for (int r=0; r<16; ++r){
			//从real_key中分离出左右28位
			for (int i=28; i<56; ++i)
				left[i-28] = real_key[i];
			for (int i=0; i<28; ++i)
				right[i] = real_key[i];

			//按shiftBits表左移
			left = leftShift(left, shiftBits[r]);
			right = leftShift(right, shiftBits[r]);

			//左右合并，压缩置换，由56位得到48位子密钥
			for (int i=28; i<56; ++i)
				real_key[i] = left[i-28];
			for (int i=0; i<28; ++i)
				real_key[i] = right[i];
			for (int i=0; i<48; ++i)
				cps_key[47-i] = real_key[56 - PC_2[i]];
			sub_key[r] = cps_key;
		}
	}

/*feistel密码函数,输入上一轮右半部份和本轮密钥*/
	bitset<32> feistel(bitset<32> R,bitset<48> k)
	{
		//1.扩展置换，R:32->48
		bitset<48> expend_R;
		for(int i=0; i<48; i++)
			expend_R[47-i] = R[32-E[i]];
		//2.expend_R和k异或(只有该步骤密钥参与计算)
		expend_R = expend_R ^ k;
		//3.S盒置换，48->32
		bitset<32> output;
		int n = 0;
		for(int i=0; i<48; i=i+6)
		{
			int row = expend_R[47-i]*2 +  expend_R[47-i-5];		//47-i~（47-i-5）六位参与row和col的计算
			int col = expend_R[47-i-1]*8 + expend_R[47-i-2]*4+ expend_R[47-i-3]*2 + expend_R[47-i-4];
			int num = S_BOX[i/6][row][col];
			bitset<4> tmp(num);
			for (int j=0; j<4; ++j)
				output[31-n-j] = tmp[3-j];
			n += 4;
		}
		//4.p-置换,32->32
		bitset<32> tmp = output;
		for(int i=0; i<32; ++i)
			output[31-i] = tmp[32-P[i]];
		return output;
	}
}

/*函数：DES加密*/
bitset<64> desEncrypt(bitset<64>& x, const bitset<48>* sub_key)	//x:明文
{
	using namespace Ndes;
	bitset<64> y;	//y：密文
	bitset<64> current_bits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> new_left;
	//1.初始置换IP
	for(int i=0; i<64; ++i)
		current_bits[63-i] = x[64-IP[i]];
	//2.获取li和ri
	for(int i=32; i<64; ++i)
		left[i-32] = current_bits[i];
	for(int i=0; i<32; ++i)
		right[i] = current_bits[i];
	//3.进行16轮迭代
	for(int r=0; r<16; ++r)
	{
		new_left = right;		
		right = left ^ feistel(right,sub_key[r]);	
		left = new_left;
	}
	//4.左右换位，并合并
	for(int i=0; i<32; ++i)
		y[i] = left[i];
	for(int i=32; i<64; ++i)
		y[i] = right[i-32];
	//5.结尾置换IP-1
	current_bits = y;
	for(int i=0; i<64; ++i)
		y[63-i] =current_bits[64-IP_1[i]];
	return y;
}

/*函数：DES解密*/
bitset<64> desDecrypt(bitset<64>& y, const bitset<48>* sub_key)
{
	using namespace Ndes;
	bitset<64> x;
	bitset<64> current_bits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> new_left;
	//1.初始置换IP
	for(int i=0; i<64; ++i)
		current_bits[63-i] = y[64-IP[i]];
	//2.获取li和ri
	for(int i=32; i<64; ++i)
		left[i-32] = current_bits[i];
	for(int i=0; i<32; ++i)
		right[i] = current_bits[i];
	//3.进行16轮迭代(子密钥逆序应用)
	for(int r=0; r<16; ++r)
	{
		new_left = right;		
		right = left ^ feistel(right,sub_key[15-r]);	
		left = new_left;
	}
	//4.左右换位，并合并
	for(int i=0; i<32; ++i)
		x[i] = left[i];
	for(int i=32; i<64; ++i)
		x[i] = right[i-32];
	//5.结尾置换IP-1
	current_bits = x;
	for(int i=0; i<64; ++i)
		x[63-i] =current_bits[64-IP_1[i]];
	return x;
}

#endif
