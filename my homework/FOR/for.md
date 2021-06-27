# 📝系統程式作業 <擴充for的功能>
* 使用老師給的sp/03-compiler/03b-compiler2的compiler.c修改for
* 有更新原本的[Makefile](Makefile)檔，可以直接make

### 修改後的[Makefile](Makefile)
```
CC := gcc
CFLAGS = -std=c99 -O0
TARGET = compiler1 compiler2

all: $(TARGET)

compiler1: lexer.c compiler1.c main.c
	$(CC) $(CFLAGS) $^ -o $@
compiler2: lexer.c compiler2.c main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o *.exe
```

### 修改的for部分 1.0
```
// F = (E) | Number | Id
int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    if(isdigit(*item)){               //判斷數字
      emit("t%d = %s\n", f, item);
    }else{
      if(isNext("++")){                //判斷++
        next();
        emit("%s = %s + 1\n", item, item);
      }
      if(isNext("--")){               //判斷--
        next();
        emit("%s = %s - 1\n", item,item);
      }
      emit("t%d = %s\n", f, item);
    }
  }
  return f;
}
```

```
void FOR() {
  int forBegin = nextLabel();
  int forMid = nextLabel();
  int forEnd = nextLabel();
  emit("(L%d)\n", forBegin);
  skip("for"); //跳過for
  skip("(");
  ASSIGN(); //for裡面判斷式用的變數定義
  int e2 = E();  //判斷式
  emit("if not T%d goto L%d\n", e2, forMid); //判斷不成立，跳出for迴圈
  skip(";");
  int e3 = E(); //變數的累加
  skip(")");
  STMT();
  emit("goto L%d\n", forBegin);
  emit("L%d", forEnd);
}
```

### for.c
```
for(i=0; i<6; i++){
    a = a + 1;
}
```

### [compiler1.c code link](compiler1.c)
<details>
  <summary><b>Show compiler1.c</b></summary>

  ```
#include <assert.h>
#include "compiler.h"

int E();
void STMT();
void IF();
void BLOCK();

int tempIdx = 0, labelIdx = 0;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)
#define emit printf

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

int isEnd() {
  return tokenIdx >= tokenTop;
}

char *next() {
  // printf("token[%d]=%s\n", tokenIdx, tokens[tokenIdx]);
  return tokens[tokenIdx++];
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    printf("skip(%s) got %s fail!\n", set, next());
    assert(0);
  }
}

// F = (E) | Number | Id
int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    if(isdigit(*item)){               //判斷數字
      emit("t%d = %s\n", f, item);
    }else{
      if(isNext("++")){                //判斷++
        next();
        emit("%s = %s + 1\n", item, item);
      }
      if(isNext("--")){               //判斷--
        next();
        emit("%s = %s - 1\n", item,item);
      }
      emit("t%d = %s\n", f, item);
    }
  }
  return f;
}

// E = F (op E)*
int E() {
  int i1 = F();
  while (isNext("+ - * / & | ! < > = <= >= == !=")) {
    char *op = next();
    int i2 = E();
    int i = nextTemp();
    emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

// ASSIGN = id '=' E;  //分配
void ASSIGN() {
  char *id = next();
  skip("=");
  int e = E();
  skip(";");
  emit("%s = t%d\n", id, e);
}

// WHILE = while (E) STMT
void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, whileEnd);
  skip(")");
  STMT();
  emit("goto L%d\n", whileBegin);
  emit("(L%d)\n", whileEnd);
}

// if (EXP) STMT (else STMT)?
void IF() {
  int ifBegin = nextLabel();
  int ifMid = nextLabel();
  int ifEnd = nextLabel();
  emit("(L%d)\n", ifBegin);
  skip("if");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, ifMid); //經if判斷不成立，到下一個條件式
  skip(")");
  STMT();
  emit("goto L%d\n", ifEnd);  //條件成立，結束
  emit("(L%d)\n", ifMid);  //第二個條件式
  if (isNext("else")) {
    skip("else");
    STMT();
    emit("goto L%d\n", ifEnd);  //條件成立，結束
  }
}

void FOR() {
  int forBegin = nextLabel();
  int forMid = nextLabel();
  int forEnd = nextLabel();
  emit("(L%d)\n", forBegin);
  skip("for"); //跳過for
  skip("(");
  ASSIGN(); //for裡面判斷式用的變數定義
  int e2 = E();  //判斷式
  emit("if not T%d goto L%d\n", e2, forMid); //判斷不成立，跳出for迴圈
  skip(";");
  int e3 = E(); //變數的累加
  skip(")");
  STMT();
  emit("goto L%d\n", forBegin);
  emit("L%d", forEnd);
}

// STMT = WHILE | BLOCK | ASSIGN
void STMT() {
  if (isNext("while"))
    return WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("for"))
    FOR();
  else if (isNext("{"))
    BLOCK();
  else
    ASSIGN();
}

// STMTS = STMT*
void STMTS() {
  while (!isEnd() && !isNext("}")) {
    STMT();
  }
}

// BLOCK = { STMTS }
void BLOCK() {
  skip("{");
  STMTS();
  skip("}");
}

// PROG = STMTS
void PROG() {
  STMTS();
}

void parse() {
  printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}
  ```
