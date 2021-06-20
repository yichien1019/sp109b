# 📝系統程式第十二週筆記20210616
## 📖 RISC-V 處理器

## 💻 程式實際操作
### 🔗 08-posix/04-fs/00-basic/io1

![](pic/io1.JPG)
<details>
  <summary><b>Show code</b></summary>

  ```

  ```
</details>

#### The result of execution
```
user@user-myubuntu:~/sp/10-riscv/02-sp/02-gcc$ riscv64-unknown-elf-gcc -c add.c -o add.elf
user@user-myubuntu:~/sp/10-riscv/02-sp/02-gcc$ riscv64-unknown-elf-objdump -d add.elf

add.elf:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <add>:
   0:	1101                	addi	sp,sp,-32
   2:	ec22                	sd	s0,24(sp)
   4:	1000                	addi	s0,sp,32
   6:	87aa                	mv	a5,a0
   8:	872e                	mv	a4,a1
   a:	fef42623          	sw	a5,-20(s0)
   e:	87ba                	mv	a5,a4
  10:	fef42423          	sw	a5,-24(s0)
  14:	fec42703          	lw	a4,-20(s0)
  18:	fe842783          	lw	a5,-24(s0)
  1c:	9fb9                	addw	a5,a5,a4
  1e:	2781                	sext.w	a5,a5
  20:	853e                	mv	a0,a5
  22:	6462                	ld	s0,24(sp)
  24:	6105                	addi	sp,sp,32
  26:	8082                	ret
user@user-myubuntu:~/sp/10-riscv/02-sp/02-gcc$ riscv64-unknown-elf-gcc -c add.s -o add.elf
user@user-myubuntu:~/sp/10-riscv/02-sp/02-gcc$ riscv64-unknown-elf-objdump -d add.elf

add.elf:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <add>:
   0:	1101                	addi	sp,sp,-32
   2:	ec22                	sd	s0,24(sp)
   4:	1000                	addi	s0,sp,32
   6:	87aa                	mv	a5,a0
   8:	872e                	mv	a4,a1
   a:	fef42623          	sw	a5,-20(s0)
   e:	87ba                	mv	a5,a4
  10:	fef42423          	sw	a5,-24(s0)
  14:	fec42703          	lw	a4,-20(s0)
  18:	fe842783          	lw	a5,-24(s0)
  1c:	9fb9                	addw	a5,a5,a4
  1e:	2781                	sext.w	a5,a5
  20:	853e                	mv	a0,a5
  22:	6462                	ld	s0,24(sp)
  24:	6105                	addi	sp,sp,32
  26:	8082                	ret
```


apt-get install qemu-system-riscv32
