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
void split(unsigned int code, int bit, int prevDiff);
int main(void)
{
	char bincode[20];
	printf("Binary code:   ");
	scanf("%s", &bincode[0]);
	encodedData = binary_decimal(bincode);
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
	printf("It took me %d clicks (%f seconds).\n",
	(int)t, ((double)t) / CLOCKS_PER_SEC);

	printf("Diff: %d, Code: %s, Decoded: %s \n", minDiff, decimal_binary(combinations[minDiffIndex], 21), decimal_binary(minDiffIndex,7));
	/*----------------------------------------------------*/
	t = clock();
	for (int i = 0; i < 10000; i++)
	{

	minDiff = 21;
	split(0, -1, 0);

	}
	t = clock() - t;
	printf("It took me %d clicks (%f seconds).\n",
		(int)t, ((double)t) / CLOCKS_PER_SEC);

	printf("Diff: %d, Code: %s, Decoded: %s \n", minDiff, decimal_binary(combinations[minDiffIndex], 21), decimal_binary(minDiffIndex, 7));
	scanf("%d", encodedData);
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
	for (int i = 0; i < 21; i++)
	{
		decimal += (str[i] - '0')*(int)pow(2, 20 - i);
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
void split(unsigned int code, int bit, int prevDiff)
{
	int tmp = encodedData ^ combinations[code];
	tmp >>= (18 - bit * 3);
	for (int i = 0; i < 3; i++)
	{
		prevDiff += tmp & 1;
		tmp >>= 1;
	}
	if (prevDiff > 6 || prevDiff > minDiff) /*отсекает лишние ветки*/
		return;
	if (bit == 6)
	{
		if (minDiff > prevDiff)
		{
			minDiff = prevDiff;
			minDiffIndex = code;
		}
		return;
	}

	++bit;
	code ^= 1 << bit;
	split(code, bit, prevDiff);
	code ^= 1 << bit;
	split(code, bit, prevDiff);

}

