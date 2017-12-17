# test

## test_sql(测试数据库)

```bash
make test_sql # build
# create a new user 创建新用户
./test_sql createuser [username] [password] [nickname]
# login 数据库服务器检查是否存在用户，密码是否匹配正确
./test_sql login [username] [password] 
# changenickname 更改用户昵称
./test_sql changenickname [username] [newnickname] 
# show all sql data 显示所有用户信息
./test_sql all
```
## client(测试服务器与客户端)
- 编译运行
```bash
make # build 
./server 8001 # 可以不输入端口号，默认8001
# another shell
./client.out 0.0.0.0 8001 # 两个参数分别为连接的IP 和端口号
```
- 客户端内：

```
--------通用模块----------
#help //显示帮助
#status //显示客户端目前状态（是否在线，如果在线的话显示用户名）
#exit// 退出程序，正常登出


---------用户管理模块----------
#signup [username] [password] [nickname]//注册用户
#login [username] [password] //登录用户
#logout  //退出登录

#showlist //显示所有已登录用户


-------------信息交流模块----------------
#sendto [sendto_username] [data] //向某一用户发送信息
	// sendto_username: 要发送的目标用户名,
	// data: 发送的数据
	
	
-------------文件传输模块----------------
#showfiles //显示服务器已存在的文件
#sendfile [filename] [dstfilename]//向服务器发送文件
	//filename： 要发送的文件名
	//dstfilename： 服务器端保存的文件名
#recvfile [filename] [id=0]// 从服务器下载文件
	//filename 服务器的文件名
	//id 断点续传的文件块，默认为0

```
- 