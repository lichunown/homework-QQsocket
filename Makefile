test:
	gcc test_sql.c -o test_sql -l sqlite3
clean:
	rm test_sql