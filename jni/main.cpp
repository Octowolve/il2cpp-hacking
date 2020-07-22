#include <pthread.h>
#include <jni.h>
#include <memory.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>

#include "includes/utils.h"
#include "includes/il2cpp.h"
#include "includes/logger.h"


void* main_thread(void*){
    do {
        sleep(1);
    } while (!isLibraryLoaded("libil2cpp.so"));
    il2cpp_handle = dlopen("libil2cpp.so", RTLD_LAZY);
    if(!il2cpp_handle){
        LOGE("Cannot open library: %s", dlerror());
    }
    else{
        il2cpp_domain_get_ il2cpp_domain_get = (il2cpp_domain_get_)dlsym(il2cpp_handle, "il2cpp_domain_get");
        il2cpp_domain_get_assemblies_ il2cpp_domain_get_assemblies = (il2cpp_domain_get_assemblies_)dlsym(il2cpp_handle, "il2cpp_domain_get_assemblies");
        il2cpp_assembly_get_image_ il2cpp_assembly_get_image = (il2cpp_assembly_get_image_)dlsym(il2cpp_handle, "il2cpp_assembly_get_image");
        il2cpp_class_from_name_ il2cpp_class_from_name = (il2cpp_class_from_name_)dlsym(il2cpp_handle, "il2cpp_class_from_name");
        il2cpp_class_get_method_from_name_ il2cpp_class_get_method_from_name = (il2cpp_class_get_method_from_name_)dlsym(il2cpp_handle, "il2cpp_class_get_method_from_name");
        sleep(2);
        Il2CppDomain* domain = il2cpp_domain_get();
        unsigned long ass_len = 0;
        const Il2CppAssembly** assembly_list = il2cpp_domain_get_assemblies(domain, &ass_len);
        while(strcmp((*assembly_list)->aname.name, "Assembly-CSharp") != 0){
            LOGD("Assembly name: %s", (*assembly_list)->aname.name);
            assembly_list++;
        }
        const Il2CppImage* image = il2cpp_assembly_get_image(*assembly_list);
        Il2CppClass* clazz = il2cpp_class_from_name(image, "<Namespace> ", "<Classname> Player");

        //octo_hook((unsigned long)il2cpp_class_get_method_from_name(clazz, "<Your Method> PlayerUpdate", 1)->methodPointer, (void*)Player_Update, (void**)&old_Player_Update)
    }

}

__attribute__((constructor))
void libil2cpp_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, main_thread, NULL);
}
