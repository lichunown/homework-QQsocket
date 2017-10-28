# This is our Web Homework

make a QQ using TCP.

## head for per message

### client to server

|HEAD_MAIN|HEAD_USER|
|-----|-----|
|char mode|char logmode; char username[16];char password[16];char nickname[16];|
|1|49|

|HEAD_MAIN|HEAD_DATA|
|-----|-----|
|char mode|char token[32];char datamode;int datalen;|
|1|37|


# build

```bash
make test
```