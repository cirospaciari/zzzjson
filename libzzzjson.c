
#include "libzzzjson.h"
#include "libzzzjson.capi.h"

//https://www.rfc-editor.org/rfc/rfc7159#section-6

zj_Allocator* zzz_init(){
    return zj_NewAllocator();
}

zj_Value* zzz_init_object(zj_Allocator* alloc){
    zj_Value* value = zj_NewValue(alloc);
    zj_SetObj(value);
    return value;
}

void zzz_add_int_property(zj_Value* obj, const char* name, int64_t value){
    zj_Value* prop = zj_NewValue(obj->A);
    zj_SetKeyEscape(prop, name);
    zj_SetLongLong(prop, value);
    zj_ObjAddFast(obj, prop);
}
void zzz_add_float_property(zj_Value* obj, const char* name, double value){
    zj_Value* prop = zj_NewValue(obj->A);
    zj_SetKeyEscape(prop, name);
    zj_SetDouble(prop, value);
    zj_ObjAddFast(obj, prop);
}

void zzz_add_string_property(zj_Value* obj, const char* name, const char* value){
    zj_Value* prop = zj_NewValue(obj->A);
    zj_SetKeyEscape(prop, name);
    zj_SetStrEscape(prop, value);
    zj_ObjAddFast(obj, prop);
}
void zzz_add_bool_property(zj_Value* obj, const char* name, bool value){
    zj_Value* prop = zj_NewValue(obj->A);
    zj_SetKeyEscape(prop, name);
    zj_SetBool(prop, value);
    zj_ObjAddFast(obj, prop);
}
void zzz_add_null_property(zj_Value* obj, const char* name){
    zj_Value* prop = zj_NewValue(obj->A);
    zj_SetKeyEscape(prop, name);
    zj_SetNull(prop);
    zj_ObjAddFast(obj, prop);
}

void zzz_add_property(zj_Value* obj, const char* name, zj_Value* array_or_obj){
    zj_SetKeyEscape(array_or_obj, name);
    zj_ObjAddFast(obj, array_or_obj);
}

zj_Value* zzz_init_array(zj_Allocator* alloc){
    zj_Value* value = zj_NewValue(alloc);
    zj_SetArray(value);
    return value;
}

void zzz_array_push_int(zj_Value* array, int64_t value){
    zj_Value* item = zj_NewValue(array->A);
    zj_SetLongLong(item, value);
    zj_ArrayAddFast(array, item);
}

void zzz_array_push_float(zj_Value* array, double value){
    zj_Value* item = zj_NewValue(array->A);
    zj_SetDouble(item, value);
    zj_ArrayAddFast(array, item);
}

void zzz_array_push_string(zj_Value* array, const char* value){
    zj_Value* item = zj_NewValue(array->A);
    zj_SetStrEscape(item, value);
    zj_ArrayAddFast(array, item);
}

void zzz_array_push_bool(zj_Value* array, bool value){
    zj_Value* item = zj_NewValue(array->A);
    zj_SetBool(item, value);
    zj_ArrayAddFast(array, item);
}

void zzz_array_push_null(zj_Value* array){
    zj_Value* item = zj_NewValue(array->A);
    zj_SetNull(item);
    zj_ArrayAddFast(array, item);
}

void zzz_array_push_value(zj_Value* array, zj_Value* array_or_obj){
    zj_ArrayAddFast(array, array_or_obj);
}

size_t zzz_stringify(zj_Value* value, const char** output){
    return zj_Stringify(value, output);
}

size_t zzz_stringify_int(zj_Allocator* alloc, int64_t value, const char** output){
    zj_Value* item = zj_NewValue(alloc);
    zj_SetLongLong(item, value);
    return zj_Stringify(item, output);
}
size_t zzz_stringify_float(zj_Allocator* alloc, double value, const char** output){
    zj_Value* item = zj_NewValue(alloc);
    zj_SetDouble(item, value);
    return zj_Stringify(item, output);
}
size_t zzz_stringify_string(zj_Allocator* alloc, const char* value, const char** output){
    zj_Value* item = zj_NewValue(alloc);
    zj_SetStrEscape(item, value);
    return zj_Stringify(item, output);
}

void zzz_close(zj_Allocator* alloc){
    zj_ReleaseAllocator(alloc);
}