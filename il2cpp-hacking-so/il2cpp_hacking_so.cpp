#include <pthread.h>
#include <jni.h>

#include "logger.h"
#include "utils.h"
#include "il2cpp.h"


void* main_thread(void*) {
    do {
        sleep(1);
    } while (!Utils::isLibraryLoaded("libil2cpp.so"));


}

__attribute__((constructor))
void libil2cpp_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, main_thread, NULL);
}
