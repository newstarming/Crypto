#include <iostream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// S盒代替函数
vector<bitset<4>> SBox = {
    bitset<4>("1100"), bitset<4>("0000"), bitset<4>("0100"), bitset<4>("1111"),
    bitset<4>("1101"), bitset<4>("0110"), bitset<4>("1011"), bitset<4>("0101"),
    bitset<4>("1010"), bitset<4>("0011"), bitset<4>("1110"), bitset<4>("1000"),
    bitset<4>("0010"), bitset<4>("1001"), bitset<4>("0001"), bitset<4>("0111")
};

// 密钥轮函数
void KeySchedule(bitset<8> &key) {
    // 这里只是一个示例，实际的密钥轮函数应更复杂
    key = key ^ bitset<8>("11011011");
}

// SPN加密函数
bitset<8> SPNEncrypt(bitset<8> plaintext, bitset<8> key) {
    int plaintextSize = plaintext.size();
    int keySize = key.size();

    if (plaintextSize % 4 != 0) {
        cout << "明文长度必须是4的倍数。" << endl;
        exit(1);
    }

    if (keySize != plaintextSize) {
        cout << "密钥长度必须与明文长度相同。" << endl;
        exit(1);
    }

    // 初始化轮密钥
    bitset<8> roundKey = key;

    // 轮函数加密
    for (int round = 0; round < 4; round++) {
        plaintext = plaintext ^ roundKey;

        // S盒代替
        for (int i = 0; i < plaintextSize / 4; i++) {
            bitset<4> nibble(plaintext.to_string().substr(i * 4, 4));
            bitset<4> substitutedNibble = SBox[nibble.to_ulong()];
            plaintext &= ~(bitset<8>(15) << (i * 4)); // 清空当前4位
            plaintext |= bitset<8>(substitutedNibble.to_ulong()) << (i * 4);
        }

        // P4置换（这里省略）

        // 密钥轮函数
        KeySchedule(roundKey);
    }

    // 最后一轮不交换
    plaintext = plaintext ^ roundKey;

    return plaintext;
}

int main() {
    string plaintextStr, keyStr;
    bitset<8> plaintext, key, ciphertext;

    cout << "请输入明文（二进制）: ";
    cin >> plaintextStr;
    cout << "请输入密钥（二进制）: ";
    cin >> keyStr;

    plaintext = bitset<8>(plaintextStr);
    key = bitset<8>(keyStr);

    ciphertext = SPNEncrypt(plaintext, key);

    cout << "加密后的密文为: " << ciphertext << endl;

    return 0;
}
