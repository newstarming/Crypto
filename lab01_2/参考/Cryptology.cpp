#include <iostream>
#include<fstream>
#include<sstream>
#include <cstring>
#include <cmath>
#include<iomanip>
#include<conio.h>
#include <windows.h>
#include <ctime>
#define l 4//S�г���
#define m 4//ÿ�ּ��ܵ�S�и���
#define size 16//
#define N 4//N+1�Ǽ�������
#define size 16//�����С 
#define T 8000//���������������������T���������ģ��������������������
using namespace std;
typedef struct tagciphertextinfo {
    char* filename;
	WORD  w;
    WORD  v;
} ciphertextinfo;
//
//---------------------------------------- һ��SPN����ļӡ�����
//
//���ܺ��� ����16λ�������ַ�
//
int BlockEncryption(char* PlainText, char* CipherText, int Key) 
{
    int Nr = N;
    int PIs[16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
    int PIp[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
        
    int K[5][4];
    int r = 5;
	int i,j;
    for (i = 0; i < r; i++) {
        for (int j = 0; j < 4; j++) {
            K[i][j] = Key >> 28 - 4 * (i + j) & 0xF;
        }
    }
    int w[4], u[4], v[4];
    for (i = 0; i < 4; i++) w[i] = PlainText[i / 2] >> ((i + 1) % 2) * 4 & 0xF;
    
    for (i = 0; i < r - 1; i++) {
        for (j = 0; j < 4; j++) {u[j] = w[j] ^ K[i][j]; }
        for (j = 0; j < 4; j++) {v[j] = PIs[u[j]];}
        int w_t[16];
        for (j = 0; j < 16; j++) w_t[j] = v[j / 4] >> (3 - j % 4) & 1; 
        for (j = 0; j < 4; j++) {
            w[j] = 0;
            for (int k = 0; k < 4; k++) 
                w[j] += w_t[PIp[j * 4 + k]] << (3 - k);
        } 
    }
    CipherText[0] = ((v[0] ^ K[4][0]) << 4) + v[1] ^ K[4][1];//���һ�� �õ����ܺ������
    CipherText[1] = ((v[2] ^ K[4][2]) << 4) + v[3] ^ K[4][3];
    return 0;
}
//
//���ܺ���
//
int BlockDecryption(char* PlainText, char* CipherText, int Key) 
{
    int Nr = N;
    int PIs[16] = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};
    int PIp[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};    
    
    int K[5][4];
    int r = 5;
	int i,j;
    for (i = 0; i < r; i++) {
        for (j = 0; j < 4; j++) {
            K[i][j] = Key >> 28 - 4 * (i + j) & 0xF;
        }
    }
    int w[4], u[4], v[4];
    
    v[0] = (((CipherText[0] >> 4) & 0xF) ^ K[4][0]) & 0xF;
    v[1] = ((CipherText[0] & 0xF) ^ K[4][1]) & 0xF;
    v[2] = (((CipherText[1] >> 4) & 0xF) ^ K[4][2]) & 0xF;
    v[3] = ((CipherText[1] & 0xF) ^ K[4][3]) & 0xF;
    
    for (i = r - 2; i >= 0; i--) {
        for (j = 0; j < 4; j++) {u[j] = PIs[v[j]];}
        for (j = 0; j < 4; j++) {w[j] = u[j] ^ K[i][j]; }
        int v_t[16];
        for (j = 0; j < 16; j++) v_t[j] = w[j / 4] >> (3 - j % 4) & 1; 
        for (j = 0; j < 4; j++) {
            v[j] = 0;
            for (int k = 0; k < 4; k++) 
                v[j] += v_t[PIp[j * 4 + k]] << (3 - k);
        } 
    }
    PlainText[0] = (w[0] << 4) + w[1];//�õ����ܺ������
    PlainText[1] = (w[2] << 4) + w[3];
    return 0;
}
//
//--------------------------------�����ӡ������ļ�
//
//ȡ�ļ���С
long filesize( FILE *fp )
{
    long save_pos;
    long size_of_file;
    save_pos = ftell( fp );//���浱ǰλ��
    fseek( fp, 0L, SEEK_END );//��ת���ļ�β��
    size_of_file = ftell( fp );//ȡ����λ��
    fseek( fp, save_pos, SEEK_SET );//�ص���ʼλ��
    return( size_of_file );
}
//
//CBCģʽ ���� �����ļ�->���ɼ����ļ�CipherFile
int CBC_ENC(FILE* CipherFile, int Key, int IV = 0)
{
    BYTE* CipherText;
    int CipherFileSize = filesize(CipherFile);
    CipherText = new BYTE[CipherFileSize];
    fseek(CipherFile, 0, SEEK_SET);
    fread(CipherText, CipherFileSize, 1, CipherFile);//��CipherFile�����ݿ�����CipherText��
    
    char inter_cipher[2];//�м���
	//
	//����CipherText
    printf("��1���ּ���ǰ:%02X%02X\n", CipherText[0] & 0xff, CipherText[1] & 0xff); //���16������ʽ
    inter_cipher[0] = ((IV >> 8) & 0xFF) ^ CipherText[0];//���ʼ�������
    inter_cipher[1] = (IV & 0xFF) ^ CipherText[1];
    printf("��1��������:%02X%02X\n", inter_cipher[0] & 0xff, inter_cipher[1] & 0xff);
    BlockEncryption(inter_cipher, inter_cipher, Key);//����
    printf("��1���ּ��ܺ�:%02X%02X\n", inter_cipher[0] & 0xff, inter_cipher[1] & 0xff);
    CipherText[0] = inter_cipher[0];
    CipherText[1] = inter_cipher[1];
	//
	//�����ļ�����
    for (int i = 2; i < CipherFileSize; i += 2) {
        printf("��%d���ּ���ǰ:%02X%02X\n", i/2+1,CipherText[i] & 0xff, CipherText[i + 1] & 0xff);
        inter_cipher[0] ^= CipherText[i];
        inter_cipher[1] ^= CipherText[i + 1];
        printf("��%d��������:%02X%02X\n", i/2+1,inter_cipher[0] & 0xff, inter_cipher[1] & 0xff);
        BlockEncryption(inter_cipher, inter_cipher, Key);
        printf("��%d���ּ��ܺ�:%02X%02X\n", i/2+1,inter_cipher[0] & 0xff, inter_cipher[1] & 0xff);
        CipherText[i] = inter_cipher[0];
        CipherText[i + 1] = inter_cipher[1];
    }
    fseek(CipherFile, 0, SEEK_SET); 
    fwrite(CipherText, CipherFileSize, 1, CipherFile);//�����ܺ�õ���CipherTextд��CipherText�ļ���
    return 0;
}
//
//�ļ�����
int FileEncryption(char* PlainFileName, char* CipherFileName, int Key) 
{
    FILE* PlainFile = fopen(PlainFileName, "rb+");
    FILE* CipherFile = fopen(CipherFileName, "wb+");//Դ�ļ�->�����ļ�CipherFile
    ciphertextinfo CipherTextInfo;
    CipherTextInfo.filename = PlainFileName;
    CipherTextInfo.w = strlen(PlainFileName);//w
    WORD CipherFileSize = filesize(PlainFile);
    CipherTextInfo.v = (CipherTextInfo.w + CipherFileSize) % 2;//v
    BYTE* ComplementZero = NULL;
    ComplementZero = new BYTE[CipherTextInfo.v % 2];
    if (CipherTextInfo.v == 2) memset(ComplementZero, 0, sizeof(ComplementZero));
    
    BYTE* PlainText  =  NULL;
    PlainText = new BYTE[CipherFileSize];
    fread(PlainText, CipherFileSize, 1, PlainFile);
    cout<<"\n�������ļ�Ϊ��<"<<CipherTextInfo.filename<<">\n\n";
    //
	//����Ҫ��ĸ�ʽд���ļ�CipherText��
    fwrite(&CipherTextInfo.w, 1, 1, CipherFile);
    fwrite(CipherTextInfo.filename, strlen(CipherTextInfo.filename), 1, CipherFile);
    fwrite(&CipherTextInfo.v, 1, 1, CipherFile);
    fwrite(PlainText, CipherFileSize, 1, CipherFile);
    fwrite(ComplementZero, CipherTextInfo.v % 2, 1, CipherFile);
    //��ʼ����
    CBC_ENC(CipherFile, Key);
    //���� �ر��ļ�
    fclose(PlainFile);
    fclose(CipherFile);
    return 0;
}
//
//CBCģʽ ����
int CBC_DEC(FILE* PlainFile, FILE* CipherFile, int Key) 
{
    BYTE* CipherText;
    WORD CipherFileSize = filesize(CipherFile);
    CipherText = new BYTE[CipherFileSize];
    fseek(CipherFile, 0, SEEK_SET);
    fread(CipherText, CipherFileSize, 1, CipherFile);   
    int i;
    char inter_plain[2];
    //���ļ�ĩβ��ʼ����
    for (i = CipherFileSize - 2; i >= 2; i -= 2) {
        inter_plain[0] = CipherText[i];
        inter_plain[1] = CipherText[i + 1];
        printf("��%d���ֽ���ǰ��%02X%02X\n", i/2+1,inter_plain[0] & 0xff, inter_plain[1] & 0xff);
        BlockDecryption(inter_plain, inter_plain, Key);
        printf("��%d��������%02X%02X\n", i/2+1,inter_plain[0] & 0xff, inter_plain[1] & 0xff);
        inter_plain[1] ^= CipherText[i - 1];
        inter_plain[0] ^= CipherText[i - 2];
        CipherText[i + 1] = inter_plain[1];
        CipherText[i] = inter_plain[0];
        printf("��%d���ֽ��ܺ�%02X%02X\n",i/2+1, inter_plain[0] & 0xff, inter_plain[1] & 0xff);
    }
    inter_plain[0] = CipherText[0];
    inter_plain[1] = CipherText[1];
    printf("��1���ֽ���ǰ��%02X%02X\n", inter_plain[0] & 0xff, inter_plain[1] & 0xff);
    BlockDecryption(inter_plain, inter_plain, Key);
    printf("��1���ֽ��ܺ�%02X%02X\n", inter_plain[0] & 0xff, inter_plain[1] & 0xff);
    CipherText[0] = inter_plain[0];
    CipherText[1] = inter_plain[1];
    //���� �ļ��� �ļ����� ->���ܺ��ļ�PlainFile��
    ciphertextinfo PlainFileInfo;
    PlainFileInfo.w = CipherText[0];
    PlainFileInfo.filename = new char[PlainFileInfo.w];
    for (i = 0; i < PlainFileInfo.w; i++) PlainFileInfo.filename[i] = CipherText[i + 1];
    PlainFileInfo.v = CipherText[1 + PlainFileInfo.w];
    BYTE* PlainText = NULL;
    int PlainTextLength = CipherFileSize - 2 - PlainFileInfo.w - PlainFileInfo.v;
    //cout<<"�ļ����ݳ���:"<<PlainTextLength<<endl;
    PlainText = new BYTE[PlainTextLength];
    for (i = 0; i < PlainTextLength; i++) PlainText[i] = CipherText[i + 2 + PlainFileInfo.w];
    //cout<<"�ļ�����:"<<PlainText<<endl;
    fseek(PlainFile, 0, SEEK_SET);
    fwrite(PlainText, PlainTextLength, 1, PlainFile);
    return 0;
}
//
//�ļ�����
int FileDecryption(char* PlainFileName, char* CipherFileName, int Key)
{
    FILE* PlainFile = fopen(PlainFileName, "wb+");//���������ļ� PlainFile��
    FILE* CipherFile = fopen(CipherFileName, "rb+");
    CBC_DEC(PlainFile, CipherFile, Key);
    fclose(PlainFile);
    fclose(CipherFile);    
    return 0;
}
//
//--------------------------------------�������������ٶ�
//
time_t EncryptionTimesTotal(int Times) //��ʱ��
{
	time_t TotalTimes = 0;
    srand((unsigned)time(0));
    for (int i = 0; i < Times; i++) {
        int Key = (rand() << 0x10) + rand();//���������ԿKey 
		//
		//�����п������ķ�����м���
        for (int PlainText_1 = 0; PlainText_1 <= 0xFF; PlainText_1++) {
            for (int PlainText_2 = 0; PlainText_2 <= 0xFF; PlainText_2++) {
                char PlainText[2];
                PlainText[0] = PlainText_1;
                PlainText[1] = PlainText_2;
                char CipherText[2];
                time_t seconds_start = time(NULL);//�����Կ����������ʱ��
                BlockEncryption(PlainText, CipherText, Key);
                time_t seconds = time(NULL) - seconds_start;//��ȥ�����Կ����������ʱ��
                TotalTimes += seconds;
            }
        }
        printf("���Ե�%d����Կ��%08X\n", i + 1, Key);
    }
    return TotalTimes;
}
double EncryptionTimes(double Times) //ƽ������ʱ��
{
    double totaltime=EncryptionTimesTotal(Times);
	return totaltime/Times;
} 


//
//---------------------------------------�ġ������������
//


void s(int *A,int *S){ //S�е��û���

	S[0]=A[14];             	S[1]=A[4]; 
	S[2]=A[13];                 S[3]=A[1]; 
	S[4]=A[2];               	S[5]=A[15]; 
	S[6]=A[11];                 S[7]=A[8];                   
	S[8]=A[3];		            S[9]=A[10];	 
	S[10]=A[6];	                S[11]=A[12]; 
	S[12]=A[5];	                S[13]=A[9];	 
	S[14]=A[0];	                S[15]=A[7]; 
	 
}//S�е����û� 
void s_1(int *A,int *S_1){ 
	int C[16]; 
	int k,i,j,temp; 
	s(A,C); 
	for(i=0;i<15;i++){ 
		k=i; 
		for(j=i+1;j<16;j++) 
			if(C[j]<C[k]) k=j; 
        temp=C[k]; 
		C[k]=C[i]; 
		C[i]=temp; 
		temp=A[k]; 
		A[k]=A[i]; 
		A[i]=temp; 
	} 
    for(i=0;i<16;i++){ 
		temp=C[i]; 
		C[i]=A[i]; 
		A[i]=C[i]; 
		S_1[i]=C[i]; 
	} 
} 
//��������Կ 
void K_setup(int (*K)[16]){ 
	const int M=32; //��ʼ��Կ�ĳ���
	int K0[M]={0,0,1,1, 1,0,1,0, 1,0,0,1, 0,1,0,0, 1,1,0,1, 0,1,1,0, 0,0,1,1, 1,1,1,1}; //Ԥ��ֵ �鱾�ϵ���Կ
	char ch; 
	int i,j; 
	char choice;
	cout<<"<ʹ���鱾����Կ�밴0���ֶ������밴1>:";
	cin>>choice;
	if(choice=='1'){
		cout<<"����32λ��Կ��0��1��ʾ����K0=";
		for(i=0;i<M;i++) {cin>>ch;  K0[i]=ch-48;}
		cout<<"\n�������ԿΪ��";
		for(i=0;i<M;i++) {
			if(i%4==0) cout<<' ';
			cout<<K0[i];
		}
	}
	else {
		cout<<"\n��ԿΪ��";
		for(i=0;i<M;i++) {
			if(i%4==0) cout<<' ';
			cout<<K0[i];
		}
	}
	for( i=1;i<N+2;i++) 
		for(j=0;j<16;j++) 
			K[i][j]=K0[(i-1)*4+j]; 
	cout<<"����ԿΪ��"<<endl; 
	for(i=1;i<N+2;i++){ 
		cout<<"K"<<i<<"="; 
		for(j=0;j<16;j++) {
			if(j%4==0) cout<<' ';
			cout<<K[i][j]; 
		}
		cout<<endl; 
	} 
} 
//P�� 
void p(int *B,int *P){   
	int i=0,j=0; 
	for(i=0;i<16;i++){ 
		j=4*(i%4)+floor(i/4.0); 
		P[i]=B[j]; 
	} 
} 
//SPN���ܺͽ����㷨 
void SPN(int *x,int *S,int *P,int (*K)[16],int *y){ 
	int W[N+1][16],U[N+1][16],V[N+1][16]; 
	int r,i,j,k; 
	for(i=0;i<16;i++) 	W[0][i]=x[i]; 
	for(r=1;r<N;r++){ 
		for(i=0;i<16;i++) 
			U[r][i]=(W[r-1][i]+K[r][i])%2; 
		for(j=1;j<=4;j++){ 
			int a=0,b=0; 
			for(k=0;k<4;k++) 
              a=a+U[r][k+4*(j-1)]*pow((double)2,(3-k)); 
		    b=S[a]; 
			for(k=0;k<4;k++){ 
				V[r][k+4*(j-1)]=b/pow((double)2,(3-k)); 
				b=b-pow((double)2,(3-k))*V[r][k+4*(j-1)]; 
			} 
		} 
		for(i=0;i<16;i++) 	W[r][i]=V[r][P[i]-1]; 
	} 
	for(i=0;i<16;i++) 
		U[N][i]=(W[N-1][i]+K[N][i])%2; 
	for(j=1;j<=4;j++){ 
		int a=0,b=0; 
		for(k=0;k<4;k++) 
			a=a+U[N][k+4*(j-1)]*pow((double)2,(3-k)); 
		b=S[a]; 
		for(k=0;k<4;k++){ 
			V[N][k+4*(j-1)]=b/pow((double)2,(3-k)); 
			b=b-pow((double)2,(3-k))*V[N][k+4*(j-1)]; 
		} 
	} 
	for(i=0;i<16;i++)  y[i]=(V[N][i]+K[N+1][i])%2; 
} 
//�����������
void LinearCryptanalysis(int *S,int *S_1,int *P,int (*K)[16]){ 
	int Q;
	int p[16][16],q[16][16]; 
    int X[16],Y[16], K1[3],V[5],U[16],U1[5]; 
	long int w; 
	int i,j,k,temp=0,n=0; 
    int *s; 
	long int h;
	cout<<"�������������ĶԵ�����(����8000)T=";
	cin>>Q;
	w=Q*16;
	s=new int[w]; 
	for(i=0;i<16;i++) 
		for(j=0;j<16;j++) 
			p[i][j]=0; 
	srand( (unsigned)time( NULL ) ); 
	for(h=0;h<w;h++) s[h]=rand()%2; 
	for(i=0;i<Q;i++){ 
		for(j=0;j<16;j++){       //���������������        
			X[j]=s[i*16+j];  
			h++; 
		} 
		SPN(X,S,P,K,Y);          //���ܵõ���Ӧ������ 
		int a[5]={0,0,0,0,0}; 
		for(j=1;j<5;j++){ 
			for(k=0;k<4;k++) 
				a[j]=a[j]+Y[4*(j-1)+k]*pow((double)2,(3-k)); 
		} 
		for( temp=0;temp<16;temp++) 
			for( n=0;n<16;n++){ 
				K1[1]=temp; 
				K1[2]=n; 
				V[2]=K1[1]^a[2]; 
				V[4]=K1[2]^a[4]; 
				U1[2]=S_1[V[2]]; 
				U1[4]=S_1[V[4]]; 
				for( k=4;k<8;k++){ 
					U[k]=U1[2]/pow((double)2,(7-k)); 
					U1[2]=U1[2]-pow((double)2,(7-k))*U[k]; 
				} 
				for(k=12;k<16;k++){ 
					U[k]=U1[4]/pow((double)2,(15-k)); 
					U1[4]=U1[4]-pow((double)2,(15-k))*U[k]; 
				} 
				int z=(X[4]+X[6]+X[7]+U[5]+U[7]+U[13]+U[15])%2; 
				if(z==0) p[temp][n]++; 
			} 
	} 
    // �õ�����ܵ�����Կ
	int max=-1; 
	int a,b; 
	for(temp=0;temp<16;temp++) 
		for(n=0;n<16;n++){ 
			q[temp][n]=abs(p[temp][n]-Q/2); 
			if(q[temp][n]>max){ 
				max=q[temp][n]; 
				a=temp;b=n; 
			} 
		} 
	//���������ѡ����Կ��ƫ��
	printf("\n�����ĺ�ѡ����Կ�������:("); 
	printf("%x",a); 
	printf(","); 
	printf("%x",b); 
	printf("),��Ӧ��ƫ��Ϊ:e="); 
	printf("%f",(p[a][b]-Q/2)/(Q*1.0)); 
	printf("\n"); 
}

//
//----------------------------------------- ���Ժ���
//
//
//���Ժ���1
void TestBlockEncrypt() 
{
    char PlainText[2];
    PlainText[0] = 0x26; PlainText[1] = 0xB7; //Ԥ������ ���̲��е�26B7��
    char CipherText[2];
    int Key = 0x3A94D63F;  //Ԥ����Կ���̲��е���Կ)
	cout<<"��ʹ�ý̲��е����� 26B7 ��Կ 3A94D63F��\n";
	//cout<<"��������Ҫ���ܵ����ģ������ַ�):";
	//cin>>PlainText[0]>>PlainText[1];
	printf("\n���������(ʮ������):%02X%02X\n",PlainText[0]&0xFF,PlainText[1]&0xFF);
    BlockEncryption(PlainText, CipherText, Key);
	//cout.setf(std::ios::hex);
	//cout<<""<<hex<<12<<' '<<CipherText[0]<<CipherText[1]<<endl;
    printf("\n���ܺ�����(ʮ������):%02X%02X\n", CipherText[0] & 0xFF, CipherText[1] & 0xFF); //������ܽ��
    BlockDecryption(PlainText, CipherText, Key);
	printf("\n���ܺ������(ʮ������):%02X%02X\n",PlainText[0]& 0xFF,PlainText[1]& 0xFF);//������ܽ��
}
//
//���Ժ���2
void TestFileEncrypt() 
{
	char choice;
	string FileRoute;
    int Key = 0x3A94D63F;  //Ԥ����Կ(�̲��е���Կ)
    char* PlainFile = "test.txt";      //Դ�ļ���ַ
    char* CipherFile = "file_enc.txt"; //���ܺ��ļ���ַ
    char* DecFile = "file_dec.txt";    //���ܺ��ļ���ַ
	//--------------------------------------------------------ѡ������ļ�
	cout<<"<-----����----- ʹ��Ԥ���ļ��밴0���ֶ������ļ�·���밴1��>";
	cin>>choice;
	if(choice=='1')
	{
		cout<<"\n��������Ҫ���ܵ��ļ�·��:";
		cin>>FileRoute;
		PlainFile=&FileRoute[0];
		cout<<"���ܺ���ļ�Ϊ��file_enc.txt���ڳ������и�Ŀ¼��!\n";
		cout<<"�������������...��";
		getch();
	}
	else
	{
		cout<<"Ԥ���ļ�Ϊtest.txt,���ܺ���ļ�Ϊ��file_enc.txt���ڳ������и�Ŀ¼��!\n";
		cout<<"�������������...��";
		getch();
	}
	cout<<"��ʼ����...\n";
    FileEncryption(PlainFile, CipherFile, Key);//���ܺ���
	cout<<"**************�������!****************\n\n";
	//-------------------------------------------------------ѡ������ļ�
	cout<<"<-----����----- ʹ��Ԥ���ļ��밴0���ֶ������ļ�·���밴1��>";
	cin>>choice;
	if(choice=='1')
	{
		cout<<"\n��������Ҫ���ܵ��ļ�·��:";
		cin>>FileRoute;
		CipherFile=&FileRoute[0];
		cout<<"���ܺ���ļ�Ϊ��file_dec.txt,�ڳ������и�Ŀ¼��!\n";
		cout<<"�������������...��";
		getch();
	}
	else
	{
		cout<<"Ԥ���ļ�Ϊfile_enc.txt,���ܺ���ļ�Ϊ��file_dec.txt,�ڳ������и�Ŀ¼��!\n";
		cout<<"�������������...��";
		getch();
	}
	cout<<"\n��ʼ����...\n\n";
    FileDecryption(DecFile, CipherFile, Key);//���ܺ���
	cout<<"**************�������!****************\n\n";
}
//
//���Ժ���3
void TestEncryptionTime()
{
	double Times;
	cout<<"������������Ե���Կ��:";
	cin>>Times;
	cout<<"������Կ�����������ʱ��ã������ĵȴ�^^�������������...\n";
	getch();
    cout<<"ƽ��ʱ��TotalTimes/Times��"<<EncryptionTimes(Times)<<"��"<<endl; 
}
//
//���Ժ���4
void TestLinearCryptanalysis() 
{
	int A[16],B[16],K[N+2][16],S[16],S_1[16],P[16];   
    for(int i=0;i<16;i++) {   
        A[i]=i;   
        B[i]=i+1;   
    }   
    K_setup(K);   //�õ�����Կ
    s(A,S);   //S��
    s_1(A,S_1);   //S�е���
    p(B,P);   //P��
    LinearCryptanalysis(S,S_1,P,K);   
}

int main()//------------------------------------------������
{
	int choice;
	while(1)
	{
		cout<<"/////////////////���Բ˵�///////////////////\n";
	    cout<<"1.SPN-����ӡ�����                        //\n";
	    cout<<"2.�ӡ������ļ�                            //\n";
	    cout<<"3.���������ٶ�                            //\n";
	    cout<<"4.�����������                            //\n";
	    cout<<"0.�˳�                                    //\n";
	    cout<<"////////////////////////////////////////////\n";
	    cout<<"��������Ա��:";
	    cin>>choice;
	    cout<<"\n\n";
	    switch(choice)
	    {
	    case 1:{
			    cout<<"һ��\tSPN-����ӡ����ܣ�\n";
                TestBlockEncrypt();//------------------------���Ժ���1
	            cout<<"\nSPN-������ܡ��������!\n";
	            cout<<"(�����������...)\n\n";
	            getch();
			    break;
		     }
	    case 2:{
		    	cout<<"����\t�ӡ������ļ���\n";
                TestFileEncrypt(); //-------------------------���Ժ���2
	            cout<<"\n�ӡ������ļ����!\n";
	            cout<<"(�����������...)\n\n";
	            getch();
		    	break;
	    	}
	    case 3:{
		    	cout<<"�������������ٶȣ�\n";
                TestEncryptionTime();//-----------------------���Ժ���3
	            cout<<"\n���������ٶ����!\n";
	            cout<<"(�����������...)\n\n";
	            getch();
		    	break;
	    	}
	    case 4:{
		    	cout<<"�ġ������������:\n";
		    	TestLinearCryptanalysis();//-------------------���Ժ���4
		    	cout<<"\n��������������!\n";
		    	cout<<"(�����������...)\n\n";
		    	getch();
		    	break;
		    }
	    case 0:return 0;
	    default: return 0;
	    }

	}
    return 0;
}
