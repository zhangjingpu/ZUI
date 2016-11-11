﻿#include <stdio.h>
#include <ZUI.h>
#if !(defined NDEBUG1)
#ifdef _WIN32
#include "play.h"
#pragma comment(lib, "play.lib")
ZuiPlay play;
ZEXPORT ZuiAny ZCALL ZuiFFTProcEx(ZuiInt ProcId, ZuiControl cp, ZuiOption p, ZuiAny Param1, ZuiAny Param2, ZuiAny Param3) {
	switch (ProcId)
	{
	case Proc_OnEvent: {
		TEventUI *event = (TEventUI *)Param1;
		if (event->Type == ZEVENT_TIMER) {
			ZuiControlInvalidate(cp);
		}
		break;
	}
	case Proc_OnPaint: {
		ZuiGraphics gp = (ZuiGraphics)Param1;
		RECT *rc = &cp->m_rcItem;
		float power[2048];
		if (ZPlayGetFFT(play, power)) {
			int fftnum = 80;

			int fpFen = 2048 / 2 / fftnum;

			float level[80];
			for (int i = 0; i < 80; i++)
			{
				int cPos = i * fpFen + fpFen / 2;
				double bandTotal = power[cPos - 2] * 0.1 + power[cPos - 1] * 0.15
					+ power[cPos] * 0.5 + power[cPos + 1] * 0.15 + power[cPos + 2] * 0.1;

				level[i] = (int)(bandTotal + 0.5);
			}
			for (int i = 0; i < fftnum; i++)
			{
				if (level[i] < 0)
					level[i] = 0;
				else if (level[i] > 128)
					level[i] = 128;
			}
			for (int i = 0; i < fftnum; i++)
			{
				if(level[i])
					level[i] =  level[i] / 64 * 23;
			}
			for (size_t i = 1; i < fftnum; i++)
			{
				ZuiDrawLine(gp, ARGB(255, 223, 112, 255), rc->left + i, rc->bottom - level[i-1], rc->left + i, rc->bottom, 1);
			}
		}
		break;
	}
	case Proc_OnInit: {
		break;
	}
	default:
		break;
	}
	return ZuiDefaultControlProc(ProcId, cp, p, Param1, Param2, Param3);
}
#endif
void __stdcall DLLDebug(){
	ZuiInit();
	


	FILE*fp;
	long flen;
	void *p;

	fp = fopen("test.xml", "rb");
	fseek(fp, 0L, SEEK_END);
	flen = ftell(fp); /* 得到文件大小 */
	p = malloc(flen); /* 根据文件大小动态分配内存空间 */
	fseek(fp, 0L, SEEK_SET); /* 定位到文件开头 */
	fread(p, flen, 1, fp); /* 一次性读取全部文件内容 */
	fclose(fp);
	
	
	ZuiControl cp = ZuiControlFindName(ZuiLayoutLoad(p, flen), L"Play_FFT");
#ifdef _WIN32
	cp->call = (ZCtlProc)ZuiFFTProcEx;
	ZuiPaintManagerSetTimer(cp, 1000, 100);
	play = ZPlayNew();
	ZPlayOpen(play);
	ZPlayStart(play);
#endif
	ZuiMsgLoop();


	return;
}
int  main(){
	DLLDebug();
}

#endif
#ifdef _WIN32
int _stdcall _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	DLLDebug();
}
#if (defined NDEBUG)
int __security_cookie = 0;     //比错误提示的名称少一个下划线

__declspec(naked) __int64 _ftol2_sse(double v)
{
	__asm {
		push        ebp
			mov         ebp, esp
			sub         esp, 20h
			and         esp, 0FFFFFFF0h
			fld         st(0)
			fst         dword ptr[esp + 18h]
			fistp       qword ptr[esp + 10h]
			fild        qword ptr[esp + 10h]
			mov         edx, dword ptr[esp + 18h]
			mov         eax, dword ptr[esp + 10h]
			test        eax, eax
			je          integer_QnaN_or_zero
			arg_is_not_integer_QnaN :
		fsubp       st(1), st
			test        edx, edx
			jns         positive
			fstp        dword ptr[esp]
			mov         ecx, dword ptr[esp]
			xor ecx, 80000000h
			add         ecx, 7FFFFFFFh
			adc         eax, 0
			mov         edx, dword ptr[esp + 14h]
			adc         edx, 0
			jmp         localexit

			positive :
		fstp        dword ptr[esp]
			mov         ecx, dword ptr[esp]
			add         ecx, 7FFFFFFFh
			sbb         eax, 0
			mov         edx, dword ptr[esp + 14h]
			sbb         edx, 0
			jmp         localexit
			integer_QnaN_or_zero :
		mov         edx, dword ptr[esp + 14h]
			test        edx, 7FFFFFFFh
			jne         arg_is_not_integer_QnaN
			fstp        dword ptr[esp + 18h]
			fstp        dword ptr[esp + 18h]
			localexit :
			leave
			ret
	}
}
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif