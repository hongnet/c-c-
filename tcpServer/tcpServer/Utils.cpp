#include "Utils.h"


//#if (defined(_WIN32) || defined(_WIN64))
//#include "windows.h"
//bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//		printf("DLL_PROCESS_ATTACH\n");
//		break;
//	case DLL_THREAD_ATTACH:
//		printf("DLL_THREAD_ATTACH\n");
//		break;
//	case DLL_THREAD_DETACH:
//		printf("DLL_THREAD_DETACH\n");
//		break;
//	case DLL_PROCESS_DETACH:
//		printf("DLL_PROCESS_DETACH\n");
//		break;
//	}
//	return TRUE;
//}
//#endif

//去除前后空格

//去除前后空格
//CUtils CUtils::g_cutilInstance;
CUtils::CUtils()
	:m_genLog(true)
{
}

CUtils::~CUtils()
{
}

int CUtils::GenerateLog(bool isGen)
{
	m_genLog = isGen;
	return 0;
}

int CUtils::GetGenertateLogValue(bool& isGen)
{
	isGen = m_genLog;
	return 0;
}

inline int CUtils::StrTrim(std::string & strVal)
{
	echo2File("strTrim enter,strVal:" + strVal);
	//去除前后空格
	int leftNonEmptyPos = strVal.find_first_not_of(" \t\r\n");
	int rightNonEmptyPos = strVal.find_last_not_of(" \r\n\t");
	if (leftNonEmptyPos != std::string::npos && rightNonEmptyPos != std::string::npos && leftNonEmptyPos >= rightNonEmptyPos)
		strVal = strVal.substr(leftNonEmptyPos, rightNonEmptyPos - leftNonEmptyPos + 1);
	echo2File("strTrim leave,strVal:" + strVal);
	return 0;
}

CUtils* CUtils::GetInstance()
{
	static CUtils g_Instan;
	return &g_Instan;
}

char* CUtils::GetCurTime(char* strDateTime)
{
	struct timeb tp_cur;
	ftime(&tp_cur);

	struct tm btm;

#if (defined(_WIN32) || defined(_WIN64))
	localtime_s(&btm, &tp_cur.time);
#else
	localtime_r(&tp_cur.time, &btm);
#endif

	sprintf(strDateTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		btm.tm_year + 1900, btm.tm_mon + 1, btm.tm_mday,
		btm.tm_hour, btm.tm_min, btm.tm_sec, tp_cur.millitm);

	return strDateTime;
}

