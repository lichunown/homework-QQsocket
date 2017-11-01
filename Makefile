default:
	make test 
	make client
test:
	gcc -g -Wall test_sql.c -o test_sql -L /usr/local/lib -l glib-2.0 -l sqlite3 

client:
	gcc -g client.c -o client.out
client-w:
	gcc -g -Wall client.c -o client.out -L /usr/local/lib -l glib-2.0 -l sqlite3 
clean:
	rm -r test_sql client.out