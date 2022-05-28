#include "libzzzjson.h"
#ifndef zj_JSON_CAPI_H
#define zj_JSON_CAPI_H



zj_Allocator* zzz_init();
zj_Value* zzz_init_object(zj_Allocator* alloc);

void zzz_add_int_property(zj_Value* obj, const char* name, int64_t value);
void zzz_add_float_property(zj_Value* obj, const char* name, double value);
void zzz_add_string_property(zj_Value* obj, const char* name, const char* value);
void zzz_add_null_property(zj_Value* obj, const char* name);
void zzz_add_bool_property(zj_Value* obj, const char* name, bool value);
void zzz_add_property(zj_Value* obj, const char* name, zj_Value* array_or_obj);

zj_Value* zzz_init_array(zj_Allocator* alloc);
void zzz_array_push_int(zj_Value* array, int64_t value);
void zzz_array_push_float(zj_Value* array, double value);
void zzz_array_push_string(zj_Value* array, const char* value);
void zzz_array_push_null(zj_Value* array);
void zzz_array_push_bool(zj_Value* array, bool value);
void zzz_array_push_value(zj_Value* array, zj_Value* array_or_obj);

size_t zzz_stringify(zj_Value* value, const char** output);
size_t zzz_stringify_int(zj_Allocator* alloc, int64_t value, const char** output);
size_t zzz_stringify_float(zj_Allocator* alloc, double value, const char** output);
size_t zzz_stringify_string(zj_Allocator* alloc, const char* value, const char** output);

void zzz_close(zj_Allocator* alloc);

#endif