CAPI_EXAMPLE_FILES := HelloWorld ParseStringfy
LIBRARY_NAME := libzzzjson
CFLAGS=-Wall -Wno-excess-initializers -O3 --signed-char -I.
default:
	$(MAKE) capi
	$(CC) $(CFLAGS) examples/HelloWorld.c bin/$(LIBRARY_NAME).a -o bin/HelloWorld

capi:
	${MAKE} clean
	$(CC) $(CFLAGS) -c -flto -fPIC $(LIBRARY_NAME).c -o bin/$(LIBRARY_NAME).o
	$(AR) rvs bin/$(LIBRARY_NAME).a bin/$(LIBRARY_NAME).o

shared:
	${MAKE} capi
	$(CC) $(CFLAGS) -c -flto -fPIC $(LIBRARY_NAME).c -o bin/$(LIBRARY_NAME).o
	$(CC) $(CFLAGS) -shared -o bin/$(LIBRARY_NAME).so  bin/$(LIBRARY_NAME).o

clean:
	rm -f *.o bin/*
all:
	$(MAKE) shared
	for FILE in $(CAPI_EXAMPLE_FILES); do $(CC) $(CFLAGS) examples/$$FILE.c bin/$(LIBRARY_NAME).a -o bin/$$FILE & done; \
	wait
	