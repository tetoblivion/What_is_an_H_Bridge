// Author: Pentagrid
// february 2017
// Bipolar stepper control trough LPT
// particularly, I used L298 integrated circuit

#include<conio.h>
#include<stdio.h>
#include<windows.h>

namespace step{
	//
	//!!! put your sequence here , one that matches your setup !!!
	char step[4]={ 16 | 1 , 32 | 4 , 16 | 2 , 32 | 8 };
	//
	struct timer{
		LARGE_INTEGER cntps,t,to;
	}t;
    HANDLE file=INVALID_HANDLE_VALUE;
	const int nn=30;
	wchar_t ports[nn][100];
	int n=0;
	int curlpt=0;
	//
	double dly=10000;
	//
	DWORD wttn=0;
	const char w=0;
	//
}



//pins 10,11,12, shorted to GND  (imitating ACK so that writefile returns)


void enumlpts(){
	using namespace step;
	int i,j;
	//
	HANDLE file2=INVALID_HANDLE_VALUE;
	wchar_t str[15];
	//
	for(i=0;i<20;i++)ports[i][0]=0;
	//
	for(i=0,n=0;i<nn;i++){
	wsprintf(str,L"\\\\.\\LPT%d",i);
	file2=CreateFile(str,GENERIC_WRITE | GENERIC_READ, 0, 0, OPEN_EXISTING, 0, NULL);
	if(file2!=INVALID_HANDLE_VALUE){
		j=0;while(str[j]!=0 && j<100){ports[n][j]=str[j];j++;}ports[n][j]=0;
		n++;
		CloseHandle(file2);
	}
	}
}


void closeport(){
	using namespace step;
	if(file!=INVALID_HANDLE_VALUE)CloseHandle(file);file=INVALID_HANDLE_VALUE;
}

void openport(int i){
	using namespace step;
	if(i>=step::n)return;
	closeport();
	file=CreateFile(ports[i],GENERIC_WRITE | GENERIC_READ, 0, 0, OPEN_EXISTING, 0, NULL);
}



int main(int argc,char* argv[]){
	using namespace step;
	int i=0,p=1;
	QueryPerformanceFrequency(&step::t.cntps);
	QueryPerformanceCounter(&step::t.t);

	dly=t.cntps.QuadPart/10;

	enumlpts();
	openport(curlpt);

	WriteFile(file,&w,1,&wttn,NULL);
	system("cls");
	printf("\n pgup/pgdn  -selectport (%ls current  n=%d) ",ports[curlpt],n);
	printf("\n space      -start/stop  ");
	printf("\n up/dn      -increse/decrese speed ");
	printf("\n left/right -direction  ");
	printf("\n q/w/e/r    -sinle step  ");
	printf("\n p          -release  ");
	printf("\n\n %.2lf steps/sec",(double)t.cntps.QuadPart/dly);
	printf("\n total steps=0");
	
	char ch=0;
	long s=0,cnt=0,cnt2=0;

	while(ch!=27){

while(!_kbhit()&&s){
  //delay(dl);
QueryPerformanceCounter(&t.t);
if(t.t.QuadPart-t.to.QuadPart>=(long long)dly){
	t.to.QuadPart=t.t.QuadPart;
  i+=p;
  if(i<0)i=3;
  if(i>3)i=0;
  WriteFile(file,&step::step[i],1,&wttn,NULL);
  //outp(0x378,step[i]);
  cnt++;
}

}


ch=_getch();
if(ch==' '){
	if(s!=0){
		s=0;
		//outp(0x378,0);
		WriteFile(file,&w,1,&wttn,NULL);
		cnt2=cnt;
		cnt=0;
	}else s=1;  //space is stop/run
}
if(ch=='4' || ch==75)p=1;                        //left
if(ch=='6' || ch==77)p=-1;                       //right
if((ch=='8' || ch==72)&&(dly/(double)1.05)>=1)dly/=(double)1.05;     //faster
if(ch=='2' || ch==80)dly*=(double)1.05;                    //slower


		if(ch==73 && curlpt<n-1){ //pgup
			curlpt++;
			openport(curlpt);
		}else if(ch==81 && curlpt>0){ //pgdn
			curlpt--;
			openport(curlpt);
		}   


char bb=0;
if(ch=='q'){bb=16|2;WriteFile(file,&bb,1,&wttn,NULL);};//outp(0x378,16|2);
if(ch=='w'){bb=32|8;WriteFile(file,&bb,1,&wttn,NULL);};//outp(0x378,32|8);
if(ch=='e'){bb=16|1;WriteFile(file,&bb,1,&wttn,NULL);};//outp(0x378,16|1);
if(ch=='r'){bb=32|4;WriteFile(file,&bb,1,&wttn,NULL);};//outp(0x378,32|4);
if(ch=='p')WriteFile(file,&w,1,&wttn,NULL);

	system("cls");
	printf("\n pgup/pgdn  -selectport (%ls current  n=%d) ",ports[curlpt],n);
	printf("\n space      -start/stop  ");
	printf("\n up/dn      -increse/decrese speed ");
	printf("\n left/right -direction  ");
	printf("\n q/w/e/r    -sinle step  ");
	printf("\n p          -release  ");
	printf("\n\n %.2lf steps/sec",(double)t.cntps.QuadPart/dly);
	printf("\n total steps=%ld",cnt2);//cnt=0;
	}

	

		
    WriteFile(file,&w,1,&wttn,NULL);
	closeport();

}