</details>

### [lexer.c code link](lexer.c)
<details>
  <summary><b>Show lexer.c</b></summary>

  ```
#include "compiler.h"

#define TMAX 10000000
#define LMAX 100

char *typeName[6] = {"Id", "Int", "Keyword", "Literal", "Char", "Op"};
char code[TMAX], *p;
char strTable[TMAX], *strTableEnd=strTable;
char *tokens[TMAX], tokenTop=0, tokenIdx=0, token[LMAX];

char *scan() {
  while (isspace(*p)) p++;

  char *start = p;
  int type;
  if (*p == '\0') return NULL;
  if (*p == '"') {
    p++;
    while (*p != '"') p++;
    p++;
    type = Literal;
  } else if (*p >='0' && *p <='9') { // 數字
    while (*p >='0' && *p <='9') p++;
    type = Int;
  } else if (isAlpha(*p) || *p == '_') { // 變數名稱或關鍵字
    while (isAlpha(*p) || isDigit(*p) || *p == '_') p++;
    type = Id;
  } else if (strchr("+-*/%%&|<>!=", *p) >= 0) {
    char c = *p++;
    if (*p == '=') p++; // +=, ==, <=, !=, ....
    else if (strchr("+-&|", c) >= 0 && *p == c) p++; // ++, --, &&, ||
    type = Op;
  } else { // 單一字元
    p++;
    type = Char;
  }
  int len = p-start;
  strncpy(token, start, len);
  token[len] = '\0';
  return token;
}

void lex(char *code) {
  printf("========== lex ==============\n");
  p = code;
  tokenTop = 0;
  while (1) {
    char *tok = scan();
    if (tok == NULL) break;
    strcpy(strTableEnd, tok);
    tokens[tokenTop++] = strTableEnd;
    strTableEnd += (strlen(tok)+1);
    printf("token=%s\n", tok);
  }
}
  ```
</details>

### [main.c code link](main.c)
<details>
  <summary><b>Show main.c</b></summary>

  ```
#include "compiler.h"

int readText(char *fileName, char *text, int size) {
  FILE *file = fopen(fileName, "r");
  int len = fread(text, 1, size, file);
  text[len] = '\0';
  fclose(file);
  return len;
}

void dump(char *strTable[], int top) {
  printf("========== dump ==============\n");
  for (int i=0; i<top; i++) {
    printf("%d:%s\n", i, strTable[i]);
  }
}

int main(int argc, char * argv[]) {
  readText(argv[1], code, TMAX);
  puts(code);
  lex(code);
  dump(tokens, tokenTop);
  parse();
  return 0;
}
  ```
</details>

