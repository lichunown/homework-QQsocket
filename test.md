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
- 测试客户端内：

```
#signup [username] [password] [nickname]//注册用户

#login [username] [password] //登录用户

#logout [token] //退出登录

#sendto [token] [sendto_username] [data] //向某一用户发送信息
	// token: 登录时返回的token, 
	// sendto_username: 要发送的目标用户名,
	// data: 发送的数据
	
#showlist //显示所有已登录用户
```