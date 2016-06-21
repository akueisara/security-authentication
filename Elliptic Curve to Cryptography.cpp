#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sys\timeb.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <string.h>


using namespace std;

class ecc {
public :
	ecc();
	// 主程式
	void exchange_key(); // 兩使用者交換key資訊
	void encrypt_ECC(); // ECC加密
	void decrypt_ECC(); // ECC解密

	// 副程式
	long int prime(long int); // 輸入長度產生prime值
	long double mod(long double, long double); // mod
	void random_point(long int, long double, long double, long int); // 產生橢圓曲線上的任一點
	long int inverse_devision(long int, long double); // inverse devision
	void xy_twopointadd(long int, long double, long double, long double, long double, long double); // 兩點加法
	void xy_Multiplypoint(long int, long int, long int, long double, long double); // 兩點乘法

private :
	long double x3,y3; // variabel global (x3,y3)
	long double x_random,y_random; // variable global random point
};
ecc::ecc() {}
struct timeb t;

// 輸入長度產生prime值
long int ecc::prime(long int len){
	srand(time(NULL));
	long int i;
	long int rand_numb,count,result_devision,check,min,max,prime,cal_prim;
	min=pow(10,len-1);//10^(bit-1)
	max=pow(10,len);//10^bit
	do{
		do{
			rand_numb=mod(rand(),max);
		}while(rand_numb<=min || rand_numb<=3);
		for(i=2;i<=sqrt(rand_numb);i++)
		{
			count=rand_numb/i;
			result_devision=floor(count);
			check=result_devision*i;
			if(check==rand_numb)
			{
				break;
			}
			else if(check<rand_numb || rand_numb==2)
			{
				continue;
			}
		}
		cal_prim=rand_numb;
	}while(check==rand_numb);
	prime=cal_prim;
	return prime;
}

//a mod b
long double ecc::mod(long double a,long double b)
{
	if(fmodl(a,b)<0)
	{
		return (fmodl(a,b)+b);
	}
	else
	{
		return fmodl(a,b);
	}

}

// 產生橢圓曲線上的任一點
void ecc::random_point(long int p, long double a4, long double a6, long int keylength){
	long int check_point,zero_one,check_root;
	long double check_x;
	if(keylength<=5){
		do{
			x_random=rand()%p+1;
			y_random=rand()%p+1;
			check_point=mod(mod(powl(y_random,2),p)-mod(mod(powl(x_random,2),p)*x_random,p)-mod(a4*x_random,p)-a6,p);
		}	while(check_point!=0);
	}
	else if(keylength>5){ // volker muller
		do{
			x_random=rand()%p+1;
			check_x=sqrtl(mod(mod(mod(powl(x_random,2),p)*x_random,p)+mod((a4*x_random),p)+a6,p));
			check_root=floor(check_x);
		}while(check_x!=check_root);
		zero_one=rand()%2;
		zero_one;
		y_random = zero_one=1 ?  mod(check_x,p) : mod(-check_x,p);
	}


}

// inverse devision
long int ecc::inverse_devision(long int p,long double devision){
	long double a1,a2,a3,b1,b2,b3,t1,t2,t3;
	long int q;
	//安裝
	a1=1;  b1=0;
	a2=0;  b2=1;
	a3=p;  b3=mod(devision,p);
	//if(b3==0){ not have inverse or result point infinity }

	do{
		q=floor(a3/b3);
		t1=a1-mod(q*b1,p);
		t2=a2-mod(q*b2,p);
		t3=a3-mod(q*b3,p);

		a1=b1;
		a2=b2;
		a3=b3;
		b1=t1;
		b2=t2;
		b3=t3;
	}while(b3!=1);
	return mod(b2,p);
}

