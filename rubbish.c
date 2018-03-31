//
// Created by Ilya on 31.03.2018.
//

//typedef union ob {
//	word aa;
//	byte a[2];
//} ob;


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