### The result of execution
```
yichien@MSI MINGW64 /d/VScode/WP/ccc/sp109b/my homework/FOR (main)
$ gcc compiler.c main.c lexer.c -o compiler

yichien@MSI MINGW64 /d/VScode/WP/ccc/sp109b/my homework/FOR (main)
$ ./compiler for.c
for(i=0; i<6; i++){
    a = a + 1;
}
========== lex ==============
token=for
token=(
token=i
token==
token=0
token=;
token=i
token=<
token=6
token=;
token=i
token=++
token=)
token={
token=a
token==
token=a
token=+
token=1
token=;
token=}
========== dump ==============
0:for
1:(
2:i
3:=
4:0
5:;
6:i
7:<
8:6
9:;
10:i
11:++
12:)
13:{
14:a
15:=
16:a
17:+
18:1
19:;
20:}
============ parse =============
(L0)
t0 = 0
i = t0
t1 = i
t2 = 6
t3 = t1 < t2
if not T3 goto L1
i = i + 1
t4 = i
t5 = a
t6 = 1
t7 = t5 + t6
a = t7
goto L0
L2
```
### 修改的for部分 2.0
```
// #define emit printf
int isTempIr = 0;
char tempIr[100000], *tempIrp = tempIr;
#define emit(...) ({ \
  if (isTempIr){ /*判斷isTempIr值是否為1*/ \
    sprintf(tempIrp, __VA_ARGS__); /*不會印出來，並將其轉成指標陣列tempIrp存起來*/ \
    tempIrp += strlen(tempIrp);\
  }\
  else { \
    printf(__VA_ARGS__); /*若值為0，則將其印出*/ \
  }\
})
```

```
void FOR() {
  int forBegin = nextLabel();
  int forMid = nextLabel();
  int forEnd = nextLabel();
  emit("(L%d)\n", forBegin);
  skip("for");   //跳過for
  skip("(");
  ASSIGN();   //for裡面判斷式用的變數定義
  int e2 = E();   //判斷式
  emit("if not T%d goto L%d\n", e2, forEnd); //判斷不成立，跳出for迴圈
  skip(";");
  isTempIr = 1;   //先給isTempIr = 1;
  int e3 = E();   //先將i++的部分暫存
  isTempIr = 0;   //這邊isTempIr = 0;下一次呼叫函數時就會印出來剛剛儲存的
  char e3str[10000];
  strcpy(e3str, tempIr);   //複製tempIr給e3str
  skip(")");
  STMT();
  emit("%s\n", e3str);    //印出e1str(i++)
  emit("goto L%d\n", forBegin);
  emit("L%d", forEnd);
}
```

