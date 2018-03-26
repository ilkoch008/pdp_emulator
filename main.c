#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

byte mem[64 * 1024];

byte b_read(adr a);         // читает из "старой памяти" mem байт с "адресом" a.
void b_write(adr a, byte val); // пишет значение val в "старую память" mem в байт с "адресом" a.
word w_read(adr a);        // читает из "старой памяти" mem слово с "адресом" a.
void w_write(adr a, word val); // пишет значение val в "старую память" mem в слово с "адресом" a.
int load_file();
int f_load_file();
void mem_dump(adr start, word n);

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

int f_load_file() {
    adr ad; // адрес, на который пишем
    unsigned int n; // число байтов в вводимом блоке
    int i;
    int k = 0; // счётчик записанных байтов
    unsigned short int x;
    FILE *f = fopen("load.txt", "r");
    while (fscanf(f, "%hx%x", &ad, &n) == 2) {
        for (i = 0; i < n; i++) {
            fscanf(f, "%hx", &x); // считали
            mem[ad + i] = (byte) x; // записали
            k++; // ну тут понятно
        }
    }
    fclose(f);
    return k;
}

void mem_dump(adr start, word n) {
    unsigned int i;
    for (i = 0; i < n; i += 2) {
        printf("%06o : %06o\n", (start + i), w_read(start + i));
    }
}

void f_mem_dump(adr start, word n) {
    unsigned int i;
    FILE * f = fopen("load1.txt", "w");
    for (i = 0; i < n; i += 2) {
        fprintf(f, "%06o : %06o\n", (start + i), w_read(start + i));
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

int main() {
    int k = 0;
    k = f_load_file();
    f_mem_dump(0x40, (word) k);
    return 0;
}