// 兩點加法=>(x3,y3)
void ecc::xy_twopointadd(long int p,long double a4, long double x1, long double x2, long double y1, long double y2)
{
	long double pembilangx,devisionx,pembilangy,devisiony,inverse_devisionx,inverse_devisiony;
	if(x1==x2 && y1==y2)
	{
		pembilangx=mod(powl(mod(3*mod(powl(x1,2),p)+a4,p),2),p);
		devisionx=mod(powl(mod(2*y1,p),2),p);
		pembilangy=mod(mod(3*mod(pow(x1,2),p)+a4,p),p);
		devisiony=mod(mod(2*y1,p),p);
		if(devisionx!=0)
		{
			inverse_devisionx=inverse_devision(p,devisionx);
			inverse_devisiony=inverse_devision(p,devisiony);
			x3=mod((mod(pembilangx*inverse_devisionx,p))-x1-x2,p);
			y3=mod(mod(pembilangy*inverse_devisiony,p)*mod((x1-x3),p)-y1,p);
		}
		else if(devisionx==0)
		{
			x3=0; // only sign
			y3=0; // only sign
		}
	}
	else if(x1!=x2 || y1!=y2)
	{
		if(x2==0 && y2==0)
		{
			y3=y1;
			x3=x1;
		}
		else if(x1==0 && y1==0)
		{
			y3=y2;
			x3=x2;
		}
		else
		{
			pembilangx=mod(powl(y2-y1,2),p);
			devisionx=mod(powl(x2-x1,2),p);
			pembilangy=mod(y2-y1,p);
			devisiony=mod(x2-x1,p);
			if(devisionx!=0)
			{
				inverse_devisionx=inverse_devision(p,devisionx);
				inverse_devisiony=inverse_devision(p,devisiony);
				x3=mod((mod(pembilangx*inverse_devisionx,p))-x1-x2,p);
				y3=mod(mod(pembilangy*inverse_devisiony,p)*mod((x1-x3),p)-y1,p);
			}
			else if(devisionx==0)
			{
				x3=0; //only sign
				y3=0; //only sign
			}
		}
	}
}

// 兩點乘法 =>(x3,y3)
// binary algorithm
void ecc::xy_Multiplypoint(long int p,long int a4, long int k, long double x, long double y){
	long int u[100],l,j,x1,y1;

	// 轉換純量到2進制
	l=1;
	while(k>0){
		if(k%2==1){
			if(k==1){
				u[l]=1;
				break;
			}
			u[l]=1;
		}
		else{
			u[l]=0;
		}
		k=k/2;
		l=l+1;
	}

	// addition with binary
	// 安裝(x3,y3)=(0,0)
	x3=0;
	y3=0;
	for(j=l;j>0;j--){
		if(u[j]==1){
			x1=x3;
			y1=y3;
			xy_twopointadd(p,a4,x1,x1,y1,y1);
			x1=x3;
			y1=y3;
			//2點相加function
			xy_twopointadd(p,a4,x1,x,y1,y);
		}
		else if(u[j]==0){
			x1=x3;
			y1=y3;
			//2點相加function
			xy_twopointadd(p,a4,x1,x1,y1,y1);
		}
	}
}   

