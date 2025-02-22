# File:    makefile
# Author:  Julio Cesar Bernal Mendez
# Brief:   Simple makefile that can be run on Windows using a git bash terminal.
#
#          It compiles and links the specified object files into one executable (Tests.exe)
#          which is also executed automatically showing all the results of the test group runners
#          called from the AllTests.c main() function. This can be done by just typing "make" in your shell
#          (see targets below).
#
#          A much better makefile explanation can be found here: https://www.throwtheswitch.org/build/make
#
# Version: 0.1
# Date:    2025-02-21


#ifeq ($(OS),Windows_NT)
#  ifeq ($(shell uname -s),) # not in a bash-like shell
#	CLEANUP = del /F /Q
#	MKDIR = mkdir
#  else # in a bash-like shell, like msys
#	CLEANUP = rm -f
#	MKDIR = mkdir -p
#  endif
#	TARGET_EXTENSION=exe
#else
#	CLEANUP = rm -f
#	MKDIR = mkdir -p
#	TARGET_EXTENSION=out
#endif

#make test: executes the specified rules for compilation and linking
test: mkdirs test/build/objs/DumbExample.o test/build/objs/TestDumbExample.o test/build/objs/AllTests.o test/build/objs/unity.o test/build/objs/unity_fixture.o test/build/Tests.exe run

#make mkdirs: creates the directory test/build/objs/ used to store the compiled .o files
mkdirs:
	mkdir -p test/build/objs/

#make run: runs Tests.exe
run:
	@echo ""
	./test/build/Tests.exe

#make clean: deletes Tests.exe and all the object files
clean:
#	rm -f test/build/Tests.exe test/build/objs/*.o
	rm -rf test/build/



#rule to compile DumbExample.c into DumbExample.o
test/build/objs/DumbExample.o: src/01_DumbExample/DumbExample.c
	gcc -c -Iinclude/01_DumbExample/ $^ -o $@

#rule to compile TestDumbExample.c into TestDumbExample.o
test/build/objs/TestDumbExample.o: test/01_DumbExample/TestDumbExample.c
	gcc -c -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ -Iinclude/01_DumbExample/ $^ -o $@

#rule to compile AllTests.c into AllTests.o
test/build/objs/AllTests.o: test/AllTests.c
	gcc -c -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ $^ -o $@

#rule to compile unity.c into unity.o
test/build/objs/unity.o: unity/src/unity.c
	gcc -c -Iunity/src/ $^ -o $@

#rule to compile unity_fixture.c into unity_fixture.o
test/build/objs/unity_fixture.o: unity/extras/fixture/src/unity_fixture.c
	gcc -c -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ $^ -o $@

#rule to link the specified .o files into Tests.exe
test/build/Tests.exe: test/build/objs/unity_fixture.o test/build/objs/unity.o test/build/objs/AllTests.o test/build/objs/TestDumbExample.o test/build/objs/DumbExample.o                      
	gcc $^ -o $@
