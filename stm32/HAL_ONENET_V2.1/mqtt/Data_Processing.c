#include "Data_Processing.h"
#include <stdio.h>
#include <string.h>




int strToHex(char* ch, char* hex)
{
	int high, low;
	int tmp = 0;
	if (ch == NULL || hex == NULL) {
		return -1;
	}

	if (strlen(ch) == 0) {
		return -2;
	}

	while (*ch) {
		tmp = (int)* ch;
		high = tmp >> 4;
		low = tmp & 15;
		*hex++ = valueToHexCh(high); //先写高字节
		*hex++ = valueToHexCh(low); //其次写低字节
		ch++;
	}
	*hex = '\0';
	return 0;
}

char valueToHexCh(const int value)
{
	char result = '\0';
	if (value >= 0 && value <= 9) {
		result = (char)(value + 48); //48为ascii编码的‘0’字符编码值
	}
	else if (value >= 10 && value <= 15) {
		result = (char)(value - 10 + 65); //减去10则找出其在16进制的偏移量，65为ascii的'A'的字符编码值
	}
	else {
		;
	}

	return result;
}























