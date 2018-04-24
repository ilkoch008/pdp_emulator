//
// Created by Ilya on 09.04.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "variables.h"

vozvr take(word ad_mode, word regi, word B) {
    vozvr res;
    switch (ad_mode) {
        case 0:
            if (t) {
                fprintf(f, "R%o ", regi);
                if(std)
                    printf("R%o ", regi);
            }
            res.adr = regi;
            res.val = reg[regi];
            return res;
        case 1:
            if (t) {
                fprintf(f, "@R%o ", regi);
                if(std)
                    printf("@R%o ", regi);
            }
            res.adr = reg[regi];
            res.val = read(reg[regi], B);
            return res;
        case 2:
            if (regi == 7 || B == 0) {
                if (t && regi == 7){
                    fprintf(f, "#%o ", w_read(reg[regi]));
                    if(std)
                        printf("#%o ", w_read(reg[regi]));
                }
                else if(t){
                    fprintf(f, "(R%o)+ ", regi);
                    if(std)
                        printf("(R%o)+ ", regi);
                }
                res.adr = reg[regi];
                res.val = read(res.adr, B);
                reg[regi] += 2;
                return res;
            } else {
                if (t){
                    fprintf(f, "(R%o)+ ", regi);
                    if(std)
                        printf("(R%o)+ ", regi);
                }
                res.adr = reg[regi];
                res.val = read(res.adr, B);
                reg[regi]++;
                return res;
            }
        case 3:
            res.adr = w_read(reg[regi]);
            if (regi == 7 || B == 0) {
                reg[regi] += 2;
                    res.val = (byte)read(w_read((word)(reg[regi] - 2)), B);
                    if (t){
                        fprintf(f, "@#%o ", res.val);
                        if(std)
                            printf("@#%o ", res.val);
                    }
                    return res;
            } else {
                reg[regi]++;
                res.val = (byte)read(w_read((word)(reg[regi] - 1)), B);
                if (t){
                    fprintf(f, "@#%o ", res.val);
                    if(std)
                        printf("@#%o ", res.val);
                }
                return res;
            }
        case 4:
            if(t){
                fprintf(f, "-(R%o) ", regi);
                if(std)
                    printf("-(R%o) ", regi);
            }
            if(B){
                reg[regi]--;
            } else {
                reg[regi]-= 2;
            }
            res.adr = reg[regi];
            res.val = read(res.adr, B);
            return res;
        case 5:
            if(t){
                fprintf(f, "@-(R%o) ", regi);
                if(std){
                    printf("@-(R%o) ", regi);
                }
            }
            reg[regi]-= 2;
            res.adr = w_read(reg[regi]);
            res.val = read(res.adr, B);
            return res;
        case 6:
            if(t) {
                yy.ui = w_read(reg[7]);
                fprintf(f, "%d(R%o) ", yy.si, regi);
                if(std)
                    printf("%d(R%o) ", yy.si, regi);
            }
            if(regi == 7) {
                res.adr = (word) (reg[regi] + w_read(reg[7]) + 2);
                res.val = read(res.adr, B);
                reg[7] += 2;
            } else {
                res.adr = reg[regi] + w_read(reg[7]);
                res.val = read(res.adr, B);
                reg[7]+= 2;
            }
            return res;
        case 7:
            yy.ui = reg[7] + w_read(reg[7]);
            res.adr = w_read(yy.ui);
            res.val = read(res.adr, B);
            return res;
        default:
            fprintf(f, "DEFAULT EXIT (take) \n");
            printf("DEFAULT EXIT (take) \n");
            break;
    }
}