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