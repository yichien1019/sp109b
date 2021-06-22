# 📝 期末作業
>主題:kilo
>作者:劉怡謙
>學號:110810519
>作品:並非100%原創

## 📖 安裝C編譯器
* Windows
需要在 Windows 中安裝某種 Linux 環境。這是因為我們的文本編輯器使用`<termios.h>`標頭在低級別與終端交互，這在 Windows 上不可用。
* macOS
當您嘗試運行該cc命令時，應彈出一個窗口，詢問您是否要安裝命令行開發人員工具。您也可以運行`xcode-select --install`以彈出此窗口。然後只需單擊“安裝”，它就會安裝一個 C 編譯器make
* Linux
`sudo apt-get install gcc make`

## 📖 make功能
`Makefile`使用以下內容創建一個按字面命名的新文件:
```
kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99
```
* `$(CC)` 是一個默認情況下 `make` 擴展為 `cc` 的變量。
* `-Wall` 代表“所有 Warnings”，並得到編譯器警告你，當它在你的程序可能不會在技術上是錯誤看到代碼，但被認為是C語言的不良或可疑的使用情況，像初始化之前使用變量。
* `-Wextra` 並 `-pedantic` 打開更多警告。如果程序編譯通過，除了在某些情況下“未使用的變量”警告之外，它不應產生任何警告。如果收到任何其他警告，請檢查以確保您的代碼與該步驟中的代碼完全匹配。
* `-std=c99` 指定C語言的確切版C99供我們使用。C99 允許我們在函數內的任何位置聲明變量，而 ANSI C要求在函數或塊的頂部聲明所有變量。

## 📖 main()功能
```
int main() {
  return 0;
}
```
* 在 C 中，您必須將所有可執行代碼放在函數中。`main()` C 中的函數是特殊的。它是您運行程序時的默認起點。當您`return`離開該`main()`函數時，程序退出並將返回的整數傳遞回操作系統。返回值`0`表示成功。
* 要編譯`kilo.c`，請 `cc kilo.c -o kilo` 在您的shell中運行。如果沒有發生錯誤，這將生成一個名為`kilo`.`-o`代表“輸出”，並指定輸出可執行文件應命名為`kilo`.
* 要運行kilo，請輸入`./kilo`您的 shell 並按<b>Enter</b>。

## 📖 原始模式 (讀取用戶的按鍵操作)
```
#include <unistd.h>
int main() {
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1); //從標準輸入中read()讀取1字節到變量中c
  return 0;
}
```
* `read()` 並 `STDIN_FILENO` 來自 `<unistd.h>`，我們要求從標準輸入中`read()`讀取1字節到變量中`c`，並繼續這樣做直到沒有更多字節可供讀取。
* 運行`./kilo`時，您的終端會連接到標準輸入，因此您的鍵盤輸入會被讀入`c`變量。
* 要退出上述程序，按<b>Ctrl-D</b>地告訴`read()`它的到達文件的末尾，或者可以隨時按下<b>Ctrl-C</b>以通知進程立即終止。

## 📖 按<b>q</b>退出?
```
#include <unistd.h>
int main() {
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
```
* 要退出該程序必須鍵入一行包含 a 的文本，`q` 然後按 <b>Enter</b>。程序將快速讀取一行文本，一次一個字符，直到讀取到`q`，此時`while`循環將停止，程序將退出。
* 之後的任何字符`q`都將在輸入隊列中保持未讀狀態，您可能會在程序退出後看到該輸入被送入 shell。

## 📖 關閉迴聲
我們可以通過:<br>
1.使用`tcgetattr()`將當前屬性讀入結構體<br>
2.手動修改結構體<br>
3.傳遞修改後的結構體以`tcsetattr()`將新的終端屬性寫回來設置終端的屬性。<br>
#### 讓我們嘗試`ECHO`以這種方式關閉該功能:
```
#include <termios.h>
#include <unistd.h>
void enableRawMode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int main() {
  enableRawMode();
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
```
`struct termios`、`tcgetattr()`、`tcsetattr()`、`ECHO`和`TCSAFLUSH`都來自`<termios.h>`
* 該`ECHO`功能使您鍵入的每個鍵都打印到終端上，因此可以看到正在鍵入的內容。因為當我們嘗試在原始模式下仔細呈現用戶界面時，這確實會妨礙我們。所以我們把它關掉。
* 可能會發現終端仍然沒有回顯鍵入的內容，只需按下<b>Ctrl-C</b>即可開始新的一行輸入到您的 shell，然後輸入`reset`並按<b>下Enter</b>。

## 📖 退出時禁用原始模式
```
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
struct termios orig_termios;
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int main() { … }
```
`atexit()`來自`<stdlib.h>`我們使用它來註冊 `disableRawMode()`要在程序退出時自動`main()`調用的`exit()` 函數
* 我們將原始終端屬性存儲在全局變量中`orig_termios`。我們將`orig_termios`結構分配給`raw`結構，以便在開始進行更改之前製作它的副本

## 📖 
## 📖 
## 📖 
## 📖 
## 📖 

## 📖 參考資料
https://github.com/antirez/kilo
https://viewsourcecode.org/snaptoken/kilo/


🖊️ editor : yi-chien Liu