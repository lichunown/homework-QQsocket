test_server:
	gcc -g -Wall test_server.c -o test_server.out -L /usr/local/lib -lglib-2.0 -lsqlite3
test_mystring:
	gcc -g -Wall test_mystring.c -o test_mystring.out
test_sql:
	gcc -g -Wall test_sql.c -o test_sql.out  -lsqlite3 #-L /usr/local/lib -lglib-2.0
test_hash:
	gcc -g -Wall test_hash.c -o test_hash.out -L /usr/local/lib -lglib-2.0
client:
	gcc -g -Wall client.c -o client.out
server:
	gcc -g -Wall server.c -o server.out -L /usr/local/lib -lglib-2.0 -lsqlite3 
clean:
	rm -f *.out 
default:
	make client
	make server
test:
	make test_server 
	make test_mystring 
	make test_sql 
	make test_hash
test_s:
	make test_server 
	make client
all:
	make test
	make 
