#include <string>
#include <bitset.h>
using namespace std;

typedef bitset<8> Byte;	//8bit定义为Byte（字节类型）
typedef bitset<32> word;


/*函数：将8个char字符转换成二进制*/
bitset<64> charToBit(const char s[8])
{
	bitset<64> bits;
	for (int i=0; i<8; ++i)
		for(int j=0; j<8; ++j)
			bits[i*8+j] = ((s[i]>>j) & 1);		//s[i]右移j位同00000001与
	return bits;
}

/*函数：将64位二进制转化为8个char*/
string bitToChar(const bitset<64>& bits)
{
	string s;
	for(int i=0; i<8; ++i)
	{
		bitset<8> temp;			//取8bit
		for(int j=0; j<8; ++j)		
			temp[j] = bits[j+i*8];
		char c = temp.to_ulong();					//暂存8bit转换成的字符
		if(c != '\0')
			s = s+c;
	}
	return s;
}

wstring bitToWchar(const bitset<64>& bits)
{
	wstring s;
	for(int i=0; i<4; ++i)
	{
		bitset<16> temp;			//取8bit
		for(int j=0; j<16; ++j)		
			temp[j] = bits[j+i*16];
		wchar_t c = 0;					//暂存8bit转换成的字符
		/*for(int k=15; k>0; --k)
		{
			c = c + pow(2,15-k);
		}*/
		c = temp.to_ulong();
		s += c;
	}
	return s;
}

/*函数：将8个char字符转换成二进制*/
bitset<64> wcharToBit(const wchar_t s[4])
{
	bitset<64> bits;
	for (int i=0; i<4; ++i)
		for(int j=0; j<16; ++j)
			bits[i*16+j] = ((s[i]>>j) & 1);		//s[i]右移j位同00000001与
	return bits;
}



unsigned int octsToInt(const char* s)
{
	unsigned int n = 0;
	for(int i=0; i<strlen(s); ++i){
		n = n*10;
		n += s[i]&0x0f;
	}
	return n;
}


