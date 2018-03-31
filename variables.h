//
// Created by Ilya on 31.03.2018.
//

#ifndef TEST_3_VARIABLES_H
#define TEST_3_VARIABLES_H

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

typedef struct adr_n {
    adr ad;
    unsigned int n;
} adr_n;

#endif //TEST_3_VARIABLES_H
