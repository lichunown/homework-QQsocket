glibdev = -L /usr/local/lib -lglib-2.0
sqlite3dev = -lsqlite3

default:
	make clean
	make client
	make server
test_server:
	gcc -g -Wall test_server.c -o test_server $(glibdev) $(sqlite3dev)
test_mystring:
	gcc -g -Wall test_mystring.c -o test_mystring
test_sql:
	gcc -g -Wall test_sql.c -o test_sql -lsqlite3 #-L /usr/local/lib -lglib-2.0
test_hash:
	gcc -g -Wall test_hash.c -o test_hash $(glibdev)
test_server_epoll:
	gcc -g -Wall test_server_epoll.c -o test_server_epoll $(glibdev) $(sqlite3dev)
test_htons:
	gcc -Wall test_int_in_socket.c -o test_int_in_socket
	gcc -Wall test_int_in_socket_server.c -o test_int_in_socket_server
client:
	gcc -Wall client.c -o client
server:
	gcc -g -Wall server.c -o server $(glibdev) $(sqlite3dev)
clean:
	rm -f *.out test_server test_mystring test_sql test_hash test_server_epoll
	rm -f client server test_int_in_socket test_int_in_socket_server
test:
	make test_*
	# make test_server 
	# make test_mystring 
	# make test_sql 
	# make test_hash
all:
	make 
	make test
	
