//
// Created by Ilya on 31.03.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "variables.h"

void f_mem_dump(adr start, word n, char* s);
void f_mem_dump_1(adr start, word n, char* s);

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
    mem[a] = val;
}

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
    adr_n *ret = malloc(sizeof(adr_n)); // инфа о том, сколько и куда записали
    ret->ad = 0;
    ret->n = 0;
    unsigned int n; // число байтов в вводимом блоке
    int i;
    //unsigned int k = 0; // счётчик записанных байтов
    unsigned short int x;
    FILE *fi = fopen("load.txt", "r");
    while (fscanf(fi, "%hx%x", &ret->ad, &ret->n) == 2) {
        //printf("%hx\n", ret->ad);
        for (i = 0; i < ret->n; i++) {
            fscanf(fi, "%hx", &x); // считали
            mem[ret->ad + i] = (byte) x; // записали
            //k++; // ну тут понятно
        }
       // f_mem_dump_1(ret->ad, (word) ret->n, "load_out_1.txt");
    }
    fclose(fi);
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

//int take_mnem(word comm) {
//    int mnem = (comm >> 12);
//    return mnem;
//}

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
    if (res.d_s_op == 2) { // double operand
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
        } else {
            res.pc_mode_src = (x >> 9);
            x = x - (res.pc_mode_src << 9);
            res.reg1 = (x >> 6);
            x = x - (res.reg1 << 6);
            res.pc_mode_dst = (x >> 3);
            x = x - (res.pc_mode_dst << 3);
            res.reg2 = x;
        }
    } else if(res.d_s_op == 1){ // single operand
        x = x - (word)(1 << 11);
        res.opcode10_6 = (x >> 6);
        x = x - (res.opcode10_6 << 6);
        res.pc_mode_dst = (x >> 3);
        x = x - (res.pc_mode_dst << 3);
        res.reg2 = x;
    } else if(res.d_s_op == 0){ // non operand
        res.opcode10_8 = (x >> 8);
        x = x - (res.opcode10_8 << 8);
        res.offset = (byte)x;
    } else {}
    return res;
}

word take_src(command com){
    if(com.reg1 != 7 && com.pc_mode_src != 0)
        reg[com.reg1] -= 2;
    switch (com.pc_mode_src) {
        case 0:
            if(t)
                fprintf(f,"R%d ", com.reg1);
            return reg[com.reg1];
        case 2:
            reg[com.reg1] += 2;
            if(t)
                fprintf(f, "#%d ", w_read(reg[com.reg1]));
            return w_read(reg[com.reg1]);
        case 3:
            reg[com.reg1] += 2;
            return w_read(w_read(reg[com.reg1]));
        case 6:
            reg[com.reg1] += 2;
            return w_read((word)(reg[com.reg1] - 2 + w_read(reg[com.reg1])));
        case 7:
            reg[com.reg1] += 2;
            return w_read(w_read((word)(reg[com.reg1] - 2 + w_read(reg[com.reg1]))));
        default:
            fprintf(f, "DEFAULT EXIT (take_src) \n");
            break;
    }
}

word take_src_B(command com){
    switch (com.pc_mode_src) {
        case 0:
            if(t)
                fprintf(f,"R%d ", com.reg1);
            return reg[com.reg1];
        case 2:
            //reg[com.reg1] += 1;
            if(t) {
                fprintf(f, "(R%d)+ ", com.reg1);
            }
            return b_read(reg[com.reg1]++);
        case 3:
            //reg[com.reg1] += 1;
            return b_read(b_read(reg[com.reg1]++));
        case 6:
            //reg[com.reg1] += 1;
            return b_read((word)(reg[com.reg1]++ + b_read(reg[com.reg1])));
        case 7:
            //reg[com.reg1] += 1;
            return b_read(b_read((word)(reg[com.reg1]++ + b_read(reg[com.reg1]))));
        default:
            fprintf(f, "DEFAULT EXIT (take_src_B\n");
            break;
    }
}

word take_dst(command com){
    switch (com.pc_mode_dst) {
        case 0:
            return reg[com.reg2];
        case 2:
            return w_read((word)(reg[com.reg2] + 2));
        case 3:
            return w_read(w_read((word)(reg[com.reg2] + 2)));
        case 6:
            return w_read((word)(reg[com.reg2] + w_read((word)(reg[com.reg2] + 2))));
        case 7:
            return w_read(w_read((word)(reg[com.reg2] + w_read((word)(reg[com.reg2] + 2)))));
        default:
            fprintf(f, "DEFAULT EXIT (take_dst)\n");
            break;
    }
}

