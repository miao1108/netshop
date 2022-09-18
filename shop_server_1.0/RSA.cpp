#include"RSA.h"

//生成大素数
mpz_t * get_primes()
{
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time(NULL));

    mpz_t key_p, key_q;
    mpz_init(key_p);
    mpz_init(key_q);

    mpz_urandomb(key_p, grt, KEY_LENGTH / 2);
    mpz_urandomb(key_q, grt, KEY_LENGTH / 2);	//随机生成两个大整数

    mpz_t * result = new mpz_t[2];
    mpz_init(result[0]);
    mpz_init(result[1]);

    mpz_nextprime(result[0], key_p);  //使用GMP自带的素数生成函数
    mpz_nextprime(result[1], key_q);

    mpz_clear(key_p);
    mpz_clear(key_q);

    return result;
}
//获取密钥对
key_pair * get_key_pair()
{
    mpz_t * p = get_primes();
    //mpz_t * q= get_primes();
    mpz_t n, e, f;
    mpz_init(n);
    mpz_init(f);
    mpz_init_set_ui(e, 65537);
    mpz_mul(n, p[0], p[1]);		//计算n=p*q
    mpz_sub_ui(p[0], p[0], 1);		//p=p-1
    mpz_sub_ui(p[1], p[1], 1);		//q=q-1
    mpz_mul(f, p[0], p[1]);		//计算欧拉函数φ(n)=(p-1)*(q-1)
    mpz_t d;
    mpz_init(d);
    mpz_invert(d, e, f);   //计算数论倒数
    key_pair * res = new key_pair;
    char * buf_n = new char[KEY_LENGTH + 16];
    char * buf_d = new char[KEY_LENGTH + 16];
    mpz_get_str(buf_n, 16, n);
    mpz_get_str(buf_d, 16, d);
    res->n = buf_n;
    res->d = buf_d;
    res->e = 65537;

    mpz_clear(p[0]);   //释放内存
    mpz_clear(p[1]);
    mpz_clear(n);
    mpz_clear(d);
    mpz_clear(e);
    mpz_clear(f);
    delete []p;
    //delete []q;
    return res;
}

//加密
char * encrypt(const char * plain,char * n, int e)
{
    mpz_t M, C, key_n;
    mpz_init_set_str(M, plain, 16);
    //gmp_printf("plain=",M);
    mpz_init_set_str(key_n, n, 16);
    mpz_init_set_ui(C, 0);
    //gmp_printf("plain=",C);
    mpz_powm_ui(C, M, e, key_n);    //使用GMP中模幂计算函数

    char * res = new char[KEY_LENGTH + 16];
    mpz_get_str(res, 16, C);

    return res;
}

string encrypt_str(string plain,char * n, int e)
{
    mpz_t M, key_n;
    long str_len=plain.length();
    string out_data;
    mpz_init(M);
    mpz_init_set_str(key_n, n, 16);
    for(int i=0;i<str_len;i++)
    {
      mpz_set_ui(M,(unsigned long)plain[i]);
      mpz_powm_ui(M, M, e, key_n);    //使用GMP中模幂计算函数
      out_data.append(mpz_get_str(NULL,16,M));
      out_data.append("\n");
    }


    //mpz_init_set_ui(C, 0);
    //gmp_printf("plain=",C);
    //mpz_powm_ui(C, M, e, key_n);    //使用GMP中模幂计算函数

    //char * res = new char[KEY_LENGTH + 16];
    //mpz_get_str(res, 10, C);
    mpz_clear(M);
    //mpz_clear(C);
    mpz_clear(key_n);
    //char *res=(char*)out_data.c_str();
    return out_data;
}
//解密
char * decrypt(const char * cipher, char * n, char * d)
{

    mpz_t M, C, key_n, key_d;
    mpz_init_set_str(C, cipher, 16);
    mpz_init_set_str(key_n, n, 16);
    mpz_init_set_str(key_d, d, 16);
    mpz_init(M);
    mpz_powm(M, C, key_d, key_n);   //使用GMP中的模幂计算函数
    char * res = new char[KEY_LENGTH + 16];
    mpz_get_str(res, 16, M);
    mpz_clear(M);
    mpz_clear(C);
    mpz_clear(key_n);
    mpz_clear(key_d);
    return res;
}

//解密
string decrypt_str(string cipher, char * n, char * d)
{
    long str_len=cipher.length();
    vector<string>C_List;
    string temp_str;
    for(int i=0;i<str_len;i++)
        {
            if(cipher.at(i)=='\n')
            {
                C_List.push_back(temp_str);
                temp_str.clear();
                continue;
            }
            temp_str+=cipher.at(i);
        }
    mpz_t M, key_n, key_d;
    mpz_init_set_str(key_n, n, 16);
    mpz_init_set_str(key_d, d, 16);
    string back_data;
    for(int i=0;i<C_List.size();i++)
        {
            //从字符串取值转化为十进制大整数
            mpz_set_str(M,C_List.at(i).c_str(),16);
            //模幂运算M=(C^d) mod n
            mpz_powm(M,M,key_d,key_n);
            //先将取到的明文ASCII转化为C语言基本类型
            //再直接将ASCII码转为字符，再将字符追加进字符串
            //这样就完美还原了明文

            unsigned long lchar=mpz_get_ui(M);
            char words=(char)lchar;
            back_data+=words;
        }
    mpz_clear(M);
    //mpz_clear(C);
    mpz_clear(key_n);
    mpz_clear(key_d);
    return back_data;
}








