

/*************************************************************

This file is part of messaging-cells.

messaging-cells is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

messaging-cells is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with messaging-cells.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2017-2018. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://messaging-cells.github.io/

messaging-cells is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------*/


#include <execinfo.h>
#include <stdio.h>

#define TRACE_SZ 100
void* TRACE[TRACE_SZ];


//extern int get_fp(void);
/*
int get_fp(void){
	return 0;
}

void 
print_symbol(void *addr){
    void *addresslist[2] = {addr, 0};
    char **symbollist = backtrace_symbols(addrlist, 1);
    // symbol name string is at symbollist[0]. print it. 
}

void 
print_backtrace(void){
    int topfp = get_fp();
    for (int i=0; i < 10; i++) {
        int fp = *(((int*)topfp) -3);
        int sp = *(((int*)topfp) -2);
        int lr = *(((int*)topfp) -1);
        int pc = *(((int*)topfp) -0);
        if ( i==0 ) print_symbol( (void*) pc); // top frame
        if (fp != 0) print_symbol( (void*) lr); // middle frame
        else print_symbol( (void*)pc); // bottom frame, lr invalid
        topfp = fp;
    }
}
*/

void func_1(int aa){
	printf("func_1. aa=%d\n", aa);

	int num = backtrace(TRACE, TRACE_SZ);
	printf("backtrace (sz=%d)= [\n", num);
	for(int aa = 0; aa < num; aa++){
		printf("\t%p\n", TRACE[aa]);
	}
	printf("]\n");
}

void func_2(char bb, int aa){
	printf("func_2. bb=%c aa=%d\n", bb, aa);
	func_1(aa + 5);
}

void func_3(char* cc, int aa){
	printf("func_3. bb=%s aa=%d\n", cc, aa);
	func_2('x', aa * 7);
}

int main(int argc, char* argv[]){
	printf("backtrace TEST\n");
	func_3("backtrace test", 345);
	return 0;
}

