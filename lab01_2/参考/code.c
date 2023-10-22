#include <stdio.h>
#include <string.h>
#define abs(a) {if(a >= 4000) a -= 4000; else a = 4000 - a;}
typedef unsigned short ushort;
typedef unsigned int uint;

const int MAX = 8005;
int n, count13[2][16][16], cnt13[16][16], cnt24[16][16];
ushort plaintext[MAX], ciphertext[MAX], tail_key;
int key51, key52, key53, key54;
uint key;

//SPN statement
const unsigned short sBox_4[16] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
const unsigned short inverse_sBox[16] = {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5};
const unsigned short pos[17] = {0x0,
								0x8000, 0x4000, 0x2000, 0x1000,
                                0x0800, 0x0400, 0x0200, 0x0100,
                                0x0080, 0x0040, 0x0020, 0x0010,
                                0x0008, 0x0004, 0x0002, 0x0001};
const unsigned short pBox[17] = {0x0,
								 0x8000, 0x0800, 0x0080, 0x0008,
								 0x4000, 0x0400, 0x0040, 0x0004,
								 0x2000, 0x0200, 0x0020, 0x0002,
								 0x1000, 0x0100, 0x0010, 0x0001};
unsigned short sBox_16[65536], spBox[65536];

void get_spBox(){  //获得spBox 
	for(int i = 0; i < 65536; ++i){
		sBox_16[i] = (sBox_4[i >> 12] << 12) | (sBox_4[(i >> 8) & 0xf] << 8) | (sBox_4[(i >> 4) & 0xf] << 4) | sBox_4[i & 0xf];
		spBox[i] = 0;
		for(int j = 1; j <= 16; ++j){
			if(sBox_16[i] & pos[j]) spBox[i] |= pBox[j];
		}
	} 
}

inline ushort read(){
	char ch;
	ushort buf = 0;
	for(int i = 0; i < 4; ){
		ch = getchar();
		if(ch >= '0' && ch <= '9'){
			buf = (buf << 4) | (ch ^ 48);
			i++;
		}
		else if(ch >= 'a' && ch <= 'z'){
			buf = (buf << 4) | (ch - 'a' + 10);
			i++;
		}
	}
	return buf;
}

void output(){
	char buf[8];  //输出缓冲区
	for(int i = 0; i < 8; ++i){
		buf[7 - i] = key & 0xf;
		if(buf[7 - i] < 10) buf[7 - i] += '0';
		else buf[7 - i] = (buf[7 - i] - 10) + 'a'; 
		key >>= 4;
	}
	for(int i = 0; i < 8; ++i) putchar(buf[i]);
	putchar('\n');
}

inline void input(){
	for(int i = 1; i <= 8000; ++i){
		plaintext[i] = read();
		ciphertext[i] = read();
	}
}

