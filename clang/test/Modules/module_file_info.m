// UNSUPPORTED: -zos, -aix
@import DependsOnModule;

// RUN: rm -rf %t %t-obj
// RUN: %clang_cc1 -triple %itanium_abi_triple -w -Wunused -fmodules -fmodule-format=raw -fimplicit-module-maps -fdisable-module-hash -fmodules-cache-path=%t -F %S/Inputs -DBLARG -DWIBBLE=WOBBLE -fmodule-feature myfeature %s
// RUN: %clang_cc1 -triple %itanium_abi_triple -module-file-info %t/DependsOnModule.pcm | FileCheck %s
// RUN: %clang_cc1 -triple %itanium_abi_triple -module-file-info %t/DependsOnModule.pcm | FileCheck %s --check-prefix=RAW

// RUN: %clang_cc1 -triple %itanium_abi_triple -w -Wunused -fmodules -fmodule-format=obj -fimplicit-module-maps -fdisable-module-hash -fmodules-cache-path=%t-obj -F %S/Inputs -DBLARG -DWIBBLE=WOBBLE -fmodule-feature myfeature %s
// RUN: %clang_cc1 -triple %itanium_abi_triple -module-file-info %t-obj/DependsOnModule.pcm | FileCheck %s
// RUN: %clang_cc1 -triple %itanium_abi_triple -module-file-info %t-obj/DependsOnModule.pcm | FileCheck %s --check-prefix=OBJ

// RAW:   Module format: raw
// OBJ:   Module format: obj
// CHECK: Generated by this Clang:

// CHECK: Module name: DependsOnModule
// CHECK: Module map file: {{.*}}DependsOnModule.framework{{[/\\]}}module.map
// CHECK: Imports module 'Module': {{.*}}Module.pcm

// CHECK: Language options:
// CHECK:   C99: Yes
// CHECK:   Objective-C: Yes
// CHECK:   modules semantics: Yes
// CHECK:   Module features:
// CHECK:     myfeature

// CHECK: Target options:
// CHECK:     Triple:
// CHECK:     CPU:
// CHECK:     TuneCPU:
// CHECK:     ABI:

// CHECK: Header search options:
// CHECK:   System root [-isysroot=]: '/'
// CHECK:   Resource dir [ -resource-dir=]: '{{.*}}clang{{.*}}'
// CHECK:   Use builtin include directories [-nobuiltininc]: Yes
// CHECK:   Use standard system include directories [-nostdinc]: No
// CHECK:   Use standard C++ include directories [-nostdinc++]: Yes
// CHECK:   Use libc++ (rather than libstdc++) [-stdlib=]:

// CHECK: Preprocessor options:
// CHECK:   Uses compiler/target-specific predefines [-undef]: Yes
// CHECK:   Uses detailed preprocessing record (for indexing): No
// CHECK:   Predefined macros:
// CHECK:     -DBLARG
// CHECK:     -DWIBBLE=WOBBLE
// CHECK: Input file: {{.*}}DependsOnModulePrivate.h
// CHECK-NEXT: Input file: {{.*}}Other.h
// CHECK-NEXT: Input file: {{.*}}SubFramework.h
// CHECK-NEXT: Input file: {{.*}}not_coroutines.h
// CHECK-NEXT: Input file: {{.*}}not_cxx.h
// CHECK-NEXT: Input file: {{.*}}other.h
// CHECK-NEXT: Input file: {{.*}}module.map
// CHECK-NEXT: Input file: {{.*}}DependsOnModule.h
// CHECK-NEXT: Input file: {{.*}}module_private.map
// CHECK-NEXT: Input file: {{.*}}module.map

// CHECK: Diagnostic options:
// CHECK:   IgnoreWarnings: Yes
// CHECK:   Diagnostic flags:
// CHECK:     -Wunused
