package com.cyc.www.cry;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.Scanner;

import javax.crypto.Cipher;
/**
 * @author ust���޳�
 * ��spn�������Թ���:�����һ�ֵļ��ܹ��̽����ƽ⣬�����д���ܹ���
 */
public class SPN_ATTACK_LINEAR {
	private static int[][]	count;
	private static String line;	
	private static String path = "D:\\cipherAndPlain.dat"; 
	//private static short Ktemp = (short)0x3A94;
	private static short[] sBox = {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};
	private static short[] sBoxOpp = {14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5}; //s�е���
	private static short[]  pBox = {1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
	private static short[] Ktemp= {0x3A94,(short)0xA94D,(short)0x94D6,0x4D63,(short)0xD63f};
	private static short[] cipherNum;
	private static short[] plainNum;
	private static int NUM = 8000;	
	private static int iT = 0,jT = 0;
	
	
	public  static void main(String[] args) {
			initCPC();
			linearAttack();
			System.out.println("��spn�����Է�������������2������Կ"+iT+"\t"+jT); 
            exhaustKeyDiff();
		}	

	/*
	 * ��ɳ�ʼ�����������������Ķԣ��������ĳ�ʼ��
	 */
	public static  void initCPC() {
		File file = new File(path);
		plainNum = new short[NUM];
		cipherNum = new short[NUM];	
//		keyForTest = new short[16][16];
		count = new int[16][16];
		try {
			if (file.exists()) {
				Scanner in = new Scanner(new FileReader(file));
				for (int i = 0; i < NUM; i++) {
					line = in.nextLine();
					String[] tokens = line.split("\\|");
					plainNum[i] = (short) (Integer.parseInt(tokens[1]));
					cipherNum[i] = (short) (Integer.parseInt(tokens[3]));
					//System.out.println(plainNum[i]+"\t"+cipherNum[i]);
				}
				in.close();
			}
			else {
				plainCry();
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		/*
		 * ��ʼ����ѡ������,����Ҫ��ʼ����ѡ��Կ�ԣ���Ϊ��Կ�Զ����й��ɵġ�
		 */
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				//keyForTest[i][j] = (short)((i<<4)+j);
				count[i][j] = 0;
			}
		} 
	} 
	
	/*
	 * ����8000�������Ķ�
	 */
	public static void plainCry() {
		cipherNum = new short[NUM];
		plainNum = new  short[NUM];
		for (int i = 0; i < cipherNum.length; i++) {
			plainNum[i] = (short)(Math.random()*32767);
		}
		short[] u = new short[4];
		short[] v = new short[4];
		short[] w = new short[5];
		//�������ϵ����ӣ�����������3��spn�û��ټ���һ�֣����k4+s��+���k5
		for (int i = 0; i < plainNum.length; i++) {
			w[0] = plainNum[i];
			for (int j = 0; j < 3; j++) {
				u[j] = (short)(w[j] ^ Ktemp[j]);
				v[j] = Sbox(u[j],sBox);
				w[j+1] = Pbox(v[j], pBox);
			}
			u[3] = (short)(w[3] ^ Ktemp[3]);
			v[3] = Sbox(u[3], sBox);
			w[4] = (short)(v[3] ^ Ktemp[4]);
			cipherNum[i] = w[4];
		}
		try {
			PrintWriter out = new PrintWriter("d:\\cipherAndPlain.dat");
			for (int i = 0; i < cipherNum.length; i++) {
				out.println("����" +"|"+ plainNum[i] +"|"+ "����" +"|"+cipherNum[i]);
				//System.out.println("���ɲ���:  "+plainNum[i]+"\t"+cipherNum[i]);
			}
			out.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}	
	}
	
	public static short Sbox(short ui,short[] sBox) {
		short t[] = {-4096,3840,240,15};//��ȡ�ض�λ������
		short[] v = new short[4];
		v[0] = (short) (((ui & t[0]) << 16)>>>28);	//����short����������ʹ��ʱ�ᱻĬ��ת��λint����������
		v[1] = (short) (((ui & t[1]) << 16)>>>24); //�������õ�ǿ������ת��
		v[2] = (short) (((ui & t[2]) << 16)>>>20);
		v[3] = (short) (((ui & t[3]) << 16)>>>16);
		//�ؼ��滻
		for (int i = 0; i < v.length; i++) {
			v[i] = (short)((sBox[v[i]])<<(12-4*i));
		}
		return (short) (v[0]+v[1]+v[2]+v[3]);
	}
	
	public static short Pbox(short vi,short[] pBox) {
		short wi = 0;
		short[] psub = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		//�����ki��ÿһλ
		for (int i = 0; i < 16; i++) {
			psub[i] = (short) ((vi >>> (15-i) & 0x0001));
		}
		for (int i = 0; i < psub.length; i++) {
			wi += psub[i] << (16-pBox[i]);
		}
		return wi;
	}
	/*
	 * �������Է���
	 */
	public static void linearAttack() {
		short U2 =0,U4=0,v2=0,v4=0;
		int  z = 0,x5=0,x7=0,x8=0;
		int u6=0,u8=0,u14=0,u16=0;
		for (int i = 0; i < cipherNum.length; i++) {
			for (int j = 0; j < 16; j++) {
				for (int k = 0; k < 16 ; k++) {
					v2 = (short)(((cipherNum[i] & 0x0F00)>>> 8) ^ j);//j��Ӧ�ĺ�ѡ��Կ�Ե�K2
					v4 = (short)(((cipherNum[i] & 0x000F)) ^ k);//k��Ӧ�ĺ�ѡ��Կ�Ե�K4
					U2 = (short)sBoxOpp[v2];
					U4 = (short)sBoxOpp[v4];	
					x5 = (plainNum[i] & 0x0800) >>>11;
					x7 = (plainNum[i] & 0x0200) >>>9;
					x8 = (plainNum[i] & 0x0100) >>>8;
					u6 = (U2 & 0x0004)>>>2;
					u8 = (U2 & 0x0001);
					u14 = (U4 & 0x0004)>>>2;
					u16 = (U4 & 0x0001);
					z = x5 ^ x7 ^ x8 ^ u6 ^ u8 ^ u14 ^ u16;
					if (z == 0) {
						count[j][k]++;
					}					
				}
			}
		}
		int max = -1;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				count[i][j] = Math.abs(count[i][j] - NUM / 2);
				if ( count[i][j] > max ){ 
					max = count[i][j];
					iT = i;
					jT= j;
				}			
			}
		}
	}	
	
	/*
	 * ��һ��ָ��������x��ָ������Կki���ܣ����ؼ��ܵ�ֵ 
	 */
	public static short spnEncry(short ki[],short[] sBox,short plainNum) {
		short[] u = new short[4];
		short[] v = new short[4];
		short[] w = new short[5];
		//�������ϵ����ӣ�����������3��spn�û��ټ���һ�֣����k4+s��+���k5
			w[0] = plainNum;
			for (int j = 0; j < 3; j++) {
				u[j] = (short)(w[j] ^ ki[j]);
				v[j] = Sbox(u[j], sBox);
				w[j+1] = Pbox(v[j], pBox);
			}
			u[3] = (short)(w[3] ^ ki[3]);
			v[3] = Sbox(u[3], sBox);
			w[4] = (short)(v[3] ^ ki[4]);
			return w[4];
	}
	
	/*
	 * ��һ����ԿK����������Կ
	 */
	public static short[] getSonkeyofKey(int Key) {
		short[] ki  = new short[5];
		for (int i = 0; i < ki.length; i++) {
			ki[i] =(short)((Key >> (16- 4 * i)) & 0x0000ffff);
		}
		return ki;
	}
	
	public static void exhaustKeyDiff() {
		int keyleft = 0;
		int kTestN = 0x60f;
		for (int left = 0; left <= 0xffff; left++) {
			int kTest = kTestN + (left << 16);
			for (short k3 = 0; k3 <= 0xf; k3++) {
				int kTestT = kTest + ( k3 << 12);
				for (int k1 = 0; k1 <= 0xf; k1++) {
					int kTestTT = kTestT + (k1<<4);// kTestTTΪ���Ե�ԭʼ��Կ
					short[] sonkey = getSonkeyofKey(kTestTT);
					
					int counter = 0,flag =1;
					while (counter <=40 && flag != 0) {
						short crytest = spnEncry(sonkey, sBox, plainNum[counter]);
						if (crytest != cipherNum[counter]) {
							flag = 0;
						}
						else if (crytest == cipherNum[counter] && counter == 39) {
							System.out.printf("ͨ����ٷ��õ���������ԿΪ��%x\n", kTestTT);
						}
						counter ++;
					}
				}
			}
		}
	}
}
