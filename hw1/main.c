#include <stdio.h>

int main(int argc, char **argv)
{
	char str[32];
	double time;
	int flag;
	int ret;
	
	FILE *fp;
	fp = fopen("/opt/school/stat778/hw1/hw1.dat", 'r');
	
	
	
	do {
		ret = fscanf(fp, "%lf %i", &time, &flag);
		printf("Return: %i; Time: %lf; Flag: &i", ret, time, flag );
	} while (!feof(fp))
	
	
	
	
	
	return fclose(fp);
}