### The result of execution
```
yichien@MSI MINGW64 /d/VScode/WP/ccc/sp109b/my homework/FOR (main)
$ gcc compiler2.c main.c lexer.c -o compiler2

yichien@MSI MINGW64 /d/VScode/WP/ccc/sp109b/my homework/FOR (main)
$ ./compiler2 for.c
for(i=0; i<6; i++){
    a = a + 1;
}
========== lex ==============
token=for
token=(
token=i
token==
token=0
token=;
token=i
token=<
token=6
token=;
token=i
token=++
token=)
token={
token=a
token==
token=a
token=+
token=1
token=;
token=}
========== dump ==============
0:for
1:(
2:i
3:=
4:0
5:;
6:i
7:<
8:6
9:;
10:i
11:++
12:)
13:{
14:a
15:=
16:a
17:+
18:1
19:;
20:}
============ parse =============
(L0)
t0 = 0
i = t0
t1 = i
t2 = 6
t3 = t1 < t2
if not T3 goto L2
t5 = a
t6 = 1
t7 = t5 + t6
a = t7
i = i + 1
t4 = i

goto L0
L2
```
### [compiler2.c code link](compiler2.c)
<details>
  <summary><b>Show compiler2.c</b></summary>

  ```
#include <assert.h>
#include "compiler.h"

int E();
void STMT();
void IF();
void BLOCK();

int tempIdx = 0, labelIdx = 0;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)
// #define emit printf
int isTempIr = 0;
char tempIr[100000], *tempIrp = tempIr;
#define emit(...) ({ \
  if (isTempIr){ /*判斷isTempIr值是否為1*/ \
    sprintf(tempIrp, __VA_ARGS__); /*不會印出來，並將其轉成指標陣列tempIrp存起來*/ \
    tempIrp += strlen(tempIrp);\
  }\
  else { \
    printf(__VA_ARGS__); /*若值為0，則將其印出*/ \
  }\
})
int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

int isEnd() {
  return tokenIdx >= tokenTop;
}

char *next() {
  // printf("token[%d]=%s\n", tokenIdx, tokens[tokenIdx]);
  return tokens[tokenIdx++];
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    printf("skip(%s) got %s fail!\n", set, next());
    assert(0);
  }
}

// F = (E) | Number | Id
int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    if(isdigit(*item)){               //判斷數字
      emit("t%d = %s\n", f, item);
    }else{
      if(isNext("++")){                //判斷++
        next();
        emit("%s = %s + 1\n", item, item);
      }
      if(isNext("--")){               //判斷--
        next();
        emit("%s = %s - 1\n", item,item);
      }
      emit("t%d = %s\n", f, item);
    }
  }
  return f;
}

// E = F (op E)*
int E() {
  int i1 = F();
  while (isNext("+ - * / & | ! < > = <= >= == !=")) {
    char *op = next();
    int i2 = E();
    int i = nextTemp();
    emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

// ASSIGN = id '=' E;  //分配
void ASSIGN() {
  char *id = next();
  skip("=");
  int e = E();
  skip(";");
  emit("%s = t%d\n", id, e);
}

// WHILE = while (E) STMT
void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, whileEnd);
  skip(")");
  STMT();
  emit("goto L%d\n", whileBegin);
  emit("(L%d)\n", whileEnd);
}

// if (EXP) STMT (else STMT)?
void IF() {
  int ifBegin = nextLabel();
  int ifMid = nextLabel();
  int ifEnd = nextLabel();
  emit("(L%d)\n", ifBegin);
  skip("if");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, ifMid); //經if判斷不成立，到下一個條件式
  skip(")");
  STMT();
  emit("goto L%d\n", ifEnd);  //條件成立，結束
  emit("(L%d)\n", ifMid);  //第二個條件式
  if (isNext("else")) {
    skip("else");
    STMT();
    emit("goto L%d\n", ifEnd);  //條件成立，結束
  }
}

void FOR() {
  int forBegin = nextLabel();
  int forMid = nextLabel();
  int forEnd = nextLabel();
  emit("(L%d)\n", forBegin);
  skip("for");   //跳過for
  skip("(");
  ASSIGN();   //for裡面判斷式用的變數定義
  int e2 = E();   //判斷式
  emit("if not T%d goto L%d\n", e2, forEnd); //判斷不成立，跳出for迴圈
  skip(";");
  isTempIr = 1;   //先給isTempIr = 1;
  int e3 = E();   //先將i++的部分暫存
  isTempIr = 0;   //這邊isTempIr = 0;下一次呼叫函數時就會印出來剛剛儲存的
  char e3str[10000];
  strcpy(e3str, tempIr);   //複製tempIr給e3str
  skip(")");
  STMT();
  emit("%s\n", e3str);    //印出e1str(i++)
  emit("goto L%d\n", forBegin);
  emit("L%d", forEnd);
}

// STMT = WHILE | BLOCK | ASSIGN
void STMT() {
  if (isNext("while"))
    return WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("for"))
    FOR();
  else if (isNext("{"))
    BLOCK();
  else
    ASSIGN();
}

// STMTS = STMT*
void STMTS() {
  while (!isEnd() && !isNext("}")) {
    STMT();
  }
}

// BLOCK = { STMTS }
void BLOCK() {
  skip("{");
  STMTS();
  skip("}");
}

// PROG = STMTS
void PROG() {
  STMTS();
}

void parse() {
  printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}
  ```
</details>