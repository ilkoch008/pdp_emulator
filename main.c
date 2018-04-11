#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "variables.h"

int main(int argc, char **argv) {
    FILE *g = fopen("param.txt", "r");
    char *param = malloc(100 * sizeof(char));
    fscanf(g, "%90s", param);
    t = 0;
    if (strstr(param, "-t") != NULL)
        t = 1;
    else
        printf("< %s > is not an executable command\n", param);
    free(param);
    fclose(g);
    FILE *fi = fopen("load.txt", "r");
    adr_n * an = f_load_file(fi);
    fclose(fi);
    f_mem_dump(an->ad, (word) an->n, "load_out.txt");
    f = fopen("load_out_res.txt", "w");
    if(t)
        fprintf(f, "TRACE \n \n");
    for (reg[7] = an->ad; reg[7] < an->ad + an->n; ) {
        if(t)
            fprintf(f, "%06o : %06o  ", reg[7], w_read(reg[7]));
        do_command(w_read(reg[7]));
        if(t) {
            print_all();
            fprintf(f, "\n");
        }
        schet ++;
        if(schet > 32*1000)
            break;
    }
    free(an);
    fclose(f);
    return 0;
}

