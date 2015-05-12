#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>

struct op
{
	char *(*f)(); 
    char *path_name; 
    char *document;
    int args;
};

char ans[1024];

char* add(char* x, char* y) 
{	
	memset(ans,0,1024);
	double xx = strtod(x, NULL);
	double yy = strtod(y, NULL);
	sprintf(ans, "%f\n", xx + yy);
	return ans;
}

char* sub(char* x, char* y) 
{	
	memset(ans,0,1024);
	double xx = strtod(x, NULL);
	double yy = strtod(y, NULL);
	sprintf(ans, "%f\n", xx - yy);
	return ans;
}	

char* mul(char* x, char* y) 
{
	memset(ans,0,1024);	
	double xx = strtod(x, NULL);
	double yy = strtod(y, NULL);
	sprintf(ans, "%f\n", xx * yy);
	return ans;
}

char* divv(char* x, char* y) 
{	
	memset(ans,0,1024);	
	double xx = strtod(x, NULL);
	double yy = strtod(y, NULL);
	if(yy == 0)
	{
		return "divide by zero \n";
	}
	sprintf(ans, "%f\n", xx / yy);
	return ans;
}

char* expp(char* x, char* y) 
{	
	memset(ans,0,1024);	
	double xx = strtod(x, NULL);
	double yy = strtod(y, NULL);
	sprintf(ans, "%f\n", pow(xx,yy));
	
	return ans;
}

char* fib(char* x, char* arg) 
{
  char buf[512];
  memset(ans,0,1024);
  strcpy(ans, "0\n1\n1\n"); // first two elements are 1's
  
  long long prev = 1;
  long long prevprev = 1;
  long long current;
  double limit = strtod(arg, NULL);
  int i = 2;

  while( i < limit) 
  {
    current = prev + prevprev;
    sprintf(buf, "%lld\n", current);
    strcat(ans, buf);
    prevprev = prev;
    prev = current;
    i ++;
  }

  return ans;

}

int prime(int x)
{	
	int i;
	int factors = 0;
	int stop = ceil(x/2);
	if(x == 1 || 0)
		return 0;
	if(x == 2)
		return 1;
	for(i = 1; i <= stop; i++)
	{
		if(x % i == 0)   // found a factor
		{ 
			factors++;
		}
		if(factors >= 2) // not prime homie
		{
			return 0;
		}
	}
	return 1;
}

char* factor(char* x, char* input)
{
	
	memset(ans,0,1024);
	char buf[1024];
	strcpy(ans, "");
	strcpy(buf, "");
	double d = strtod(input, NULL);

	if(fabs(d - floor(d)) > 0.001)
	{
		return "Integer ONLY error\n";
	}

	int num = (int) d;
	
	if(prime(num) && d > 0)   // if it's prime just return input and 1
  	{	
    	sprintf(buf, "%d\n", num);
    	strcat(ans, buf);
    	return ans;
  	}

	if(num == 0 || num == 1)
	{
		sprintf(ans,"%d\n",d);
		return ans;
	}
	
	if(num < 0)
	{
		num = num * -1;
	}
	
	int i = 2;
	int prime_limit = num;

	for(i = 2; i <= prime_limit; i++)
	{
		if(prime(i))
		{
			if( (num % i) == 0)
			{
				sprintf(buf, "%d\n", i);
				strcat(ans, buf);
				prime_limit = prime_limit/i;
			}
		}
	}
	
	return ans;
}


