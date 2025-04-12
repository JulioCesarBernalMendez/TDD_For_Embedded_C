# File:    makefile
# Author:  Julio Cesar Bernal Mendez
# Brief:   Simple makefile that can be run on Windows using a git bash terminal.
#
#          It compiles and links the specified object files into one executable (UnityTests.exe)
#          which is also executed automatically showing all the results of the test group runners
#          called from the AllUnityTests.c main() function. This can be done by just typing "make" in your shell
#          (see that and other targets below).
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

####################################### Unity make rules #######################################

#make test_unity: executes the specified rules for Unity compilation/linking and runs the executable
test_unity: objects_unity \
            test_unity/build/UnityTests.exe
	@echo ""
	./test_unity/build/UnityTests.exe

#make test_unity_verbose: executes the specified rules for Unity compilation/linking and runs the executable in verbose mode
test_unity_verbose: objects_unity \
                    test_unity/build/UnityTests.exe
	@echo ""
	./test_unity/build/UnityTests.exe -v

#make debug_unity: executes the specified rules for Unity compilation/linking and runs the executable in gdb debug mode
debug_unity: objects_unity \
             test_unity/build/UnityTests.exe
	gdb ./test_unity/build/UnityTests.exe

#make objects_unity: executes the specified rules for the directory creation and compilation used for Unity testing
objects_unity: mkdirs_unity \
               test_unity/build/objs/unity.o test_unity/build/objs/unity_fixture.o \
               test_unity/build/objs/DumbExample.o test_unity/build/objs/TestDumbExample.o \
               test_unity/build/objs/LedDriver.o test_unity/build/objs/TestLedDriver.o \
               test_unity/build/objs/RuntimeErrorStub.o \
               test_unity/build/objs/AllUnityTests.o

#make mkdirs_unity: creates the directory test_unity/build/objs/ used to store the compiled .o files used for Unity testing
mkdirs_unity:
	mkdir -p test_unity/build/objs/

#make clean_unity: deletes UnityTests.exe and all the object files used for Unity testing
clean_unity:
#	rm -f test_unity/build/UnityTests.exe test_unity/build/objs/*.o
	rm -rf test_unity/build/

####################################### Unity compilation and linking #######################################

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

#rule to compile AllUnityTests.c into AllUnityTests.o
test_unity/build/objs/AllUnityTests.o: test_unity/AllUnityTests.c
	gcc -c -g -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ $^ -o $@

#rule to compile unity.c into unity.o
test_unity/build/objs/unity.o: unity/src/unity.c
	gcc -c -g -Iunity/src/ $^ -o $@

#rule to compile unity_fixture.c into unity_fixture.o
test_unity/build/objs/unity_fixture.o: unity/extras/fixture/src/unity_fixture.c
	gcc -c -g -Iunity/extras/fixture/src/ -Iunity/src/ -Iunity/extras/memory/src/ $^ -o $@

#rule to link the specified .o files into UnityTests.exe
test_unity/build/UnityTests.exe: test_unity/build/objs/unity_fixture.o test_unity/build/objs/unity.o \
                                 test_unity/build/objs/TestDumbExample.o test_unity/build/objs/DumbExample.o \
                                 test_unity/build/objs/TestLedDriver.o test_unity/build/objs/LedDriver.o \
                                 test_unity/build/objs/RuntimeErrorStub.o \
                                 test_unity/build/objs/AllUnityTests.o
	gcc $^ -o $@

####################################### CppUTest make rules #######################################

#make test_cpputest: executes the specified rules for CppUTest compilation/linking and runs the executable
test_cpputest: objects_cpputest \
               test_cpputest/build/CppUTestTests.exe
	@echo ""
	./test_cpputest/build/CppUTestTests.exe

#make debug_cpputest: executes the specified rules for CppUTest compilation/linking and runs the executable in gdb debug mode
debug_cpputest: objects_cpputest \
                test_cpputest/build/CppUTestTests.exe
	gdb ./test_cpputest/build/CppUTestTests.exe

#make objects_cpputest: executes the specified rules for the directory creation and compilation used for CppUTest testing
objects_cpputest: mkdirs_cpputest \
                  test_cpputest/build/objs/DummyDriver.o test_cpputest/build/objs/TestDummyDriver.o \
				  test_cpputest/build/objs/LightControllerSpy.o test_cpputest/build/objs/LightControllerSpyTest.o \
				  test_cpputest/build/objs/FakeTimeService.o test_cpputest/build/objs/FakeTimeServiceTest.o \
                  test_cpputest/build/objs/LightScheduler.o test_cpputest/build/objs/LightSchedulerTest.o \
				  test_cpputest/build/objs/RandomMinute.o test_cpputest/build/objs/RandomMinuteTest.o \
                  test_cpputest/build/objs/AllCppUTestTests.o

#make mkdirs_cpputest: creates the directory test_cpputest/build/objs/ used to store the compiled .o files used for CppUTest testing
mkdirs_cpputest:
	mkdir -p test_cpputest/build/objs/

#make clean_cpputest: deletes CppUTestTests.exe and all the object files used for CppUTest testing
clean_cpputest:
	rm -rf test_cpputest/build/

####################################### CppUTest compilation and linking #######################################

