v1.0:\
完成了文本编辑器的工作，理论上可以完成：\
0.默认文件名称为a.o,支持自己新建文件\
1.按正常的键：输入\
2.enter：换行\
3.backspace：删除\
3.1：修复在文件没有内容的时候按该键的影响，即不会操作到内存外（我sb了，写if忘了写else）\
4.esc退出\
5.支持显示行号，列号，但是没有每一行的行号（说白了就是我鸽了）\
注：delete键会闪退，不知道为什么\
过程中修复和经常遇到的问题：内存操作不当\
使用的危险技术：指针&链表\
V2.0:\
1:补上行号（每开启新的一行，就输出行号，删除一行，同时删除行号）
（由于需要颜色不同，故使用新的窗口）(好像只能显示99行)\
2:支持命令退出（不使用esc）(潜在bug 当你按下esc然后输入[会出现未知情况)\
3:支持光标移动之后,判断能否移动的边界（不能翻页)\
3.1：在把中间的一行删除之后，
如何把下面的东西都上移一行，并且对于文件的读写也要保持一致，这怎么做到？\
4:（代码重构2）模块化编程，分了两个源文件
---
前段小总结：技术细节：\
1：新建tmp文件作为中间文件，使用链表，便于删除\
2：失误：将arrow key视为esc+[+N，但实际上有宏定义KEY_UP
V3.0\
优化了思路结构，更加简洁与符合人类思维：只操作存储数据的数组，
然后每次刷新整个屏幕，从头重新输出。\
1：能任意插入,删除（使用二维数组保存）\
2:向右移动光标到行位或向左到行头，换行\
3:中间换行\
4:过长行bug（留给4.0吧）\
5：新建文件\