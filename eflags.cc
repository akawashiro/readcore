#include "eflags.h"

#include <cstdint>

#define __AC(X, Y) (X##Y)
#define _AC(X, Y) __AC(X, Y)
#define _UL(x) (_AC(x, UL))
#define _BITUL(x) (_UL(1) << (x))
#define X86_EFLAGS_CF_BIT 0 /* Carry Flag */
#define X86_EFLAGS_CF _BITUL(X86_EFLAGS_CF_BIT)
#define X86_EFLAGS_FIXED_BIT 1 /* Bit 1 - always on */
#define X86_EFLAGS_FIXED _BITUL(X86_EFLAGS_FIXED_BIT)
#define X86_EFLAGS_PF_BIT 2 /* Parity Flag */
#define X86_EFLAGS_PF _BITUL(X86_EFLAGS_PF_BIT)
#define X86_EFLAGS_AF_BIT 4 /* Auxiliary carry Flag */
#define X86_EFLAGS_AF _BITUL(X86_EFLAGS_AF_BIT)
#define X86_EFLAGS_ZF_BIT 6 /* Zero Flag */
#define X86_EFLAGS_ZF _BITUL(X86_EFLAGS_ZF_BIT)
#define X86_EFLAGS_SF_BIT 7 /* Sign Flag */
#define X86_EFLAGS_SF _BITUL(X86_EFLAGS_SF_BIT)
#define X86_EFLAGS_TF_BIT 8 /* Trap Flag */
#define X86_EFLAGS_TF _BITUL(X86_EFLAGS_TF_BIT)
#define X86_EFLAGS_IF_BIT 9 /* Interrupt Flag */
#define X86_EFLAGS_IF _BITUL(X86_EFLAGS_IF_BIT)
#define X86_EFLAGS_DF_BIT 10 /* Direction Flag */
#define X86_EFLAGS_DF _BITUL(X86_EFLAGS_DF_BIT)
#define X86_EFLAGS_OF_BIT 11 /* Overflow Flag */
#define X86_EFLAGS_OF _BITUL(X86_EFLAGS_OF_BIT)
#define X86_EFLAGS_IOPL_BIT 12 /* I/O Privilege Level (2 bits) */
#define X86_EFLAGS_IOPL (_AC(3, UL) << X86_EFLAGS_IOPL_BIT)
#define X86_EFLAGS_NT_BIT 14 /* Nested Task */
#define X86_EFLAGS_NT _BITUL(X86_EFLAGS_NT_BIT)
#define X86_EFLAGS_RF_BIT 16 /* Resume Flag */
#define X86_EFLAGS_RF _BITUL(X86_EFLAGS_RF_BIT)
#define X86_EFLAGS_VM_BIT 17 /* Virtual Mode */
#define X86_EFLAGS_VM _BITUL(X86_EFLAGS_VM_BIT)
#define X86_EFLAGS_AC_BIT 18 /* Alignment Check/Access Control */
#define X86_EFLAGS_AC _BITUL(X86_EFLAGS_AC_BIT)
#define X86_EFLAGS_VIF_BIT 19 /* Virtual Interrupt Flag */
#define X86_EFLAGS_VIF _BITUL(X86_EFLAGS_VIF_BIT)
#define X86_EFLAGS_VIP_BIT 20 /* Virtual Interrupt Pending */
#define X86_EFLAGS_VIP _BITUL(X86_EFLAGS_VIP_BIT)
#define X86_EFLAGS_ID_BIT 21 /* CPUID detection */
#define X86_EFLAGS_ID _BITUL(X86_EFLAGS_ID_BIT)

std::vector<std::string> X86EFlagsToStrings(uint64_t eflags) {
  std::vector<std::string> flags;
  if (eflags & X86_EFLAGS_CF) {
    flags.push_back("Carry");
  }
  if (eflags & X86_EFLAGS_FIXED) {
    flags.push_back("Fixed");
  }
  if (eflags & X86_EFLAGS_PF) {
    flags.push_back("Parity");
  }
  if (eflags & X86_EFLAGS_AF) {
    flags.push_back("Auxiliary carry");
  }
  if (eflags & X86_EFLAGS_ZF) {
    flags.push_back("Zero");
  }
  if (eflags & X86_EFLAGS_SF) {
    flags.push_back("Sign");
  }
  if (eflags & X86_EFLAGS_TF) {
    flags.push_back("Trap");
  }
  if (eflags & X86_EFLAGS_IF) {
    flags.push_back("Interrupt");
  }
  if (eflags & X86_EFLAGS_DF) {
    flags.push_back("Direction");
  }
  if (eflags & X86_EFLAGS_OF) {
    flags.push_back("Overflow");
  }
  if (eflags & X86_EFLAGS_IOPL) {
    flags.push_back("I/O Privilege Level");
  }
  if (eflags & X86_EFLAGS_NT) {
    flags.push_back("Nested Task");
  }
  if (eflags & X86_EFLAGS_RF) {
    flags.push_back("Resume");
  }
  if (eflags & X86_EFLAGS_VM) {
    flags.push_back("Virtual Mode");
  }
  if (eflags & X86_EFLAGS_AC) {
    flags.push_back("Alignment Check");
  }
  if (eflags & X86_EFLAGS_VIF) {
    flags.push_back("Virtual Interrupt");
  }
  if (eflags & X86_EFLAGS_VIP) {
    flags.push_back("Virtual Interrupt Pending");
  }
  if (eflags & X86_EFLAGS_ID) {
    flags.push_back("CPUID detection");
  }
  return flags;
}
