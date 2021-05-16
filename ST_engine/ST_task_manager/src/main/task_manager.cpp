/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <task_manager.hpp>
#include <fstream>
#include <sstream>

static bool singleton_initialized;

#ifdef _MSC_VER
#include <Windows.h>
#include <malloc.h>
#include <cstdio>

typedef BOOL (WINAPI *LPFN_GLPI)(
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
        PDWORD);


// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest)?1:0);
        bitTest/=2;
    }

    return bitSetCount;
}

/**
 * Function taken from MSDN - https://docs.microsoft.com/en-us/windows/desktop/api/sysinfoapi/nf-sysinfoapi-getlogicalprocessorinformation
 * @return number of physical processing cores
 */
int get_cpu_core_count(){
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = nullptr;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = nullptr;
    DWORD returnLength = 0;
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;

    glpi = (LPFN_GLPI) GetProcAddress(
            GetModuleHandle(TEXT("kernel32")),
            "GetLogicalProcessorInformation");
    if (nullptr == glpi) {
        return (1);
    }

    while (!done){
        auto rc = static_cast<DWORD>(glpi(buffer, &returnLength));

        if (FALSE == rc){
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER){
                if (buffer)
                    free(buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
                        returnLength);

                if (nullptr == buffer){
                    return (2);
                }
            }
            else{
                return (3);
            }
        }
        else{
            done = TRUE;
        }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength){
        switch (ptr->Relationship){
            case RelationNumaNode:
                // Non-NUMA systems report a single record of this type.
                numaNodeCount++;
                break;

            case RelationProcessorCore:
                processorCoreCount++;

                // A hyperthreaded core supplies more than one logical processor.
                logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
                break;

            case RelationCache:
                // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache.
                Cache = &ptr->Cache;
                if (Cache->Level == 1){
                    processorL1CacheCount++;
                }
                else if (Cache->Level == 2){
                    processorL2CacheCount++;
                }
                else if (Cache->Level == 3){
                    processorL3CacheCount++;
                }
                break;

            case RelationProcessorPackage:
                // Logical processors share a physical package.
                processorPackageCount++;
                break;

            default:
                break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }
    free(buffer);
    return static_cast<int>(processorCoreCount);
}
#else
#include <unistd.h>

int get_cpu_core_count() {
    int cores = 0;
    std::ifstream file;
    file.open("/proc/cpuinfo");
    if(file.is_open()){
        std::string temp;
        while(!file.eof()){
            getline(file, temp);
            if(!temp.empty()) {
                if(temp.find("cpu cores") != std::string::npos){
                    std::stringstream extract;
                    extract << temp;
                    std::string word;
                    while(!extract.fail()){
                        extract >> word;
                        if(std::isdigit(word.at(0))){
                            std::stringstream convert;
                            convert << word;
                            convert >> cores;
                        }
                    }
                    break;
                }
            }
        }
        file.close();
    }
    return cores;
}
#endif

/**
 * The function each task thread runs.
 * @param arg A pointer to the task_manager.
 * @return Always 0. (This function only returns when at engine-shutdown).
 */
int task_manager::task_thread(task_manager* self){
	ST::task* work;
	while(self->run_threads){
	    self->work_sem->wait();
        if(self->global_task_queue.try_dequeue(work)){ //get a function pointer and data
            task_manager::do_work(work);
        }
	}
    return 0;
}

/**
 * Runs a function pointer from a task with it's associated data.
 * Waits for any other tasks that are dependencies to the current one and waits for them.
 * @param work The ST::task object containing job data.
 */
void task_manager::do_work(ST::task* work) {
    if(work->dependency != nullptr){ //wait for dependency to finish
        work->dependency->wait();
        delete work->dependency;
    }
    work->task_func(work->data); // call it
    if(work->lock != nullptr) {
        work->lock->notify(); //increment the semaphore
    }
    delete work;
}

