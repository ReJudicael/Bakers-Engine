#pragma once

#include <Tracy.hpp>
#include "DebugTracyGL.hpp"

#ifdef TRACY_ENABLE
void* operator new(std::size_t count);
void operator delete(void* ptr) noexcept;
#endif
/**
 * Contains debug utilities like tracy profiler
 */
namespace Core::Debug
{
}