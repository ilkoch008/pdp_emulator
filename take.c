//
// Created by Ilya on 09.04.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "variables.h"

word take(word ad_mode, word regi, word B) {
    switch (ad_mode) {
        case 0:
            if (t)
                fprintf(f, "R%o ", regi);
            return reg[regi];
        case 1:
            if (t)
                fprintf(f, "@%o ", regi);
            return read(reg[regi], B);
        case 2:
            if (regi == 7 || B == 0) {
                if (t)
                    fprintf(f, "#%o ", w_read(reg[regi]));
                reg[regi] += 2;
                return read((word)(reg[regi] - 2), B);
            } else {
                if (t)
                    fprintf(f, "#%o ", b_read(reg[regi]));
                reg[regi]++;
                return read((word)(reg[regi]-1), B);
            }
        case 3:
            if (regi == 7 || B == 0) {
                reg[regi] += 2;
//                if(w_read((word)(reg[regi] - 2)) == 0177564)
//                    ostat = 0377;
                if(B){
                    xx.uby = (byte)read(w_read((word)(reg[regi] - 2)), B);
                    if (t)
                        fprintf(f, "@#%o ", xx.uby);
                    return xx.uby;
                } else {
                    yy.ui = read(w_read((word) (reg[regi] - 2)), B);
                }
            } else {
                reg[regi]++;
//                if(w_read((word)(reg[regi] - 1)) == 0177564)
//                    ostat = 0377;
                xx.uby = (byte)read(w_read((word)(reg[regi] - 1)), B);
                if (t)
                    fprintf(f, "@#%o ", xx.uby);
                return xx.uby;
            }
            if (t)
                fprintf(f, "@#%o ", yy.ui);
            return yy.ui;
        case 4:
            if(t)
                fprintf(f, "-(R%o)", regi);
            if(B){
                reg[regi]--;
            } else {
                reg[regi]-= 2;
            }
            return read(reg[regi], B);
        case 5:
            if(t)
                fprintf(f, "@-(R%o)", regi);
            reg[regi]-= 2;
            return read(w_read(reg[regi]), B);
        case 6:
            if(t)
                fprintf(f, "2(R%o)", regi);
            yy.ui = (word)(reg[regi] + w_read(reg[regi]) + 2);
            reg[regi]+= 2;
            return yy.ui;
        case 7:
            yy.ui = reg[7] + w_read(reg[7]);
            return read(w_read(yy.ui), B);
        default:
            fprintf(f, "DEFAULT EXIT (take) \n");
            break;
    }
}