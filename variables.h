//
// Created by Ilya on 31.03.2018.
//

#ifndef TEST_3_VARIABLES_H
#define TEST_3_VARIABLES_H

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

int N, Z, V, C;

typedef union s_byte{
    char sby;
    unsigned char uby;
} s_byte;

typedef union s_word{
    short int si;
    unsigned short int ui;
} s_word;

// N indicating a negative value
// Z indicating a zero (equal) condition
// V indicating an overflow condition, and
// C indicating a carry condition.

s_byte xx;
s_word yy;
s_word zz;
word src, dst;
FILE * f;
int t; // для трассировки
byte mem[64 * 1024];
word reg[8];
/*
* 0-5 - Стандартные регистры
* 6 - Stack Pointer
* 7 - Program Counter
*/


typedef struct adr_n { // для возврата адреса и числа байтов из
    adr ad; // из f_load_file
    unsigned int n;
} adr_n;

typedef struct comm {
    word B;
    word d_s_op; // 2 if comm is double-operand, 1 if single, 0 if no operand
    word pc_mode_src; // program counter addressing mode
    word pc_mode_dst;
    word opcode14_12;
    word opcode11_9;
    word opcode10_6;
    word opcode10_8;
    byte offset;
    word st_mode; // stack addressing mode
    word reg1; // first register ( [0;7] if needed)
    word reg2; // second register ( [0;7] if needed)
} command;

#endif //TEST_3_VARIABLES_H