#rule to compile DummyDriver.c into DummyDriver.o
test_cpputest/build/objs/DummyDriver.o: src/03_DummyDriver/DummyDriver.c
	gcc -c -g -Iinclude/03_DummyDriver/ $^ -o $@

#rule to compile TestDummyDriver.cpp into TestDummyDriver.o
test_cpputest/build/objs/TestDummyDriver.o: test_cpputest/03_DummyDriver/TestDummyDriver.cpp
	g++ -c -g -Iinclude/03_DummyDriver/ -Icpputest/include/CppUTest/ $^ -o $@

#rule to compile LightControllerSpy.c into LightControllerSpy.o
test_cpputest/build/objs/LightControllerSpy.o: test_cpputest/04_LightScheduler/LightControllerSpy/LightControllerSpy.c
	gcc -c -g -Itest_cpputest/04_LightScheduler/LightControllerSpy/ -Iinclude/04_LightScheduler/LightController/ $^ -o $@

#rule to compile LightControllerSpyTest.ccp into LightControllerSpyTest.o
test_cpputest/build/objs/LightControllerSpyTest.o: test_cpputest/04_LightScheduler/LightControllerSpy/LightControllerSpyTest.cpp
	g++ -c -g -Icpputest/include/CppUTest/ -Itest_cpputest/04_LightScheduler/LightControllerSpy/ -Iinclude/04_LightScheduler/LightController/ $^ -o $@

#rule to compile FakeTimeService.c into FakeTimeService.o
test_cpputest/build/objs/FakeTimeService.o: test_cpputest/04_LightScheduler/FakeTimeService/FakeTimeService.c
	gcc -c -g -Itest_cpputest/04_LightScheduler/FakeTimeService/ -Iinclude/04_LightScheduler/TimeService/ -Iinclude/util/ $^ -o $@

#rule to compile FakeTimeServiceTest.cpp into FakeTimeServiceTest.o
test_cpputest/build/objs/FakeTimeServiceTest.o: test_cpputest/04_LightScheduler/FakeTimeService/FakeTimeServiceTest.cpp
	g++ -c -g -Icpputest/include/CppUTest/ -Itest_cpputest/04_LightScheduler/FakeTimeService/ -Iinclude/04_LightScheduler/TimeService/ -Iinclude/04_LightScheduler/ -Iinclude/04_LightScheduler/LightController/ -Iinclude/util/ $^ -o $@

#rule to compile RandomMinute.c into RandomMinute.o
test_cpputest/build/objs/RandomMinute.o: src/04_LightScheduler/RandomMinute/RandomMinute.c
	gcc -c -g -Iinclude/04_LightScheduler/RandomMinute/ $^ -o $@

#rule to compile RandomMinuteTest.cpp into RandomMinuteTest.o
test_cpputest/build/objs/RandomMinuteTest.o: test_cpputest/04_LightScheduler/RandomMinute/RandomMinuteTest.cpp
	g++ -c -g -Icpputest/include/CppUTest/ -Iinclude/04_LightScheduler/RandomMinute/ $^ -o $@

#rule to compile LightScheduler.c into LightScheduler.o
test_cpputest/build/objs/LightScheduler.o: src/04_LightScheduler/LightScheduler.c
	gcc -c -g -Iinclude/04_LightScheduler/ -Iinclude/04_LightScheduler/LightController/ -Iinclude/04_LightScheduler/TimeService/ -Iinclude/util/ $^ -o $@

#rule to compile LightSchedulerTest.ccp into LightSchedulerTest.o
test_cpputest/build/objs/LightSchedulerTest.o: test_cpputest/04_LightScheduler/LightSchedulerTest.cpp
	g++ -c -g -Icpputest/include/CppUTest/ -Iinclude/04_LightScheduler/ -Itest_cpputest/04_LightScheduler/LightControllerSpy/ -Iinclude/04_LightScheduler/LightController/ -Itest_cpputest/04_LightScheduler/FakeTimeService/ -Iinclude/04_LightScheduler/TimeService/ -Iinclude/util/ $^ -o $@

#rule to compile AllCppUTestTests.cpp into AllCppUTestTests.o
test_cpputest/build/objs/AllCppUTestTests.o: test_cpputest/AllCppUTestTests.cpp
	g++ -c -g -Icpputest/include/CppUTest/ $^ -o $@

#rule to link the specified .o files into CppUTestTests.exe
test_cpputest/build/CppUTestTests.exe: test_cpputest/build/objs/DummyDriver.o test_cpputest/build/objs/TestDummyDriver.o \
                                       test_cpputest/build/objs/LightControllerSpy.o test_cpputest/build/objs/LightControllerSpyTest.o \
                                       test_cpputest/build/objs/FakeTimeService.o test_cpputest/build/objs/FakeTimeServiceTest.o \
                                       test_cpputest/build/objs/LightScheduler.o test_cpputest/build/objs/LightSchedulerTest.o \
									   test_cpputest/build/objs/RandomMinute.o test_cpputest/build/objs/RandomMinuteTest.o \
                                       test_cpputest/build/objs/AllCppUTestTests.o
	g++ $^ -Lcpputest/lib -lCppUTest -o $@
