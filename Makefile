default:
	make test 
	make client
test:
	gcc -g -Wall test_sql.c -o test_sql  -lsqlite3 #-L /usr/local/lib -lglib-2.0
	gcc -g -Wall test_mystring.c -o test_mystring
client:
	gcc -g client.c -o client.out
client-w:
	gcc -g -Wall client.c -o client.out #-L /usr/local/lib -lglib-2.0 -lsqlite3 
clean:
	rm -r test_sql client.out