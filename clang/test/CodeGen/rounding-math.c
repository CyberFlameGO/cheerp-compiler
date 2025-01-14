// RUN: %clang_cc1 -fexperimental-strict-floating-point -S -emit-llvm -ffp-exception-behavior=strict -Wno-unknown-pragmas %s -o - | FileCheck %s
// RUN: %clang_cc1 -fexperimental-strict-floating-point -S -emit-llvm -frounding-math -Wno-unknown-pragmas %s -o - | FileCheck %s

float PR47807 = -8.6563630030e-03;

// nextUp(1.F) == 0x1.000002p0F

struct S {
  float f;
};

static struct S var_01 = {0x1.000001p0};
struct S *func_01() {
  return &var_01;
}

struct S var_02 = {0x1.000001p0};

struct S *func_03() {
  static struct S var_03 = {0x1.000001p0};
  return &var_03;
}

// CHECK: @var_01 = {{.*}} %struct._Z1S { float 1.000000e+00 }
// CHECK: @var_02 = {{.*}} %struct._Z1S { float 1.000000e+00 }
// CHECK: @func_03.var_03 = {{.*}} %struct._Z1S { float 1.000000e+00 }

#pragma STDC FENV_ROUND FE_UPWARD

static struct S var_04 = {0x1.000001p0};
struct S *func_04() {
  return &var_04;
}

struct S var_05 = {0x1.000001p0};

struct S *func_06() {
  static struct S var_06 = {0x1.000001p0};
  return &var_06;
}

// CHECK: @var_04 = {{.*}} %struct._Z1S { float 0x3FF0000020000000 }
// CHECK: @var_05 = {{.*}} %struct._Z1S { float 0x3FF0000020000000 }
// CHECK: @func_06.var_06 = {{.*}} %struct._Z1S { float 0x3FF0000020000000 }

