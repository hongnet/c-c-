#pragma once
#ifndef _CUTILS_CLASS_
#define _CUTILS_CLASS_
#include <sstream>
#include <iostream>
#include <sys/timeb.h>
#include "time.h"

#if (defined(_WIN32) || defined(_WIN64))
#  define HUTILS_EXTERN  __declspec(dllexport)
#  define HUTILS_API __stdcall
#elif defined(__linux__)
#  define HUTILS_EXTERN 
#  define HUTILS_API
#else
#  define HUTILS_EXTERN
#  define HUTILS_API
#endif

class HUTILS_EXTERN CUtils
{

public:
	CUtils();
	~CUtils();
	int GenerateLog(bool isGen);
	int GetGenertateLogValue(bool& isGen);
	//去除前后空格
	 int StrTrim(std::string& strVal);
	static CUtils* GetInstance();
	//精确到微妙时间
	char* GetCurTime(char* strDateTime);

private:
	bool m_genLog;
	//static CUtils g_cutilInstance;
};

#define echo2File(logStr) \
{ \
	bool m_genLog = true; \
	CUtils::GetInstance()->GenerateLog(m_genLog); \
	if(m_genLog) \
{ \
	char strDateTime[32]; \
	CUtils::GetInstance()->GetCurTime(strDateTime); \
	std::string shellCommand("echo "); \
	shellCommand += "\"" +std::string(strDateTime)+"-"+__FILE__+"-"+ __func__+"-"+std::to_string(__LINE__)+"-"+ logStr + "\" >> httpSDK.txt"; \
	system(shellCommand.c_str()); \
} \
}

#endif 