/**
 * Initializes the task manager.
 * Starts as many worker threads as there are logical cores in the system - 1.
 */
task_manager::task_manager(){

    if(singleton_initialized){
        throw std::runtime_error("The task manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    //check how many threads we have
    thread_num = static_cast<uint8_t>(get_cpu_core_count());

    //initialize semaphore for worker threads
    work_sem = new semaphore;

    uint16_t task_thread_count = thread_num - 1;
    if(task_thread_count == 0){
        task_thread_count = 1;
    }
    fprintf(stdout, "This system has %d physical cores\nStarting %d task threads\n", thread_num, task_thread_count);

    //if we can't tell or there is only one core, then start one worker thread
    if(thread_num == 0 || thread_num == 1){
        thread_num = 2;
    }

	for (uint16_t i = 0; i < thread_num - 1; i++) {
		task_threads.emplace_back(std::thread(task_thread, this));
	}
}

/**
 * Initializes the task manager.
 * Starts worker threads equal to thread_num.
 * @param msg_bus A pointer to the global message bus.
 */
task_manager::task_manager(uint8_t thread_num){

    if(singleton_initialized){
        throw std::runtime_error("The task manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    //initialize semaphore for worker threads
    work_sem = new semaphore;

    this->thread_num = thread_num;

    auto total_threads = static_cast<uint8_t>(get_cpu_core_count());
    fprintf(stdout, "This system has %d physical cores\nStarting %d task threads\n", total_threads, thread_num);

    if(thread_num == 0 || thread_num == 1){
        this->thread_num = 2;
    }

    for (uint16_t i = 0; i <  this->thread_num - 1; i++) {
        task_threads.emplace_back(std::thread(task_thread, this));
    }
}

/**
 * This method will start a completely new thread, separate from the worker threads.
 * @param thread_func The function pointer.
 * @param data The data to supplied to it.
 */
void task_manager::start_thread(int (*thread_func)(void*), void* data){
    std::thread(thread_func, data);
}

/**
 * Closes the task manager.
 * Waits for all threads to exit properly and finishes any unfinished tasks.
 */
task_manager::~task_manager(){
    singleton_initialized = false;

    run_threads = false;
    for(int i = 0; i < thread_num-1; i++){
        work_sem->notify();
    }
    for(int i = 0; i < thread_num-1; i++) {
        task_threads[i].join();
    }

    //finish running any remaining tasks
	ST::task* new_task;
	while(global_task_queue.try_dequeue(new_task)){ //get a function pointer and data
        delete new_task->lock;
		delete new_task;
	}
    delete work_sem;
}

/**
 * Start a new task on one of the task threads.
 * @param arg The task object to use.
 * @return A task_id that can be used to wait for this task.
 */
task_id task_manager::start_task(ST::task* arg){
    //Apparently a bit cheaper to create a new semaphore instead of reusing old ones
    //This may depend on the platform (OS implementation of Semaphores)
    arg->lock = new semaphore;
    global_task_queue.enqueue(arg);
    work_sem->notify();
    return arg->lock;
}

/**
 * Same as start_task except no task_id (lock) is created and the task has no dependencies.
 * @param arg The task object to use.
 */
void task_manager::start_task_lockfree(ST::task* arg){
    global_task_queue.enqueue(arg);
    work_sem->notify();
}

/**
 * Wait for a task to finish and do work from the work queue while waiting.
 * @param id The ID of the task.
 */
void task_manager::work_wait_for_task(task_id id){
    if(id != nullptr) {
        while(!id->try_wait()) {
            ST::task* work;
            if(run_threads && global_task_queue.try_dequeue(work)){ //get a function pointer and data
                do_work(work);
            }
        }
        delete id;
    }
}

/**
 * Wait for a task to finish and do work from the work queue while waiting.
 * @param id The ID of the task.
 */
void task_manager::wait_for_task(task_id id){
    if(id != nullptr) {
        id->wait();
        delete id;
    }
}