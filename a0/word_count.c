#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// int count;

int word_count(FILE *f) {
    int wc = 0;
    char character = '\0';
    bool is_word = false; // to help with multiple tabs, spaces, etc. in a row.
    
    while ((character = fgetc(f)) != EOF) {
        if (isspace(character)) {
            if (is_word) {
                is_word = false;
                wc++;
            }

        } else {
            is_word = true;
        }
        
    }
    if (is_word) {
        wc++;
    }
    return wc;
}



int main(int argc, char* argv[]) {
    int count = 0;

    FILE *f = fopen(argv[1], "r");
    count = word_count(f);
    fclose(f);
    printf("%d %s\n", count, argv[1]);
    return 0;
}
