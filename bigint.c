#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void multiply_single_digit(char *res, char* num1, char digit) {

	int len = strlen(num1);

	int x = digit - '0';
    
	int carry = 0;

	for (int i = 0; i < len; i++) {

		int digitmul = (num1[len - i - 1] - '0') * x + carry;

		if (digitmul >= 10) {
			*(res--) = '0' + (digitmul - 10 * (digitmul / 10));
			carry = digitmul / 10;
		} else {
			*(res--) = '0' + digitmul;
			carry = 0;
		}

	}

	*res = '0' + carry;

	return;
}


void stradd(char *buffer, char **nums, int row, int col) {

	int carry = 0;

	for (int i = 0; i < col; i++) {

		int sum = 0;

		for (int j = 0; j < row; j++)
			sum += nums[j][col - i - 1] - '0';

		sum += carry;

		if (sum >= 10) {

			*(buffer--) = '0' + sum - 10 * (sum / 10);
			carry = sum / 10;

		} else {
			*(buffer--) = '0' + sum;
			carry = 0;
		}

	}

	*(buffer--) = '0' + carry;
}


char *multiply(char *num1, char *num2) {

	int len_num1 = strlen(num1), len_num2 = strlen(num2);

	int bufferlen = len_num1 + len_num2;

	char **multiplands = (char **)calloc(len_num2, sizeof(char *));

	for (int j = 0; j < len_num2; j++) {
		multiplands[j] = (char *)calloc(bufferlen, sizeof(char));

		memset(multiplands[j], '0', bufferlen);
		multiply_single_digit(multiplands[j] + bufferlen - j - 1, num1, num2[len_num2 - j - 1]);

	}

	char *res = (char *) calloc(bufferlen + 1, sizeof(char));

	memset(res, '0', bufferlen + 1);

	stradd(&res[bufferlen], multiplands, len_num2, bufferlen);

	for (int i = 0; i < len_num2; i++)
		free(multiplands[i]);

	free(multiplands);


	int finallen = bufferlen + 1;
	
	while (*res == '0'  && finallen != 1) {
		res++;
		finallen--;
	}

	char *finalres = (char *)calloc(finallen + 1, sizeof(char));

	memcpy(finalres, res, finallen);

	finalres[finallen] = '\0';

	free(res - (bufferlen + 1 - finallen));

	return finalres;

}

// int main(void) {
//
// 	char num1[] = "12333333333333333";
// 	char num2[] = "0";
//
// 	char *res = multiply(num1, num2);
// 	int len = strlen(num1) + strlen(num2) + 1;
//
//
// 	printf("res=%s\n", res);
// 	// printf("%.*s\n", len, res);
//
// 	free(res);
//
// 	return 0;
// }
