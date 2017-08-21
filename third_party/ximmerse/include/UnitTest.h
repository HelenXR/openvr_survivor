#ifndef __XIM_Unit_TEST_H__
#define __XIM_Unit_TEST_H__

#ifndef XIM_API
#if defined(_WIN32)||defined(_WIN64)
#define XIM_API __declspec(dllimport)
#else
#define XIM_API
#endif
#endif

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

	XIM_API int UNIT_TEST_Main_XHawkDecoder();

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif