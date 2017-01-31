﻿#include <ZUI.h>

extern rb_root *Global_ControlClass;
//创建控件
ZuiControl NewZuiControl(ZuiText classname, ZuiAny Param1, ZuiAny Param2, ZuiAny Param3) {
    ZuiControl p = (ZuiControl)malloc(sizeof(ZControl));
    if (p) {
        memset(p, 0, sizeof(ZControl));
        p->m_pManager = NULL;
        p->m_pParent = NULL;
        p->m_bUpdateNeeded = TRUE;
        p->m_bMenuUsed = FALSE;
        p->m_bVisible = TRUE;
        p->m_bFocused = FALSE;
        p->m_bEnabled = TRUE;
        p->m_bMouseEnabled = TRUE;
        p->m_bKeyboardEnabled = TRUE;
        p->m_bFloat = FALSE;
        p->m_bSetPos = FALSE;
        p->m_chShortcut = '\0';
        p->m_nTooltipWidth = 300;

        p->m_cXY.cx = p->m_cXY.cy = 0;
        p->m_cxyFixed.cx = p->m_cxyFixed.cy = 0;
        p->m_cxyMin.cx = p->m_cxyMin.cy = 0;
        p->m_cxyMax.cx = p->m_cxyMax.cy = 9999;


        p->m_piFloatPercent.left = p->m_piFloatPercent.top = p->m_piFloatPercent.right = p->m_piFloatPercent.bottom = 0.0f;

        p->call = &ZuiDefaultControlProc;

        p->m_rAttribute = rb_new();

        //通知重载的对象开始创建
        //没有重载的
        /*查找类名*/
        wchar_t name[256];
        int l = wcslen(classname);
        if (l > 255)
            return p;
        memset(name, 0, 256 * sizeof(wchar_t));
        memcpy(name, classname, l * sizeof(wchar_t));
        wcslwr(name);
        rb_node * node = rb_search((key_t)Zui_Hash(name), Global_ControlClass);
        if (node) {
            p->m_sUserData = ((ZCtlProc)node->data)(Proc_OnCreate, p, 0, Param1, Param2, Param3);
            p->call = ((ZCtlProc)node->data);
            return p;
        }
        //在模版内查找
        node = rb_search((key_t)Zui_Hash(name), Global_TemplateClass);
        if (node) {
            ZuiLoadTemplate(node->data, p);
        }
        return p;
    }
    return NULL;
}
//销毁控件
void FreeZuiControl(ZuiControl p, ZuiBool Delayed)
{
    if (!Delayed)
        ZuiControlCall(Proc_OnDestroy, p, NULL, NULL, NULL);
    else
        ZuiPaintManagerAddDelayedCleanup(p->m_pManager, p);
}
//-------------------------------------------------------------------------------------------------

