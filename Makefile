CAPI_EXAMPLE_FILES := HelloWorld ParseStringfy
LIBRARY_NAME := libzzzjson
CFLAGS=-Wall -Wno-excess-initializers -O3 --signed-char -I.
default:
	$(MAKE) capi
	$(CC) $(CFLAGS) examples/HelloWorld.c *.a -o examples/HelloWorld

capi:
	${MAKE} clean
	$(CC) $(CFLAGS) -c -flto -fPIC $(LIBRARY_NAME).c -o $(LIBRARY_NAME).o
	$(AR) rvs $(LIBRARY_NAME).a $(LIBRARY_NAME).o

shared:
	$(MAKE) clean
	$(CC) $(CFLAGS) -c -flto -fPIC $(LIBRARY_NAME).c -o $(LIBRARY_NAME).o
	$(CC) $(CFLAGS) -shared -o $(LIBRARY_NAME).so  $(LIBRARY_NAME).o -Wl,-rpath=$(CURDIR)/zzzjson

clean:
	rm -f *.o $(CAPI_EXAMPLE_FILES) $(LIBRARY_NAME).a $(LIBRARY_NAME).so
all:
	$(MAKE) shared
	for FILE in $(CAPI_EXAMPLE_FILES); do $(CC) $(CFLAGS) examples/$$FILE.c *.a -o examples/$$FILE & done; \
	wait
	