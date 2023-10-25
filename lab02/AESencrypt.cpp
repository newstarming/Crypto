#include<iostream>
#include<cstring>

using namespace std;

// S盒置换表
unsigned char sBox[] =
{ /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
   0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76, /*0*/
   0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0, /*1*/
   0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15, /*2*/
   0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75, /*3*/
   0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84, /*4*/
   0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf, /*5*/
   0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8, /*6*/
   0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2, /*7*/
   0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73, /*8*/
   0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb, /*9*/
   0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79, /*a*/
   0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08, /*b*/
   0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a, /*c*/
   0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e, /*d*/
   0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf, /*e*/
   0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16  /*f*/
};

// 存储轮转密钥
unsigned char w[11][4][4];

// 十六进制字符串转换为
void hexStringToCharArray(const string& hexStr, unsigned char* output);

// 字节替代，将输入状态矩阵（4x4字节矩阵）中的每个字节替换为S盒（Substitution Box）中对应的值
void SubBytes(unsigned char state[][4]);

// 对AES状态矩阵进行行位移操作
void ShiftRows(unsigned char state[][4]);

// 对AES状态矩阵的每一列进行列混淆
void MixColumns(unsigned char state[][4]);

// 有限域GF(2^8)上的乘法
unsigned char FFmul(unsigned char a, unsigned char b);

// 将轮密钥（Round Key）与状态矩阵进行异或运算
void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

// 根据主密钥扩展生成轮密钥
void KeyExpansion(unsigned char* key, unsigned char w[][4][4]);

// 加密,按照AES-128的规范，对输入数据进行一系列的替代、位移、混淆和轮密钥操作
unsigned char* Cipher(unsigned char* input);

// 打印
void print(unsigned char* state);



int main() {
   string keyT, plaintextT;

   // 储存明文
   unsigned char Plaintext[16];

   // 储存密钥
   unsigned char Key[16];

   // 存储密文
   unsigned char Ciphertext[16];

   cin >> keyT;
   cin >> plaintextT;

   unsigned char Sbox[256];

   memcpy(Sbox, sBox, 256);

   // 调用函数进行转换
    hexStringToCharArray(plaintextT, Plaintext);
    hexStringToCharArray(keyT, Key);

    KeyExpansion(Key, w);

    print(Cipher(Plaintext));


   return 0;
}

// 十六进制字符串转换为
void hexStringToCharArray(const string& hexStr, unsigned char* output) {
    size_t length = hexStr.length();
    for (size_t i = 0; i < length; i += 2) {
        string byteString = hexStr.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(stoul(byteString, nullptr, 16));
        output[i / 2] = byte;
    }
}

// 字节替代，将输入状态矩阵（4x4字节矩阵）中的每个字节替换为S盒（Substitution Box）中对应的值
void SubBytes(unsigned char state[][4])
{
   int r, c;
   for (r = 0; r < 4; r++)
   {
       for (c = 0; c < 4; c++)
       {
           state[r][c] = sBox[state[r][c]];
       }
   }
}

// 对AES状态矩阵进行行位移操作
void ShiftRows(unsigned char state[][4])
{
   unsigned char t[4];
   int r, c;
   for (r = 1; r < 4; r++)
   {
       for (c = 0; c < 4; c++)
       {
           t[c] = state[r][(c + r) % 4];
       }
       for (c = 0; c < 4; c++)
       {
           state[r][c] = t[c];
       }
   }
}

// 对AES状态矩阵的每一列进行列混淆
void MixColumns(unsigned char state[][4])
{
   unsigned char t[4];
   int r, c;
   for (c = 0; c < 4; c++)
   {
       for (r = 0; r < 4; r++)
       {
           t[r] = state[r][c];
       }
       for (r = 0; r < 4; r++)
       {
           state[r][c] = FFmul(0x02, t[r])
               ^ FFmul(0x03, t[(r + 1) % 4])
               ^ FFmul(0x01, t[(r + 2) % 4])
               ^ FFmul(0x01, t[(r + 3) % 4]);
       }
   }
}

// 有限域GF(2^8)上的乘法
unsigned char FFmul(unsigned char a, unsigned char b)
{
   unsigned char bw[4];
   unsigned char res = 0;
   int i;
   bw[0] = b;
   for (i = 1; i < 4; i++)
   {
       bw[i] = bw[i - 1] << 1;
       if (bw[i - 1] & 0x80)
       {
           bw[i] ^= 0x1b;
       }
   }
   for (i = 0; i < 4; i++)
   {
       if ((a >> i) & 0x01)
       {
           res ^= bw[i];
       }
   }
   return res;
}

// 将轮密钥（Round Key）与状态矩阵进行异或运算
void AddRoundKey(unsigned char state[][4], unsigned char k[][4])
{
   int r, c;
   for (c = 0; c < 4; c++)
   {
       for (r = 0; r < 4; r++)
       {
           state[r][c] ^= k[r][c];
       }
   }
}

// 根据主密钥扩展生成轮密钥
void KeyExpansion(unsigned char* key, unsigned char w[][4][4])
{
   int i, j, r, c;
   unsigned char rc[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
   for (r = 0; r < 4; r++)
   {
       for (c = 0; c < 4; c++)
       {
           w[0][r][c] = key[r + c * 4];
       }
   }
   for (i = 1; i <= 10; i++)
   {
       for (j = 0; j < 4; j++)
       {
           unsigned char t[4];
           for (r = 0; r < 4; r++)
           {
               t[r] = j ? w[i][r][j - 1] : w[i - 1][r][3];
           }
           if (j == 0)
           {
               unsigned char temp = t[0];
               for (r = 0; r < 3; r++)
               {
                   t[r] = sBox[t[(r + 1) % 4]];
               }
               t[3] = sBox[temp];
               t[0] ^= rc[i - 1];
           }
           for (r = 0; r < 4; r++)
           {
               w[i][r][j] = w[i - 1][r][j] ^ t[r];
           }
       }
   }
}

// 加密,按照AES-128的规范，对输入数据进行一系列的替代、位移、混淆和轮密钥操作
unsigned char* Cipher(unsigned char* input)
{
   unsigned char state[4][4];
   int i, r, c;

   for (r = 0; r < 4; r++)
   {
       for (c = 0; c < 4; c++)
       {
           state[r][c] = input[c * 4 + r];
       }
   }

   AddRoundKey(state, w[0]);

   for (i = 1; i <= 10; i++)
   {
       SubBytes(state);
       ShiftRows(state);
       if (i != 10)MixColumns(state);
       AddRoundKey(state, w[i]);
   }

   for (r = 0; r < 4; r++)
   {
       for (c = 0; c < 4; c++)
       {
           input[c * 4 + r] = state[r][c];
       }
   }

   return input;
}

// 打印
void print(unsigned char* state)
{
	int i;
	for(i=0; i<16; i++)
	{
        printf("%s%X", state[i] < 16 ? "0" : "", state[i]);
	}
}