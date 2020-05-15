#include "CoreMinimal.h"

#ifdef TRACY_ENABLE

void* operator new(std::size_t count)
{
	void* ptr = malloc(count);
	TracyAlloc(ptr, count);
	return ptr;
}

void operator delete(void* ptr) noexcept
{
	TracyFree(ptr);
	free(ptr);
}

#endif
