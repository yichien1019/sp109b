# 📝系統程式第十三週筆記20210526
## HTTP協定
### 🔖 何謂傳輸協定？
以人類來說，在台灣，我們多數使用的是繁體中文，國際之間常見的溝通語言則是英文，套用到機器之間也會有既定的格式和語法。
### 🔖  HTTP 的組成結構
HTTP 的組成結構，分別是 HTTP Request 與 HTTP Response 這兩個大項目
![](pic/http1.png)
#### 請求時的HTTP Request
從客戶端發出 HTTP Request 時，通常會定義以下的資訊，以瀏覽 Google 首頁為例
![](pic/http2.png)
常見的 Method 方法如下：
* GET：讀取資料
* POST：新增資料（常搭配 form 標籤使用）
* PATCH：修改資料
* PUT：修改資料（若該筆資料不存在，則自動新增資料）
* DELETE：刪除資料
通常一個 Method 會搭配一個 URL，也會對應伺服器端一組特定的資源，而 Message Body 的內容則取決於每次的動作。
若單純是使用 GET 方法瀏覽網頁，則 Message Body 為空，但是，若以填寫表單為範例，那客戶端就會送出資料，這筆資料就會被傳送到伺服器的資料庫，此時，我們則會使用 POST 方法將資料送進 Message Body，提交給某個指定的 URL，進而建立資料或更新資料。
#### 回傳時的HTTP Request
從伺服器端回傳 HTTP Response 時，通常會定義以下的資訊，以瀏覽 Google 首頁為例
![](pic/http3.png)
狀態碼 (status code)不僅有助於我們瞭解網路發生問題的原因，也能幫助你與工程師日常的溝通協作，如：
* 1XX 訊息類 (收到請求，請求者繼續執行操作)
* 2XX 成功類 (操作被成功接受並處理)，例如：200 成功回應
* 3XX 重定向類 (需進一步操作才能完成)，例如：301 成功轉向
* 4XX 客戶端錯誤類 (請求語法錯誤或無法完成請求)，例如：404 找不到資源
* 5XX 伺服器錯誤類 (後端的問題)，例如：500 伺服器錯誤

## 💻 程式實際操作
### 🔗 08-posix/06-net/05-http/helloWebServer 
![](pic/helloWebServer.jpg)
![](pic/localhost8080.jpg)
<details>
  <summary><b>Show code</b></summary>

  ```
#include "../net.h"
 
char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/plain; charset=UTF-8\r\n"
"Content-Length: 14\r\n\r\n"
"Hello World!\r\n";

int main(int argc, char *argv[]) {
  int port = (argc >= 2) ? atoi(argv[1]) : PORT;
	net_t net;
	net_init(&net, TCP, SERVER, port, NULL);
	net_bind(&net);
	net_listen(&net);
  printf("Server started at port: %d\n", net.port);
  int count=0;
  while (1) {
		int client_fd = net_accept(&net);
    printf("%d:got connection, client_fd=%d\n", count++, client_fd);
    int n = write(client_fd, response, strlen(response));
    fsync(client_fd);
    assert(n > 0);
    sleep(1);
    close(client_fd);
  }
}
  ```
</details>

#### The result of execution
```
user@user-myubuntu:~/sp/08-posix/06-net/05-http$ make
gcc -std=c99 -O0 helloWebServer.c ../net.c -o helloWebServer
gcc -std=c99 -O0 headPrintServer.c ../net.c -o headPrintServer
gcc -std=c99 -O0 htmlServer.c ../net.c httpd.c -o htmlServer
user@user-myubuntu:~/sp/08-posix/06-net/05-http$ ./helloWebServer 
Server started at port: 8080
0:got connection, client_fd=4
1:got connection, client_fd=4
```
#### The result of execution (補充)
![](pic/helloWebServercurl.JPG)
```
user@user-myubuntu:~$ curl http://localhost:8080
Hello World!
user@user-myubuntu:~$ curl -v http://localhost:8080
*   Trying 127.0.0.1:8080...
* TCP_NODELAY set
* Connected to localhost (127.0.0.1) port 8080 (#0)
> GET / HTTP/1.1
> Host: localhost:8080
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Type: text/plain; charset=UTF-8
< Content-Length: 14
< 
Hello World!
* Connection #0 to host localhost left intact
```
### 🔗 08-posix/06-net/05-http/htmlServer 
![](pic/htmlServer.jpg)
<details>
  <summary><b>Show code</b></summary>

  ```
#include "../net.h"
#include "httpd.h"

int main(int argc, char *argv[]) {
  int port = (argc >= 2) ? atoi(argv[1]) : PORT;
	net_t net;
	net_init(&net, TCP, SERVER, port, NULL);
	net_bind(&net);
	net_listen(&net);
  printf("Server started at port: %d\n", net.port);
  while (1) {
    int client_fd = net_accept(&net);
    if (client_fd == -1) {
      printf("Can't accept");
      continue;
    }
    char header[TMAX], path[SMAX];
    readHeader(client_fd, header);
    printf("===========header=============\n%s\n", header);
    parseHeader(header, path);
    printf("path=%s\n", path);
    if (strstr(path, ".htm") != NULL) {
      printf("path contain .htm\n");
      responseFile(client_fd, path);
    } else {
      printf("not html => no response!\n");
    }
    sleep(1);
    close(client_fd);
  }
}
  ```
</details>

#### The result of execution
```
user@user-myubuntu:~/sp/08-posix/06-net/05-http$ ./htmlServer 
Server started at port: 8080
===========header=============
GET / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.68.0
Accept: */*


path=/
not html => no response!
===========header=============
GET /index.html HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.68.0
Accept: */*


path=/index.html
path contain .htm
responseFile:fpath=./web/index.html
===========header=============
GET /hello.html HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.68.0
Accept: */*


path=/hello.html
path contain .htm
responseFile:fpath=./web/hello.html
```
#### The result of execution (補充)
![](pic/htmlServercurl.JPG)
```
user@user-myubuntu:~$ curl http://localhost:8080/index.html
<html>
<body> 
<ul>
  <li><a href="hello.html">hello.html</a></li>
  <li><a href="http://misavo.com">misavo.com</a></li>
</ul>
</body>
</html>user@user-myubuntu:~$ curl http://localhost:8080/hello.html
<html>
<body>
Hello! 

<a href="https://tw.youtube.com/">YouTube</a>
</body>
</html>user@user:~/sp/08-posix/06-net/05-http$ curl -v http://localhost:8080/index.html
*   Trying 127.0.0.1:8080...
* TCP_NODELAY set
* Connected to localhost (127.0.0.1) port 8080 (#0)
>===========header=============
GET /index.html HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.68.0
Accept: */*


path=/index.html
path contain .htm
responseFile:fpath=./web/index.html
 GET /index.html HTTP/1.1
> Host: localhost:8080
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Type: text/html; charset=UTF-8
< Content-Length: 142
< 
<html>
<body> 
<ul>
  <li><a href="hello.html">hello.html</a></li>
  <li><a href="http://misavo.com">misavo.com</a></li>
</ul>
</body>
* Connection #0 to host localhost left intact
```
### 🔗 08-posix/06-net/05-http/
![](pic/.jpg)
<details>
  <summary><b>Show code</b></summary>

  ```

  ```
</details>

#### The result of execution
```

```


### 🔗 08-posix/06-net/05-http/ 
![](pic/.jpg)
<details>
  <summary><b>Show code</b></summary>

  ```

🖊️editor : yi-chien Liu


https://www.facebook.com/ccckmit/videos/10158997110611893