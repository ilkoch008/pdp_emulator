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
void f_mem_dump(adr start, word n);
int take_mnem(word comm);
void do_command(word comm);
command take_com(word x);

int main(int argc, char **argv) {
    t = 1;
    if (argc > 1)
        if (strcmp(argv[1], "-t") == 0)
            t = 1;
        else
            printf("< %s > is not an executable command\n", argv[1]);
    adr_n *an = f_load_file();
    f_mem_dump(an->ad, (word) an->n);
    f = fopen("load_out_res.txt", "w");
    for (reg[7] = an->ad; reg[7] < an->ad + an->n; reg[7] += 2) {
        if(t)
            fprintf(f, "%06o : %06o  ", reg[7], w_read(reg[7]));
        do_command(w_read(reg[7]));
        if(t)
            fprintf(f,"\n");
    }
    free(an);
    fclose(f);
    return 0;
}

