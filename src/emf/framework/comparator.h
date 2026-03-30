#ifndef EMF_FRAMEWORK_COMPARATOR_Z80_H
#define EMF_FRAMEWORK_COMPARATOR_Z80_H 1

#include "emulator/z80/common/cpu_z80.h"


#ifdef __cplusplus
extern "C" {
#endif


void emf_comparator_ctor();

int  emf_comparator_areIdentical(const Z80State* p1, const Z80State* p2);
void emf_comparator_printf(const Z80State* p);
void emf_comparator_sprintf(char* pDest, const char* pPrelude, const Z80State* p);

void emf_comparator_cpu_report_add(int op, int nextByte);
void emf_comparator_cpu_report_printf();


#ifdef __cplusplus
}
#endif

#endif // EMF_FRAMEWORK_COMPARATOR_Z80_H
