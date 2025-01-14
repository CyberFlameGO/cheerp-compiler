; RUN: llc < %s
; PR7170

target triple = "x86_64-unknown-linux"

%big = type [131072 x i8]

declare void @foo(%big* byval(%big) align 1)

define void @bar(%big* byval(%big) align 1 %x) {
  call void @foo(%big* byval(%big) align 1 %x)
  ret void
}
