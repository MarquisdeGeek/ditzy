#include <stdio.h>

#include "emf/framework/comparator.h"


static int opcodeChecker[256];
static int extChecker[256];
static int extOpcode = 0xFD;


void emf_comparator_ctor() {
    for(int i=0;i<256;++i) {
        extChecker[i] = 0;
        opcodeChecker[i] = 0;
    }
}


int emf_comparator_areIdentical(const Z80State* p1, const Z80State* p2) {
    if (
        p1->pc != p2->pc ||
        p1->sp != p2->sp ||
        p1->ix != p2->ix ||
        p1->iy != p2->iy ||

        p1->a != p2->a ||
        p1->b != p2->b ||
        p1->c != p2->c ||
        p1->d != p2->d ||
        p1->e != p2->e ||
        p1->h != p2->h ||
        p1->l != p2->l ||

        p1->prime_b != p2->prime_b ||
        p1->prime_c != p2->prime_c ||
        p1->prime_h != p2->prime_h ||
        p1->prime_l != p2->prime_l ||
    
        0
    ) {
        return 0;
    }
        
    return 1;
}


void emf_comparator_printf(const Z80State* pState) {
char str[1024];

    emf_comparator_sprintf(str, NULL, pState);

    printf("%s\n", str);
}


void emf_comparator_sprintf(char* pDest, const char* pPrelude, const Z80State* pState) {
    sprintf(pDest, "%d (%s) : a=%d b=%d c=%d d=%d e=%d h=%d l=%d ix=%d iy=%d sp=%d  [b'=%d c'=%d h'=%d l'=%d]",
            (int)pState->pc, pPrelude?pPrelude:"",
            (int)pState->a, 
            (int)pState->b, (int)pState->c, (int)pState->d, 
            (int)pState->e, (int)pState->h, (int)pState->l,     
            (int)pState->ix, 
            (int)pState->iy, (int)pState->sp,
            (int)pState->prime_b, (int)pState->prime_c,
            (int)pState->prime_h, (int)pState->prime_l
        );
}


void emf_comparator_cpu_report_add(int op, int nextByte) {
#if EMF_COMPARATOR_CPU_REPORTS

    opcodeChecker[op]++;
    if (op == extOpcode) {
        extChecker[nextByte]++;
    }
    
#endif
}

void emf_comparator_cpu_report_printf() {

    printf("Checking opcode :\n");

    for(int i=0;i<256;++i) {
        if (opcodeChecker[i]) {
            printf("%d : %d\n", i, opcodeChecker[i]);
        }
    }


    printf("Checking opcode ext : %x\n", extOpcode);

    for(int i=0;i<256;++i) {
        if (extChecker[i]) {
            printf("%d : %d\n", i, extChecker[i]);
        }
    }

    printf("Complete list:\n");
    for(int i=0;i<256;++i) {
        printf("%d : %d\n", i, extChecker[i]);
    }
}
