#include <stdio.h>
#include <stdlib.h>
#define SYSFS_GPIOPATH "/sys/class/gpio/"
#define GPIO_P9_12 60

//gpio functions prototypes
void exportGPIO(int GPIONum);
void unexportGPIO(int GPIONum);
void setGPIODirection(char * direction);
void setValue(int value);
int readFile(char * buffer,char * filePath, char * filename);
void writeFile(char * filePath, char * filename);


int main(void)
{
	char * buffer=(char *)malloc(10*sizeof(char));

	//char gpioValue, gpioDirection;
	printf("File operations in C\n");
	readFile(buffer,SYSFS_GPIOPATH,"gpio60/direction");

	printf("Value read : %s\n", buffer);

	return 0;
}

void exportGPIO(int GPIONum)
{

}

void unexportGPIO(int intGPIONum)
{

}

void setGPIODirection(char* direction)
{

}

void setValue(int value)
{

}

int readFile(char * buffer, char * filePath, char * fileName)
{

	char fullPath[30];
	int count=0;
	char tempChar;

	FILE *fp;

	sprintf(fullPath,"%s/%s",filePath,fileName);
	if((fp=fopen(fullPath,"rb"))==NULL)
	{
		printf("\n Unable to read the file :  %s ",filePath);
		return 1;
	}
	else
	{
		printf("\n File is being read.....");
	}

	while((tempChar = fgetc(fp))!='\n')
	{
	  buffer[count++]=tempChar;
	}


	fclose(fp);

	return 0;
}






