# HDU模拟文件系统使用说明
### 使用方法：
```
$ rm -rf build
$ cd build
$ cmake ..
$ make -j8
```
### 主要命令
命令名 | 参数1 | 参数2
---|--- | ---
mkdir | 绝对路径|
rmdir | 绝对路径|
cd | 绝对路径|
ls | |
create | 绝对路径|
open | 绝对路径|
write | -c -co -a| 标识符（open返回）
close | 标识符 |
rm | 绝对路径 |
save | |