//控件默认处理函数
ZEXPORT ZuiAny ZCALL ZuiDefaultControlProc(ZuiInt ProcId, ZuiControl p, ZuiAny UserData, ZuiAny Param1, ZuiAny Param2, ZuiAny Param3) {

    switch (ProcId)
    {
    case Proc_CoreInit: {
        return TRUE;
        break;
    }
    case Proc_GetControlFlags: {
        return 0;
    }
    case Proc_Activate: {
        if (!p->m_bVisible) return FALSE;
        if (!p->m_bEnabled) return FALSE;
    }
    case Proc_SetVisible: {
        if (p->m_bVisible == (BOOL)Param1)
            return 0;
        BOOL v = p->m_bVisible;
        p->m_bVisible = (BOOL)Param1;
        if (p->m_bFocused)
            p->m_bFocused = FALSE;
        if (!(BOOL)Param1 && p->m_pManager && p->m_pManager->m_pFocus == p) {
            ZuiPaintManagerSetFocus(p->m_pManager, NULL, TRUE);
        }
        if (p->m_bVisible != v) {
            ZuiControlNeedParentUpdate(p);
        }
        break;
    }
    case Proc_SetText: {
        ZuiControlInvalidate(p);
        if (!p->m_sText)
            p->m_sText = _wcsdup((ZuiText)Param1);
        if (wcscmp(p->m_sText, (ZuiText)Param1) == 0)
            return 0;
        free(p->m_sText);
        p->m_sText = _wcsdup((ZuiText)Param1);
        break;
    }
    case Proc_GetText: {
        return p->m_sText;
    }
    case Proc_SetName: {
        if (!p->m_sName)
            p->m_sName = _wcsdup((ZuiText)Param1);
        if (wcscmp(p->m_sName, (ZuiText)Param1) == 0)
            return 0;
        free(p->m_sName);
        p->m_sName = _wcsdup((ZuiText)Param1);
        break;
    }
    case Proc_SetTooltip: {
        if (!p->m_sToolTip)
            p->m_sToolTip = _wcsdup((ZuiText)Param1);
        if (wcscmp(p->m_sToolTip, (ZuiText)Param1) == 0)
            return 0;
        p->m_sToolTip = _wcsdup((ZuiText)Param1);
        break;
    }
    case Proc_GetPos: {
        return (void *)&p->m_rcItem;
        break;
    }
    case Proc_SetPos: {// 只有控件为float的时候，外部调用SetPos和Move才是有效的，位置参数是相对父控件的位置
        RECT *rc = (RECT *)Param1;
        BOOL bNeedInvalidate = (BOOL)Param2;
        if (rc->right < rc->left) rc->right = rc->left;
        if (rc->bottom < rc->top) rc->bottom = rc->top;

        RECT invalidateRc = p->m_rcItem;
        if (IsRectEmpty(&invalidateRc)) invalidateRc = *rc;

        if (p->m_bFloat) {
            ZuiControl pParent = p->m_pParent;
            if (pParent != NULL) {
                RECT *rcParentPos = (RECT *)ZuiControlCall(Proc_GetPos, pParent, NULL, NULL, NULL);
                RECT rcCtrl = { rcParentPos->left + rc->left, rcParentPos->top + rc->top,
                    rcParentPos->left + rc->right, rcParentPos->top + rc->bottom };
                p->m_rcItem = rcCtrl;

                LONG width = rcParentPos->right - rcParentPos->left;
                LONG height = rcParentPos->bottom - rcParentPos->top;
                RECT rcPercent = { (LONG)(width*p->m_piFloatPercent.left), (LONG)(height*p->m_piFloatPercent.top),
                    (LONG)(width*p->m_piFloatPercent.right), (LONG)(height*p->m_piFloatPercent.bottom) };
                p->m_cXY.cx = rc->left - rcPercent.left;
                p->m_cXY.cy = rc->top - rcPercent.top;
                p->m_cxyFixed.cx = rc->right - rcPercent.right - p->m_cXY.cx;
                p->m_cxyFixed.cy = rc->bottom - rcPercent.bottom - p->m_cXY.cy;
            }
        }
        else {
            memcpy(&p->m_rcItem, rc, sizeof(RECT));
        }
        if (p->m_pManager == NULL)
            return 0;

        if (!p->m_bSetPos) {
            p->m_bSetPos = TRUE;
            ZuiControlCall(Proc_OnSize, p, p->m_rcItem.right - p->m_rcItem.left, p->m_rcItem.bottom - p->m_rcItem.top, NULL);
            p->m_bSetPos = FALSE;
        }

        p->m_bUpdateNeeded = FALSE;

        if (bNeedInvalidate && p->m_bVisible) {
            Rect_Join(&invalidateRc, &p->m_rcItem);
            ZuiControl pParent = p;
            RECT rcTemp;
            RECT *rcParent;
            while (pParent = pParent->m_pParent) {
                if (!pParent->m_bVisible)
                    return 0;
                rcTemp = invalidateRc;
                rcParent = (RECT *)ZuiControlCall(Proc_GetPos, pParent, NULL, NULL, NULL);
                if (!IntersectRect(&invalidateRc, &rcTemp, rcParent))
                    return 0;
            }
            ZuiPaintManagerInvalidateRect(p->m_pManager, &invalidateRc);
        }
        break;
    }
    case Proc_SetManager: {
        p->m_pManager = (ZuiPaintManager)Param1;
        p->m_pParent = (ZuiControl)Param2;
        if (Param3 && p->m_pParent)
            ZuiControlCall(Proc_OnInit, p, NULL, NULL, NULL);
        break;
    }
    case Proc_OnEvent: {
        switch (((TEventUI *)Param1)->Type)
        {
        case ZEVENT_SETCURSOR:
        {
            SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
            return 0;
        }
        case ZEVENT_SETFOCUS:
        {
            p->m_bFocused = TRUE;
            ZuiControlInvalidate(p);
            return 0;
        }
        case ZEVENT_KILLFOCUS:
        {
            p->m_bFocused = FALSE;
            ZuiControlInvalidate(p);
            return 0;
        }
        case ZEVENT_MOUSELEAVE: {
            ZuiControlNotify(L"onmouseleave", p, NULL, NULL, NULL, NULL, NULL, NULL);
        }
                                break;
        case ZEVENT_MOUSEENTER: {
            ZuiControlNotify(L"onmouseenter", p, ((TEventUI *)Param1)->ptMouse.x, JS_TNUMBER, ((TEventUI *)Param1)->ptMouse.y, JS_TNUMBER, NULL, NULL);
        }
                                break;
        case ZEVENT_LBUTTONDOWN: {
            ZuiControlNotify(L"onlbuttondown", p, ((TEventUI *)Param1)->ptMouse.x, JS_TNUMBER, ((TEventUI *)Param1)->ptMouse.y, JS_TNUMBER, NULL, NULL);
        }
                                 break;
        case ZEVENT_LBUTTONUP: {
            ZuiControlNotify(L"onclick", p, ((TEventUI *)Param1)->ptMouse.x, JS_TNUMBER, ((TEventUI *)Param1)->ptMouse.y, JS_TNUMBER, NULL, NULL);
            break;
        }
        case ZEVENT_CHAR: {
            ZuiControlNotify(L"onchar", p, &((TEventUI *)Param1)->wParam, JS_TSHRSTR, NULL, NULL, NULL, NULL);
        }
                          break;
        default:
            break;
        }
        if (p->m_pParent != NULL)
            ZuiControlCall(Proc_OnEvent, p->m_pParent, Param1, NULL, NULL);
        break;
    }
    case Proc_GetMinWidth: {
        return (void *)p->m_cxyMin.cx;
        break;
    }
    case Proc_SetMinWidth: {
        if (p->m_cxyMin.cx == (LONG)Param1)
            return 0;
        if ((LONG)Param1 < 0)
            return 0;
        p->m_cxyMin.cx = (LONG)Param1;
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetMaxWidth: {
        return (void *)p->m_cxyMax.cx;
        break;
    }
    case Proc_SetMaxWidth: {
        if (p->m_cxyMax.cx == (LONG)Param1)
            return 0;
        if ((LONG)Param1 < 0)
            return 0;
        p->m_cxyMax.cx = (LONG)Param1;
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetMinHeight: {
        return (void *)p->m_cxyMin.cy;
        break;
    }
    case Proc_SetMinHeight: {
        if (p->m_cxyMin.cy == (LONG)Param1)
            return 0;
        if ((LONG)Param1 < 0)
            return 0;
        p->m_cxyMin.cy = (LONG)Param1;
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetMaxHeight: {
        return (void *)p->m_cxyMax.cy;
        break;
    }
    case Proc_SetMaxHeight: {
        if (p->m_cxyMax.cy == (LONG)Param1)
            return 0;
        if ((LONG)Param1 < 0)
            return 0;
        p->m_cxyMax.cy = (LONG)Param1;
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetWidth: {
        return (void *)(p->m_rcItem.right - p->m_rcItem.left);
        break;
    }
    case Proc_GetHeight: {
        return (void *)(p->m_rcItem.bottom - p->m_rcItem.top);
        break;
    }
    case Proc_GetX: {
        return (void *)p->m_rcItem.left;
        break;
    }
    case Proc_GetY: {
        return (void *)p->m_rcItem.top;
        break;
    }
    case Proc_SetFloatPercent: {
        memcpy(&p->m_piFloatPercent, Param1, sizeof(RECT));
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetPadding: {
        return (void *)&p->m_rcPadding;
        break;
    }
    case Proc_SetPadding: {
        memcpy(&p->m_rcPadding, Param1, sizeof(RECT));
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetFixedXY: {
        return (void *)&p->m_cXY;
        break;
    }
    case Proc_SetFixedXY: {
        p->m_cXY.cx = (LONG)Param1;
        p->m_cXY.cy = (LONG)Param2;
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetFixedWidth: {
        return (void *)p->m_cxyFixed.cx;
        break;
    }
    case Proc_SetFixedWidth: {
        if ((LONG)Param1 < 0)
            return 0;
        p->m_cxyFixed.cx = (LONG)Param1;
        if (!Param2)
            ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetFixedHeight: {
        return (void *)p->m_cxyFixed.cy;
        break;
    }
    case Proc_SetFixedHeight: {
        if ((LONG)Param1 < 0)
            return 0;
        p->m_cxyFixed.cy = (LONG)Param1;
        if (!Param2)
            ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_GetRelativePos: {
        ZuiControl pParent = p->m_pParent;
        if (pParent != NULL) {
            RECT *rcParentPos = (RECT *)ZuiControlCall(Proc_GetPos, pParent, NULL, NULL, NULL);
            RECT rcRelativePos = p->m_rcItem;
            OffsetRect(&rcRelativePos, -rcParentPos->left, -rcParentPos->top);
            return &rcRelativePos;
        }
        break;
    }
    case Proc_SetFloat: {
        if (p->m_bFloat == (BOOL)Param1)
            return 0;
        p->m_bFloat = (BOOL)Param1;
        ZuiControlNeedParentUpdate(p);
        break;
    }
    case Proc_SetEnabled: {
        if (p->m_bEnabled == Param1)
            return 0;

        p->m_bEnabled = Param1;
        ZuiControlInvalidate(p);
        break;
    }
    case Proc_SetFocus: {
        if (p->m_pManager != NULL)
            ZuiPaintManagerSetFocus(p->m_pManager, p, FALSE);
        break;
    }
    case Proc_SetDrag: {
        if (p->m_drag == Param1)
            return 0;

        p->m_drag = Param1;
        break;
    }
    case Proc_EstimateSize: {
        return (void *)&p->m_cxyFixed;
        break;
    }
    case Proc_FindControl: {
        if (((UINT)Param3 & ZFIND_VISIBLE) != 0 && !p->m_bVisible)
            return NULL;
        if (((UINT)Param3 & ZFIND_ENABLED) != 0 && !p->m_bEnabled)
            return NULL;//激活
        if (((UINT)Param3 & ZFIND_HITTEST) != 0 && (!p->m_bMouseEnabled || !ZuiIsPointInRect(&p->m_rcItem, Param2)))
            return NULL;
        if (((UINT)Param3 & ZFIND_UPDATETEST) != 0 && ((FINDCONTROLPROC)Param1)(p, Param2) != NULL)
            return NULL;
        return ((FINDCONTROLPROC)Param1)(p, Param2);
        break;
    }
    case Proc_SetBkColor: {
        p->m_BkgColor = Param1;
        ZuiControlInvalidate(p);
        break;
    }
    case Proc_SetBorderColor: {
        p->m_dwBorderColor = Param1;
        ZuiControlInvalidate(p);
        break;
    }
    case Proc_SetBkImage: {
        if (p->m_BkgImg)
            ZuiResDBDelRes(p->m_BkgImg);
        p->m_BkgImg = Param1;
        ZuiControlInvalidate(p);
        break;
    }
    case Proc_OnPaint: {
        //开始绘制
        ZuiControlCall(Proc_OnPaintBkColor, p, Param1, Param2, NULL);
        ZuiControlCall(Proc_OnPaintBkImage, p, Param1, Param2, NULL);
        ZuiControlCall(Proc_OnPaintStatusImage, p, Param1, Param2, NULL);
        ZuiControlCall(Proc_OnPaintText, p, Param1, Param2, NULL);
        ZuiControlCall(Proc_OnPaintBorder, p, Param1, Param2, NULL);
        break;
    }
    case Proc_OnPaintBkColor: {
        ZuiGraphics gp = (ZuiGraphics)Param1;
        RECT *rc = &p->m_rcItem;
        if (p->m_BkgColor)
            ZuiDrawFillRect(gp, p->m_BkgColor, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top);
        break;
    }
    case Proc_OnPaintBkImage: {
        ZuiGraphics gp = (ZuiGraphics)Param1;
        RECT *rc = &p->m_rcItem;
        if (p->m_BkgImg) {
            ZuiImage img = p->m_BkgImg->p;
            ZuiDrawImageEx(gp, img, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, 0, 0, img->Width, img->Height, 255);
        }
        break;
    }
    case Proc_OnPaintStatusImage: {
        ZuiGraphics gp = (ZuiGraphics)Param1;
        RECT *rc = &p->m_rcItem;
        break;
    }
    case Proc_OnPaintText: {
        ZuiGraphics gp = (ZuiGraphics)Param1;
        RECT *rc = &p->m_rcItem;

        break;
    }
    case Proc_OnPaintBorder: {
        ZuiGraphics gp = (ZuiGraphics)Param1;
        RECT *rc = &p->m_rcItem;
        if (p->m_dwBorderColor)
            ZuiDrawRect(gp, p->m_dwBorderColor, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, 1);
        break;
    }
    case Proc_OnDestroy: {
        if (p->m_sText)
            free(p->m_sText);
        if (p->m_sToolTip)
            free(p->m_sToolTip);
        if (p->m_BkgImg)
            ZuiResDBDelRes(p->m_BkgImg);
        if (p->m_pParent && !Param1)
            ZuiControlCall(Proc_Layout_Remove, p->m_pParent, p, TRUE, NULL);
        if (p->m_pManager != NULL)
            ZuiPaintManagerReapObjects(p->m_pManager, p);
        free(p);
        break;
    }
    case Proc_SetAttribute: {
        if (wcscmp(Param1, L"text") == 0) ZuiControlCall(Proc_SetText, p, Param2, NULL, NULL);
        else if (wcscmp(Param1, L"tooltip") == 0) ZuiControlCall(Proc_SetTooltip, p, Param2, NULL, NULL);
        else if (wcscmp(Param1, L"width") == 0) ZuiControlCall(Proc_SetFixedWidth, p, _wtoi(Param2), NULL, NULL);
        else if (wcscmp(Param1, L"height") == 0) ZuiControlCall(Proc_SetFixedHeight, p, _wtoi(Param2), NULL, NULL);
        else if (wcscmp(Param1, L"minwidth") == 0) ZuiControlCall(Proc_SetMinWidth, p, _wtoi(Param2), NULL, NULL);
        else if (wcscmp(Param1, L"minheight") == 0) ZuiControlCall(Proc_SetMinHeight, p, _wtoi(Param2), NULL, NULL);
        else if (wcscmp(Param1, L"maxwidth") == 0) ZuiControlCall(Proc_SetMaxWidth, p, _wtoi(Param2), NULL, NULL);
        else if (wcscmp(Param1, L"maxheight") == 0) ZuiControlCall(Proc_SetMaxHeight, p, _wtoi(Param2), NULL, NULL);
        else if (wcscmp(Param1, L"bkcolor") == 0) {
            while (*(wchar_t *)Param2 > L'\0' && *(wchar_t *)Param2 <= L' ') (wchar_t *)Param2 = CharNext((wchar_t *)Param2);
            if (*(wchar_t *)Param2 == L'#') (wchar_t *)Param2 = CharNext((wchar_t *)Param2);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul((wchar_t *)Param2, &pstr, 16);
            ZuiControlCall(Proc_SetBkColor, p, clrColor, NULL, NULL);
        }
        else if (wcscmp(Param1, L"drag") == 0) ZuiControlCall(Proc_SetDrag, p, wcscmp(Param2, L"true") == 0 ? TRUE : FALSE, NULL, NULL);
        else if (wcscmp(Param1, L"bkimage") == 0) ZuiControlCall(Proc_SetBkImage, p, ZuiResDBGetRes(Param2, ZREST_IMG), NULL, NULL);
        else if (wcscmp(Param1, L"padding") == 0) {
            RECT rcPadding = { 0 };
            LPTSTR pstr = NULL;
            rcPadding.left = _tcstol(Param2, &pstr, 10);  ASSERT(pstr);
            rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
            rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
            rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
            ZuiControlCall(Proc_SetPadding, p, &rcPadding, NULL, NULL);
        }
        else if (wcscmp(Param1, L"bordercolor") == 0) {
            while (*(wchar_t *)Param2 > L'\0' && *(wchar_t *)Param2 <= L' ') (wchar_t *)Param2 = CharNext((wchar_t *)Param2);
            if (*(wchar_t *)Param2 == L'#') (wchar_t *)Param2 = CharNext((wchar_t *)Param2);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul((wchar_t *)Param2, &pstr, 16);
            ZuiControlCall(Proc_SetBorderColor, p, clrColor, NULL, NULL);
        }
        else if (wcscmp(Param1, L"name") == 0) ZuiControlCall(Proc_SetName, p, Param2, NULL, NULL);
        else if (wcscmp(Param1, L"float") == 0) {
            if (wcschr(Param2, ',') == 0) {
                ZuiControlCall(Proc_SetFloat, p, wcscmp(Param2, L"true") == 0 ? TRUE : FALSE, NULL, NULL);
            }
            else {
                TPercentInfo piFloatPercent = { 0 };
                LPTSTR pstr = NULL;
                piFloatPercent.left = _tcstod(Param2, &pstr);  ASSERT(pstr);
                piFloatPercent.top = _tcstod(pstr + 1, &pstr);    ASSERT(pstr);
                piFloatPercent.right = _tcstod(pstr + 1, &pstr);  ASSERT(pstr);
                piFloatPercent.bottom = _tcstod(pstr + 1, &pstr); ASSERT(pstr);
                ZuiControlCall(Proc_SetFloatPercent, p, &piFloatPercent, NULL, NULL);
                ZuiControlCall(Proc_SetFloat, p, TRUE, NULL, NULL);
            }
        }
        else if (wcscmp(Param1, L"visible") == 0) ZuiControlCall(Proc_SetVisible, p, wcscmp(Param2, L"true") == 0 ? TRUE : FALSE, NULL, NULL);
        else if (wcscmp(Param1, L"pos") == 0) {
            RECT rcPos = { 0 };
            ZuiText pstr = NULL;
            rcPos.left = _tcstol(Param2, &pstr, 10);  ASSERT(pstr);
            rcPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
            rcPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
            rcPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
            ZuiControlCall(Proc_SetFixedXY, p, rcPos.left, rcPos.top, NULL);
            ZuiControlCall(Proc_SetFixedWidth, p, rcPos.right - rcPos.left, NULL, NULL);
            ZuiControlCall(Proc_SetFixedHeight, p, rcPos.bottom - rcPos.top, NULL, NULL);
        }
        else if (wcscmp(Param1, L"enabled") == 0) ZuiControlCall(Proc_SetEnabled, p, wcscmp(Param2, L"true") == 0 ? TRUE : FALSE, NULL, NULL);
        else {
            ZuiAttribute att;
            rb_node *old = rb_search((key_t)Zui_Hash(Param1), p->m_rAttribute);
            if (old) {
                att = (ZuiAttribute)old->data;
                free(att->v);
            }
            else {
                att = malloc(sizeof(ZAttribute));
                rb_insert((key_t)Zui_Hash(Param1), att, p->m_rAttribute);
            }
            att->type = ZAttType_String;
            att->v = _wcsdup(Param2);
            att->vlen = wcslen(att->v);
        }
        break;
    }
    case Proc_GetAttribute: {

        break;
    }
    case Proc_JsHas: {
        if (wcscmp(Param1, L"root") == 0) ZuiBuilderJs_pushControl(Param2, p->m_pManager->m_pRoot);
        else if (wcscmp(Param1, L"parent") == 0)
            ZuiBuilderJs_pushControl(Param2, p->m_pParent);
        else if (wcscmp(Param1, L"text") == 0) js_pushstring(Param2, ZuiControlCall(Proc_GetText, p, NULL, NULL, NULL));
        else if (wcscmp(Param1, L"tooltip") == 0) js_pushstring(Param2, p->m_sToolTip);
        else if (wcscmp(Param1, L"width") == 0) js_pushnumber(Param2, p->m_cxyFixed.cx);
        else if (wcscmp(Param1, L"height") == 0) js_pushnumber(Param2, p->m_cxyFixed.cy);
        else if (wcscmp(Param1, L"minwidth") == 0) js_pushnumber(Param2, p->m_cxyMin.cx);
        else if (wcscmp(Param1, L"minheight") == 0) js_pushnumber(Param2, p->m_cxyMin.cy);
        else if (wcscmp(Param1, L"maxwidth") == 0) js_pushnumber(Param2, p->m_cxyMax.cx);
        else if (wcscmp(Param1, L"maxheight") == 0) js_pushnumber(Param2, p->m_cxyMax.cy);
        else if (wcscmp(Param1, L"bkcolor") == 0) js_pushnumber(Param2, p->m_BkgColor);
        else if (wcscmp(Param1, L"drag") == 0) js_pushboolean(Param2, p->m_drag);
        else if (wcscmp(Param1, L"clos") == 0) return 1;
        else if (wcscmp(Param1, L"rect") == 0) {
            js_newobject(Param2);
            js_pushnumber(Param2, p->m_rcItem.left);
            js_setproperty(Param2, -2, L"left");
            js_pushnumber(Param2, p->m_rcItem.top);
            js_setproperty(Param2, -2, L"top");
            js_pushnumber(Param2, p->m_rcItem.bottom);
            js_setproperty(Param2, -2, L"bottom");
            js_pushnumber(Param2, p->m_rcItem.right);
            js_setproperty(Param2, -2, L"right");
        }
        else if (wcscmp(Param1, L"enabled") == 0) js_pushboolean(Param2, p->m_bEnabled);
        else if (wcscmp(Param1, L"visible") == 0) js_pushboolean(Param2, p->m_bVisible);
        break;
    }
    case Proc_JsPut: {
        js_State *J = Param2;
        if (wcscmp(Param1, L"text") == 0) ZuiControlCall(Proc_SetText, p, js_tostring(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"tooltip") == 0) ZuiControlCall(Proc_SetTooltip, p, js_tostring(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"width") == 0) ZuiControlCall(Proc_SetFixedWidth, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"height") == 0) ZuiControlCall(Proc_SetFixedHeight, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"minwidth") == 0) ZuiControlCall(Proc_SetMinWidth, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"minheight") == 0) ZuiControlCall(Proc_SetMinHeight, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"maxwidth") == 0) ZuiControlCall(Proc_SetMaxWidth, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"maxheight") == 0) ZuiControlCall(Proc_SetMaxHeight, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"bkcolor") == 0) ZuiControlCall(Proc_SetBkColor, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"drag") == 0) ZuiControlCall(Proc_SetDrag, p, js_toboolean(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"bkimage") == 0) ZuiControlCall(Proc_SetBkImage, p, ZuiResDBGetRes(js_tostring(J, -1), ZREST_IMG), NULL, NULL);
        else if (wcscmp(Param1, L"padding") == 0) {
            RECT rcPadding = { 0 };
            LPTSTR pstr = NULL;
            rcPadding.left = _tcstol(js_tostring(J, -1), &pstr, 10);  ASSERT(pstr);
            rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
            rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
            rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
            ZuiControlCall(Proc_SetPadding, p, &rcPadding, NULL, NULL);
        }
        else if (wcscmp(Param1, L"bordercolor") == 0) ZuiControlCall(Proc_SetBorderColor, p, js_toint32(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"name") == 0) ZuiControlCall(Proc_SetName, p, js_tostring(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"float") == 0) {

        }
        else if (wcscmp(Param1, L"visible") == 0) ZuiControlCall(Proc_SetVisible, p, js_toboolean(J, -1), NULL, NULL);
        else if (wcscmp(Param1, L"enabled") == 0) ZuiControlCall(Proc_SetEnabled, p, js_toboolean(J, -1), NULL, NULL);
        else
        {
            ZuiAttribute att;
            rb_node *old = rb_search((key_t)Zui_Hash(Param1), p->m_rAttribute);
            if (old) {
                att = (ZuiAttribute)old->data;
                free(att->v);
            }
            else {
                att = malloc(sizeof(ZAttribute));
                rb_insert((key_t)Zui_Hash(Param1), att, p->m_rAttribute);
            }

            if (js_iscallable(J, -1)) {
                att->type = ZAttType_JsCall;
                js_Object *obj = js_toobject(J, -1);
                att->v = obj;
            }
            else
            {
                att->type = ZAttType_String;
                att->v = _wcsdup(js_tostring(J, -1));
                att->vlen = wcslen(att->v);
            }
        }
        break;
    }
    case Proc_JsCall: {
        if (wcscmp(Param1, L"clos") == 0) {
            ZuiPaintManagerAddDelayedCleanup(p->m_pManager, p);
            //FreeZuiControl(p);
        }
        break;
    }
    default:
        break;
    }
    return 0;
}

//调用控件函数
ZEXPORT ZuiAny ZCALL ZuiControlCall(ZuiInt ProcId, ZuiControl p, ZuiAny Param1, ZuiAny Param2, ZuiAny Param3) {
    if (!p)
        return NULL;
    if (p->call)
    {
        return p->call(ProcId, p, p->m_sUserData, Param1, Param2, Param3);
    }
    return NULL;
}
ZuiBool ZuiControlNotifyPushJs(js_State *J, ZuiAny Param, ZuiInt Type)
{
    switch (Type)
    {
    case JS_TSHRSTR:
    case JS_TLITSTR:
    case JS_TMEMSTR:
        js_pushstring(J, Param);
        break;
    case JS_TBOOLEAN:
        js_pushboolean(J, Param);
        break;
    case JS_TNUMBER:
        js_pushnumber(J, (int)Param);
        break;
    case JS_TOBJECT:
        js_pushobject(J, Param);
        break;
    default:
        return FALSE;
        break;
    }
    return TRUE;
}
ZEXPORT ZuiAny ZCALL ZuiControlNotify(ZuiText msg, ZuiControl p, ZuiAny Param1, ZuiInt p1Type, ZuiAny Param2, ZuiInt p2Type, ZuiAny Param3, ZuiInt p3Type) {
    //先通知js
    rb_node * node = rb_search((key_t)Zui_Hash(msg), p->m_rAttribute);
    if (node) {
        ZuiAttribute att = node->data;
        if (att->type == ZAttType_String)
        {
            ZuiBuilderJs_pushControl(p->m_pManager->m_js, p);
            js_setglobal(p->m_pManager->m_js, L"ec");
            ZuiBuilderJsLoad(p->m_pManager->m_js, att->v, att->vlen);
        }
        else {
            int ret = 1;
            js_pushobject(p->m_pManager->m_js, att->v);
            ZuiBuilderJs_pushControl(p->m_pManager->m_js, p);
            if (Param1)
            {
                if (ZuiControlNotifyPushJs(p->m_pManager->m_js, Param1, p1Type))
                    ret++;
            }
            if (Param2)
            {
                if (ZuiControlNotifyPushJs(p->m_pManager->m_js, Param2, p2Type))
                    ret++;
            }
            if (Param3)
            {
                if (ZuiControlNotifyPushJs(p->m_pManager->m_js, Param3, p3Type))
                    ret++;
            }
            js_pcall(p->m_pManager->m_js, ret);
            if (!js_isundefined(p->m_pManager->m_js, -1)) {
                ZuiAny r = 0;
                if (js_isuserdata(p->m_pManager->m_js, -1, NULL)) {
                    r = js_touserdata(p->m_pManager->m_js, -1, NULL);
                }
                js_pop(p->m_pManager->m_js, 1);
                return r;
            }
            js_pop(p->m_pManager->m_js, 1);
        }

    }
    if (p->m_pNotify)
    {
        return p->m_pNotify(msg, p, p->m_sUserData, Param1, Param2, Param3);
    }
    return NULL;
}

ZEXPORT ZuiVoid ZCALL ZuiControlRegNotify(ZuiControl p, ZNotifyProc pNotify) {
    if (p)
    {
        p->m_pNotify = pNotify;
    }
}

ZEXPORT ZuiVoid ZCALL ZuiClientToScreen(ZuiControl p, ZuiPoint pt) {
    if (p && pt) {
        ClientToScreen(p->m_pManager->m_hWndPaint, pt);
    }
}

ZEXPORT ZuiVoid ZCALL ZuiScreenToClient(ZuiControl p, ZuiPoint pt) {
    if (p && pt) {
        ScreenToClient(p->m_pManager->m_hWndPaint, pt);
    }
}

//-------------------------------------------------------------------------------------------------

ZEXPORT ZuiControl ZCALL ZuiControlFindName(ZuiControl p, ZuiText Name) {
    if (!p)
        return NULL;
    if (!p->m_pManager)
        return NULL;
    if (!p->m_pManager->m_pRoot)
        return NULL;
    return (ZuiControl)ZuiControlCall(Proc_FindControl, p->m_pManager->m_pRoot, __FindControlFromName, Name, (void *)(ZFIND_ALL));
}

ZEXPORT ZuiVoid ZCALL ZuiControlInvalidate(ZuiControl p)
{
    if (!p->m_bVisible) return;

    RECT invalidateRc = p->m_rcItem;

    ZuiControl pParent = p;
    RECT rcTemp;
    RECT *rcParent;
    while (pParent = pParent->m_pParent)
    {
        rcTemp = invalidateRc;
        rcParent = (RECT *)ZuiControlCall(Proc_GetPos, pParent, NULL, NULL, NULL);
        if (!IntersectRect(&invalidateRc, &rcTemp, rcParent))
        {
            return;
        }
    }

    if (p->m_pManager != NULL) ZuiPaintManagerInvalidateRect(p->m_pManager, &invalidateRc);
}

ZEXPORT ZuiVoid ZCALL ZuiControlNeedUpdate(ZuiControl p)
{
    if (!p->m_bVisible) return;
    p->m_bUpdateNeeded = TRUE;
    ZuiControlInvalidate(p);

    if (p->m_pManager != NULL)
        p->m_pManager->m_bUpdateNeeded = TRUE;
}

ZEXPORT ZuiVoid ZCALL ZuiControlNeedParentUpdate(ZuiControl p)
{
    if (p->m_pParent) {
        ZuiControlNeedUpdate(p->m_pParent);
        ZuiControlInvalidate(p->m_pParent);
    }
    else {
        ZuiControlNeedUpdate(p);
    }

    if (p->m_pManager != NULL)
        p->m_pManager->m_bUpdateNeeded = TRUE;
}

ZEXPORT ZuiVoid ZCALL ZuiControlEvent(ZuiControl p, TEventUI *event)
{
    ZuiControlCall(Proc_OnEvent, p, event, NULL, NULL);
}

ZEXPORT ZuiVoid ZCALL ZuiControlMove(ZuiControl p, SIZE szOffset, BOOL bNeedInvalidate)
{
    RECT invalidateRc = p->m_rcItem;
    p->m_rcItem.left += szOffset.cx;
    p->m_rcItem.top += szOffset.cy;
    p->m_rcItem.right += szOffset.cx;
    p->m_rcItem.bottom += szOffset.cy;

    if (bNeedInvalidate && p->m_pManager == NULL && p->m_bVisible) {
        Rect_Join(&invalidateRc, &p->m_rcItem);
        ZuiControl pParent = p;
        RECT rcTemp;
        RECT *rcParent;
        while (pParent = pParent->m_pParent) {
            if (!pParent->m_bVisible) return;
            rcTemp = invalidateRc;
            rcParent = (RECT *)ZuiControlCall(Proc_GetPos, pParent, NULL, NULL, NULL);
            if (!IntersectRect(&invalidateRc, &rcTemp, rcParent)) return;
        }
        ZuiPaintManagerInvalidateRect(p->m_pManager, &invalidateRc);
    }
}

//-------------------------------------------------------------------------------------------------











