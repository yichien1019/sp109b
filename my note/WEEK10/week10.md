# 📝系統程式第十週筆記20210505

## 💻 程式實際操作
### 🔗 08-posix/02-thread/producerConsumer 
![](pic/producerConsumer.jpg)
#### The result of execution
```
user@user-myubuntu:~/sp/08-posix/02-thread$ gcc producerConsumer.c -o producerConsumer -lpthread
user@user-myubuntu:~/sp/08-posix/02-thread$ ./producerConsumer
Producer ID = 0 created!
Consumer ID = 1 created!
Producer ID = 2 created!
Consumer ID = 3 created!
Producer 2. Put 1 in slot 0
incremented in!
Producer 2. Put 2 in slot 1
incremented in!
```

### 🔗 08-posix/02-thread/philospher 
![](pic/philospher.jpg)
#### The result of execution
```
user@user-myubuntu:~/sp/08-posix/02-thread$ gcc philospher.c -o philospher -lpthread
user@user-myubuntu:~/sp/08-posix/02-thread$ ./philospher 
Switch=false
        Think 2 1
        Eat 2 1
        Think 2 2
        Eat 2 2
        Think 2 3
        Eat 2 3
        Think 2 4
        Eat 2 4
        Think 2 5
        Eat 2 5
    Think 1 1
    Eat 1 1
    Think 1 2
    Eat 1 2
    Think 1 3
    Eat 1 3
    Think 1 4
    Eat 1 4
    Think 1 5
    Eat 1 5
 Think 0 1
 Eat 0 1
 Think 0 2
 Eat 0 2
 Think 0 3
 Eat 0 3
 Think 0 4
 Eat 0 4
 Think 0 5
 Eat 0 5
```
* 三個哲學家 三隻筷子 一人吃五次

## 📖 補充資料
* [Dijkstra算法](http://nthucad.cs.nthu.edu.tw/~yyliu/personal/nou/04ds/dijkstra.html)
* [哲學家用餐問題](https://zh.wikipedia.org/wiki/%E5%93%B2%E5%AD%A6%E5%AE%B6%E5%B0%B1%E9%A4%90%E9%97%AE%E9%A2%98)
* [生產者消費者問題](https://zh.wikipedia.org/wiki/%E7%94%9F%E4%BA%A7%E8%80%85%E6%B6%88%E8%B4%B9%E8%80%85%E9%97%AE%E9%A2%98?fbclid=IwAR1QCqhZ57x1X0UI1wZsPfEDrQlEwkw3l8CbZW5XTkWiKOeZKoZnKluhzxM)


🖊️editor : yi-chien Liu


