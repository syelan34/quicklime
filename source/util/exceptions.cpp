#include "exceptions.h"

namespace ql {
	// typedef void (*handler_func)(ERRF_ExceptionData *info);

	// inline void installExceptionHandler(handler_func handler) {
	// 	uint32_t *tls = (uint32_t *)getThreadLocalStorage();
	// 	tls[16]		  = (uint32_t)handler;
	// 	tls[17]		  = 1;
	// 	tls[18]		  = 1;
	// }

	// inline void uninstallExceptionHandler(void) {
	// 	uint32_t *tls = (uint32_t *)getThreadLocalStorage();
	// 	tls[16]		  = (uint32_t)nullptr;
	// 	tls[17]		  = 1;
	// 	tls[18]		  = 0;
	// }
	
	// void print_func(void) {
	//     auto trace = std::stacktrace::current();
 //        std::for_each(trace.rbegin(), trace.rend(), [](auto &entry) {
 //            Console::log("%s:%u %s", entry.source_file().c_str(), entry.source_line(), entry.description().c_str());
 //        });
 //    }
	// extern "C" {
	//     void(*print_func_ptr)(void) = print_func;
	//     extern void __real___cxa_throw(void *thrown_exception, std::type_info *tinfo, void (*dest) (void *) );
	//     void __wrap___cxa_throw (void *thrown_exception, std::type_info *tinfo, void (*dest) (void *) ) {
 //            print_func_ptr();
 //            __real___cxa_throw(thrown_exception, tinfo, dest);
 //       	}
	// }
   	
} // namespace ql
