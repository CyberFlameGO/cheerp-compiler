// RUN: %clang_cc1 -triple %itanium_abi_triple %s -emit-llvm -o - | FileCheck %s

struct V { short X, Y; };
int bar() {
  struct V bar;
  // CHECK: call void asm
  __asm__ volatile("foo %0\n" :: "r"(bar));
  return bar.X;
}
