CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c18 -O2 
LIBS=-lnumc


INCLUDE_PATH=/usr/include
LIB_PATH=/usr/lib64

TARGET=libcsv.a
OUTPUT=release


MODULES += csv.o


TEST += test.o
TEST += csv.o


all: env $(MODULES)
	ar -crs $(OUTPUT)/$(TARGET) $(MODULES)


%.o:
	$(CC) $(CFLAGS) -c $<


-include dep.list


exec: all
	$(OUTPUT)/$(TARGET)


.PHONY: env dep clean install


test: env $(TEST)
	$(CC) $(CFLAGS) $(TEST) $(LIBS) -o $(OUTPUT)/test

	$(OUTPUT)/test


install:
	cp -v $(OUTPUT)/$(TARGET) $(LIB_PATH)/$(TARGET)
	mkdir -p $(INCLUDE_PATH)/numc/
	cp -v src/include/*.h $(INCLUDE_PATH)/numc/


dep:
	$(CC) -MM src/*.c test/*.c > dep.list


env:
	mkdir -pv $(OUTPUT)


clean: 
	rm -rvf $(OUTPUT)
	rm -vf ./*.o



