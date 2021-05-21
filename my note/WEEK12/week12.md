# 📝系統程式第十二週筆記20210519
##



## 💻 程式實際操作
### 🔗 08-posix/04-fs/00-basic/io1
![](io1.JPG)
#### The result of execution
```
root@user-myubuntu:/home/user/sp/08-posix/04-fs/00-basic# gcc io1.c -o io1
root@user-myubuntu:/home/user/sp/08-posix/04-fs/00-basic# ./io1
n=10
a=3, b=4
```

### 🔗 08-posix/04-fs/04-stderr/stderr1
![](stderr1.JPG)
#### The result of execution
```
root@user-myubuntu:/home/user/sp/08-posix/04-fs/04-stderr# gcc stderr1.c -o stderr1
root@user-myubuntu:/home/user/sp/08-posix/04-fs/04-stderr# ./stderr1 
Warning: xxx
Error: yyy
```

### 🔗 08-posix/07-nonblocking/blocking1 
![](blocking1 .JPG)
#### The result of execution
```
root@user-myubuntu:/home/user/sp/08-posix/07-nonblocking# gcc blocking1.c -o blocking1
root@user-myubuntu:/home/user/sp/08-posix/07-nonblocking# ./blocking1 
1
buf is 1

test
2
buf is 2
```

### 🔗 08-posix/07-nonblocking/nonblocking1 
![](nonblocking1.JPG)
#### The result of execution
```
root@user-myubuntu:/home/user/sp/08-posix/07-nonblocking# gcc nonblocking1.c -o nonblocking1
root@user-myubuntu:/home/user/sp/08-posix/07-nonblocking# ./nonblocking1 
read /dev/tty: Resource temporarily unavailable
no input,buf is null
```

### 🔗 08-posix/06-net/00-time/time
![](time.JPG)
#### The result of execution
```
root@user-myubuntu:/home/user/sp/08-posix/06-net/00-time# gcc time.c -o time
root@user-myubuntu:/home/user/sp/08-posix/06-net/00-time# ./time
Wed May 19 14:54:10 2021
Wed May 19 14:54:11 2021
Wed May 19 14:54:12 2021
```

### 🔗 sp/08-posix/06-net/01-timeTcp1/client
![](serverclient1.JPG)
#### The result of execution
```
root@user-myubuntu:/home/user/sp/08-posix/06-net/01-timeTcp1# make
gcc -std=c99 -O0 server.c -o server
gcc -std=c99 -O0 client.c -o client
root@user-myubuntu:/home/user/sp/08-posix/06-net/01-timeTcp1# ./server&
[1] 15755
root@user-myubuntu:/home/user/sp/08-posix/06-net/01-timeTcp1# ./client
Wed May 19 15:02:50 2021
```

### 🔗 08-posix/06-net/03-telnet1/client
![](serverclient2.JPG)
#### The result of execution
```
root@user-myubuntu:/home/user/sp/08-posix/06-net/03-telnet1# make
gcc -Wall -std=gnu99 server.c ../net.c -o server
gcc -Wall -std=gnu99 client.c ../net.c -o client
root@user-myubuntu:/home/user/sp/08-posix/06-net/03-telnet1# ./server&
[1] 15948
root@user-myubuntu:/home/user/sp/08-posix/06-net/03-telnet1# ./client
connect to server 127.0.0.1 success!
127.0.0.1 $ ls
cmd=ls
client
client.c
Makefile
README.md
server
server.c
```