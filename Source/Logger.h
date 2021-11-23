#pragma once
class Logger
{
public:
	// ���O�o��
	static void Print(const char* format, ...);
};

#if 	defined(_DEBUG)
#define	LOG(...)		{ Logger::Print(__VA_ARGS__); }
#else
#define	LOG(...)		{}
#endif
