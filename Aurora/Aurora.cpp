//Copyright 2022 Yang Y., All rights reserved.
//快速幂模运算代码参考https://blog.csdn.net/Pumpkin_Tung/article/details/121443351
#include<conio.h>
#include<string>
#include"aes.h"
//懒得写密钥扩展了
//这个AES密钥是256个Base64字符组成的的字符串,经过转换变成192个八位二进制数直接作为密钥组,加密总共加密12轮甚至11轮罢。
//密钥用summonAESKey.py生成
string key = "1145141919810Shimokita5ZUgHJOK0BHEv6,AH3S,RrkQU58zxG,BLf/sguM9fAocN7xwUfb,v8L8eaTGpYzdy7HTiKqPXj6D6,7ej3NfqoL29Av7ZOWPj5l1HqiVNG60i+g1GF,J+qr3sa+32MZuuiZynOyJwFGP,bFEKFDDGSmb0QrRL3Pl8M3ay68o0inrPBYiz+Btm9fhWt1cQ9TZxTWQ0QQBsMqAIV9mB2Udx1DEaGfZPFpR8c148QnN0T";//AES密钥,256位

int getNum(char a)
{
	int i = a;
	return i;
}

char getChar(int a)
{
	char i = a;
	return i;
}

typedef unsigned long long ull;

ull a, b, p;
ull fastPow(ull n)
{
	if (n == 1) return a;
	if ((n & 1) == 1) {
		cout << p;
		ull k = fastPow(n >> 1) % p;
		return k * k % p * a % p;
	}
	else {
		ull k = fastPow(n >> 1) % p;
		return k % p * k % p;
	}

}
ull powMod(ull a, ull b, ull p)
{
	ull sum = 1;
	while (b) {
		if (b & 1) {
			sum = (sum * a) % p;
			//b--;
		}
		b >>= 1;
		a = a * a % p;
	}
	return sum;
}
ull gcd(ull a, ull b)
{
	return b != 0 ? gcd(b, a % b) : a;
}

struct coordinate {
	int x;
	int y;
};

void main()
{
	init();
	//int tmp114 = sbox[32 / 16][32 % 16];
	//cout << tmp114 << endl; 
	//cout << invsbox[tmp114/16][tmp114 % 16] << endl;
	//cout << endl;
init:
	int mode = 1;
	cout << "RSA(1),类AES(2),还是ECC(3)加密？如果你想要生成AES密钥请输入(4)。如果你不知道这是啥请输入其他数字。" << endl;
	cin >> mode;
	if (mode == 1)//rsa
	{
		ull p, q, f, n, e, d, c;
		int length;
		string str;
		char m;
		while (mode != 0)
		{
			cout << "请选择工作模式:加密(1)解密(2)生成密钥(3)退出(0)" << endl;
			cin >> mode;
			switch (mode) {
			case 1: {
				cout << "请输入要加密的字符串:";
				cin.ignore(1);
				getline(cin, str);
				cout << "请输入公钥:";
				cin >> e;
				cout << "请输入n:";
				cin >> n;
				cout << "密文:\n{";
				for (int i = 0; i < str.size(); i++)
				{
					cout << powMod(getNum(str[i]), e, n) << ",";
				}
				cout << "}" << endl;
				break;
			}
			case 2: {
				cout << "请输入解密次数:";
				cin >> length;
				cout << "请输入私钥:";
				cin >> d;
				cout << "请输入n:";
				cin >> n;
				str = "";
				while (length > 0)
				{
					length--;
					cout << "请输入密文:";
					cin >> c;
					m = getChar(powMod(c, d, n));
					str.push_back(m);
				}
				cout << "明文是:" << str << endl;
				break;
			}
			case 3: {
				cout << "请输入两个素数p,q,建议选大点但是不能太大:" << endl;
				cin >> p;
				cin >> q;
				cout << "选取一个素数e，使得e与(p-1)(q-1)互素(所有大于p和q的素数都满足要求):";
				cin >> e;
				n = p * q;
				f = (p - 1) * (q - 1);
				d = 1;
				while (true)
				{
					if ((e * d) % f == 1)
					{
						break;
					}
					d += 1;
				}
				cout << "公钥e:" << e << endl;
				cout << "私钥d:" << d << endl;
				cout << "n:" << n << endl;
				break;
			}
			default:break;
			}
		}
	}
	else if (mode == 2)//类aes
	{
		cout << "请输入密钥(256字节):";
		cin >> key;
		aes(key);
	}
	else if (mode == 3)//ecc
	{

	}
	else if (mode == 4)
	{
		system("python summonAESKey.py");
		goto init;
	}
	else
	{
		cout << "RSA是非对称加密算法，AES是对称加密算法，ECC是利用椭圆曲线的比RSA快的非对称加密算法。\n如果使用对称加密算法，你需要把密钥私下送给接收方或者加密后发送给接收方。\n如果使用非对称加密算法，你需要保留私钥，把公钥发给对方，用对面给的公钥加密，用自己的私钥解密对面用你的公钥加密的信息。" << endl;
		goto init;
	}
	cout << "按下任意键退出..." << endl;
	while (true)
	{
		if (_kbhit())
		{
			break;
		}
	}
}