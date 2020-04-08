C语言数独解决方案验证器（Windows和Linux下分别实现）
==============
Multi-threaded Sudoku Solver written in C. (Implemented within Windows / inux)
--------------

数独谜题采用 9x9 网格，其中每行、每列以及每 3x3 个子网格都要包括数字 0-9。设计一个多线程应用程序，以判定数独谜题的解决是否有效。方案：通过创建线程，检查如下条件：

- 一个线程用于检查每列包含数字 1-9；
- 一个线程用于检查每行包含数字 1-9；
- 9 个线程，用于检查每 3x3 个子网格包含数字 1-9。

总共创建 11 个单独的线程，以验证数独谜题。

解法 / The solution is as follows:
--------------

1. Create 1 thread to check if all rows contain all numbers from 1-9.
2. Create 1 thread to check if all columns contain all numbers from 1-9.
3. Create 9 threads to check if all 3x3 squares formed inside the board contain all numbers from 1-9.

编译命令 / Compilation instructions
--------------
Please use the following command to compile the source code (future versions will include the Makefile):

    gcc main.c -lpthread
	
数独表输入文件 / Sudoku Input
--------------
在sudoku_board文件中指定 9*9 的数独表，程序运行时将从中读取。

格式：共 9 行，每行 9 个数字（1-9），其中每两个数字之间空一格，每行最后一个数字后无空格

**换行采用Unix标准（LF）**
