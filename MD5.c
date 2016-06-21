#include <stdio.h> 
#include <string.h>
#include <math.h>

#define BYTE unsigned char 

#define UINT unsigned int

/* R rotates x left y bits. */
#define R(x, y) (((x) << (y)) | ((x) >> (32-(y))))

BYTE* MD5_prepare_data(const BYTE* data,int* new_len)
{
int rest,fill,size;
BYTE* new_data;
UINT bit_len;
int len = strlen(data);
//printf("%d\n", len);

//輸入的數據進行補位，使得如果數據長度len對512求餘的結果是448。
//即數據擴展至(K*512+448)bit。即K*64+56 byte，K為整數。
//具體補位操作：先補一個1，然後補0至滿足上述要求
rest = len % 56;
if (rest <= 56) fill = 56 - rest;
else fill = (64 - rest) + 56;

new_data = (BYTE*)malloc(len + fill + 8);
if (NULL == new_data) return NULL;

if (len > 0) memcpy(new_data,data,len);
if (fill > 0) memset(new_data + len,0x80,1);
if (fill > 1) memset(new_data + len + 1,0,fill - 1);

size = fill + len;
//補數據長度：在以上結果後面加上一個以64 bit表示的填充前數據長度bit_len。
//這時，數據就被填補成長度為512位的倍數。
bit_len = len * 8;//byte轉成bit

memset(new_data + size + 0,(bit_len & 0x000000FF), 1);
memset(new_data + size + 1,(bit_len & 0x0000FF00) >> 8, 1);
memset(new_data + size + 2,(bit_len & 0x00FF0000) >> 16,1);
memset(new_data + size + 3,(bit_len & 0xFF000000) >> 24,1);

memset(new_data + size + 4,0,4);

*new_len = size + 8;

return new_data;
}

//////////////////////MD5主程式////////////////////////////

int MD5(const BYTE* data)
{
  int x,y,new_len,i;
  UINT h0,h1,h2,h3;
  UINT a,b,c,d;
  UINT abcd[4];
  UINT f,g,temp;
  UINT *X;
  BYTE* buf;

  //r specifies the per-round shift amounts
  static UINT r[64]={7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

  //Use binary integer part of the sines of integers as constants:
  static UINT K[64];  
  for(x = 0; x < 64; x++)
  {
    K[x] = (UINT)floor((((const double)0xFFFFFFFF + 1.0)* fabs(sin(x+1))));
  }
  
//Pre-processing:
  //append "1" bit to message
  //append "0" bits until message length in bits ≡ 448 (mod 512)
  //append bit length of message as 64-bit little-endian integer to message
  buf = MD5_prepare_data(data,&new_len);//獲得new_data
  if (buf == NULL) return -1;
  
//初始化MD5參數:4個32bit整數(h0,h1,h2,h3)用來計算message digest (16進制)
  abcd[0] = 0x67452301;
  abcd[1] = 0xefcdab89;
  abcd[2] = 0x98badcfe;
  abcd[3] = 0x10325476;
  
//Process the message in successive 512-bit chunks: 
  
  for(x = 0; x < new_len / 64; x++)
  {
        
    //Initialize hash value for this chunk:           
    a = abcd[0] ;
    b = abcd[1] ;
    c = abcd[2] ;
    d = abcd[3] ; 
    
    //for each 512-bit chunk of message 
      //break chunk into sixteen 32-bit little-endian words w[i]
    UINT W[16];
    for(y = 0; y < 16; y++)
    {
      W[y] =  buf[64 * x + 4 * y + 0];
      W[y] += buf[64 * x + 4 * y + 1] << 8;
      W[y] += buf[64 * x + 4 * y + 2] << 16;
      W[y] += buf[64 * x + 4 * y + 3] << 24;
    }
    
    //Main loop:
    for(i = 0; i < 64; i++)
  {   
        
    if (i>=0 && i<=15)//Round1
    {     
     f = ((b & c) | ((~b) & d));// F(x, y, z) = (((x) & (y)) | ((~x) & (z)))

     g = i;
    }
    else if (i>=16 && i<=31)//Round2
    { 
     f = ((b & d) | (c & (~d)));// G(x, y, z) = (((x) & (z)) | ((y) & (~z)))
     g = (5*i + 1) % 16;
    }
    else if (i>=32 && i<=47)//Round3
    { 
     f = b ^ c ^ d;//H(x, y, z) = ((x) ^ (y) ^ (z))
     g = (3*i + 5) % 16;
    }
    else if (i>=48 && i<=63)//Round 4
    { 
     f = c ^ (b | (!d)); // I(x, y, z) = ((y) ^ ((x) | (~z))) 
     g = (7*i) % 16;

    }
     
     temp = d;
     d = c;
     c = b;
     b = R((a + f + K[i] + W[g]),r[i]) + b;
     a = temp;

   }
   
    //Add this chunk's hash to result so far: 
    abcd[0] += a;
    abcd[1] += b;
    abcd[2] += c;
    abcd[3] += d;
    
  }

  printf("MD5(\"%s\")=",data);

  printf("%02X%02X%02X%02X",(abcd[0] & 0x000000FF),(abcd[0] & 0x0000FF00) >> 8,(abcd[0] & 0x00FF0000) >> 16,(abcd[0] & 0xFF000000) >> 24);

  printf("%02X%02X%02X%02X",(abcd[1] & 0x000000FF),(abcd[1] & 0x0000FF00) >> 8,(abcd[1] & 0x00FF0000) >> 16,(abcd[1] & 0xFF000000) >> 24);

  printf("%02X%02X%02X%02X",(abcd[2] & 0x000000FF),(abcd[2] & 0x0000FF00) >> 8,(abcd[2] & 0x00FF0000) >> 16,(abcd[2] & 0xFF000000) >> 24);

  printf("%02X%02X%02X%02X",(abcd[3] & 0x000000FF),(abcd[3] & 0x0000FF00) >> 8,(abcd[3] & 0x00FF0000) >> 16,(abcd[3] & 0xFF000000) >> 24);

  printf("\n");

  return 0;
}

//test
int main()
{
    
  BYTE *data[512];
  char s;

  do{
      printf("\nInput:");
      gets(data);
      printf("Output:");
      MD5(data);
      printf("Restart?(Y/N)");
      fflush(stdout);
      scanf("%c",&s);
      fflush(stdin);
      
      while((s!='Y')&&(s!='N')&&(s!='y')&&(s!='n'))
       {
          printf("Please enter the right (Y/N):");
          scanf("%c",&s);
          fflush(stdin);
      }
  }while((s!='n')&&(s!='N'));
  printf("Bye Bye\n");
  system("pause");
  return 0;
}
