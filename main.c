#include "variables.h"

int main(int argc, char **argv) {
    int i, k;
    FILE *g = fopen("param.txt", "r");
    char  parampampam[300];
    parampampam[299] = '\0';
    char * pointer;
    fgets(parampampam, 250, g);
    fprintf(f, "%s\n", parampampam);
    t = 0;
    std = 1;
    nom flags = nom_flags(parampampam);
    fclose(g);
    FILE *fi = fopen(flags.load[0], "r");
    adr_n * an = f_load_file(fi);
    fclose(fi);
    f_mem_dump(an->ad, (word) an->n, flags.load[1]);
    f = NULL;
    if(std == 0){
        f = fopen(flags.load[2], "w");
    } else {
        f = stdout;
    }
    ostat = 0377;
    int test;
    if(t)
        fprintf(f, "TRACE \n \n");
    for (reg[7] = flags.start; ; ) {
        if(t){
            fprintf(f, "%06o : %06o  ", reg[7], w_read(reg[7]));
        }
        test = do_command(w_read(reg[7]));
        if(test == 1)
            break;
        if(t) {
            print_all();
            fprintf(f, "\n");
        }
        schet ++;
        if(schet > 32*1000)
            break;
    }

    if(std){
        fprintf(f, "\n<<<Enter a string to finish>>>\n");
        scanf("%20s", flags.load[4]);
    }

    //printf("LOLOLO\n");

    for(i = 0; i < 10; i++){
        free(flags.load[i]);
    }

    //free(parampampam);
    free(an);
    fclose(f);
    return 0;
}

