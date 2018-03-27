#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

byte mem[64 * 1024];
word R[8] = {0};
/*
* 0-5 - Стандартные регистры
* 6 - Stack Pointer
* 7 - Program Counter
*/

typedef struct adr_n{
    adr ad;
    unsigned int n;
} adr_n;

byte b_read(adr a);         // читает из "старой памяти" mem байт с "адресом" a.
void b_write(adr a, byte val); // пишет значение val в "старую память" mem в байт с "адресом" a.
word w_read(adr a);        // читает из "старой памяти" mem слово с "адресом" a.
void w_write(adr a, word val); // пишет значение val в "старую память" mem в слово с "адресом" a.
int load_file();
adr_n * f_load_file();
void mem_dump(adr start, word n);
void f_mem_dump(adr start, word n);
int take_mnem(word comm);
void do_command(word comm);

int main(int argc, char **argv) {
    //printf("%s\n%s", argv[0], argv[1]);
    adr_n * an = f_load_file();
    //printf("%d\n", an->ad);
    f_mem_dump(an->ad, (word) an->n);
    for (R[7] = an->ad; R[7] < an->ad + an->n; R[7]+=2){
        do_command(w_read(R[7]));
    }
    free(an);
    return 0;
}

//typedef union ob {
//	word aa;
//	byte a[2];
//} ob;

byte b_read(adr a) {
    return mem[a];
}

void b_write(adr a, byte val) {
    mem[a] = val;
}

//word w_read (adr a){
//	ob x;
//	x.a[0] = mem[a];
//	x.a[1] = mem[a+1];
//	return x.aa;
//}
//
//void w_write (adr a, word val){
//	ob x;
//	x.aa = val;
//	mem[a] = x.a[0];
//	mem[a+1] = x.a[1];
//}

word w_read(adr a) {
    word x;
    x = mem[a];
    x += mem[a + 1] * 256;
    return x;
}

void w_write(adr a, word val) {
    mem[a] = (byte) val;
    mem[a + 1] = (byte) (val >> 8);
}

int load_file() {
    adr ad; // адрес, на который пишем
    unsigned int n; // число байтов в вводимом блоке
    int i;
    int k = 0;
    unsigned short int x;
    while (scanf("%hx%xu", &ad, &n) == 2) {
        for (i = 0; i < n; i++) {
            scanf("%hxu", &x);
            mem[ad + i] = (byte) x;
            k++;
        }
    }
    return k;
}

adr_n * f_load_file() { // ЧИТАЕМ ПОКА ПО ОДНОЙ ПРОГЕ!!!
    adr ad; // адрес, на который пишем
    adr_n * ret = malloc(sizeof(adr_n)); // инфа о том, сколько и куда записали
    ret->ad = 0;
    ret->n = 0;
    unsigned int n; // число байтов в вводимом блоке
    int i;
    //unsigned int k = 0; // счётчик записанных байтов
    unsigned short int x;
    FILE *f = fopen("load.txt", "r");
    while (fscanf(f, "%hx%x", &ret->ad, &ret->n) == 2) {
        //printf("%hx\n", ret->ad);
        for (i = 0; i < ret->n; i++) {
            fscanf(f, "%hx", &x); // считали
            mem[ret->ad + i] = (byte) x; // записали
            //k++; // ну тут понятно
        }
    }
//    ret.ad = ad;
//    ret.n = k;
    //printf("%hx\n", ret->ad);
    fclose(f);
    return ret;
}

void mem_dump(adr start, word n) {
    unsigned int i;
    for (i = 0; i < n; i += 2) {
        printf("%06o : %06o\n", (start + i), w_read(start + i));
    }
}

void f_mem_dump(adr start, word n) {
    unsigned int i;
    FILE * f = fopen("load_out.txt", "w");
    for (i = 0; i < n; i += 2) {
        fprintf(f, "%06o : %06o\n", (start + i), w_read(start + i));
    }
    fclose(f);
}

int take_mnem(word comm){
    int mnem = (comm >> 12);
    return mnem;
}

void do_command(word comm){
    //printf("%06o\n", comm);
    int mnem = take_mnem(comm);
    //printf("%d\n", mnem);
    //printf("_______________________________\n");
    FILE * f = fopen("load_out_res.txt", "w");
    switch (mnem){
        case 1:// MOV
            if((comm >> 6)%64 == 027) {
                R[7] += 2;
                R[comm % 64] = w_read(R[7]);
            }
            break;
//----------------------------------------------
        case 6:// ADD
            R[comm % 64] = R[comm % 64] + R[(comm >> 6) % 64];
            break;
//----------------------------------------------
        case 0:
            fprintf(f, "------------ halted ------------\n");
            fprintf(f, "r0=%06o r2=%06o r4=%06o sp=%06o\n", R[0], R[2], R[4], R[6]);
            fprintf(f, "r1=%06o r3=%06o r5=%06o pc=%06o\n", R[1], R[3], R[5], R[7]);
//----------------------------------------------
        default:

            break;
    }
    fclose(f);
}

void test_mem() {
    byte b0, b1;
    word w;
    w = 0x0d0c;
    w_write(4, w);
    b0 = b_read(4);
    b1 = b_read(5);
    printf("%04x = %02x%02x\n", w, b1, b0);
    assert(b0 == 0x0c);
    assert(b1 == 0x0d);
}


