#include <string>
#include <bitset.h>
using namespace std;

typedef bitset<8> Byte;	//8bit����ΪByte���ֽ����ͣ�
typedef bitset<32> word;


/*��������8��char�ַ�ת���ɶ�����*/
bitset<64> charToBit(const char s[8])
{
	bitset<64> bits;
	for (int i=0; i<8; ++i)
		for(int j=0; j<8; ++j)
			bits[i*8+j] = ((s[i]>>j) & 1);		//s[i]����jλͬ00000001��
	return bits;
}

/*��������64λ������ת��Ϊ8��char*/
string bitToChar(const bitset<64>& bits)
{
	string s;
	for(int i=0; i<8; ++i)
	{
		bitset<8> temp;			//ȡ8bit
		for(int j=0; j<8; ++j)		
			temp[j] = bits[j+i*8];
		char c = temp.to_ulong();					//�ݴ�8bitת���ɵ��ַ�
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
		bitset<16> temp;			//ȡ8bit
		for(int j=0; j<16; ++j)		
			temp[j] = bits[j+i*16];
		wchar_t c = 0;					//�ݴ�8bitת���ɵ��ַ�
		/*for(int k=15; k>0; --k)
		{
			c = c + pow(2,15-k);
		}*/
		c = temp.to_ulong();
		s += c;
	}
	return s;
}

/*��������8��char�ַ�ת���ɶ�����*/
bitset<64> wcharToBit(const wchar_t s[4])
{
	bitset<64> bits;
	for (int i=0; i<4; ++i)
		for(int j=0; j<16; ++j)
			bits[i*16+j] = ((s[i]>>j) & 1);		//s[i]����jλͬ00000001��
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


