; RUN: llc -march=x86 < %s -O0
; REQUIRES: default_triple

define i32 @main() {
entry:
  call void @f()
  ret i32 0
}

define void @f() gc "ocaml" {
entry:
  %ptr.stackref = alloca i8*
  %gcroot = bitcast i8** %ptr.stackref to i8**
  call void @llvm.gcroot(i8** %gcroot, i8* null)
  ret void
}

declare void @llvm.gcroot(i8**, i8*) nounwind