void ecc::exchange_key()
{
	ofstream file_output;
	file_output.open("key.txt");
	ftime(&t);
	long int time_start, time_fthissh;
	float time_gen,second;
	long double a4,a6;
	long int p;
	long int private1,private2;
	long int x_key1,y_key1,x_key2,y_key2;
	long double x_public1,y_public1,x_public2,y_public2;
	char strbit[1000],buffer[1000];
	long int bit,count_input;
	long double det;

	srand(time(NULL));

	cout<<"\n-------------------exchange key----------------\n";
	cout<<"\n產生質數 p :\n";
	count_input=0;
	start:
	cout<<" =>輸入key length :";
	strcpy(strbit,gets(buffer));
	if (atoi(strbit)<=0 || atoi(strbit)>9 ||strlen(strbit)!=1){
		cout<<"\n<WARNING> : 'key length must be integer between 1 to 9'\n\n";
		count_input++;
		if(count_input%4==0){
			system("cls");
		}
		goto start;
	}
	bit=atoi(strbit); // bit is keylength
	time_start=time(NULL);//時間:產生key開始
	p=prime(bit);
	cout<<"\np = "<<p<<"\n";

	cout<<"\n產生curve : \n";
	do{
		a4=rand()%p;
		a6=rand()%p;
	}while(mod((4*mod(mod(powl(a4,2),p)*a4,p))+(27*mod(powl(a6,2),p)),p)==0);
	det=mod((4*mod(mod(powl(a4,2),p)*a4,p))+(27*mod(powl(a6,2),p)),p);
	cout<<" =>a4 = "<<long(a4)<<", "<<"a6 = "<<long(a6)<<"\n";
	cout<<" =>4*a4^3 + 27*a6^2 (mod "<<p<<")的值 = " <<long(det)<<"\n\n";
	cout<<"橢圓曲線 :"<<" "<<"y^2 = x^3 + "<<long(a4)<<"x + "<<long(a6);

	cout<<"\n\n決定 random point (x,y) : \n";
	random_point(p, a4, a6, bit); // 產生隨機點
	cout<<" =>被選出的random point = ("<<long(x_random)<<","<<long(y_random)<<")";

	cout<<"\n\n產生 private key : \n";
	do{
		// private的bound value ={1,..,p-1}
		private1=rand()%(p-1)+1;
		private2=rand()%(p-1)+1;
	}while(private1<1 || private2<1);
	//private1=2;

	cout<<"private1 = "<<private1<<"\n";
	cout<<"private2 = "<<private2<<"\n\n";

	cout<<"計算 public key : \n";
	xy_Multiplypoint(p,a4,private1,x_random,y_random);
	x_public1=x3;
	y_public1=y3;
	xy_Multiplypoint(p,a4,private2,x_random,y_random);
	x_public2=x3;
	y_public2=y3;
	cout<<" =>public1 ="<<private1<<"*("<<long(x_random)<<","<<long(y_random)<<")=("<<long(x_public1)<<","<<long(y_public1)<<")\n";
	cout<<" =>public2 ="<<private2<<"*("<<long(x_random)<<","<<long(y_random)<<")=("<<long(x_public2)<<","<<long(y_public2)<<")\n\n";

	cout<<"計算 private key : \n";
	xy_Multiplypoint(p,a4,private1,x_public2,y_public2);
	x_key1=x3;
	y_key1=y3;
	xy_Multiplypoint(p,a4,private2,x_public1,y_public1);
	x_key2=x3;
	y_key2=y3;
	cout<<" =>key1 ="<<private1<<"*("<<long(x_public2)<<","<<long(y_public2)<<")=("<<long(x_key1)<<","<<long(y_key1)<<")\n";
	cout<<" =>key2 ="<<private2<<"*("<<long(x_public1)<<","<<long(y_public1)<<")=("<<long(x_key2)<<","<<long(y_key2)<<")\n\n";

	//輸入key資訊
	file_output<<p<<" "<<long(a4)<<" "<<long(a6)<<" ";
	file_output<<long(x_random)<<" "<<long(y_random)<<" ";
	file_output<<private1<<" "<<private2<<" ";
	file_output<<long(x_public1)<<" "<<long(y_public1)<<" ";
	file_output<<long(x_public2)<<" "<<long(y_public2)<<" ";
	file_output<<x_key1<<" "<<y_key1<<" ";
	file_output<<x_key2<<" "<<y_key2<<" ";
	time_fthissh=time(NULL);//時間:產生key結束
	second=(t.millitm/1000.0);
	time_gen=(time_fthissh-time_start)+second;
	cout<<endl;
	cout<<"產生key的時間 = "<<time_gen<<" "<<"second"<<endl;
}

