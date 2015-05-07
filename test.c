int codes[128];
int minDiff;
int diffIndex;
void split(int bit);
int decode(int code);

struct{
	unsigned isActive : 1;
	int diff;
	int code;
}nodes[2][2][2];

int void main
{
split(0, -1, 0);
}

int decode(int code)
{
	nodes[0][0][0].isActive = 1;
	for(char i = 0; i < 7; i++)
	{
		for(char r1 = 0; r1 < 2; r1++)
		{
			for(char r2 = 0; r1 < 2; r2++)
			{
				if(nodes[r1][r2][i & 1].isActive)
				{
					nodes[r1][r2][i & 1].isActive = 0;
					for(char bit = 0; bit < 2; bit++)
					{
						nodes[r1][r2][i & 1].code ^= (r1 ^ bit) << 2;
						nodes[r1][r2][i & 1].code ^= (r2 ^ bit) << 1;
						nodes[r1][r2][i & 1].code ^= (r1 ^ bit);
						diffcode = nodes[r1][r2][i & 1].code ^ (encodedData >> (21-bit*3));
						char nextNode = (i &  1) ^ 1;
						for(int i = 0; i < 3; i++)
						{
							nodes[r1][r2][i & 1].diff += diffcode & 1;
							diffcode >>= 1;
						}
						if(nodes[bit][r1][nextNode].isActive = 0)
						{
							nodes[bit][r1][nextNode].code = (nodes[r1][r2][i & 1].code<<1)+ bit;
							nodes[bit][r1][nextNode].diff = nodes[r1][r2][i & 1].diff;
						}
						else if(nodes[bit][r1][nextNode].diff > nodes[r1][r2][i & 1].diff)
						{
							nodes[bit][r1][nextNode].code = (nodes[r1][r2][i & 1].code<<1)+ bit;
							nodes[bit][r1][nextNode].diff = nodes[r1][r2][i & 1].diff;
						}
					}
				}
			}
		}
	}
}
void split(int code, int bit, int prevDiff)
{
	code ^= 1<<bit;
	int tmp = encodedData ^ codes[code];
	tmp >>= (21-bit*3);
	for(int i = 0; i < 3; i++)
	{
		prevDiff += tmp & 1;
		tmp >>= 1;
	}
	if(prevDiff >= 6 || bit == 6)
		return;
	if(bit == 6 && minDiff > prevDiff)
		minDiff = prevDiff;
		diffIndex = code;
	++bit;
	split(code, bit, prevDiff);
	split(code, bit, prevDiff);

}