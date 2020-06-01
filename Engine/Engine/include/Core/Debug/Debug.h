#pragma once

#include <Tracy.hpp>

#ifdef TRACY_ENABLE
void* operator new(std::size_t count);
void operator delete(void* ptr) noexcept;

#define SET_THREAD_NAME(name) tracy::SetThreadName(name);
#else
#define SET_THREAD_NAME(name) 
#endif

#include "DebugTracyGL.hpp"

/**
 * Contains debug utilities like tracy profiler
 */
namespace Core::Debug
{
}