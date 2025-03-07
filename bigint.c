#include <immintrin.h>  // For AVX2 intrinsics
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define mod10(x) (x - 10 * (x / 10))


void convert_ascii_to_int_avx2(char *buffer, int length) {
    __m256i zero_offset = _mm256_set1_epi8('0');  // Create vector with '0' for subtraction

    int i;
    for (i = 0; i <= length - 32; i += 32) {
        __m256i data = _mm256_loadu_si256((__m256i *)(buffer + i)); 
        data = _mm256_sub_epi8(data, zero_offset);
        _mm256_storeu_si256((__m256i *)(buffer + i), data);
    }

    for (; i < length; i++) {
        buffer[i] -= '0';
    }
}

void convert_int_to_ascii_avx2(char *buffer, int length) {
    __m256i zero_offset = _mm256_set1_epi8('0');  // Create vector with '0' for subtraction
	//
    int i;
    for (i = 0; i <= length - 32; i += 32) {  // Process 32 bytes at a time
        __m256i data = _mm256_loadu_si256((__m256i *)(buffer + i));  // Load 32 bytes
        data = _mm256_add_epi8(data, zero_offset);  // Subtract '0' from each byte
        _mm256_storeu_si256((__m256i *)(buffer + i), data);  // Store back the result
    }

    // Process remaining bytes
    for (; i < length; i++) {
        buffer[i] += '0';
    }
}


void multiply(char num1[], char num2[], char result[]) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int lenR = len1 + len2;
    short *res = calloc(lenR, sizeof(short));

    if (len1 > len2) {
        multiply(num2, num1, result);
        return;
    }
        

    convert_ascii_to_int_avx2(num1, len1);
    convert_ascii_to_int_avx2(num2, len2);

    // Reverse the strings for easier multiplication
    for (int i = 0; i < len1 / 2; i++) {
        char temp = num1[i];
        num1[i] = num1[len1 - i - 1];
        num1[len1 - i - 1] = temp;
    }
    for (int i = 0; i < len2 / 2; i++) {
        char temp = num2[i];
        num2[i] = num2[len2 - i - 1];
        num2[len2 - i - 1] = temp;
    }


    // Perform multiplication
    for (int i = 0; i < len1; i++) {

        int j = 0;

        __m256i multiplier = _mm256_set1_epi16((short)num1[i]);  // Create vector with '0' for subtraction
        int xlen2 = len2;

        for (j = 0; j < xlen2 - 16; j += 16) {

            __m256i multiplicands = _mm256_cvtepi8_epi16(_mm_loadu_si128((__m128i*)(num2 + j)));
            __m256i tres = _mm256_loadu_si256((__m256i*)(res + i + j));;
            tres = _mm256_add_epi16(tres, _mm256_mullo_epi16(multiplier, multiplicands));
            _mm256_storeu_si256((__m256i*)(res + i + j), tres);

        }

        for (; j < xlen2; j++) {
            res[i + j] += num1[i] * num2[j];
        }

        if (i % 400 == 0 || i == len1 - 1) {
            for (j = 0 - i; j < len2; j++) {
                res[i + j + 1] += res[i + j] / 10;
            }

            for (j = -i; j < len2; j++) {
                res[i + j] = mod10(res[i + j]);
            }

        }


    }

    // Convert back to string
    int idx = lenR - 1;
    while (idx > 0 && res[idx] == 0) idx--; // Remove leading zeros
	//
    for (int i = 0; i <= idx; i++)
        result[i] = res[idx - i];

    convert_int_to_ascii_avx2(result, idx + 1);
    result[idx + 1] = '\0';

    free(res);
}
