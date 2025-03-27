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

#make test: executes the specified rules for compilation/linking and runs the executable
test: objects \
      test_unity/build/Tests.exe
	@echo ""
	./test_unity/build/Tests.exe

#make test_verbose: executes the specified rules for compilation/linking and runs the executable in verbose mode
test_verbose: objects\
              test_unity/build/Tests.exe
	@echo ""
	./test_unity/build/Tests.exe -v

#make debug: executes the specified rules for compilation/linking and runs the executable in gdb debug mode
debug: objects \
       test_unity/build/Tests.exe
	gdb ./test_unity/build/Tests.exe

#make objects: executes the specified rules for the directory creation and compilation
objects: mkdirs \
         test_unity/build/objs/unity.o test_unity/build/objs/unity_fixture.o \
         test_unity/build/objs/DumbExample.o test_unity/build/objs/TestDumbExample.o \
         test_unity/build/objs/LedDriver.o test_unity/build/objs/TestLedDriver.o \
         test_unity/build/objs/RuntimeErrorStub.o \
         test_unity/build/objs/AllTests.o

#make mkdirs: creates the directory test_unity/build/objs/ used to store the compiled .o files
mkdirs:
	mkdir -p test_unity/build/objs/

#make clean: deletes Tests.exe and all the object files
clean:
#	rm -f test_unity/build/Tests.exe test_unity/build/objs/*.o
	rm -rf test_unity/build/


#rule to compile RuntimeErrorStub.c into RuntimeErrorStub.o
test_unity/build/objs/RuntimeErrorStub.o: mocks/RuntimeErrorStub.c
	gcc -c -g -Iinclude/util/ -Imocks/ $^ -o $@

#rule to compile LedDriver.c into LedDriver.o
test_unity/build/objs/LedDriver.o: src/02_LedDriver/LedDriver.c
	gcc -c -g -Iinclude/02_LedDriver/ -Iinclude/util/ $^ -o $@

#rule to compile TestLedDriver.c into TestLedDriver.o
test_unity/build/objs/TestLedDriver.o: test_unity/02_LedDriver/TestLedDriver.c
	gcc -c -g -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ -Iinclude/02_LedDriver/ -Imocks/ $^ -o $@

#rule to compile DumbExample.c into DumbExample.o
test_unity/build/objs/DumbExample.o: src/01_DumbExample/DumbExample.c
	gcc -c -g -Iinclude/01_DumbExample/ $^ -o $@

#rule to compile TestDumbExample.c into TestDumbExample.o
test_unity/build/objs/TestDumbExample.o: test_unity/01_DumbExample/TestDumbExample.c
	gcc -c -g -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ -Iinclude/01_DumbExample/ $^ -o $@

#rule to compile AllTests.c into AllTests.o
test_unity/build/objs/AllTests.o: test_unity/AllTests.c
	gcc -c -g -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ $^ -o $@

#rule to compile unity.c into unity.o
test_unity/build/objs/unity.o: unity/src/unity.c
	gcc -c -g -Iunity/src/ $^ -o $@

#rule to compile unity_fixture.c into unity_fixture.o
test_unity/build/objs/unity_fixture.o: unity/extras/fixture/src/unity_fixture.c
	gcc -c -g -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ $^ -o $@

#rule to link the specified .o files into Tests.exe
test_unity/build/Tests.exe: test_unity/build/objs/unity_fixture.o test_unity/build/objs/unity.o \
                      test_unity/build/objs/TestDumbExample.o test_unity/build/objs/DumbExample.o \
                      test_unity/build/objs/TestLedDriver.o test_unity/build/objs/LedDriver.o \
                      test_unity/build/objs/RuntimeErrorStub.o \
                      test_unity/build/objs/AllTests.o
	gcc $^ -o $@
