<<<<<<< HEAD
**principle : 所见即所得**

v1.0:

完成了文本编辑器的工作，理论上可以完成：

0.默认文件名称为a.o,支持自己新建文件

1.按正常的键：输入

2.enter：换行 

3.backspace：删除 

3.1：修复在文件没有内容的时候按该键的影响，即不会操作到内存外

4.esc退出 

5.支持显示行号，列号，但是没有每一行的行号（说白了就是我鸽了） 

注：delete键会闪退，不知道为什么 

过程中修复和经常遇到的问题：内存操作不当 

使用的危险技术：指针&链表 

---
V2.0: 

1:补上行号（每开启新的一行，就输出行号，删除一行，同时删除行号）
（由于需要颜色不同，故使用新的窗口）(好像只能显示99行) 

2:支持命令退出（不使用esc）(潜在bug 当你按下esc然后输入会出现未知情况) 

3:支持光标移动之后,判断能否移动的边界(不能翻页)

3.1：在把中间的一行删除之后，
如何把下面的东西都上移一行，并且对于文件的读写也要保持一致，这怎么做到？ 

4:（代码重构2）模块化编程，分了两个源文件

---

前段小总结：技术细节： 

1：新建tmp文件作为中间文件，使用链表，便于删除 

2：失误：将arrow key视为esc+[+N，但实际上有宏定义KEY_UP

---
V3.0 

优化了思路结构，更加简洁与符合人类思维：只操作存储数据的数组，

然后每次刷新整个屏幕，从头重新输出。

1：能任意插入,删除（使用二维数组保存） 

2:向右移动光标到行位或向左到行头，换行 

3:中间换行 

4:过长行bug（留给4.0吧） 

5：新建文件 

---

V4.0 

0:Multiple modes 

0.1:上下翻页，左右换行

1:过长 

2:翻页 

3:del 

4:Command line arguments. 

5:就差Word Completion和Search and Substitution 

6：发现：如果把源文件分成多个模块，没改变的模块不会重复编译，提高编译效率 

7:去除了一些很无聊的warnings 

(新增mod.h&mod.cpp) 

V4.1 

1:修整在更行后的bug 

2:光标用类 

log: 

重大发现：

vector的size类型是unsigned int 

1-0会变成很大的正数 

worklists 

0:show everything properly 

1:multiple modes 

2:move cursor in normal/insert 

3:edit in insert(including add del ' n' at end
& add del ' n' between ) 

4:shortcut key in normal,which may changes the context

5:quit save jump (**search**) find history in command 

6:arguments when start the program 

7:**word completion** 

V4.2(release) 

1：完成代码补全拿完extension 

2：改变tab（查看了共享文档的问题） 

3:完成代码美化 

---

就这样吧 

一些(特性)说明： 

1：单词补全的左右移动是F1&F2 

2:默认文件名为a.o 

3：tab默认4个空格 

4:命令行只有esc（或者退出程序）才能退出，这意味着你可以多jmp几次 

5:命令行模式不能左右移动光标（不然为什么不直接复制edit的代码呢 

---

V4.21 

修复了行号在屏幕中但光标在屏幕外的情况

---

2023.5.13 **update to github**

美化了README

文件说明：
1. words_alpha.txt是实现自动补全单词的文件，**重要**
2. src/trie（包括上面的txt文件）为大作业提供的utility，非自己手写
3. 运行环境：Ubuntu，具体编译指令及要求等见pdf
=======
ECHO is off.
hello git
>>>>>>> origin/main
