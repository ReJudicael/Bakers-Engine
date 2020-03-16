#pragma once

#ifdef ENGINE_EXPORTS
#define BAKERS_API extern "C" _declspec(dllexport)
#define BAKERS_API_CLASS class _declspec(dllexport)
#else
#define BAKERS_API extern "C" _declspec(dllimport)
#define BAKERS_API_CLASS class _declspec(dllimport)
#endif