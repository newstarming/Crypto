#include<stdio.h>
#include<iostream>

unsigned short sbox[16]={0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7};//s�� 
unsigned short pbox[16]={1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};//p��
unsigned short vsbox[16]={0xe,0x3,0x4,0x8,0x1,0xc,0xa,0xf,0x7,0xd,0x9,0x6,0xb,0x2,0x0,0x5};//s�����
unsigned short vpbox[16]={1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};//p�����û� 

int read();//���ٶ���
void spn(unsigned int key,unsigned short x);
void roundkeys(int i,unsigned short &k,unsigned int key);//��ȡ����Կ
void sreplace(unsigned short u,unsigned short &v);//s�д���
void preplace(unsigned short v,unsigned short &w);//p���û�
void vsreplace(unsigned short v,unsigned short &u);//s�������
void vpreplace(unsigned short w,unsigned short &v);//p�����û� 

int main()
{
	int i,n;
	unsigned int key;
	unsigned short x;
	std::cin >> n;
	getchar();
	for(i=0;i<n;i++){
		key=read();
		x=read();
		spn(key,x);
	}
} 

int read(){//���ٶ��� 
	char ch;
	int i=0;
	int x=0;
	ch=getchar();
	while(ch!=' '&&ch!='\n')
	{
		x*=16;
		if(ch>='0'&&ch<='9') x+=ch-'0';
		else x+=ch-'a'+10;
		ch=getchar();
	} 
	return x;
} 

void spn(unsigned int key,unsigned short x)
{
	unsigned short w=x;
	unsigned short k,u,v;
	unsigned short y; 
	int n=4;
	int i;
	for(i=1;i<=n-1;i++){//���ּ��� 
		roundkeys(i,k,key);
		u=k^w;
		sreplace(u,v);
		preplace(v,w);
	}
	roundkeys(n,k,key);
	u=k^w;
	sreplace(u,v);
	roundkeys(n+1,k,key);
	y=v^k;
	printf("%04x ",y);
	
    y^=0x1;
	v=y^k;
	vsreplace(v,u);
	roundkeys(n,k,key);
	w=u^k;
	for(i=n-1;i>=1;i--)//���ֽ��� 
	{
		vpreplace(w,v);
		vsreplace(v,u);
		roundkeys(i,k,key);
		w=u^k;
	} 
	x=w;
	printf("%04x\n",x);
} 

void roundkeys(int i,unsigned short &k,unsigned int key)//��ȡ����Կ 
{
	unsigned int temp=0xffff0000;
	i-=1;
	temp=temp>>(4*i);
	k=(key&temp)>>(4-i)*4; 
}

void sreplace(unsigned short u,unsigned short &v)//s�д���
{
	unsigned short temp[]={0xf000,0x0f00,0x00f0,0x000f};
	unsigned short j,t=0x0000;
	int i;
	for(i=0;i<4;i++){
		j=(u&temp[i])>>(4*(3-i));
		t|=(sbox[j]<<(4*(3-i)));
	}
	v=t; 
} 

void preplace(unsigned short v,unsigned short &w)//p���û�
{
	unsigned int temp=0x10000;
	unsigned short t,j=0x0000;
	int i;
	for(i=0;i<16;i++){
		temp=temp>>1;
		t=(v&temp)>>(15-i);
		j|=(t<<(16-pbox[i]));
	} 
	w=j;
} 

void vsreplace(unsigned short v,unsigned short &u)//s�������
{
	unsigned short temp[]={0xf000,0x0f00,0x00f0,0x000f};
	unsigned short j,t=0x0000;
	int i;
	for(i=0;i<4;i++){
		j=(v&temp[i])>>(4*(3-i));
		t|=(vsbox[j]<<(4*(3-i)));
	}
	u=t;
} 

void vpreplace(unsigned short w,unsigned short &v)//p�����û�
{
	unsigned int temp=0x10000;
	unsigned short t,j=0x0000;
	int i;
	for(i=0;i<16;i++){
		temp=temp>>1;
		t=(w&temp)>>(15-i);
		j|=(t<<(16-vpbox[i]));
	} 
	v=j;
} 


