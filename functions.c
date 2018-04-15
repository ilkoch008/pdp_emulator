//
// Created by Ilya on 31.03.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "variables.h"

void print_all() {
    fprintf(f, "\n------------ print all ------------\n");
    fprintf(f, "r0=%06o r2=%06o r4=%06o sp=%06o\n", reg[0], reg[2], reg[4], reg[6]);
    fprintf(f, "r1=%06o r3=%06o r5=%06o pc=%06o\n", reg[1], reg[3], reg[5], reg[7]);
    fprintf(f, "N=%06o  Z=%06o  V=%06o  C=%06o\n", N, Z, V, C);
    fprintf(f, "-----------------------------------\n");
}

byte b_read(adr a) {
    return mem[a];
}

void b_write(adr a, byte val) {
    if(a < 8) {
        reg[a] = val;
    } else {
        mem[a] = val;
        if (a == 0177566) {
            if (t) {
                fprintf(f, " ---------------> %c", odata);
            } else {
                fprintf(f, "%c", odata);
            }
        }
    }
}

word w_read(adr a) {
    word x;
    x = mem[a];
    x += mem[a + 1] * 256;
    return x;
}

word read(adr a, word B){
    if(B)
        return b_read(a);
    else
        return w_read(a);
}

void w_write(adr a, word val) {
    if(a < 8){
        reg[a] = val;
    } else {
        mem[a] = (byte) val;
        mem[a + 1] = (byte) (val >> 8);
    }
}

