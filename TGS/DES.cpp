#include"DES.h"
#include <QDebug>
#include<iostream>
using namespace std;

int IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
             60, 52, 44, 36, 28, 20, 12, 4,
             62, 54, 46, 38, 30, 22, 14, 6,
             64, 56, 48, 40, 32, 24, 16, 8,
             57, 49, 41, 33, 25, 17, 9,  1,
             59, 51, 43, 35, 27, 19, 11, 3,
             61, 53, 45, 37, 29, 21, 13, 5,
             63, 55, 47, 39, 31, 23, 15, 7 };

int IP_1[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
              39, 7, 47, 15, 55, 23, 63, 31,
              38, 6, 46, 14, 54, 22, 62, 30,
              37, 5, 45, 13, 53, 21, 61, 29,
              36, 4, 44, 12, 52, 20, 60, 28,
              35, 3, 43, 11, 51, 19, 59, 27,
              34, 2, 42, 10, 50, 18, 58, 26,
              33, 1, 41,  9, 49, 17, 57, 25 };

int PC_1[56] = { 57, 49, 41, 33, 25, 17, 9,
               1, 58, 50, 42, 34, 26, 18,
              10,  2, 59, 51, 43, 35, 27,
              19, 11,  3, 60, 52, 44, 36,
              63, 55, 47, 39, 31, 23, 15,
               7, 62, 54, 46, 38, 30, 22,
              14,  6, 61, 53, 45, 37, 29,
              21, 13,  5, 28, 20, 12,  4 };

int PC_2[48] = { 14, 17, 11, 24,  1,  5,
               3, 28, 15,  6, 21, 10,
              23, 19, 12,  4, 26,  8,
              16,  7, 27, 20, 13,  2,
              41, 52, 31, 37, 47, 55,
              30, 40, 51, 45, 33, 48,
              44, 49, 39, 56, 34, 53,
              46, 42, 50, 36, 29, 32 };

int E[48] = { 32,  1,  2,  3,  4,  5,
            4,  5,  6,  7,  8,  9,
            8,  9, 10, 11, 12, 13,
           12, 13, 14, 15, 16, 17,
           16, 17, 18, 19, 20, 21,
           20, 21, 22, 23, 24, 25,
           24, 25, 26, 27, 28, 29,
           28, 29, 30, 31, 32,  1 };

