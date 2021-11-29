# include<stdio.h>
int int_to_binary(int *arr){
int bin,i;
int multiplier=1;
for(i=6;i>=0;--i){
bin+=(multiplier*arr[i]);
multiplier*=10;
}

return bin;

}


void main(){

int ar[7]={0,0,1,0,0,1,0};
int res=int_to_binary(ar);
printf("%07d",res);


}
