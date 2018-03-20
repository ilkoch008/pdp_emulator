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
void load_file();
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
    mem[a] = (word) val;
    mem[a + 1] = (word) (val >> 8);
}

void load_file() {
    adr ad; // адрес, на который пишем
    unsigned int n; // число байтов в вводимом блоке
    int i;
    unsigned short int x;
    while (scanf("%hx%xu", &ad, &n) == 2) {
        for (i = 0; i < n; i++) {
            scanf("%hxu", &x);
            mem[ad + i] = x;
        }
    }
}

void mem_dump(adr start, word n) {
    unsigned int i;
    for (i = 0; i < n; i += 2) {
        printf("%06o : %06o\n", (start + i), w_read(start + i));
    }
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
    load_file();
    mem_dump(0x40, 4);
    return 0;
}
