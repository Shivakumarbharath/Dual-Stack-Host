#include <stdio.h>
# include <time.h>
int main()
{
    printf("Hello World\nEnter time in <DD-MM-YYYY : >");

struct tm time_;
char string[11];
fgets(string,11,stdin);
sscanf(string,"%2d-%2d-%4d",&time_.tm_mday,&time_.tm_mon,&time_.tm_year);
printf("%d-%d-%d\n",time_.tm_mday,time_.tm_mon,time_.tm_year);
    return 0;
}
