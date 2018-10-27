
#if defined(NDM_IMPLEMENTATION)
#undef NDM_IMPLEMENTATION
	#define NDM_LIST_IMPLEMENTATION
	#define NDM_MEMORY_IMPLEMENTATION
	#define NDM_OUTPUT_IMPLEMENTATION
	#define NDM_STRING_IMPLEMENTATION
	#define NDM_FILE_IMPLEMENTATION
#endif // NDM_IMPLEMENTATION

#include "ndm_string.h"
#include "ndm_file.h"
#include "ndm_output.h"
#include "ndm_list.h"
#include "ndm_memory.h"