int main(){
	get_spBox();
	
	scanf("%d", &n);
	bool flag;
	ushort u1, u2, u3, u4;
	ushort x_init, y_init, z;
	ushort x[13], y[5];
	for(int i = 0; i < n; ++i){
		input();
		flag = false;
		//linear24();  //先分析第2、4位 
		memset(cnt24, 0, 256 * sizeof(int));
		for(int group = 1; group <= 8000; ++group){
			//提前处理要用到的对应位值 
			x_init = plaintext[group];
			for(int pos = 1; pos <= 12; ++pos){
				x[pos] = (x_init >> (16 - pos)) & 0x1;
			}
			y_init = ciphertext[group];
			for(int pos = 1, k = 12; pos <= 4; ++pos, k -= 4){
				y[pos] = (y_init >> k) & 0xf;
			}
			
			for(int L1 = 0; L1 < 16; ++L1){
				for(int L2 = 0; L2 < 16; ++L2){
					u2 = inverse_sBox[L1 ^ y[2]];
					u4 = inverse_sBox[L2 ^ y[4]];
					z = (x[5] ^ x[7] ^ x[8] ^ (u2 >> 2) ^ u2 ^ (u4 >> 2) ^ u4) & 0x1;
					if(!z) cnt24[L1][L2]++;
				}
			}
		}
		
		//处理count相加值
		for(int L1 = 0; L1 < 16; ++L1){
			for(int L2 = 0; L2 < 16; ++L2){
				abs(cnt24[L1][L2]);
			}
		}
		
		//外循环 
		for(int round24 = 0; round24 < 64; ++round24){
			//计算最大的2、4位对应密钥值 
			int max24 = -1;
			for(int L1 = 0; L1 < 16; ++L1){
				for(int L2 = 0; L2 < 16; ++L2){
					if(cnt24[L1][L2] > max24){
						max24 = cnt24[L1][L2];
						key52 = L1;
						key54 = L2;
					}
				}
			}
			cnt24[key52][key54] = 0;
			
			//根据2、4位对应密钥值计算1、3位对应密钥值； 
			//linear13();
			memset(count13, 0, 512 * sizeof(int));
			for(int group = 1; group <= 8000; ++group){
				//提前处理要用到的对应位值 
				x_init = plaintext[group];
				for(int pos = 1; pos <= 12; ++pos){
					x[pos] = (x_init >> (16 - pos)) & 0x1;
				}
				y_init = ciphertext[group];
				for(int pos = 1, k = 12; pos <= 4; ++pos, k -= 4){
					y[pos] = (y_init >> k) & 0xf;
				}
				//开始计算count 
				for(int L1 = 0; L1 < 16; ++L1){
					for(int L2 = 0; L2 < 16; ++L2){
						u1 = inverse_sBox[y[1] ^ L1];
						u2 = inverse_sBox[y[2] ^ key52];
						u3 = inverse_sBox[y[3] ^ L2];
						u4 = inverse_sBox[y[4] ^ key54];
						
						z = (x[1] ^ x[2] ^ x[4] ^ (u1 >> 3) ^ (u2 >> 3) ^ (u3 >> 3) ^ (u4 >> 3)) & 0x1;
						if(!z) count13[0][L1][L2]++;
						z = (x[9] ^ x[10] ^ x[12] ^ (u1 >> 1) ^ (u2 >> 1) ^ (u3 >> 1) ^ (u4 >> 1)) & 0x1;
						if(!z) count13[1][L1][L2]++;
					}
				}
			}
			
			//处理count相加值
			for(int L1 = 0; L1 < 16; ++L1){
				for(int L2 = 0; L2 < 16; ++L2){
					abs(count13[0][L1][L2]);
					abs(count13[1][L1][L2]);
					cnt13[L1][L2] = count13[0][L1][L2] + count13[1][L1][L2];
				}
			}
			
						
			for(int round13 = 0; round13 < 2; ++round13){
				int max13 = -1;
				for(int L1 = 0; L1 < 16; ++L1){
					for(int L2 = 0; L2 < 16; ++L2){
						if(cnt13[L1][L2] > max13){
							max13 = cnt13[L1][L2];
							key51 = L1;
							key53 = L2;
						}
					} 
				}
				cnt13[key51][key53] = 0;
				tail_key = (key51 << 12) | (key52 << 8) | (key53 << 4) | key54;				
				
				//穷举
				int count, fore_key, k1, k2, k3, k4, k5;
				for(fore_key = 0; fore_key < 65536; ++fore_key){
					key = (fore_key << 16) | tail_key;
					//get_roundKey();
					k5 = tail_key;
					k4 = (key >> 4) & 0xffff;
					k3 = (key >> 8) & 0xffff;
					k2 = (key >> 12) & 0xffff;
					k1 = (key >> 16) & 0xffff;
					for(count = 1; count < 4; ++count){
						//encrypt: ciphertext = sBox_16[spBox[spBox[spBox[plaintext ^ k1] ^ k2] ^ k3] ^ k4] ^ k5; 
						if((sBox_16[spBox[spBox[spBox[plaintext[count] ^ k1] ^ k2] ^ k3] ^ k4] ^ k5) != ciphertext[count]) break; 
					}
					if(count == 4){
						flag = true;
						break;
					}
				}
				if(flag) break; 
			}
			if(flag) break;
		}
		output();
	}
	
	return 0;
}
