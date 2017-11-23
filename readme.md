# 这是我们的网络程序设计大作业

做一个类似于QQ的聊天程序，具有用户的登录注册等功能。

分为服务器和客户端两部分。客户端发起连接，服务器响应，建立的TCP连接始终保留。

客户端发起的每次请求，都会先发送一个固定大小的数据报头。如果有其他数据（比如给另一用户发送消息），则在报头发送完成后发送数据段。服务器接受消息并处理后，返回一个是否成功的提示（同时可能有附加数据）。



# [数据交换文档](./datachange.md)


# 编译

- to use sqlite3(for ubuntu)

```bash
sudo apt-get install sqlite3 
sudo apt-get install libsqlite3-dev
```

- use glib
  (glib download)[http://ftp.acc.umu.se/pub/GNOME/sources/glib/2.54/]

```
wget http://ftp.gnome.org/pub/gnome/sources/glib/2.54/glib-2.54.2.tar.xz
tar -xvJf ./glib-2.54.2.tar.xz
cd ./glib-2.54.2.tar.xz

sudo apt-get install gcc
sudo apt-get install zlib1g-dev
sudo apt-get install pkg-config
sudo apt-get install libffi-dev
sudo apt-get install gettext
sudo apt-get install libmount-dev
sudo apt-get install libpcre3-dev

make
make install

# show the gcc option 
pkg-config glib-2.0 --libs


sudo cp -r /usr/local/include/glib-2.0/* /usr/include/  
sudo cp /usr/local/lib/glib-2.0/include/glibconfig.h /usr/include/
```
(ubuntu完整安装glib心得)[http://blog.csdn.net/andylauren/article/details/51346388]

- build test

```bash
make test
```
- build server

```bash
make 
```
- build all

```bash
make all
```

# [test](./test.md)


# TODO

- 密码加密算法



# 版本更新

## 2017-11-22
- ​能够实现测试客户端与服务器的数据交换。
  - 服务器sendtoFd函数传入数据必须为动态申请的内存地址。发送数据后自动free。
  - 由于c语言结构体存在对齐问题，决定删除HEAD_DATA_ALL数据结构，调用为两次发送HEAD_MAIN和HEAD_DATA。
  - 在HEAD_USER中删除为使HEAD_DATA_ALL 和HEAD_USER_ALL大小一样的补充占位char。
- 新增展示登录用户功能`#showlist`

## 2017-11-23 Version0.0.2
- 支持两客户端之间的数据转发。真正具有QQ的数据传输功能。
- 客户端异常结束时，服务器自动登出用户。
- 同一用户名，只允许一个客户端存在。新登录的客户会将原有用户挤下线。