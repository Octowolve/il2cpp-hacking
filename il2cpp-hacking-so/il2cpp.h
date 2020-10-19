#pragma once
#include "types.h"
#include <dlfcn.h>

typedef Il2CppString* (*il2cpp_string_new_t)(const char* str);
typedef Il2CppImage* (*il2cpp_assembly_get_image_t)(Il2CppAssembly* assembly);
typedef Il2CppAssembly* (*il2cpp_domain_assembly_open_t)(Il2CppDomain* domain, const char* name);
typedef Il2CppClass* (*il2cpp_class_from_name_t)(Il2CppImage* assembly, const char* name_space, const char* name);
typedef Il2CppClass* (*il2cpp_class_from_index_t)(Il2CppImage* assembly, size_t index);
typedef Il2CppMethod* (*il2cpp_class_get_methods_t)(Il2CppClass* klass, void** iter);
typedef Il2CppMethod* (*il2cpp_class_get_method_from_name_t)(Il2CppClass* klass, const char* name, int paramcount);
typedef Il2CppObject* (*il2cpp_runtime_invoke_t)(Il2CppMethod* method, void* obj, void** params, Il2CppObject* exec);
typedef Il2CppArray* (*il2cpp_array_new_t)(Il2CppClass* klass, uint32_t length);
typedef Il2CppField* (*il2cpp_class_get_fields_t)(Il2CppClass* klass, void** iter);
typedef Il2CppField* (*il2cpp_class_get_field_from_name_t)(Il2CppClass* klass, const char* name);
typedef void (*il2cpp_field_get_value_t)(Il2CppObject* object, Il2CppField* field, void* out);
typedef void (*il2cpp_field_set_value_t)(Il2CppObject* object, Il2CppField* field, void* value);
typedef void (*il2cpp_field_static_get_value_t)(Il2CppField* field, void* value);
typedef void (*il2cpp_field_static_set_value_t)(Il2CppField* field, void* value);
typedef uint32_t(*il2cpp_method_get_token_t) (Il2CppMethod* method);
typedef uint32_t(*il2cpp_object_get_size_t)(Il2CppObject* object);
typedef uint32_t(*il2cpp_array_get_byte_length_t)(Il2CppArray* arr);
typedef uint32_t(*il2cpp_array_get_length_t)(Il2CppArray* arr);
typedef const char16_t* (*il2cpp_string_chars_t) (Il2CppString* string);
typedef const PropertyInfo* (*il2cpp_class_get_property_from_name_t) (Il2CppClass* klass, const char* name);
typedef Il2CppMethod* (*il2cpp_property_get_get_method_t) (PropertyInfo* prop);

