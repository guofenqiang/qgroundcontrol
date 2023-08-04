target = libBZConv
src = $(wildcard *.cpp)
objects = $(patsubst %.cpp, %.o, $(src))
 
$(target):$(objects)
	gcc -shared  $^ -o $@.so
	ar rcs $@.a $^
 
%.o:%.c
	gcc -c -fpic $<
.PHONY:clean
clean:
	rm -f $(objects) $(target).so $(target).a

