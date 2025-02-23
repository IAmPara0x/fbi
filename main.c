#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "bigint.h"

#define MAX_LINE_LENGTH 4096

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
        
						char *actual = multiply(x,y);

						if (strcmp(actual, expected) != 0) {
								printf("[FAILED] x = %s, y = %s, expected = %s, actual= %s\n", x,y,actual, expected);
								assert(strcmp(actual, expected) == 0);
						}

						free(actual);


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
