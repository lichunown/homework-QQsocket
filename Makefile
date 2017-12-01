glibdev = `pkg-config --libs glib-2.0  --cflags --static` # -L /usr/local/lib -l glib-2.0
sqlite3dev = `pkg-config --libs sqlite3`
glibstatic = `pkg-config --libs glib-2.0 --cflags --static`
sqlite3static = `pkg-config --libs sqlite3 --cflags --static`
default:
	make clean
	make client
	make server
test_mystring:
	gcc -g -Wall test_mystring.c -o test_mystring
test_sql:
	gcc -g -Wall test_sql.c -o test_sql -lsqlite3
test_hash:
	gcc -g -Wall test_hash.c -o test_hash $(glibdev)
test_htons:
	gcc -Wall test_int_in_socket.c -o test_int_in_socket
	gcc -Wall test_int_in_socket_server.c -o test_int_in_socket_server
client:
	gcc -Wall client.c -o client 
client2:
	gcc -Wall client2.c -o client2
server:
	gcc -g -Wall server.c -o server $(glibdev) $(sqlite3dev) 
clean:
	rm -f *.out test_server test_mystring test_sql test_hash test_server_epoll
	rm -f client server test_int_in_socket test_int_in_socket_server
test:
	make test_mystring 
	make test_sql 
	make test_hash
	make test_htons
static:
	gcc -Wall client.c -o client --static
	gcc -g -Wall server.c -o server_static $(glibstatic) $(sqlite3static) --static
all:
	make 
	make test
	
