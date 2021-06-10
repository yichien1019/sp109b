# 📝系統程式作業 <擴充if的功能>
* 使用老師給的sp/03-compiler/03b-compiler2的compiler.c修改if

### 修改的if部分
```
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
```

### if.c
```
if (x>5)
{
    t=1;
}
else if (x<=0){
    t=2;
}else{
    t=3;
}
```

### [compiler.c code link](compiler.c)
<details>
  <summary><b>Show compiler.c</b></summary>

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
    emit("t%d = %s\n", f, item);
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

// ASSIGN = id '=' E;
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


// STMT = WHILE | BLOCK | ASSIGN
void STMT() {
  if (isNext("while"))
    return WHILE();
  else if (isNext("if"))
    IF();
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
yichien@MSI MINGW64 /d/VScode/WP/ccc/sp109b/my homework/IF (main)
$ gcc compiler.c main.c lexer.c -o compiler

yichien@MSI MINGW64 /d/VScode/WP/ccc/sp109b/my homework/IF (main)
$ ./compiler if.c
if (x>5)       
{
    t=1;       
}
else if (x<=0){
    t=2;       
}else{
    t=3;       
}
========== lex ==============
token=if
token=(
token=x
token=>
token=5
token=)
token={
token=t
token==
token=1
token=;
token=}
token=else
token=if
token=(
token=x
token=<=
token=0
token=)
token={
token=t
token==
token=2
token=;
token=}
token=else
token={
token=t
token==
token=3
token=;
token=}
========== dump ==============
0:if
1:(
2:x
3:>
4:5
5:)
6:{
7:t
8:=
9:1
10:;
11:}
12:else
13:if
14:(
15:x
16:<=
17:0
18:)
19:{
20:t
21:=
22:2
23:;
24:}
25:else
26:{
27:t
28:=
29:3
30:;
31:}
============ parse =============
(L0)
t0 = x
t1 = 5
t2 = t0 > t1
if not T2 goto L1
t3 = 1
t = t3
goto L2
(L1)
(L3)
t4 = x
t5 = 0
t6 = t4 <= t5
if not T6 goto L4
t7 = 2
t = t7
goto L5
(L4)
t8 = 3
t = t8
goto L5
goto L2
```
