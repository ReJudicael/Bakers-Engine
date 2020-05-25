#pragma once

#ifdef ENGINE_EXPORTS
#define BAKERS_API extern "C" _declspec(dllexport)
#define BAKERS_API_CLASS class _declspec(dllexport)
#define BAKERS_API_STRUCT struct _declspec(dllexport)
#else
#define BAKERS_API extern "C" _declspec(dllimport)
#define BAKERS_API_CLASS class _declspec(dllimport)
#define BAKERS_API_STRUCT struct _declspec(dllimport)
#endif
#ifdef GAME_EXPORTS
#define BAKERS_GAME extern "C" _declspec(dllexport)
#define BAKERS_GAME_CLASS class _declspec(dllexport)
#define BAKERS_GAME_STRUCT struct _declspec(dllexport)
#else
#define BAKERS_GAME extern "C" _declspec(dllimport)
#define BAKERS_GAME_CLASS class _declspec(dllimport)
#define BAKERS_GAME_STRUCT struct _declspec(dllimport)
#endif