void ecc::encrypt_ECC()
{
	ftime(&t);
	long int time_start, time_fthissh;
	float time_gen,second;
	long int x_public1_gen,y_public1_gen;
	long int x_key1_gen;
	long int loop,i,loop_p;
	long int iter;
	long int k,sum_block;
	char fplaintext_in[500],fplaintext_out[500],buffer[500],fplaintext_inTemp[500],fplaintext_outTemp[500];
	long int count_input;

	srand(time(NULL));
	again1:
	cout<<"\n\n-------------------encrypt ECC----------------\n\n";
	cout<<"\n產生質數 p :\n";

	char *datakey="key.txt";
	long double *m_key; // 因為有些 m_key的運算會導致超過9 digit
	ifstream file_dtkey;

	file_dtkey.open(datakey);
	m_key=new long double[50];
	loop=0;
	while(!file_dtkey.eof())
	{
		file_dtkey>>m_key[loop+1];
		loop++;
	}
	iter=loop-1; //because read by number
	file_dtkey.close();
	//檢查key.txt是否存在
	if(iter==0){
		cout<<"\n<WARNING> : 'File "<<datakey<<" is NULL, please enter keylength to key exchange!'\n\n";
		exchange_key();
		system("cls");
		goto again1;
	}
	for(i=1;i<=iter;i++)
	{
		cout<<long(m_key[i])<<" ";
	}
	cout<<"\n\n";

	cout<<"user1相關加密資訊:"<<"\n";
	cout<<" =>prime = "<<long(m_key[1])<<"\n";
	cout<<" =>a4 = "<<long(m_key[2])<<"\n";
	cout<<" =>a6 = "<<long(m_key[3])<<"\n";
	cout<<" =>random point(P) = ("<<long(m_key[4])<<","<<long(m_key[5])<<")\n";
	cout<<" =>public2 = ("<<long(m_key[10])<<","<<long(m_key[11])<<")\n";
	cout<<" =>private1 = "<<long(m_key[6])<<"\n\n";

	count_input=0;
	again: //如果沒找到file
	strcpy(fplaintext_out,"chipertext_"); //重設fplaintext_out

	cout<<"請輸入欲加密的檔案名稱或路徑 = ";
	buffer[0]=char(500);
	strcpy(fplaintext_in,gets(buffer));

	//輸入明文
	FILE *input;
	long int m_plaintext;
	input = fopen(fplaintext_in, "r");

	//檢查明文檔案是否存在
	if (input== NULL){
		cout<<"\n<WARNING> : 'File "<< fplaintext_in<<" not found, enter name file plaintext with correct!'\n\n";

		count_input++;
		if(count_input%4==0){
			system("cls");
		}
		goto again;
	}

	strcpy(fplaintext_inTemp,strrev(fplaintext_in));
	int pos_slash=0;
	for(i=1;i<=int(strlen(fplaintext_inTemp));i++){
		pos_slash++;
		if(fplaintext_inTemp[i]==92){ //char slash '\'
			break;
		}
	}
	pos_slash= int(strlen(fplaintext_inTemp)) - pos_slash; // from example key.txt become chipertext_key.txt

	//pengkodisian if pos_slash = 0 or != 0
	if(pos_slash==0){ // if on current folder
		//kond. 1"<<"\n";
		strcpy(fplaintext_out,strcat(fplaintext_out,strrev(fplaintext_in))); //fplaintext_out = embed fplaintext_in with chipertext_.txt
	}
	else{ // if with path dynamic
		//kond. 2"<<"\n";
		strcpy(fplaintext_in,strrev(fplaintext_in));

		// split name path become 2 part :
		//part1
		int loop_take=0;
		for(i=0;i<pos_slash;i++){
			fplaintext_outTemp[loop_take]=fplaintext_in[i];
			loop_take++;
		}
		fplaintext_outTemp[loop_take]= '\0';  // to destroy noise
		strcpy(fplaintext_out,strcat(fplaintext_outTemp,fplaintext_out));

		//part2
		loop_take=0;
		for(i=pos_slash;i< int(strlen(fplaintext_inTemp));i++){
			fplaintext_outTemp[loop_take]=fplaintext_in[i];
			loop_take++;
		}
		fplaintext_outTemp[loop_take]= '\0';  // to destroy noise
		strcpy(fplaintext_out,strcat(fplaintext_out,fplaintext_outTemp));
	}
	cout<<"\n加密後的檔案 = ";
	printf(fplaintext_out);
	cout<<"\n";

	time_start=time(NULL);//時間:加密開始
	ofstream file_output; // 加密結果寫入file fplaintext_out
	file_output.open(fplaintext_out);

	cout<<"\nprocess encrypt/ user1 Count( plaintext[] ^ x_key1) :"<<"\n";
	cout<<"Please wait.";
	loop_p=0;
	sum_block=1000;
	while((m_plaintext=fgetc(input))!= EOF){
		if(loop_p%sum_block==0)
		{
			do{
				k=mod(rand(), m_key[1]); //k is private1_gen have characteristic dynamic
			}while(k<2);

			//計算k*P (P是曲線上被選擇的點)
			xy_Multiplypoint(m_key[1],m_key[2],k,m_key[4],m_key[5]); //x_Multiplypoint(prime,a4,private1_gen,xrandom,yrandom)
			x_public1_gen=x3;
			y_public1_gen=y3; // m_key[4] is x_random, m_key[5] is y_random
			file_output<<x_public1_gen<<" "<<y_public1_gen<<" "; //將傳送給user 2

			//計算k*public key user2
			xy_Multiplypoint(m_key[1],m_key[2],k,m_key[10],m_key[11]); //x_Multiplypoint(prime,a4,private1_gen,xpublic2,ypublic2)
			x_key1_gen=x3;
		}
		m_plaintext=(m_plaintext^x_key1_gen); //XOR
		file_output<<std::hex<<m_plaintext<<" ";
		loop_p++;
		//animation event process encrypt
		if(loop_p%100000==0)
		{
			cout<<".";
		}
	}
	cout<<"!";
	iter=loop_p; //because read by character
	cout << file_output << endl;
	fclose(input);



	cout<<"\n\nsum character plaintext = "<<iter;
	cout<<"\n\n";
	cout<<"加密完成!\n";
	time_fthissh=time(NULL);//時間:加密結束
	second=(t.millitm/1000.0);
	time_gen=(time_fthissh-time_start)+second;
	cout<<endl;
	cout<<"加密花費的時間 = "<<time_gen<<" "<<"second"<<endl;

	ifstream inFile;
	inFile.open(fplaintext_out);
	string str;

	while(getline(inFile,str)) {
		cout << str << endl;
	}
}


