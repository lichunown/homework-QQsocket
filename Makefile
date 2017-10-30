default:
	make test 
	make client
test:
	gcc -g -Wall test_sql.c -o test_sql -l sqlite3
client:
	gcc -g client.c -o client.out
client-w:
	gcc -g -Wall client.c -o client.out
clean:
	rm -r test_sql client.out