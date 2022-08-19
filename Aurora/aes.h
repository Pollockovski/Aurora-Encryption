//Copyright 2022 Yang Y., All rights reserved.
#pragma once
#include"base64.h"

using namespace std;

vector<vector<int>> sbox(16, vector<int>(16));
vector<vector<int>> invsbox(16, vector<int>(16));

string base16 = "0123456789ABCDEF";

int xorr (int a_, int b_)
{
	return (!a_ && b_) || (a_ && !b_);
}

vector<string> localization(24);

void init()
{
	//十进制s盒对应方法:sbox[a/16][a%16],a是数值
	//python脚本生成s盒，可以直接给下行加个//然后额外写一行sbox={*python生成的东西*}
	sbox = { {131,67,179,117,187,4,151,210,61,152,133,48,42,23,202,206},{105,190,122,72,111,156,149,191,158,86,249,33,26,32,107,169},{108,153,159,112,233,240,199,24,74,138,85,64,62,215,235,69},{47,220,109,8,135,125,181,99,236,175,232,115,106,119,70,180},{227,63,22,121,186,136,44,173,79,73,100,31,93,110,101,228},{10,230,193,21,103,13,247,37,198,102,224,157,219,163,43,246},{217,20,142,176,1,84,218,139,118,83,184,75,171,154,27,80},{197,124,81,208,50,87,248,16,143,7,140,6,166,137,144,252},{57,82,19,225,204,18,65,194,52,148,168,94,211,40,203,89},{114,147,68,196,201,188,34,146,39,88,241,3,104,216,164,55},{200,239,12,245,213,161,165,120,231,253,155,209,237,9,195,66},{250,132,238,229,90,141,29,35,45,78,91,2,167,185,56,28},{5,162,126,251,97,223,150,71,14,96,207,25,60,30,182,221},{59,41,116,113,174,0,205,172,254,128,46,222,53,145,123,76},{130,255,38,49,58,36,189,95,92,226,134,177,192,51,129,244},{77,54,214,170,127,98,178,243,234,17,160,183,242,212,11,15} };
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			invsbox[sbox[i][j] / 16][sbox[i][j] % 16] = i * 16 + j;
		}
	}
}

int xr(int a, int b)
{
	string bin_a, bin_b;
	bin_a = toBinary(a);
	bin_b = toBinary(b);
	string bin_c = "00000000";
	for (int i = 0; i < 8; i++)
	{
		if (bin_a[i] == bin_b[i])
		{
			bin_c[i] = '0';
		}
		else {
			bin_c[i] = '1';
		}
	}
	int c = fromBinary(bin_c);
	return c;
}

vector<vector<int>> xr(vector<vector<int>> a, vector<vector<int>> b)
{
	vector<vector<int>> c(4, vector<int>(4));
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c[i][j] = xr(a[i][j], b[i][j]);
		}
	}
	return c;
}

unsigned char XTIME(unsigned char x)
{
	return ((x << 1) ^ ((x & 0x80) ? 0x1b : 0x00));
}

unsigned char multiply(unsigned char a, unsigned char b)
{
	unsigned char temp[8] = { a };
	unsigned char tempmultiply = 0x00;
	int i = 0;
	for (i = 1; i < 8; i++) {
		temp[i] = XTIME(temp[i - 1]);
	}
	tempmultiply = (b & 0x01) * a;
	for (i = 1; i <= 7; i++) {
		tempmultiply ^= (((b >> i) & 0x01) * temp[i]);
	}
	return tempmultiply;
}

vector<vector<int>> mixColumn(vector<vector<int>> a)
{
	vector<vector<int>> b = { {2,3,1,1}, {1,2,3,1}, {1,1,2,3}, {3,1,1,2} };
	vector<vector<int>> c(4, vector<int>(4));
	int d;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int m = 0; m < 4; m++)
			{
				d = multiply(b[i][m],a[m][j]);
				c[i][j] = xr(c[i][j],fromBinary(toBinary(d)));
			}
		}
	}
	return c;
}

vector<vector<int>> invMixColumn(vector<vector<int>> a)
{
	vector<vector<int>> b = { {14,11,13,9}, {9,14,11,13}, {13,9,14,11}, {11,13,9,14} };
	vector<vector<int>> c(4, vector<int>(4));
	int d;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int m = 0; m < 4; m++)
			{
				d = multiply(b[i][m], a[m][j]);
				c[i][j] = xr(c[i][j], d);
			}
		}
	}
	return c;
}

vector<vector<int>> addRoundKeys(vector<vector<int>> a, vector<vector<int>> key, int n)
{
	vector<vector<int>> b(4, vector<int>(4));
	vector<vector<int>> c(4, vector<int>(4));
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			b[i][j] = key[i][j + 4 * n];
		}
	}
	c = xr(a, b);
	return c;
}

vector<vector<int>> byteSubstitution(vector<vector<int>> a)
{
	vector<vector<int>> b = a;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			b[i][j] = sbox[a[i][j] / 16][a[i][j] % 16];
		}
	}
	return b;
}

