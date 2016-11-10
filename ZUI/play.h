#ifndef __PLUGIN_H__
#define __PLUGIN_H__

typedef struct avplay;
typedef struct ao_context;
typedef struct vo_context;
typedef struct source_context;
typedef struct SwsContext;
typedef struct _ZPlay
{
	struct avplay *m_avplay;//
	struct ao_context *m_audio;//��Ƶ��Ⱦ��
	struct vo_context *m_video;//��Ƶ��Ⱦ��
	struct source_context *m_source;//ý��Դ
	unsigned char* m_framebuffer;
	// ����ת��֡���ݸ�ʽ.
	struct SwsContext* m_swsctx;
	// ��Ƶ��.
	int m_image_width;
	// ��Ƶ��.
	int m_image_height;
	// �Ƿ����ÿ�߱�.
	BOOL m_keep_aspect;
	// ��߱�.
	float m_window_aspect;
}*ZuiPlay, ZPlay;

__declspec(dllexport) ZuiPlay __stdcall ZPlayNew();
__declspec(dllexport) BOOL __stdcall ZPlayOpen(ZuiPlay p);
__declspec(dllexport) VOID __stdcall ZPlayStop(ZuiPlay p);
__declspec(dllexport) VOID __stdcall ZPlayStart(ZuiPlay p);
__declspec(dllexport) BOOL __stdcall ZPlayGetFFT(ZuiPlay p, float *power);
#endif // __PLUGIN_H__
