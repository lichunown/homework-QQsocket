# test

## test_sql(测试数据库)

```bash
make test_sql.out # build
# create a new user
./test_sql.out createuser [username] [password] [nickname]
# login 
./test_sql.out login [username] [password] 
# changenickname
./test_sql.out changenickname [username] [newnickname] 
# show all sql data
./test_sql.out all
```
## test_s(测试服务器与客户端)
```bash
make test_s # build 
./test_server.out 8001 # 可以不输入端口号，默认8001
# another shell
./client.out 0.0.0.0 8001 # 两个参数分别为连接的IP 和端口号
```