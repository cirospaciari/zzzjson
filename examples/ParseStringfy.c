#include "libzzzjson.min.h"
#include <stdio.h>


int main()
{
    zj_Allocator* allocator = zzz_init();
    const char *src_json;
    size_t size = zzz_stringify_string(allocator, "\tXablau\nHello", &src_json);
    printf("\n\n(%ld)%s\n", size, src_json);
    zzz_close(allocator);
}