int S[8][4][16] = {
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

int P[32] = { 16,  7, 20, 21,
           29, 12, 28, 17,
            1, 15, 23, 26,
            5, 18, 31, 10,
            2,  8, 24, 14,
           32, 27,  3,  9,
           19, 13, 30,  6,
           22, 11,  4, 25 };

int shift[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };


bitset<56> K_init_pmt(bitset<64> K)
{
    bitset<56> K0;
    //K0.reset();
    for (int i = 0; i < 56; i++)
        K0[i] = K[64-PC_1[i]];
    return K0;
}

bitset<28> Shift(bitset<28> D,int n)
{
    bitset<28> shift_D;
    if (shift[n]==2)
    {
        int temp1 = D[0];
        int temp2 = D[1];
        shift_D = D >> shift[n];
        shift_D[26] = temp1;
        shift_D[27] = temp2;
    }
    else
    {
        int temp = D[0];
        shift_D = D >> shift[n];
        shift_D[27] = temp;
    }
    return shift_D;
}

bitset<48> K_cp_pmt(bitset<28> Ci, bitset<28> Di)
{
    bitset<48> Ki;
    for (int i = 0; i < 48; i++)
    {
        if (PC_2[i] <= 28) Ki[i] = Ci[PC_2[i]-1];
        else Ki[i] = Di[PC_2[i]-28-1];
    }
    return Ki;
}

int init_pmt(bitset<64> M, bitset<32>* L0, bitset<32>* R0)//！！！！！！
{
    for (int i = 0; i < 64; i++)
    {
        if (i <= 31)//L0
            (*L0)[i]= M[IP[i]-1];//指针！！！！！
        else//R0
            (*R0)[i-32] = M[IP[i]-1];
    }
    return 0;
}

bitset<64> term_pmt(bitset<64> Round16)
{
    bitset<64> Cipher;
    for (int i = 0; i < 64; i++)
    {
        Cipher[i] = Round16[IP_1[i]-1];
    }
    return Cipher;
}

bitset<48> ep_pmt(bitset<32> R)
{
    bitset<48> ER;
    for (int i = 0; i < 48; i++)
    {
        ER[i] = R[E[i]-1];
    }
    return ER;
}

bitset<32> s_box(bitset<48> KR)
{
    bitset<32> SR;
    for (int i = 0; i < 8; i++)
    {
        int row = KR[i * 6] * 2 + KR[i * 6 + 5];
        int col = KR[i * 6 + 1] * 8 + KR[i * 6 + 2] * 4 + KR[i * 6 + 3] * 2 + KR[i * 6 + 4];
        int temp = S[i][row][col];
        bitset<4> si=temp;//！！！
        for (int j = 0; j < 4; j++)
        {
            SR[i * 4 + j] = si[3-j];//！！！
        }
    }
    return SR;
}

bitset<32> p_pmt(bitset<32> SR)
{
    bitset<32> PR;
    for (int i = 0; i < 32; i++)
    {
        PR[i] = SR[P[i] - 1];
    }
    return PR;
}

bitset<64> cycle(bitset<48> Keys[16], bitset<64>M, bool en_or_de)
{
    bitset<64> Round16,Cipher;
    bitset<48> ER, KR;
    bitset<32> L, R, SR, PR, temp;

    init_pmt(M, &L, &R);

    for (int i = 0; i < 16; i++)
    {
        ER = ep_pmt(R);

        if(en_or_de)
            KR = ER ^ Keys[i];
        else
            KR = ER ^ Keys[15-i];

        SR = s_box(KR);
        PR = p_pmt(SR);
        temp = R;
        R = L ^ PR;
        L = temp;
    }

    for (int i = 0; i < 64; i++)
    {
        if (i <= 31) Round16[i] = R[i];
        else Round16[i] = L[i - 32];
    }

    Cipher = term_pmt(Round16);

    return Cipher;
}

//string进制转换
string Hex2bin (string strHex)
{
    string sReturn = "";
    unsigned int len = strHex.length();
    for (unsigned int i = 0; i<len; i++)
    {
        switch ( strHex[i])
        {
            case '0': sReturn.append ("0000"); break;
            case '1': sReturn.append ("0001"); break;
            case '2': sReturn.append ("0010"); break;
            case '3': sReturn.append ("0011"); break;
            case '4': sReturn.append ("0100"); break;
            case '5': sReturn.append ("0101"); break;
            case '6': sReturn.append ("0110"); break;
            case '7': sReturn.append ("0111"); break;
            case '8': sReturn.append ("1000"); break;
            case '9': sReturn.append ("1001"); break;
            case 'a': sReturn.append ("1010"); break;
            case 'b': sReturn.append ("1011"); break;
            case 'c': sReturn.append ("1100"); break;
            case 'd': sReturn.append ("1101"); break;
            case 'e': sReturn.append ("1110"); break;
            case 'f': sReturn.append ("1111"); break;
            case 'A': sReturn.append ("1010"); break;
            case 'B': sReturn.append ("1011"); break;
            case 'C': sReturn.append ("1100"); break;
            case 'D': sReturn.append ("1101"); break;
            case 'E': sReturn.append ("1110"); break;
            case 'F': sReturn.append ("1111"); break;
        }
    }
    return sReturn;
}

string Bin2hex (string strBin)
{
    string sReturn = "";
    unsigned int len = strBin.length();
    for (unsigned int i = 0; i<len; i+=4)
    {
        int b=((strBin[i]-'0')*8+(strBin[i+1]-'0')*4+(strBin[i+2]-'0')*2+(strBin[i+3])-'0');
        switch (b)
        {
            case 0: sReturn.append ("0"); break;
            case 1: sReturn.append ("1"); break;
            case 2: sReturn.append ("2"); break;
            case 3: sReturn.append ("3"); break;
            case 4: sReturn.append ("4"); break;
            case 5: sReturn.append ("5"); break;
            case 6: sReturn.append ("6"); break;
            case 7: sReturn.append ("7"); break;
            case 8: sReturn.append ("8"); break;
            case 9: sReturn.append ("9"); break;
            case 10: sReturn.append ("A"); break;
            case 11: sReturn.append ("B"); break;
            case 12: sReturn.append ("C"); break;
            case 13: sReturn.append ("D"); break;
            case 14: sReturn.append ("E"); break;
            case 15: sReturn.append ("F"); break;
        }
        //        sReturn.append();
    }
    return sReturn;
}

//获取密钥
QByteArray Get_K()
{
    QByteArray K;
    K.resize(5);
    for(int i=0;i<64;i++)
    {
        K[i] = rand()%2;
    }
    return K;
}

//每一分片DES加密
bitset<64> DES_encrypt(string strK,string strM,int mode)
{
        bitset<64> K(strK),M(strM),Cipher;
        bitset<56> K0;
        bitset<48> Keys[16] = {};
        bitset<28> C, D;

        if(mode){
            //将M转换为bitset顺序
            for (int i = 0; i < 32; i++)
            {
                int temp = M[i];
                M[i] = M[63 - i];
                M[63 - i] = temp;
            }
        }
        K0 = K_init_pmt(K);
        for (int i = 0; i < 28; i++)
        {
            C[i] = K0[i];
            D[i] = K0[i + 28];
        }

        for (int i = 0; i < 16; i ++ )
        {
            C = Shift(C, i);
            D = Shift(D, i);
            Keys[i] = K_cp_pmt(C, D);
        }

        Cipher=cycle(Keys, M, mode);
//         for (int k = 0; k < Cipher.size(); k++)
//         {
//             if (k % 8 == 0) printf(" ");
//         }

        return Cipher;
}

//补位
void Supplement(int* mode,int* str_len,string* str_m)
{
    if(mode)//加密时补位
    {
        if((*str_len)%16)
        {
            int n_add=8-((*str_len)%16)/2;
            //char add=('0'+n_add);
            string add;
            switch (n_add)
            {
                case 0: add="40"; break;
                case 1: add="31"; break;
                case 2: add="32"; break;
                case 3: add="33"; break;
                case 4: add="34"; break;
                case 5: add="35"; break;
                case 6: add="36"; break;
                case 7: add="37"; break;
                case 8: add="38"; break;
            }
            while(n_add)
            {
                (*str_m)=(*str_m)+add;
                n_add--;
            }
        }
        else
            (*str_m)=(*str_m)+"4040404040404040";
    }
}

//整体DES加密
QByteArray Encrypt(QByteArray K,QByteArray M,int mode)
{
    K=K.toHex();//QBt->十六进制QBt
    M=M.toHex();//QBt->十六进制QBt
    QString qstr_k=K,qstr_m=M,qstr_c;//十六进制QBt->QString
    string str_k=qstr_k.toStdString();//QString->string
    string str_m=qstr_m.toStdString();//QString->string
    int str_len=str_m.length();
    int *md=&mode;
    int *strl=&str_len;
    string *stringm=&str_m;
    if(mode)
        Supplement( md, strl, stringm);//补位
    string bin_k=Hex2bin(str_k);
    string bin_m=Hex2bin(str_m);//string->二进制string
    bitset<64> bit_k(bin_k);//密钥
    vector<bitset<64>> bit_m;//明文
    string str_c="";//string密文
    QByteArray qbit_c;//QByteArray密文
    int len=bin_m.length();
    for(int i=0;i<len/64;i++)//分片加密
    {
        bitset<64> piece_c=DES_encrypt(bin_k,bin_m.substr(0,64),mode);//每个分片加密
        bin_m.erase(0,64);
        string str_c_piece=piece_c.to_string();
        if(mode==0)
            for (int i = 0,j = str_c_piece.size() - 1; i < j; ++i, --j)
                swap(str_c_piece[i], str_c_piece[j]);
        str_c=str_c+str_c_piece;
    }
    string hex_c=Bin2hex(str_c);
    if(mode==0)
    {
        string add = hex_c.substr(hex_c.length() - 1, 1);
        int n = atoi(add.c_str());
        if (n)
            hex_c.erase(hex_c.length() - n * 2, n * 2);
        else
            hex_c.erase(hex_c.length() - 16, 16);
    }
    qstr_c= QString::fromStdString(hex_c);
    qbit_c=qstr_c.toUtf8();
    qbit_c=QByteArray::fromHex(qbit_c);
    return qbit_c;
}

QByteArray DES_en(QByteArray K,QByteArray M){return Encrypt(K,M,1);}
QByteArray DES_de(QByteArray K,QByteArray C){return Encrypt(K,C,0);}



