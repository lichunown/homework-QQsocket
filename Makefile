test:
	gcc -g -Wall test_sql.c -o test_sql.out  -lsqlite3 #-L /usr/local/lib -lglib-2.0
	gcc -g -Wall test_server.c -o test_server.out -L /usr/local/lib -lglib-2.0 -lsqlite3
	gcc -g -Wall test_mystring.c -o test_mystring.out
	gcc -g -Wall test_hash.c -o test_hash.out -L /usr/local/lib -lglib-2.0

client:
	gcc -g -Wall client.c -o client.out
server:
	gcc -g -Wall server.c -o server.out -L /usr/local/lib -lglib-2.0 -lsqlite3 
clean:
	rm -f test_sql client.out *.out test_hash test_mystring
default:
	client server