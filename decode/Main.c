#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

unsigned int originalData;
unsigned int encodedData;
unsigned int combinations[128];
int minDiffIndex;
int minDiff;
struct 
{
	unsigned input : 1;
	unsigned reg1 : 1;
	unsigned reg2 : 1;
	unsigned out1 : 1;
	unsigned out2 : 1;
	unsigned out3 : 1;
} status;
void encode(void);
int binary_decimal(char *str);
char *decimal_binary(int n, int size);
int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		char bincode[21];
		printf("Enter input code:   ");
		scanf("%s", &bincode[0]);
		encodedData = binary_decimal(bincode);
	}
	else
	{
		printf("Decoding: %s \n", argv[1]);
		encodedData = binary_decimal(argv[1]);
	}
	/*encodedData = 0x179C7F;*/
	for (int i = 0; i < 128; i++)
	{
		originalData = i;
		status.reg2 = 0;
		status.reg1 = 0;
		for (int j = 0; j < 7; j++)
		{
			status.input = (originalData)& 1;
			originalData >>=  1;
			encode();
			combinations[i] = (combinations[i] << 1) + status.out1;
			combinations[i] = (combinations[i] << 1) + status.out2;
			combinations[i] = (combinations[i] << 1) + status.out3;
		}
	}

	clock_t t;
	t = clock();
	for (int i = 0; i < 10000; i++)
	{
	minDiff = 21;
	for (int i = 0; i < 128; i++)
	{
		int diff = 0;
		int diffcode = encodedData ^ combinations[i];
		for (int j = 0; j < 21; j++)
		{
			diff += diffcode & 1;
			diffcode >>= 1;
		}
		if (diff <= minDiff)
		{
			minDiff = diff;
			minDiffIndex = i;
		}
	}
	}
	t = clock() - t;
	printf("Bruteforce result: %d clicks (%f seconds).\n",
	(int)t, ((double)t) / CLOCKS_PER_SEC);
	printf("Diff: %d, Code: %s, Decoded: %s \n", minDiff, decimal_binary(combinations[minDiffIndex], 21), decimal_binary(minDiffIndex,7));
	printf("Press any key to exit...\n");  
	getchar(); 
}

void encode(void)
{
	status.out1 = status.reg1 ^ status.input;
	status.out2 = status.reg2 ^ status.input;
	status.out3 = status.reg1 ^ status.input;
	status.reg2 = status.reg1;
	status.reg1 = status.input;
}
int binary_decimal(char *str)
{
	int decimal = 0;
	int i;
	while (str[i] != '\0')
	{
		if(str[i] != '1' && str[i] != '0')
		{
			printf("ERROR: Input must be in binary format.\n");
			exit(0);
		}
		decimal += (str[i] - '0')*(int)pow(2, 20 - i);
		i++;
	}
	if(i != 21)
	{
		printf("ERROR: Input must be 21 digits in length.\n");
		exit(0);
	}
	return decimal;
}

char *decimal_binary(int n, int size)
{
	int c, d, count;
	char *pointer;

	count = 0;
	pointer = (char*)malloc(size+1);
	for (c = size-1; c >= 0; c--)
	{
		d = n >> c;

		if (d & 1)
			*(pointer + count) = 1 + '0';
		else
			*(pointer + count) = 0 + '0';

		count++;
	}
	*(pointer + count) = '\0';
	return  pointer;
}