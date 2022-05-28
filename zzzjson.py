import cffi
from datetime import datetime
ffi = cffi.FFI()
ffi.cdef("""
typedef struct zj_Allocator zj_Allocator;
typedef struct zj_Value zj_Value;

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
""")

lib = ffi.dlopen("./libzzzjson.so")


def serialize_obj(alloc, obj):
    if not isinstance(obj, dict) and hasattr(obj, "__dict__"):
        obj = obj.__dict__
    if not isinstance(obj, dict):
        return None

    json_obj = lib.zzz_init_object(alloc)
    
    for key in obj:
        name = key.encode("utf-8")
        value = obj[key]
        if value == None:
            lib.zzz_add_null_property(json_obj, name)
        elif isinstance(value, bool):
            lib.zzz_add_bool_property(json_obj, name, value)
        elif isinstance(value, int):
            lib.zzz_add_int_property(json_obj, name, ffi.cast("int64_t", value))
        elif isinstance(value, float):
            lib.zzz_add_float_property(json_obj, name, ffi.cast("double", value))
        elif isinstance(value, str):
            data = value.encode("utf-8")
            lib.zzz_add_string_property(json_obj, name, data)
        elif isinstance(value,(list, tuple)):
            json_array_item = serialize_array(alloc, value)
            lib.zzz_add_property(json_obj, name, json_array_item)
        elif isinstance(value, dict):
            json_obj_item = serialize_obj(alloc, value)
            if json_obj_item != None:
                lib.zzz_add_property(json_obj, name, json_obj_item)
        elif isinstance(value, datetime):
            data = (value.isoformat() + "Z").encode("utf-8")
            lib.zzz_add_string_property(json_obj, name, data)
        elif hasattr(value, "__dict__"):
            json_obj_item = serialize_obj(alloc, value.__dict__)
            if json_obj_item != None:
                lib.zzz_add_property(json_obj, name, json_obj_item)

    return json_obj

def serialize_array(alloc, array):
    json_array = lib.zzz_init_array(alloc)
    for value in array:
        if value == None:
            lib.zzz_array_push_null(json_array)
        elif isinstance(value, bool):
            lib.zzz_array_push_bool(json_array, value)
        elif isinstance(value, int):
            lib.zzz_array_push_int(json_array, ffi.cast("int64_t", value))
        elif isinstance(value, float):
            lib.zzz_array_push_float(json_array, ffi.cast("double", value))
        elif isinstance(value, str):
            data = value.encode("utf-8")
            lib.zzz_array_push_string(json_array, data)
        elif isinstance(value,(list, tuple)):
            json_array_item = serialize_array(alloc, value)
            lib.zzz_array_push_value(json_array, json_array_item)
        elif isinstance(value, dict):
            json_obj_item = serialize_obj(alloc, value)
            if json_obj_item != None:
                lib.zzz_array_push_value(json_array, json_obj_item)
        elif isinstance(value, datetime):
            data = (value.isoformat() + "Z").encode("utf-8")
            lib.zzz_array_push_string(json_array, data)
        elif hasattr(value, "__dict__"):
            json_obj_item = serialize_obj(alloc, value.__dict__)
            if json_obj_item != None:
                lib.zzz_array_push_value(json_array, json_obj_item)

    return json_array


def parse(json):
    #(1.0).is_integer() usar para converter pra int ou alterar lib para em vez de Number usar Int e Float (validar no parse mas cuidado com 1.121e+37 e 1.121e-37 por exemplo)
    pass
    
def stringify(value):
    alloc = lib.zzz_init()
    buffer = ffi.new("char**")
    result = None

    if value == None:
        result = "null"
    elif isinstance(value, bool):
        result = "true" if value else "false"
    elif isinstance(value, dict):
        json_obj = serialize_obj(alloc, value)
        if json_obj != None:
            size = lib.zzz_stringify(json_obj, buffer)
            buffer_address = ffi.addressof(buffer, 0)[0]
            if buffer_address != ffi.NULL:
                result = ffi.string(buffer_address, size).decode("utf-8")

    elif isinstance(value,(list, tuple)):
        json_array = serialize_array(alloc, value)
        size = lib.zzz_stringify(json_array, buffer)
        buffer_address = ffi.addressof(buffer, 0)[0]
        if buffer_address != ffi.NULL:
            result = ffi.string(buffer_address, size).decode("utf-8")

    elif isinstance(value, int):
        size = lib.zzz_stringify_int(alloc, ffi.cast("int64_t", value), buffer)
        buffer_address = ffi.addressof(buffer, 0)[0]
        if buffer_address != ffi.NULL:
            result = ffi.string(buffer_address, size).decode("utf-8")

    elif isinstance(value, float):
        size = lib.zzz_stringify_float(alloc, ffi.cast("double", value), buffer)
        buffer_address = ffi.addressof(buffer, 0)[0]
        if buffer_address != ffi.NULL:
            result = ffi.string(buffer_address, size).decode("utf-8")

    elif isinstance(value, datetime):
        data = (value.isoformat() + "Z").encode("utf-8")
        size = lib.zzz_stringify_string(alloc, data, buffer)
        buffer_address = ffi.addressof(buffer, 0)[0]
        if buffer_address != ffi.NULL:
            result = ffi.string(buffer_address, size).decode("utf-8")

    elif isinstance(value, str):
        data = value.encode("utf-8")
        size = lib.zzz_stringify_string(alloc, data, buffer)
        buffer_address = ffi.addressof(buffer, 0)[0]
        if buffer_address != ffi.NULL:
            result = ffi.string(buffer_address, size).decode("utf-8")
    elif hasattr(value, "__dict__"):
        json_obj = serialize_obj(alloc, value.__dict__)
        if json_obj != None:
            size = lib.zzz_stringify(json_obj, buffer)
            buffer_address = ffi.addressof(buffer, 0)[0]
            if buffer_address != ffi.NULL:
                result = ffi.string(buffer_address, size).decode("utf-8")
    
    lib.zzz_close(alloc)
    return result