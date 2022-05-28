#include "libzzzjson.capi.h"
#include <stdio.h>

void print_jsonvalue(zj_Value *src)
{
    const zj_JSONType *t;
    t = zj_Type(src);
    if (t == 0)
        return;
    switch (*t)
    {
    case zj_JSONTypeArray:
    {
        zj_Value *next = zj_Begin(src);
        uint32_t size = zj_SizeOf(src);
        printf("(%d)[", size);
        int i = 0;
        while (next != 0)
        {   
            if(i > 0) printf(", ");
            i++;
            print_jsonvalue(next);
            next = zj_Next(next);
        }
        printf("]");
        break;
    }
    case zj_JSONTypeObject:
    {
        
        zj_Value *next = zj_Begin(src);
        uint32_t size = zj_SizeOf(src);
        printf("(%d){", size);
        int i = 0;
        while (next != 0)
        {
            if(i > 0) printf(", ");
            i++;
            printf(" %s: ", zj_GetUnEscapeKey(next));
            print_jsonvalue(next);
            next = zj_Next(next);
        }
        printf("}");
        break;
    }
     case zj_JSONTypeBool:
    {
        const bool *b = zj_GetBool(src);
        printf(*b ? "true" : "false");
        break;
    }
    case zj_JSONTypeNull:
    {
        printf("null");
        break;
    }
    case zj_JSONTypeString:
    {
        uint32_t size = src->N->Len;
        const char *str = zj_GetUnEscapeStr(src);
        if (str == 0){
            break;
        }
        printf("(%d)%s",size, str);
        break;
    }
    case zj_JSONTypeNumber:
    {
        const char *str = zj_GetNumStr(src);
        if (str == 0)
            break;
        
        printf(str);
        break;
    }
    default:
        break;
    }
}
// zj_Value* get_JSON_Position( zj_Allocator *allocator,int x, int y, int z){

//     zj_Value *position = zj_NewValue(allocator);
//     zj_SetObj(position);
//     zj_Value *x_key = zj_NewValue(allocator);
//     zj_SetKeyFast(x_key, "x");
//     zj_SetInt(x_key, x);

//     zj_ObjAddFast(position, x_key);

//     zj_Value *y_key = zj_NewValue(allocator);
//     zj_SetKeyFast(y_key, "y");
//     zj_SetInt(y_key, x);

//     zj_ObjAddFast(position, y_key);

//     zj_Value *z_key = zj_NewValue(allocator);
//     zj_SetKeyFast(z_key, "z");
//     zj_SetInt(z_key, z);
    
//     zj_ObjAddFast(position, z_key);
//     return position;
// }

zj_Value* get_Buggy(zj_Allocator *allocator){
    zj_Value* position = zzz_init_object(allocator);
    zzz_add_string_property(position, "string", "{\"message\":\"Hello, World\"}");
    return position;
}
zj_Value* get_JSON_Position( zj_Allocator *allocator,int x, int y, int z){

    zj_Value* position = zzz_init_object(allocator);
    zzz_add_int_property(position, "x", x);
    zzz_add_int_property(position, "y", y);
    zzz_add_int_property(position, "z", z);
    return position;
}
int main()
{
    // const char *src_json = "[\"\\\\\\tHello\",{\"key\":true},false,{\"key1\":true},[null,false,[],"
    //                        "true],[\"\",123,\"str\"],null]";
    // zj_Allocator *A = zj_NewAllocator();
    // zj_Value *src_v = zj_NewValue(A);
    // bool ret = zj_ParseFast(src_v, src_json);

    // if (ret != true)
    // {
    //     printf("ParseFast Fail!\n");
    //     return 1;
    // }

    zj_Allocator* allocator = zzz_init();
    // zj_Value *src = zj_NewValue(allocator);
    // const char *json = "[\"\\\\\\tHello\",{\"key\":true},false,{\"key1\":true},[null,false,[],"
    //                        "true],[\"\",123,\"str\"],null]";
    // bool ret = zj_ParseFast(src, json);
    // if (ret != true)
    // {
    //     printf("ParseFast Fail!\n");
    //     return 1;
    // }

    // zj_Value * src = get_JSON_Position(allocator, 10, 20, 30);
    zj_Value* src = get_Buggy(allocator);
    print_jsonvalue(src);
    const char *src_json;
    size_t size = zzz_stringify(src, &src_json);
    // //clone string 
    // char *new_json = calloc(size + 1, sizeof(char));
    // memcpy(new_json, src_json, size);
    // //print original
    printf("\n\n(%ld)%s\n", size, src_json);
    // //clean original
    zzz_close(allocator);
    // //print clone 
    // printf("\n\n(%ld)%s\n", size, new_json);
    // //clean clone
    // free(new_json);

    // zj_Allocator* allocator = zzz_init();
    // const char *src_json;
    // size_t size = zzz_stringify_string(allocator, "\tXablau\nHello", &src_json);
    // printf("\n\n(%ld)%s\n", size, src_json);
    // zzz_close(allocator);

    return 0;
}
