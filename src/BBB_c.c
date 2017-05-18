#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SYSFS_GPIOPATH "/sys/class/gpio"
#define GPIO_P9_12_DIR "gpio60"
#define GPIO_ATTRIB_DIRECTION "direction"
#define GPIO_ATTRIB_VALUE "value"

char DIRECTION_OUT[]="out";
char DIRECTION_IN[]="in";

char * readGPIO(char * gpioPath, char * gpioName, char * gpioProp);
int writeGPIO(char * buffer, char * gpioPath, char * gpioName, char * gpioProp);
int toggleGPIO(char * internalGPIONum, char * gpioPath, int state);

char bufferData[10]={'0'};
char * ptrBufferData = bufferData;

int main(void)
{
	//char * bufferData=(char *)malloc(10*sizeof(char));


	char optionChar;//for main and sub-menu
	int choice;
	char * value;//for storing readGPIO return values

	printf("\n##  GPIO manipulation through Sysfs in C [ROOT ACCESS NEEDED] ##\n\n");
	printf("\nWARNING : Refer to your device GPIO headers before using this!! ##\n\n");

	while(optionChar!='x')
	{

			printf("\n\n-- MENU ---------------------------\n");
			printf("1. Export GPIO               : %s \n",GPIO_P9_12_DIR);
			printf("2. UnExport GPIO             : %s \n",GPIO_P9_12_DIR);
			printf("3. Read   DIRECTION  of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("4. Read   VALUE      of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("5. Write  DIRECTION  of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("6. Write  VALUE      of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("x. EXIT                          \n");

			optionChar = getchar();

			switch(optionChar)
			{
			case '1':
				printf("Exporting GPIO %s ",GPIO_P9_12_DIR);

				ptrBufferData="60";
				if(toggleGPIO(ptrBufferData,SYSFS_GPIOPATH,1)==0)
					printf(" %s exported \n", GPIO_P9_12_DIR );

				break;
			case '2':
				printf("UnExporting GPIO %s",GPIO_P9_12_DIR);

				ptrBufferData="60";
				if(toggleGPIO(ptrBufferData,SYSFS_GPIOPATH,0)==0)
					printf(" %s unexported \n", GPIO_P9_12_DIR);

				break;
			case '3':
				printf("Reading GPIO DIRECTION...... \n");

				if( (value = readGPIO(SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_DIRECTION))!=NULL)
				{
					printf("%s [DIRECTION] = %s \n",GPIO_P9_12_DIR, value);
					free(value);
				}
				break;
			case '4':
				printf("Reading GPIO VALUE........... \n");

				if((value=readGPIO(SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_VALUE))!=NULL)
				{
					printf("%s [VALUE] = %s \n",GPIO_P9_12_DIR, value);
					free(value);
				}
				break;
			case '5':
				printf("Writing GPIO DIRECTION........\n");

				choice=0;
				printf("Choose a direction -> \n 1.IN \n 2.OUT \n");
				scanf("%d",&choice);

				if(choice==1)
				{
					//snprintf(bufferData,2,"%s",DIRECTION_IN);
					ptrBufferData=DIRECTION_IN;
					if(writeGPIO(ptrBufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_DIRECTION)!=-1)
					{
						printf(" DIRECTION changed for %s\n to IN",GPIO_P9_12_DIR);
					}
				}
				else
				if(choice==2)
				{
					//snprintf(bufferData,3,DIRECTION_OUT);
					ptrBufferData=DIRECTION_OUT;
					if(writeGPIO(ptrBufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_DIRECTION)!=-1)
					{
						printf(" DIRECTION changed for %s\n to OUT",GPIO_P9_12_DIR);
					}
				}
				else
				{
					printf("Invalid choice given !\n");
				}
				break;
			case '6':
				printf("Writing GPIO VALUE..........\n");

				choice=0;

				printf("Choose the VALUE -> \n 1.Enable \n 2.Disable \n");
				scanf("%d",&choice);

				if(choice==1)
				{
					ptrBufferData="1";
					if(writeGPIO(ptrBufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_VALUE)!=-1)
					{
						printf(" VALUE changed for %s\n to 1", GPIO_P9_12_DIR);
					}
				}
				else
				if(choice==2)
				{
					ptrBufferData="0";
					if(writeGPIO(ptrBufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_VALUE)!=-1)
					{
						printf(" VALUE changed for %s\n to 0", GPIO_P9_12_DIR);
					}
				}
				else
				{
					printf("Invalid choice given !\n");
				}

				break;
			case 'x':
				//free(bufferData);
				printf("Bye!....\n");
				//exit(0);
				break;
			}//switch

	}//while

	return 0;
}

/**
 * Used to export and unexport GPIO pins
 * For export , set state =1
 * For unexport, set state =0
 */
int toggleGPIO(char * internalGPIONum, char * gpioPath, int state)
{
	FILE *fp;
	char fullPath[80];


	if(state==1)
		sprintf(fullPath,"%s/export",gpioPath);
	else
		sprintf(fullPath,"%s/unexport",gpioPath);

	printf("Reading the export path %s \n",fullPath);
	if((fp=fopen(fullPath,"w"))==NULL)
	{
		perror("Unable to export GPIO!");
		return -1;
	}
	else
	{
		printf("File accessed to write : %s \n",fullPath);
		fputs(internalGPIONum,fp);
	}

	int returnValue=fclose(fp);
	if(returnValue!=0)
		perror("Error while closing file stream !");

	return returnValue;

}

int writeGPIO(char * buffer, char * gpioPath, char * gpioName, char * gpioProp)
{
	//Form the path..
	//Open the file stream in write mode
	//write the chars
	//check whether write is successful
	//close the file stream
	//return appr. exit code

	char fullPath[80];
	FILE *fp;
	sprintf(fullPath,"%s/%s/%s",gpioPath,gpioName,gpioProp);

	if((fp=fopen(fullPath,"rb+"))==NULL)
	{
		perror("Unable to write to the GPIO file!");
		return -1;
	}
	else
	{
		printf("File accessed to write : %s \n",fullPath);
		fputs(buffer,fp);
	}

	int returnValue=fclose(fp);
	if(returnValue!=0)
		perror("Error while closing file stream !");

	return returnValue;


}


char * readGPIO(char * gpioPath, char * gpioName, char * gpioProp)
{

	char fullPath[80];
	char * value=(char *)malloc(10*sizeof(char *));

	FILE *fp;

	sprintf(fullPath,"%s/%s/%s",gpioPath,gpioName,gpioProp);
	if((fp=fopen(fullPath,"r+"))==NULL)
	{
		perror("Unable to read the GPIO file!");
		free(value);
		return NULL;
	}
	else
	{
		printf("File accessed to read : %s \n", fullPath);
	}


	 if( fgets (value, 10, fp)!=NULL )
	 {
		 puts(value);
	 }


	if(fclose(fp)!=0)
	{
		perror("Error while closing file stream !");
		free(value);
		return NULL;
	}

	return value;

}