struct Il2cppRuntime {
	static void* GetIL2CPPModule()
	{
		auto mod = dlopen("libil2cpp.so", RTLD_LAZY);
		if (mod != NULL) return mod;
	}
	static Il2CppImage* il2cpp_assembly_get_image(const char* name)
	{
		auto il2cpp_assembly_get_image = (il2cpp_assembly_get_image_t)dlsym(GetIL2CPPModule(), "il2cpp_assembly_get_image");
		auto assembly = (il2cpp_domain_assembly_open_t)dlsym(GetIL2CPPModule(), "il2cpp_domain_assembly_open");

		return il2cpp_assembly_get_image(assembly(NULL, name));
	}
	static Il2CppClass* il2cpp_class_from_name(Il2CppImage* assembly, const char* name_space, const char* name)
	{
		auto il2cpp_class_from_name = (il2cpp_class_from_name_t)dlsym(GetIL2CPPModule(), "il2cpp_class_from_name");

		return il2cpp_class_from_name(assembly, name_space, name);
	}
	static Il2CppClass* il2cpp_class_from_index(Il2CppImage* assembly, size_t index)
	{
		auto il2cpp_class_from_index = (il2cpp_class_from_index_t)dlsym(GetIL2CPPModule(), "il2cpp_image_get_class");

		return il2cpp_class_from_index(assembly, index);
	}
	static uint32_t il2cpp_method_get_token(Il2CppMethod* method)
	{
		auto il2cpp_method_get_token = (il2cpp_method_get_token_t)dlsym(GetIL2CPPModule(), "il2cpp_method_get_token");

		return il2cpp_method_get_token(method);
	}
	static Il2CppMethod* il2cpp_class_get_method_from_name(Il2CppClass* klass, const char* name, int paramcount)
	{
		auto il2cpp_class_get_method_from_name = (il2cpp_class_get_method_from_name_t)dlsym(GetIL2CPPModule(), "il2cpp_class_get_method_from_name");

		return il2cpp_class_get_method_from_name(klass, name, paramcount);
	}
	static Il2CppClass* il2cpp_class_from_token(Il2CppImage* assembly, uint32_t token)
	{
		for (uint32_t x = 0; x < assembly->typeCount; x++)
		{
			Il2CppClass* klass = il2cpp_class_from_index(assembly, x);
			if (klass == nullptr)
				continue;

			if (klass->token == 0)
				continue;

			if (klass->token == token)
				return klass;
		}
		return nullptr;
	}
	static Il2CppField* il2cpp_class_get_fields(Il2CppClass* klass, void** iterator)
	{
		auto il2cpp_class_get_fields = (il2cpp_class_get_fields_t)dlsym(GetIL2CPPModule(), "il2cpp_class_get_fields");

		return il2cpp_class_get_fields(klass, iterator);
	}
	static Il2CppMethod* il2cpp_class_get_methods(Il2CppClass* klass, void** iterator)
	{
		auto il2cpp_class_get_methods = (il2cpp_class_get_methods_t)dlsym(GetIL2CPPModule(), "il2cpp_class_get_methods");

		return il2cpp_class_get_methods(klass, iterator);
	}
	static Il2CppMethod* il2cpp_class_get_method_from_token(Il2CppClass* klass, uint32_t token)
	{
		void* iterator = nullptr;
		while (auto method = il2cpp_class_get_methods(klass, &iterator)) {
			if (method == nullptr) continue;
			if (il2cpp_method_get_token(method) == token) {
				return method;
			}
		}
		return nullptr;
	}
	static Il2CppField* il2cpp_field_from_token(Il2CppClass* klass, uint32_t token)
	{
		void* iter = nullptr;
		Il2CppField* field;
		while (field = il2cpp_class_get_fields(klass, &iter)) {
			if (field->token == 0)
				continue;

			if (field->token == token)
				return field;
		}
		return nullptr;
	}
	static Il2CppField* il2cpp_field_from_name(Il2CppClass* klass, const char* name)
	{
		auto il2cpp_field_from_name = (il2cpp_class_get_field_from_name_t)dlsym(GetIL2CPPModule(), "il2cpp_class_get_field_from_name");

		return il2cpp_field_from_name(klass, name);
	}
	static Il2CppMethod* il2cpp_property_get_get_method(Il2CppClass* klass, const char* name) {
		il2cpp_class_get_property_from_name_t il2cpp_string_chars = (il2cpp_class_get_property_from_name_t)dlsym(GetIL2CPPModule(), "il2cpp_class_get_property_from_name");
		il2cpp_property_get_get_method_t il2cpp_string_chars_2 = (il2cpp_property_get_get_method_t)dlsym(GetIL2CPPModule(), "il2cpp_property_get_get_method");
		return il2cpp_string_chars_2((PropertyInfo*)il2cpp_string_chars(klass, name));
	}
	static void il2cpp_field_get_value(Il2CppObject* object, Il2CppField* field, void* out)
	{
		auto il2cpp_field_get_value = (il2cpp_field_get_value_t)dlsym(GetIL2CPPModule(), "il2cpp_field_get_value");

		return il2cpp_field_get_value(object, field, out);
	}
	static void il2cpp_field_set_value(Il2CppObject* object, Il2CppField* field, void* value)
	{
		auto il2cpp_field_set_value = (il2cpp_field_set_value_t)dlsym(GetIL2CPPModule(), "il2cpp_field_set_value");

		return il2cpp_field_set_value(object, field, value);
	}
	static void il2cpp_field_static_get_value(Il2CppField* field, void* value)
	{
		auto il2cpp_field_static_get_value = (il2cpp_field_static_get_value_t)dlsym(GetIL2CPPModule(), "il2cpp_field_static_get_value");

		return il2cpp_field_static_get_value(field, value);
	}
	static void il2cpp_field_static_set_value(Il2CppField* field, void* value)
	{
		auto il2cpp_field_static_set_value = (il2cpp_field_static_set_value_t)dlsym(GetIL2CPPModule(), "il2cpp_field_static_set_value");

		return il2cpp_field_static_set_value(field, value);
	}
	static void* GetFieldValue(Il2CppObject* object, Il2CppField* info)
	{
		void* result = nullptr;
		il2cpp_field_get_value(object, info, &result);
		return result;
	}
	static void SetFieldValue(Il2CppObject* object, Il2CppField* field, void* value)
	{
		return il2cpp_field_set_value(object, field, value);
	}
	static void* GetStaticFieldValue(Il2CppField* field)
	{
		void* result = nullptr;
		il2cpp_field_static_get_value(field, &result);
		return result;
	}
	static void SetStaticFieldValue(Il2CppField* field, void* value)
	{
		return il2cpp_field_static_set_value(field, value);
	}
	static Il2CppObject* il2cpp_runtime_invoke(Il2CppMethod* method, void* obj, void** params)
	{
		auto il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t)dlsym(GetIL2CPPModule(), "il2cpp_runtime_invoke");

