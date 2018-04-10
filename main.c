#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "variables.h"



byte b_read(adr a);         // читает из "старой памяти" mem байт с "адресом" a.
void b_write(adr a, byte val); // пишет значение val в "старую память" mem в байт с "адресом" a.
word w_read(adr a);        // читает из "старой памяти" mem слово с "адресом" a.
void w_write(adr a, word val); // пишет значение val в "старую память" mem в слово с "адресом" a.
int load_file();
adr_n *f_load_file();
void mem_dump(adr start, word n);
void f_mem_dump(adr start, word n, char* s);
int take_mnem(word comm);
void do_command(word comm);
command take_com(word x);
void print_all();

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

    adr_n * an = f_load_file();
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
    }
    free(an);
    fclose(f);
    return 0;
}

