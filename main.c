#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

int main(int argc, char **argv) {
    //printf("%s\n%s", argv[0], argv[1]);
    adr_n *an = f_load_file();
    //printf("%d\n", an->ad);
    f_mem_dump(an->ad, (word) an->n);
    for (R[7] = an->ad; R[7] < an->ad + an->n; R[7] += 2) {
        do_command(w_read(R[7]));
    }
    free(an);
    return 0;
}






