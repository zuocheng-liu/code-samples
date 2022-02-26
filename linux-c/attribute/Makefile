.PHONY : all clean

all : main main_lws main_lsw

libstrong.so :
	gcc -shared -fPIC -o libstrong.so strong.c
libweak.so :
	gcc -shared -fPIC -o libweak.so weak.c
main :
	gcc -o main main.c weak.c strong.c

main_lws : libstrong.so libweak.so
	gcc -o main_lws main.c -L. -lweak -lstrong 
main_lsw : libstrong.so libweak.so
	gcc -o main_lsw main.c -L. -lstrong -lweak 
clean :
	rm -f  *.so main main_lws main_lsw
