000000:		ostat = 177564
000000:		odata = 177566
000000:		
000000:		.=200
000200:			
000200:			
000200:		;R5 - само число
000200:		;R0 - адрес печатаемой цифры
000200:		;R4 - кол-во цифр
000200:		;R2 - для asr sob 
000200:			
000200:		CHIS: .WORD 123456
	 123456
000202:		N:	  .WORD 5 ; сколько сдвигаем цифр
	 000005
000204:		K:	  .WORD 12 ; сколько печатается
	 000012
000206:		STR:  .BLKB 10
000216:		
000216:		STREND: .BLKB 2
000220:		
000220:		.=1000
001000:		main:
001000:			mov #main, sp
	 012706
	 001000
001004:			mov CHIS, R5
	 016705
	 177170
001010:			mov #STREND, R0
	 012700
	 000216
001014:			movb #12, (R0)
	 112710
	 000012
001020:			movb #15, 1(R0)
	 112760
	 000015
	 000001
001026:			movb #12, STR
	 112767
	 000012
	 177152
001034:			movb #15, STR+1
	 112767
	 000015
	 177145
001042:			mov @#N, R4
	 013704
	 000202
001046:			mov R5, -(sp) ; положили число в стек
	 010546
001050:			
001050:		A1:
001050:			bic #177770, R5
	 042705
	 177770
001054:			add #'0, R5
	 062705
	 000060
001060:			movb R5, -(R0)
	 110540
001062:			mov (sp)+, R5
	 012605
001064:			
001064:			ash #3, R5
	 072527
	 000003
001070:		
001070:			
001070:			mov R5, -(sp)
	 010546
001072:			sob R4, A1
	 077412
001074:			movb #'0, -(R0)
	 112740
	 000060
001100:			tst (sp)+
	 005726
001102:			beq A2
	 001401
001104:			incb (R0)
	 105210
001106:			
001106:		A2:
001106:			mov #STR, -(sp)
	 012746
	 000206
001112:			mov @#K, -(sp)
	 013746
	 000204
001116:			jsr pc, puts
	 004767
	 000002
001122:			halt
	 000000
001124:			
001124:		puts:
001124:			mov R0, -(sp)
	 010046
001126:			mov R1, -(sp)
	 010146
001130:			mov R2, -(sp)
	 010246
001132:			mov 12(sp), R1
	 016601
	 000012
001136:			mov 10(sp), R2
	 016602
	 000010
001142:			
001142:		l1:
001142:			movb (R1)+, R0
	 112100
001144:			jsr pc, putchar
	 004767
	 000016
001150:			sob R2, l1
	 077204
001152:			
001152:			mov (sp)+, R2
	 012602
001154:			mov (sp)+, R1
	 012601
001156:			mov (sp)+, R0
	 012600
001160:			inc (sp)
	 005216
001162:			inc (sp)
	 005216
001164:			rts pc
	 000207
001166:		
001166:		putchar:
001166:			tstb @#ostat
	 105737
	 177564
001172:			bpl putchar
	 100375
001174:			movb R0, @#odata
	 110037
	 177566
001200:			rts pc
	 000207