void ecc::decrypt_ECC()
{
	ftime(&t);
	long int time_start, time_fthissh;
	float time_gen,second;
	long int x_key2_gen;
	long int i,loop_s,loop_p;
	long int loop,iter;
	char plaintext2;
	char fchipertext_in[500],fchipertext_out[500],buffer[500],fchipertext_inTemp[500],fchipertext_outTemp[500];
	long int sum_block,check_stop,stop,x_public1_gen,y_public1_gen,count_input;

	srand(time(NULL));
	again1:
	cout<<"\n\n-------------------Schema decrypt ElGamal----------------\n\n";
	cout<<"擷取exchange key的結果:\n";
	char *datakey="key.txt";
	long double *m_key; // 因為有些 m_key的運算會導致超過9 digit
	//ofstream file_datakey;
	ifstream file_dtkey;
	file_dtkey.open(datakey);
	m_key=new long double[50];
	loop=0;
	while(!file_dtkey.eof())
	{
		file_dtkey>>m_key[loop+1];
		loop++;
	}
	iter=loop-1; //because read by number
	file_dtkey.close();
	if(iter==0){
		cout<<"\n<WARNING> : 'File "<<datakey<<" is NULL, please enter keylength to key exchange!'\n\n";
		exchange_key();
		system("cls");
		encrypt_ECC();
		system("cls");
		goto again1;
	}
	for(i=1;i<=iter;i++)
	{
		cout<<long(m_key[i])<<" ";
	}
	cout<<"\n\n";

	cout<<"user2解密相關資訊:"<<"\n";
	cout<<" =>prime = "<<long(m_key[1])<<"\n";
	cout<<" =>a4 = "<<long(m_key[2])<<"\n";
	cout<<" =>a6 = "<<long(m_key[3])<<"\n";
	cout<<" =>random point(P) = ("<<long(m_key[4])<<","<<long(m_key[5])<<")\n";
	cout<<" =>public2 = ("<<long(m_key[10])<<","<<long(m_key[11])<<")\n";
	cout<<" =>private2 = "<<long(m_key[7])<<"\n\n";

	count_input=0;
	again: //如果沒找到file
	strcpy(fchipertext_out,"plaintext_"); //重設fchipertext_out

	cout<<"請輸入欲解密的檔案名稱或路徑 = ";
	buffer[0]=char(500); // this is space to patch character input start from null
	strcpy(fchipertext_in,gets(buffer));

	//輸入密文
	FILE *input;
	long int m_chipertext;
	long int m_chipertext_16;
	input = fopen(fchipertext_in, "r");

	//檢查file chipertext是否存在
	if (input== NULL){
		cout<<"\n<WARNING> : 'File "<< fchipertext_in<<" not found, enter name file chipertext with correct!'\n\n";
		count_input++;
		if(count_input%4==0){
			system("cls");
		}
		goto again;
	}

	strcpy(fchipertext_inTemp,strrev(fchipertext_in));
	int pos_slash=0;
	for(i=1;i<=int(strlen(fchipertext_inTemp));i++){
		pos_slash++;
		if(fchipertext_inTemp[i]==92){ //char slash '\'
			//cout<<"\npos_slash = "<<pos_slash<<"\n";
			break;
		}
	}
	pos_slash= int(strlen(fchipertext_inTemp)) - pos_slash; // from example key.txt become key_chipertext.txt

	//pengkodisian if pos_slash = 0 or != 0
	if(pos_slash==0){ // if on current folder
		//kond. 1"<<"\n";
		strcpy(fchipertext_out,strcat(fchipertext_out,strrev(fchipertext_in))); //fplaintext_out = embed fplaintext_in with _chipertext.txt
	}
	else{ // if with path dynamic
		//kond. 2"<<"\n";
		strcpy(fchipertext_in,strrev(fchipertext_in));

		// split name path become 2 part :
		//part1
		int loop_take=0;
		for(i=0;i<pos_slash;i++){
			fchipertext_outTemp[loop_take]=fchipertext_in[i];
			loop_take++;
		}
		fchipertext_outTemp[loop_take]= '\0';  // to destroy noise
		strcpy(fchipertext_out,strcat(fchipertext_outTemp,fchipertext_out));

		//part2
		loop_take=0;
		for(i=pos_slash;i< int(strlen(fchipertext_inTemp));i++){
			fchipertext_outTemp[loop_take]=fchipertext_in[i];
			loop_take++;
		}
		fchipertext_outTemp[loop_take]= '\0';  // to destroy noise
		strcpy(fchipertext_out,strcat(fchipertext_out,fchipertext_outTemp));
	}
	cout<<"\n解密後的檔案 = ";
	printf(fchipertext_out); // animation green colour at name file result decrypt
	cout<<"\n";

	time_start=time(NULL);//時間:解密開始
	ofstream file_output; // 寫入解密的結果至file fchipertext_out
	file_output.open(fchipertext_out);


	ifstream file_count;  // 讀取 file fchipertext_in
	file_count.open(fchipertext_in);
	loop=0;
	while(!file_count.eof())
	{
		file_count>>m_chipertext;
		loop++;
	}
	iter=loop-1;

	cout<<"\nsum all character chipertext = "<<iter;
	cout<<"\n\n";
	cout<<"decrypt/ user2 Count( x_key2 ^ chipertext[]) :"<<"\n";
	sum_block=1000;
	check_stop=iter/(sum_block+2);
	if(iter%(sum_block+2)!=0)
	{
		stop=iter-2*(check_stop+1);
	}
	else
	{
		stop=iter-2*check_stop;
	}
	cout<<"sum character plaintext = "<<stop;
	cout<<"\n\n";
	cout<<"Please wait.";
	ifstream file_dt;
	file_dt.open(fchipertext_in);
	loop_p=0;
	loop_s=1;
	do{
		file_dt>>m_chipertext;
		if(loop_p%(sum_block+2)==0 || (loop_p-1)%(sum_block+2)==0)
		{
			if(loop_p%(sum_block+2)==0)
			{
				x_public1_gen=m_chipertext;
			}
			else if((loop_p-1)%(sum_block+2)==0)
			{
				y_public1_gen=m_chipertext;

				//計算private2*public1_gen
				//x_Multiplypoint(prime,a4,private2,x_public1_gen,y_public1_gen)
				xy_Multiplypoint(m_key[1],m_key[2],m_key[7],x_public1_gen,y_public1_gen);
				x_key2_gen=x3;
			}
		}
		else if(loop_p%(sum_block+2)!=0 && (loop_p-1)%(sum_block+2)!=0)
		{
			plaintext2=char(x_key2_gen^m_chipertext); //XOR
			file_output<<plaintext2;
			loop_s++;
		}

		loop_p++;
		//animation event process encrypt
		if(loop_p%100000==0)
		{
			cout<<".";
		}
	}while(loop_s<=stop);
	cout<<"!";
	cout<<"\n\n解密完成!\n";
	time_fthissh=time(NULL);//時間:解密結束
	second=(t.millitm/1000.0);
	time_gen=(time_fthissh-time_start)+second;
	cout<<endl;
	cout<<"解密花費的時間 = "<<time_gen<<" "<<"second"<<endl;
}

