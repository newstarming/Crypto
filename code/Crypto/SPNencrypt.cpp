//#include <iostream>
//#include <cstring>
//
//// S盒
//int pi_s[16] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
//// P盒
//int pi_p[16] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };
//// 储存明文
//char* x = (char*)malloc(16 * sizeof(char)); 
//// 储存密钥
//char* K = (char*)malloc(32 * sizeof(char)); 
//
//
//// 计算第n轮的子密钥
//char* key_n(int n, char* key)
//{
//    int index = 0;
//    for (int i = 4 * n - 4; i < 4 * n + 12; i++)
//    {
//        key[index] = K[i];
//        index++;
//    }
//    return key;
//}
//
//// S盒代替
//char* encrypt_pi_s(char* clear)
//{
//    for (int i = 0; i < 4; i++)
//    {
//        int dec = 0;
//        for (int j = 0; j < 4; j++)
//        {
//            dec *= 2;
//            dec += (clear[4 * i + j] - '0');
//        }
//        int dec_ex = pi_s[dec];
//        for (int j = 0; j < 4; j++)
//        {
//            clear[4 * i + 3 - j] = ((dec_ex % 2) + '0');
//            dec_ex /= 2;
//        }
//    }
//    return clear;
//}
//
//// 末置换
//char* encrypt_pi_p(char* clear)
//{
//    char temp[16];
//    for (int i = 0; i < 16; i++)
//    {
//        temp[i] = clear[i];
//    }
//    for (int i = 0; i < 16; i++)
//    {
//        clear[i] = temp[pi_p[i]];
//    }
//    return clear;
//}
//
//
//// 异或
//char* encrypt_XOR(char* clear, char* key)
//{
//    for (int i = 0; i < 16; i++)
//    {
//        clear[i] = (((clear[i] - '0') ^ (key[i] - '0')) + '0');
//    }
//    return clear;
//}
//
////int main()
////{
////    //std::cout << "请输入明文：\n";
////    std::cin >> x;
////    //std::cout << "请输入明文：\n";
////    std::cin >> K;
////
////    char* y = x;
////    char* keyN = (char*)malloc(16 * sizeof(char));
////    for (int i = 1; i <= 4; i++)
////    {
////        keyN = key_n(i, keyN);
////        if (i == 4)
////        {
////            y = encrypt_pi_s(encrypt_XOR(y, keyN));
////        }
////        else
////        {
////            y = encrypt_pi_p(encrypt_pi_s(encrypt_XOR(y, keyN)));
////        }
////    }
////
////    keyN = key_n(5, keyN);
////    y = encrypt_XOR(y, keyN);
////
////    //std::cout << "密文如下：\n";
////    std::cout << y;
////
////    return 0;
////}