#ifndef DES_H
#define DES_H

#include <iostream>
#include <bitset>
#include <string>
#include <qbytearray.h>
#include <qdatastream.h>
#include <qbuffer.h>
#include <time.h>

using namespace std;


//秘钥初始置换——PC_1
bitset<56> K_init_pmt(bitset<64> K);
//秘钥压缩置换——PC_2
bitset<48> K_cp_pmt(bitset<28> Ci,bitset<28> Di);
//初始置换——IP
int init_pmt(bitset<64> M,bitset<32>* L0, bitset<32>* R0);
//终止置换——P_1
bitset<64> term_pmt(bitset<64>Cipher);
//扩展置换——E
bitset<48> ep_pmt(bitset<32> R);
//S盒子——S
bitset<32> s_box(bitset<48> KR);
//P置换——P
bitset<32> p_pmt(bitset<32> SR);
//循环左移——shift
bitset<28> Shift(bitset<28> D,int n);
//轮次循环
bitset<64> cycle(bitset<48>Keys[16], bitset<64>M, bool en_or_de);
//string进制转换
string Hex2bin (string strHex);
string Bin2hex (string strBin);
//分片补位至64位
void Supplement(int* mode,int* str_len,string* str_m);
//DES加密函数
bitset<64> DES_encrypt(string K,string M,int mode);
//加密
QByteArray Encrypt(QByteArray K,QByteArray M,int mode);

//加解密过程的K、M、C都是二进制QByteArray类型
//加解密
QByteArray DES_en(QByteArray K,QByteArray M);//加密
QByteArray DES_de(QByteArray K,QByteArray C);//解密
//生成密钥
QByteArray Get_K();


#endif // DES_H
