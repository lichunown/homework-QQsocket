test:
	gcc -g -Wall test_sql.c -o test_sql  -lsqlite3 #-L /usr/local/lib -lglib-2.0
<<<<<<< HEAD
	gcc -g -Wall test_mystring.c -o test_mystring
	gcc -g -Wall test_server.c -o test_server -L /usr/local/lib -lglib-2.0 -lsqlite3
=======
	gcc -g -Wall test_mystring.c -o test_mystring.out
	gcc -g -Wall test_hash.c -o test_hash -L /usr/local/lib -lglib-2.0
>>>>>>> 7e7263ee43b22d2fcf1a2d58acacb8ede31d96aa
client:
	gcc -g -Wall client.c -o client.out
server:
	gcc -g -Wall server.c -o server.out -L /usr/local/lib -lglib-2.0 -lsqlite3 
clean:
	rm -f test_sql client.out *.out test_hash test_mystring
default:
	client server