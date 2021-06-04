# 📝系統程式第五週筆記20210324
## 📖 虛擬機
### 🔖虛擬機 V.S. 模擬器
* 模擬器 : 模擬電腦行為的軟體
* 虛擬機 : 模擬處理器指令級的軟體
![](pic/虛擬機.jpg)

### 🔖虛擬機架構
* 記憶體機 : 可以直接對記憶體變數進行運算
* 暫存器機 : 必須將變數載入暫存器中，才能進行運算
* 堆疊機 : 取出堆疊上層元素進行運算，結果存回堆疊中
![](pic/虛擬機架構.jpg)
![](pic/虛擬機組合語言.jpg)

## 📖 [C4編譯器](https://hackmd.io/@srhuang/Bkk2eY5ES)的語法
### 🔖簡介
* C4 是[Robert Swierczek](https://github.com/rswier/) 寫的一個小型 C 語言編譯器，全部 527 行的原始碼都在 [c4.c](https://github.com/ccc-c/c4/blob/master/c4.c) 裡 
* C4 編譯完成後，會產生一種《堆疊機機器碼》放在記憶體內，然後 虛擬機 會立刻執行該機器碼

### 🔖Code
* [原始碼](https://github.com/rswier/c4)
* [含註解的原始碼](https://github.com/comzyh/c4/blob/comment/c4.c)

### 🔖支援的語法
項目 | 語法
-----|-------------------
判斷 | if ... else
迴圈 | while (...)
區塊 | {...}
函數呼叫 | f()
函數定義 | int f(....)
傳回值 | return 
陣列存取 | a[i] 
數學運算 | +-*/%, ++, --, +=, -=, *=, /=, %=
位元運算 | &|^~
邏輯運算 |  ! && || 
列舉 | enum ...
運算式 | (a*3+5)/b 
指定 | x = (a*3+5)/b
取得大小 | sizeof
強制轉型 | (int*) ptr; (char) a;
基本型態 | int, char
指標 | *ptr 
遞迴 | int f(n) { ... return f(n-1) + f(n-2); }
陣列存取 | a[i]

## 💻 程式實際操作
### 🔗 06-vm/01-jvm/HelloWorld
![](pic/HelloWorld.JPG)

#### The result of execution
```
ubuntu@primary:~/sp/06-vm/01-jvm$ javac HelloWorld.java
ubuntu@primary:~/sp/06-vm/01-jvm$ ls
HelloWorld.class  HelloWorld.java  Java的JVM虛擬機.md  README.md
ubuntu@primary:~/sp/06-vm/01-jvm$ java HelloWorld
Hello World!
```
### 🔗 06-vm/02-pitifulvm/src/tests/Factorial
![](pic/Factorial.JPG)

#### The result of execution
```
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src$ javac tests/Factorial.java
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src$ ls tests/Factorial.*
tests/Factorial.class  tests/Factorial.java
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src$ cd tests
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src/tests$ java Factorial
43954714
```
### 🔗 06-vm/02-pitifulvm/src/tests/Primes
![](pic/Primes.JPG)
#### The result of execution
```
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src$ make
  CC+LD         jvm
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src$ ls
LICENSE  Makefile  README.md  jvm  jvm.c  mk  tests
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src$ cd tests
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src/tests$ javac Primes.java
ubuntu@primary:~/sp/06-vm/02-pitifulvm/src/tests$ java Primes
```

## 📖 補充資料
* [JVM](https://www.guru99.com/java-virtual-machine-jvm.html?fbclid=IwAR1hnhkJMrl2wCRRq-CBoeTsISNvNSFwagEhaJ3cRCOcD2l6Hq29apIltWw)
* [Java virtual machine](https://en.wikipedia.org/wiki/Java_virtual_machine?fbclid=IwAR0U8LW8QqhLhx3ErFI6-Gfokdgd-h7oYdz3O633ocfDATqyarZnmT1bSWU)
* [Fabrice Bellard](https://en.wikipedia.org/wiki/Fabrice_Bellard?fbclid=IwAR2b0O2ayX2rr4jH0TDDqx6XO6uF3WpHXv3J41LGr2zBkORgUiAO6AV4xC8)
* [QEMU](https://zh.wikipedia.org/wiki/QEMU)


🖊️ editor : yi-chien Liu