int main ()
{
	//declaration name object from class
	ecc run;
	char str_choice[1000],buffer[1000]; //to animation colour
	buffer[0]=char(1000); // this is space to patch character input start from null
	int _choice;
	cout.setf(ios::fixed,ios::floatfield);   // float field set to fixed
	do {
		system("cls");

		printf("===========================================\n");
		printf("Elliptic Curve Cryptography");
		printf("\n===========================================\n");
		cout<<"1. Create exchange key\n";
		cout<<"2. Encrypt ECC\n";
		cout<<"3. Decrypt ECC\n";
		cout<<"4. Exit \n";
		cout<<"Enter your choice :";
		strcpy(str_choice,gets(buffer));
		_choice=atoi(str_choice);
		switch(_choice)
		{
		case 1:
		{
			run.exchange_key();
			cout<<endl;
			cout<<"\npress any key to be continue...... \n";
			getch ();
			break;
		}
		case 2:
		{
			run.encrypt_ECC();
			cout<<endl;
			cout<<"\npress any key to be continue...... \n";
			getch ();
			break;
		}
		case 3:
		{
			run.decrypt_ECC();
			cout<<endl;
			cout<<"\npress any key to be continue...... \n";
			getch ();
			break;
		}
		}
	}while (_choice!=4);
}
