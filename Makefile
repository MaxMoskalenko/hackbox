compile: 
	@gcc hackbox.c -o hackbox
	@echo "Compilation success\n"

help: compile
	./hackbox help

ls: compile 
	./hackbox ls
	./hackbox ls .
	./hackbox ls test
	./hackbox ls error/path
	./hackbox ls -l ./
	./hackbox ls . test

cat: compile
	./hackbox cat test/hello.txt
	./hackbox cat error/path
	./hackbox cat test/hello.txt test/.secretfile

cp: compile
	./hackbox cp test/hello.txt ./hello-2.txt
	./hackbox cp hello-2.txt hello-3
	./hackbox cp hello
	./hackbox cp error.txt somewhere

mkdir: compile
	./hackbox mkdir new_dir
	./hackbox mkdir test
	./hackbox mkdir
	./hackbox mkdir test/file
	./hackbox mkdir test/dir1 test/dir2

clean:
	@rm hello-2.txt hello-3
	@rmdir new_dir test/file test/dir1 test/dir2