		return il2cpp_runtime_invoke(method, obj, params, NULL);
	}
	static Il2CppArray* il2cpp_array_new(Il2CppClass* klass, uint32_t length)
	{
		auto il2cpp_array_new = (il2cpp_array_new_t)dlsym(GetIL2CPPModule(), "il2cpp_array_new");

		return il2cpp_array_new(klass, length);
	}
	static Il2CppString* il2cpp_string_new(const char* str)
	{
		auto il2cpp_string = (il2cpp_string_new_t)dlsym(GetIL2CPPModule(), "il2cpp_string_new");

		return il2cpp_string(str);
	}
	static const char16_t* il2cpp_string_chars(Il2CppString* string)
	{
		auto il2cpp_string_chars = (il2cpp_string_chars_t)dlsym(GetIL2CPPModule(), "il2cpp_string_chars");

		return il2cpp_string_chars(string);
	}
	static uint32_t il2cpp_object_get_size(Il2CppObject* object)
	{
		auto il2cpp_object_get_size = (il2cpp_object_get_size_t)dlsym(GetIL2CPPModule(), "il2cpp_object_get_size");

		return il2cpp_object_get_size(object);
	}
	static uint32_t il2cpp_array_get_byte_length(Il2CppArray* array)
	{
		auto il2cpp_array_get_byte_length = (il2cpp_array_get_byte_length_t)dlsym(GetIL2CPPModule(), "il2cpp_array_get_byte_length");

		return il2cpp_array_get_byte_length(array);
	}
	static uint32_t il2cpp_array_get_length(Il2CppArray* array)
	{
		auto il2cpp_array_get_length = (il2cpp_array_get_length_t)dlsym(GetIL2CPPModule(), "il2cpp_array_length");

		return il2cpp_array_get_length(array);
	}
	template <typename T>
	static inline T GetMethod(DWORD Offset)
	{
		return (T)((uint8_t*)GetIL2CPPModule() + Offset);
	}
	template <typename T>
	static inline T GetField(void* obj, DWORD Offset)
	{
		return *(T*)((uint8_t*)obj + Offset);
	}
	template <typename T>
	static void SetField(void* obj, DWORD Offset, T val)
	{
		*(T*)((uint8_t*)obj + Offset) = val;
	}
};

