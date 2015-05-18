#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

unsigned int encodedData;
unsigned int result;
int decode(int code); /*Функция декодирования*/
int encode(int); /*Функция кодирования*/
int binary_decimal(char *str); 
char *decimal_binary(int n, int size);
int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		char bincode[22];
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
	
	clock_t t;
	t = clock();
	for (int i = 0; i < 100000; i++)
	{
		result = decode(encodedData);
	}
	t = clock() - t;
	printf("100000x decode cycles time: %d clicks (%f seconds).\n",
	(int)t, ((double)t) / CLOCKS_PER_SEC);
	printf("Code: %s, Decoded: %s \n", decimal_binary(encode(result), 21), decimal_binary(result, 7));
	printf("Press any key to exit...\n");  
	//getch();
}

int encode(int code)
{
	struct
	{
		unsigned input : 1;
		unsigned reg1 : 1;
		unsigned reg2 : 1;
	} status;

	int encoded = 0;
	status.reg2 = 0;
	status.reg1 = 0;
	for (int j = 0; j < 7; j++)
	{

		status.input = ((code)& 64) >> 6;
		code <<= 1;
		encoded = (encoded << 1) + status.reg1 ^ status.input;
		encoded = (encoded << 1) + status.reg2 ^ status.input;
		encoded = (encoded << 1) + status.reg1 ^ status.input;
		status.reg2 = status.reg1;
		status.reg1 = status.input;
	}
	return encoded;

}

int decode(int code)
{
	/*Структура узла состояния*/
	struct{
		unsigned isActive : 1;
		int diff;
		int code;
	}nodes[8][2][2];
	/*Обнуляем узлы*/
	memset(nodes, 0, sizeof(nodes));
	nodes[0][0][0].isActive = 1;
	/*Всякие переменные, которые потом пригодятся*/
	int diffcode = 0;
	int possibleCode = 0;
	int nextNode = 0;
	int nextDiff = 0;
	/*Проход по 7 битам*/
	for (int i = 0; i < 7; i++)
	{
		/*Проход по состояниям регистров*/
		for (int r1 = 0; r1 < 2; r1++)
		{
			for (int r2 = 0; r2 < 2; r2++)
			{
				/*Если состояние было достигнуто на прошлом шаге*/
				if (nodes[i][r1][r2].isActive)
				{
					/*Рассматриваем вариант сигнала 1 или 0 на входе*/
					for (int bit = 0; bit < 2; bit++)
					{
						/*Набираем предположительно закодированную комбинацию*/
						possibleCode = 0;
						possibleCode ^= (r1 ^ bit) << 2;
						possibleCode ^= (r2 ^ bit) << 1;
						possibleCode ^= (r1 ^ bit);
						/*Расчитываем разницу */
						diffcode = possibleCode ^ (code >> (18 - i * 3));
						nextNode = i + 1;
						nextDiff = nodes[i][r1][r2].diff;
						for (int k = 0; k < 3; k++)
						{
							nextDiff += diffcode & 1;
							diffcode >>= 1;
						}
						/*Проверяем значение разницы в узле и записываем*/
						if (nodes[nextNode][bit][r1].isActive == 0)
						{
							nodes[nextNode][bit][r1].code = (nodes[i][r1][r2].code << 1) + bit;
							nodes[nextNode][bit][r1].diff = nextDiff;
							nodes[nextNode][bit][r1].isActive = 1;
						}
						else if (nodes[nextNode][bit][r1].diff > nextDiff)
						{
							nodes[nextNode][bit][r1].code = (nodes[i][r1][r2].code << 1) + bit;
							nodes[nextNode][bit][r1].diff = nextDiff;
						}
					}
				}
			}
		}
	}
	/*Находим наименьшую разницу и возвращаем код*/
	diffcode = 21;
	for (int r1 = 0; r1 < 2; r1++)
	{
		for (int r2 = 0; r2 < 2; r2++)
		{
			if (nodes[7][r1][r2].diff < diffcode)
			{
				diffcode = nodes[7][r1][r2].diff;
				possibleCode = nodes[7][r1][r2].code;
			}
		}
	}
	return possibleCode;
}

int binary_decimal(char *str)
{
	int decimal = 0;
	int i = 0;
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

