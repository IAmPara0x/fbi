#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#ifdef BENCH_FBI
#include "bigint.h"
#else
#include <gmp.h>
#endif

#define MAX_LINE_LENGTH 8192


void parse_csv(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
		bool parsed_column = false;

    while (fgets(line, sizeof(line), fp) != NULL) {

        if (!parsed_column) {
                parsed_column = true;
                continue;
        }
				
        // Remove trailing newline, if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Tokenize the line by commas
        char *x      = strtok(line, ",");
        char *y      = strtok(NULL, ",");
        char *expected = strtok(NULL, ",");

       
        if (x && y && expected) {
        
#ifdef BENCH_FBI

            char actual[4096];
            multiply(x,y,actual);
            
            if (strcmp(actual, expected) != 0) {
                printf("[FAILED] x = %s, y = %s, expected = %s, actual= %s\n", x,y, expected, actual);
                assert(strcmp(actual, expected) == 0);
            }

#else
            mpz_t a, b, result;

            mpz_init(a);
            mpz_init(b);
            mpz_init(result);

            mpz_set_str(a, x, 10);
            mpz_set_str(b, y, 10);
           
            mpz_mul(result, a, b);

            
            mpz_clear(a);
            mpz_clear(b);
            mpz_clear(result);

#endif


        } else {
      
            fprintf(stderr, "Malformed line or missing columns.\n");

        }
    }

    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    parse_csv(argv[1]);
		printf("PASSED ALL TEST CASES!\n");
    return EXIT_SUCCESS;
}
