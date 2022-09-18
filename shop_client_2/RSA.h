#ifndef RSA_H
#define RSA_H
#include <iostream>
#include<cstring>
#include<ctime>
#include<vector>
#include <cstdio>
#include"gmp.h"
#define KEY_LENGTH 1024  //公钥的长度


using namespace std;

struct key_pair
{
    char * n;
    char * d;
    int e;
};

key_pair * get_key_pair();
string decrypt_str(string cipher, char * n, char * d);
string encrypt_str(string plain,char * n, int e);//解密字符串
char * encrypt(const char * plain,char * n, int e);//加密数字
char * decrypt(const char * cipher, char * n, char * d);//解密数字
#endif // RSA_H

