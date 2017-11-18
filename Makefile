glibdev = -L /usr/local/lib -lglib-2.0
sqlite3dev = -lsqlite3

default:
	make client
	make server
test_server:
	gcc -g -Wall test_server.c -o test_server.out $(glibdev) $(sqlite3dev)
test_mystring:
	gcc -g -Wall test_mystring.c -o test_mystring.out
test_sql:
	gcc -g -Wall test_sql.c -o test_sql.out  -lsqlite3 #-L /usr/local/lib -lglib-2.0
test_hash:
	gcc -g -Wall test_hash.c -o test_hash.out $(glibdev)
test_server_epoll:
	gcc -g -Wall test_server_epoll.c -o test_server_epoll.out $(glibdev) $(sqlite3dev)
client:
	gcc -Wall client.c -o client.out
server:
	gcc -g -Wall server.c -o server.out $(glibdev) $(sqlite3dev)
clean:
	rm -f *.out 
test:
	make test_server 
	make test_mystring 
	make test_sql 
	make test_hash
all:
	make test
	make 
