# 2025-01-02

jit的版本的运行速度，终于比 normal 的 c++ 代码快了...

这里模拟的就是一个复杂的浮点数计算，要控制循环次数N，如果 N 再 *10，jit 就没优势了，也不知道为啥

llvm-config --version
19.1.1
llvm-config --includedir
/usr/lib/llvm-19/include