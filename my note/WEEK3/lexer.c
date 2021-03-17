#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TMAX 10000000
#define SMAX 100000

enum { Id, Int, Keyword, Literal, Char }; //列舉 0,1,2,3,4

char *typeName[5] = {"Id", "Int", "Keyword", "Literal", "Char"};

char code[TMAX];
char strTable[TMAX], *strTableEnd=strTable;
char *tokens[TMAX];  //字元指標
int tokenTop=0;
int types[TMAX];

#define isDigit(ch) ((ch) >= '0' && (ch) <='9')

#define isAlpha(ch) (((ch) >= 'a' && (ch) <='z') || ((ch) >= 'A' && (ch) <= 'Z'))

int readText(char *fileName, char *text, int size) {
  FILE *file = fopen(fileName, "r");
  int len = fread(text, 1, size, file);
  text[len] = '\0';
  fclose(file);
  return len;
}

/* strTable =
#\0include\0"sum.h"\0int\0main\0.....
*/
char *next(char *p) {
  while (isspace(*p)) p++;  //跳過空白

  char *start = p; //         include "sum.h"
                   //         ^      ^
                   //  start= p      p
  int type;
  if (*p == '\0') return NULL;  //如跳過空白就沒有直接傳回null
  if (*p == '"') {
    p++;
    while (*p != '"') p++;
    p++;
    type = Literal;
  } else if (*p >='0' && *p <='9') { // 數字
    while (*p >='0' && *p <='9') p++;
    type = Int;  //只支援整數
  } else if (isAlpha(*p) || *p == '_') {  // 變數名稱或關鍵字id //不能數字開頭
    while (isAlpha(*p) || isDigit(*p) || *p == '_') p++;
    type = Id;
  } else { // 單一字元 ex.等號,括號,星號
    p++;
    type = Char;
  }
  int len = p-start;
  char *token = strTableEnd;
  strncpy(strTableEnd, start, len);
  strTableEnd[len] = '\0';
  strTableEnd += (len+1);
  types[tokenTop] = type;
  tokens[tokenTop++] = token;
  printf("token=%s\n", token);
  return p;
}

void lex(char *code) {
  char *p = code;
  while (1) {
    p = next(p);
    if (p == NULL) break;
  }
}

void dump(char *strTable[], int top) {
  for (int i=0; i<top; i++) {
    printf("%d:%s\n", i, strTable[i]);
  }
}

int main(int argc, char * argv[]) {
  readText(argv[1], code, sizeof(code));  //要減一才不會有非法存取問題
  puts(code);
  lex(code);
  dump(tokens, tokenTop);
}

