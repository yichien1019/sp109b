# 📝系統程式第三週筆記20210310
## 📖 compiler 
> 1.bnf ebnf 生成語法<br>
> 2.運算是 編譯器<br>
> 3.lexer 讓字串變成token<br> 
> 4.exp/if/while<br>

## 📖 openfile模式
模式 | 內容
------ | -----
r | 讀取(檔案需存在)
w | 新建檔案寫入(檔案可不存在，若存在則清空)
a | 附加到文件中。寫入操作的數據追加在文件末尾的。該文件如果它不存在會被創建
r+ | 讀取舊資料並寫入(檔案需存在且游標指在開頭)
w+ | 清空檔案內容，新寫入的東西可在讀出(檔案可不存在，會自行新增)
a+ | 資料附加到舊檔案後面(游標指在EOF)，可讀取資料
b | 二進位模式
rw+ | 可讀取可寫入 若已存在就直接寫入 沒有就開新的檔案

## 📖 指標(指向記憶體位置)


code  strtable 差別 第二個=個有/0當結尾

tokens  字元指標
指標 指向記憶體位置
char *p= &x; *表示字元指標 也可寫作char* p 但是榮一誤解 char *p,r; *q 為字元指標 r為字元  ;&x x的位置

enum 列舉
## 📖 gcc disable warning

## 💻 程式實際操作
### Lexer(詞彙解析) 
#### Code(加註解)
[lexer.c](./lexer.c)

#### The result of execution
```
yichien@MSI MINGW64 /d/VScode/WP/ccc/109b/sp109b/sp/03-compiler/02-lexer (master)
$ ./lexer sum.c
#include "sum.h"

int main() {
  int t = sum(10);
  printf("sum(10)=%d\n", t);
}
token=#
token=include
token="sum.h"
token=int
token=main
token=(
token=)
token={
token=int
token=t
token==
token=sum
token=(
token=10
token=)
token=;
token=printf
token=(
token="sum(10)=%d\n"
token=,
token=t
token=)
token=;
token=}
0:#
1:include
2:"sum.h"
3:int
4:main
5:(
6:)
7:{
8:int
9:t
10:=
11:sum
12:(
13:10
14:)
15:;
16:printf
17:(
18:"sum(10)=%d\n"
19:,
20:t
21:)
22:;
23:}
```
## 📖 補充資料
* [RISC-V](https://zh.wikipedia.org/wiki/RISC-V)
* [QEMU](https://www.qemu.org/)