word push_dst(command com, word x){
    switch (com.pc_mode_dst) {
        case 0:
            if(t)
                fprintf(f, "R%d ", com.reg2);
            reg[com.reg2] = x;
            break;
        case 2:
            reg[com.reg1] += 2;
            w_write(reg[com.reg1], x);
            reg[com.reg1] -= 2;
            break;
        case 3:
            reg[com.reg1] += 2;
            w_write(w_read(reg[com.reg1]), x);
            reg[com.reg1] -= 2;
            break;
        case 6:
            reg[com.reg1] += 2;
            w_write((word)(reg[com.reg1] - 2 + w_read(reg[com.reg1])), x);
            reg[com.reg1] -= 2;
            break;
        case 7:
            reg[com.reg1] += 2;
            w_write(w_read((word)(reg[com.reg1] - 2 + w_read(reg[com.reg1]))), x);
            reg[com.reg1] -= 2;
            break;
        default:
            fprintf(f, "DEFAULT EXIT (push_dst)\n");
            break;
    }
}

void do_command(word comm) {

    if (comm == 0) {
        if(t)
            fprintf(f, "HALT \n");
        fprintf(f, "\n--------------- halted ---------------\n");
        fprintf(f, "r0=%06o r2=%06o r4=%06o sp=%06o\n", reg[0], reg[2], reg[4], reg[6]);
        fprintf(f, "r1=%06o r3=%06o r5=%06o pc=%06o\n", reg[1], reg[3], reg[5], reg[7]);
        fprintf(f, "N=%06o  Z=%06o  V=%06o  C=%06o\n", N, Z, V, C);
        exit(0);
    }
    command com = take_com(comm);
    if(com.d_s_op == 2){  // DOUBLE OPERAND-----------------------------------------------------------------------------
        if(com.opcode14_12 == 7){
            switch (com.opcode11_9){
                case 7: //SOB
                    if(t)
                        fprintf(f, "SOB ");
                    reg[com.reg1]--;
                    if(reg[com.reg1] > 0)
                        reg[7]-= 2*(com.reg2 + 8*com.pc_mode_dst);
                    break;
                default:
                    fprintf(f, "DEFAULT EXIT \n");
                    break;
            }

        } else { //  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            switch (com.opcode14_12) {
                case 1: // MOV
                    if(t)
                        fprintf(f,"MOV");
                    if(com.B == 0) {
                        printf(" ");
                        yy.ui = take_src(com);
                        src = (word)yy.si;
                        Z = src == 0 ? 1 : 0;
                        push_dst(com, src);
                    } else {
                        if(t)
                            fprintf(f, "B ");
                        xx.uby = (byte)take_src_B(com);
                        src = (word)xx.sby;
                        Z = src == 0 ? 1 : 0;
                        push_dst(com, src);
                    }
                    break;
                case 6: // ADD
                    if(t)
                        fprintf(f, "ADD ");
                    yy.ui = take_dst(com);
                    zz.ui = take_src(com);
                    push_dst(com, (word)(yy.si + zz.si));
                    break;
                default:
                    fprintf(f, "DEFAULT EXIT \n");
                    break;
            }
        }
    } else if(com.d_s_op == 1){  // SINGLE OPERAND----------------------------------------------------------------------
        switch (com.opcode10_6){
            case 8: // CLR
                if(t)
                    fprintf(f, "CLR ");
                N = 0; Z = 1; V = 0; C = 0;
                push_dst(com, 0);
                break;
            default:
                fprintf(f, "DEFAULT EXIT \n");
                break;
        }
    } else if(com.d_s_op == 0) { // s_byte instructions: NO OPERAND-----------------------------------------------------
        switch (com.opcode10_8){
            case 1: // BR
                if(t)
                    fprintf(f, "BR ");
                xx.uby = com.offset;
                reg[7] += 2 * xx.sby;
                break;
            case 3: // BEQ
                if(t)
                    fprintf(f, "BEQ ");
                if(Z == 1) {
                    xx.uby = com.offset;
                    reg[7] += 2 * xx.sby;
                }
                break;
            default:
                fprintf(f, "DEFAULT EXIT (do_command !?!?!?)\n");
                break;

        }
    } else { fprintf(f, "take_com error"); }
}

//void test_mem() {
//    byte b0, b1;
//    word w;
//    w = 0x0d0c;
//    w_write(4, w);
//    b0 = b_read(4);
//    b1 = b_read(5);
//    printf("%04x = %02x%02x\n", w, b1, b0);
//    assert(b0 == 0x0c);
//    assert(b1 == 0x0d);
//}