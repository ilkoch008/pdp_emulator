#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "variables.h"

int main(int argc, char **argv) {
    int i, k;
    FILE *g = fopen("param.txt", "r");
    char ** parampampam = malloc(10 * sizeof(char*));
    for(i = 0; i < 10; i++){
        parampampam[i] = malloc(100 * sizeof(char));
    }
    for(i = 0; i < 10; i++) {
        k = fscanf(g, "%90s", parampampam[i]);
        if(k != 1)
            break;
    }
    t = 0;
    if (strstr(parampampam[0], "-t") != NULL)
        t = 1;
    else if (strstr(parampampam[0], "-q") != NULL)
        t = 0;
    else
        printf("< %s > is not an executable command\n", parampampam[0]);
    if (strstr(parampampam[1], "-std") != NULL)
        std = 1;
    else if (strstr(parampampam[1], "-file") != NULL)
        std = 0;
    else
        printf("< %s > is not an executable command\n", parampampam[1]);
    fclose(g);
    // TODO: read filename from params DONE
    FILE *fi = fopen(parampampam[2], "r");
    adr_n * an = f_load_file(fi);
    fclose(fi);
    f_mem_dump(an->ad, (word) an->n, parampampam[3]);
    // TODO: write to file or to stdout. Defined in params. DONE
    f = NULL;
    if(std == 0){
        f = fopen(parampampam[4], "w");
    }
    ostat = 0377;
    int test;
    if(t)
        fprintf(f, "TRACE \n \n");
    // TODO: read start address from params DONE
    for (reg[7] = (word)strtoll(parampampam[5], NULL, 8); ; ) {
        if(t){
            fprintf(f, "%06o : %06o  ", reg[7], w_read(reg[7]));
            if(dst)
                printf("%06o : %06o  ", reg[7], w_read(reg[7]));
        }
        test = do_command(w_read(reg[7]));
        if(test == 1)
            break;
        if(t) {
            print_all();
            fprintf(f, "\n");
            if(std){
                print_all_std();
                printf("\n");
            }
        }
        schet ++;
        if(schet > 32*1000)
            break;
    }

    if(std) {
        printf("\n<<<Enter a string to finish>>>\n");
        scanf("%s", parampampam[9]);
    }

    for(i = 0; i < 10; i++){
        free(parampampam[i]);
    }

    free(parampampam);
    free(an);
    fclose(f);
    return 0;
}

