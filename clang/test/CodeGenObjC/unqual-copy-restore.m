// RUN: %clang_cc1 %s -triple %itanium_abi_triple -fobjc-arc -S -emit-llvm -o /dev/null

// rdar://problem/28488427 - Don't crash if the argument type and the parameter
// type in an indirect copy restore expression have different qualification.
@protocol P1
@end

typedef int handler(id<P1> *const p);

int main() {
  id<P1> i1 = 0;
  handler *func = 0;
  return func(&i1);
}