void write(adr a, word B, word val){
    if(B)
        b_write(a, (byte) val);
    else
        w_write(a, val);
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

adr_n * f_load_file(FILE * fi) { // ЧИТАЕМ ПОКА ПО ОДНОЙ ПРОГЕ!!!
    adr ad; // адрес, на который пишем
    adr_n *ret = malloc(sizeof(adr_n)); // инфа о том, сколько и куда записали
    ret->ad = 0;
    ret->n = 0;
    unsigned int n; // число байтов в вводимом блоке
    int i;
    //unsigned int k = 0; // счётчик записанных байтов
    unsigned short int x;
    while (fscanf(fi, "%hx%x", &ret->ad, &ret->n) == 2) {
        //printf("%hx\n", ret->ad);
        for (i = 0; i < ret->n; i++) {
            fscanf(fi, "%hx", &x); // считали
            //mem[ret->ad + i] = (byte) x; // записали
            b_write((adr)(ret->ad + i), (byte)x);
            //k++; // ну тут понятно
        }
       // f_mem_dump_1(ret->ad, (word) ret->n, "load_out_1.txt");
    }

    return ret;
}

void mem_dump(adr start, word n) {
    unsigned int i;
    for (i = 0; i < n; i += 2) {
        printf("%06o : %06o = %016i\n", ((adr)(start + i)), w_read((adr)(start + i)), w_read((adr)(start + i)));
    }
}

void f_mem_dump(adr start, word n, char* s) {
    unsigned int i;
    FILE *fil = fopen(s, "w");
    for (i = 0; i < n; i += 2) {
        fprintf(fil, "%06o : %06o\n", ((adr)(start + i)), w_read((adr)(start + i)));
    }
    fclose(fil);
}

void f_mem_dump_1(adr start, word n, char* s) {
    unsigned int i;
    FILE *fi = fopen(s, "w");
    for (i = 0; i < n; i += 1) {
        fprintf(fi, "%06o : %06o\n", ((adr)(start + i)), b_read((adr)(start + i)));
    }
    fclose(fi);
}

command take_com(word x) {
    command res = {0};
    res.B = (x >> 15);
    x = x -  (res.B << 15);
    if ((x >> 12) != 0)
        res.d_s_op = 2;
    else if ((x >> 11) == 1)
        res.d_s_op = 1;
    else
        res.d_s_op = 0;
    switch (res.d_s_op) { // double operand
        case 2:// register - - - src/dst
        res.opcode14_12 = (x >> 12);
        x = x -  (res.opcode14_12 << 12);
        if (res.opcode14_12 == 7) {
            res.opcode11_9 = (x >> 9);
            x = x - (res.opcode11_9 << 9);
            res.reg1 = (x >> 6);
            x = x -(res.reg1 << 6);
            res.pc_mode_dst = (x >> 3);
            x = x - (res.pc_mode_dst << 3);
            res.reg2 = x;
        } else { // source - - - destination
            res.pc_mode_src = (x >> 9);
            x = x - (res.pc_mode_src << 9);
            res.reg1 = (x >> 6);
            x = x - (res.reg1 << 6);
            res.pc_mode_dst = (x >> 3);
            x = x - (res.pc_mode_dst << 3);
            res.reg2 = x;
        }
            break;
        case 1: // single operand
        x = x - (word)(1 << 11);
        res.opcode10_6 = (x >> 6);
        x = x - (res.opcode10_6 << 6);
        res.pc_mode_dst = (x >> 3);
        x = x - (res.pc_mode_dst << 3);
        res.reg2 = x;
            break;
        case 0: // non operand
        res.opcode10_8 = (x >> 8);
        x = x - (res.opcode10_8 << 8);
        res.offset = (byte)x;
            break;
        default:
            fprintf(f, "DEFAULT EXIT (take_com) \n");
            break;
    }
    return res;
}


int do_command(word comm) {

    if (comm == 0) {
        if(t)
            fprintf(f, "HALT \n");
        fprintf(f, "\n--------------- halted ---------------\n");
        fprintf(f, "r0=%06o r2=%06o r4=%06o sp=%06o\n", reg[0], reg[2], reg[4], reg[6]);
        fprintf(f, "r1=%06o r3=%06o r5=%06o pc=%06o\n", reg[1], reg[3], reg[5], reg[7]);
        fprintf(f, "N=%06o  Z=%06o  V=%06o  C=%06o\n", N, Z, V, C);
        return 1;
    }
    word save = comm;
    command com = take_com(comm);
    reg[7] += 2;
    word BB;
    BB = (save >> 15);
    save = save -  (BB << 15);
    int indik;

    int i;
    for(i = 0; ; i++) {
        if (func_list[i].opc == (func_list[i].msk & save)) {
            indik = func_list[i].instr(com);
            if(indik == 0)
                break;
        }
    }
    return 0;
}

int sob(command com){
    if(t)
        fprintf(f, "SOB ");
    reg[com.reg1]--;
    if(reg[com.reg1] > 0)
        reg[7]-= 2*(com.reg2 + 8*com.pc_mode_dst);
    return 0;
}

int mov(command com){
    if (t)
        fprintf(f, "MOV");
    if (com.B == 0) {
        if (t)
            fprintf(f, " ");
        SRC = take(com.pc_mode_src, com.reg1, com.B);
        DST = take(com.pc_mode_dst, com.reg2, com.B);
        yy.ui = SRC.val;
        src = (word)yy.si;
        Z = src == 0 ? 1 : 0;
        w_write(DST.adr, SRC.val);

    } else {
        if (t)
            fprintf(f, "B ");
        SRC = take(com.pc_mode_src, com.reg1, com.B);
        DST = take(com.pc_mode_dst, com.reg2, com.B);
        xx.uby = (byte)SRC.val;
        src = (word)xx.sby;
        Z = src == 0 ? 1 : 0;
        b_write(DST.adr, (byte)SRC.val);
    }
    return 0;
}

int add(command com){
    if(t)
        fprintf(f, "ADD ");
    DST = take(com.pc_mode_dst, com.reg2, com.B);
    SRC = take(com.pc_mode_src, com.reg1, com.B);
    yy.ui = DST.val;
    zz.ui = SRC.val;
    short int save = yy.si;
    yy.si = yy.si + zz.si;
    N = yy.si < 0 ? 1 : 0;
    Z = yy.si == 0 ? 1 : 0;
    if(save < 0 && zz.si < 0 && yy.si > 0) V = 1;
    else if (save > 0 && zz.si > 0 && yy.si < 0) V = 1;
    else V = 0;
    w_write(DST.adr, (word)(yy.si));
    return 0;
}

int tst(command com){
    if(com.B){ // TSTB
        if(t)
            fprintf(f, "TSTB ");
        DST = take(com.pc_mode_dst, com.reg2, com.B);
        xx.uby = (byte)DST.val;
        N = xx.sby < 0 ? 1 : 0;
        Z = xx.sby == 0 ? 1 : 0;
        V = 0; C = 0;
    } else { // TST
        if (t)
            fprintf(f, "TST ");
        DST = take(com.pc_mode_dst, com.reg2, com.B);
        yy.ui = DST.val;
        N = yy.si < 0 ? 1 : 0;
        Z = yy.si == 0 ? 1 : 0;
        V = 0;
        C = 0;
    }
    return 0;
}

int br(command com){
    if(t)
        fprintf(f, "BR ");
    xx.uby = com.offset;
    reg[7] += 2 * xx.sby;
    return 0;
}

int beq(command com){
    if(t)
        fprintf(f, "BEQ ");
    if(Z == 1) {
        xx.uby = com.offset;
        reg[7] += 2 * xx.sby;
    }
    return 0;
}

int bpl(command com) {
    if (N == 0 && com.B == 1) {
        if(t)
            fprintf(f, "BPL ");
        xx.uby = com.offset;
        reg[7] += 2 * xx.sby;
        return 0;
    } else if (N == 1 && com.B == 1) {
        if(t)
            fprintf(f, "BPL ");
        return 0;
    } else {
        return 1;
    }
}

int jsr(command com){
    if(t)
        fprintf(f, "JSR ");
    DST = take(com.pc_mode_dst, com.reg2, com.B);
    w_write(reg[6], reg[com.opcode10_6]);
    reg[6]-= 2;
    reg[com.opcode10_6] = reg[7];
    reg[7] = DST.adr;
    return 0;
}

int rts(command com){
    if(t)
        fprintf(f, "RTS ");
    dst = (word)(com.offset - (1 << 7));
    reg[7] = reg[dst];
    reg[6]+= 2;
    reg[dst] = w_read(reg[6]);
    return 0;
}

int nothing(command com){
    if(t)
        fprintf(f, "Ha! loh, net tut takoy function\n ");
    return 0;
}

int clr(command com){
    if(t)
        fprintf(f, "CLR ");
    DST = take(com.pc_mode_dst, com.reg2, com.B);
    N = 0; Z = 1; V = 0; C = 0;
    w_write(DST.adr, 0);
}

func func_list[100] =
        {
                {0177000, 0077000, sob},
                {0170000, 0010000, mov},
                {0170000, 0060000, add},
                {0177700, 0005700, tst},
                {0177400, 0000400, br},
                {0177400, 0001400, beq},
                {0177400, 0000000, bpl},
                {0177000, 0004000, jsr},
                {0177770, 0000200, rts},
                {0177700, 0005000, clr},
                {0000000, 0000000, nothing}
        };