vector<vector<int>> invByteSubstitution(vector<vector<int>> a)
{
	vector<vector<int>> b = a;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			b[i][j] = invsbox[a[i][j] / 16][a[i][j] % 16];
		}
	}
	return b;
}

vector<vector<int>> invShiftRows(vector<vector<int>> a)
{
	vector<vector<int>> b = a;
	for (int i = 1; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i + j < 4)
			{
				b[i][j] = a[i][i + j];
			}
			else {
				b[i][j] = a[i][i + j - 4];
			}
		}
	}
	return b;
}

vector<vector<int>> shiftRows(vector<vector<int>> a)
{
	vector<vector<int>> b = a;
	for (int i = 1; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j - i >= 0)
			{
				b[i][j] = a[i][j - i];
			}
			else {
				b[i][j] = a[i][j - i + 4];
			}
		}
	}
	return b;
}

vector<vector<int>> AESEncryption(vector<vector<int>> a, vector<vector<int>> key)
{
	vector<vector<int>> temp = addRoundKeys(a, key, 0);
	for (int i = 1; i < 11; i++)
	{
		temp = byteSubstitution(temp);
		temp = shiftRows(temp);
		temp = mixColumn(temp);
		temp = addRoundKeys(temp, key, i);
	}
	temp = byteSubstitution(temp);
	temp = shiftRows(temp);
	temp = addRoundKeys(temp, key, 11);
	return temp;
}

vector<vector<int>> AESDecryption(vector<vector<int>> a, vector<vector<int>> key)
{
	vector<vector<int>> temp = addRoundKeys(a, key, 11);
	temp = invByteSubstitution(temp);
	temp = invShiftRows(temp);
	for (int i = 10; i > 0; i--)
	{
		temp = addRoundKeys(temp, key, i);
		temp = invMixColumn(temp);
		temp = invShiftRows(temp);
		temp = invByteSubstitution(temp);
	}
	temp = addRoundKeys(temp, key, 0);
	return temp;
}

void aes(string key)
{
	init();
	vector<int> k(192);
	vector<int> k_temp(3);
	vector<vector<int>> m(4, vector<int>(4));
	vector<vector<int>> c(4, vector<int>(4));
	vector<vector<int>> k1(4, vector<int>(48));
	if (key.length() > 256)
	{
		key.resize(256);
	}
	else if (key.length() < 256)
	{
		while (key.size() < 256)
		{
			key.push_back('A');
		}
	}
	string s="0000";
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			s[j] = key[j + i * 4];
		}
		k_temp = fromBase64toInt(s);
		for (int j = 0; j < 3; j++)
		{
			k[j + i * 3] = k_temp[j];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 48; j++)
		{
			k1[i][j] = k[i * 48 + j];
		}
	}
	int mode = 1;
	while (mode != 0)
	{
		cout << "选择工作模式:加密(1)解密(2)退出(0)" << endl;
		cin >> mode;
		vector<int> encrypted;
		vector<string> encrypted_str = {};
		string inp, otp;
		switch (mode) {
		case 1: {
			cout << "请输入明文:" << endl;
			cin.ignore();
			getline(cin, inp);
			cout << "密文:";
			int ia = 0;
			while (inp.size() > 0)
			{
				ia++;
				if (inp.size() >= 16)//输入
				{
					for (int j = 0; j < 4; j++)
					{
						for (int i = 0; i < 4; i++)
						{
							m[i][j] = inp[j * 4 + i];
						}
					}
					reverse(inp.begin(), inp.end());
					inp.resize(inp.size() - 16);
					reverse(inp.begin(), inp.end());
				}
				else {
					for (int j = 0; j < 4; j++)
					{
						for (int i = 0; i < 4; i++)
						{
							if (4 * j + i < inp.size())
							{
								m[i][j] = inp[j * 4 + i];
							}
							else {
								m[i][j] = 32;
							}
						}
					}
					inp.resize(0);
				}
				char temp1, temp2;
				m = AESEncryption(m, k1);
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						cout << base16[m[j][i] / 16];
						cout << base16[m[j][i] % 16];
					}
				}
			}
			cout << endl;
			break;
		}
		case 2: {

			string inp, otp;
			cout << "请输入密文:" << endl;
			cin >> inp;
			if (inp.size() % 32 != 0)
			{
				cout << "请输入正确格式的密文" << endl;
				break;
			}
			cout << "明文:";
			vector<int> encrypted, temp;
			while (inp.size() > 0)
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						m[j][i] = base16.find(inp[i * 8 + j * 2]) * 16 + base16.find(inp[i * 8 + j * 2 + 1]);
					}
				}
				c = AESDecryption(m, k1);
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						cout << (char)c[j][i];
					}
				}
				reverse(inp.begin(), inp.end());
				inp.resize(inp.size() - 32);
				reverse(inp.begin(), inp.end());
			}
			cout << endl;
			break;
		}
		default:break;
		}
	}
}