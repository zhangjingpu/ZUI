﻿#include <ZUI.h>




ZEXPORT ZuiAny ZCALL ZuiSplitterBarProc(ZuiInt ProcId, ZuiControl cp, ZuiSplitterBar p, ZuiAny Param1, ZuiAny Param2, ZuiAny Param3) {
    switch (ProcId)
    {
    case Proc_CoreInit:
        return TRUE;
    case Proc_OnCreate: {
        p = (ZuiSplitterBar)malloc(sizeof(ZSplitterBar));
        memset(p, 0, sizeof(ZSplitterBar));
        //保存原来的回调地址,创建成功后回调地址指向当前函数
        p->old_call = cp->call;
        return p;
    }
                        break;
    case Proc_OnDestroy: {
        ZCtlProc old_call = p->old_call;

        free(p);

        return old_call(ProcId, cp, 0, Param1, Param2, Param3);
        break;
    }
    case Proc_OnEvent: {
        TEventUI *event = (TEventUI *)Param1;
        switch (event->Type)
        {
        case ZEVENT_SETCURSOR:
        {
            if (cp->m_rcItem.right - cp->m_rcItem.left < cp->m_rcItem.bottom - cp->m_rcItem.top)
                SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
            else
                SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
            return 0;
        }
        break;
        case ZEVENT_LBUTTONDOWN: {
            p->mousedown = TRUE;
        }
                                 break;
        case ZEVENT_LBUTTONUP: {
            p->mousedown = FALSE;
        }
                               break;
        case ZEVENT_MOUSEMOVE: {
            if (p->mousedown && cp->m_pParent)
            {
                ZuiInt index;
                if ((index = ZuiControlCall(Proc_Layout_GetItemIndex, cp->m_pParent, cp, NULL, NULL)) > 0)
                {
                    ZuiControl lp = ZuiControlCall(Proc_Layout_GetItemAt, cp->m_pParent, index - 1, NULL, NULL);//上一个控件
                    ZuiControl np = ZuiControlCall(Proc_Layout_GetItemAt, cp->m_pParent, index + 1, NULL, NULL);//下一个控件
                    if (lp && np) {
                        if (cp->m_rcItem.right - cp->m_rcItem.left < cp->m_rcItem.bottom - cp->m_rcItem.top) {
                            if (event->ptMouse.x - lp->m_rcItem.left > lp->m_cxyMin.cx && np->m_rcItem.right - lp->m_rcItem.left - event->ptMouse.x + lp->m_rcItem.left - cp->m_rcItem.right + cp->m_rcItem.left > np->m_cxyMin.cx) {
                                ZuiInt width = np->m_rcItem.right - lp->m_rcItem.left;
                                if (p->type == 1) {
                                    ZuiControlCall(Proc_SetFixedWidth, lp, event->ptMouse.x - lp->m_rcItem.left, TRUE, NULL);
                                }
                                else if (p->type == 3)
                                {
                                    ZuiControlCall(Proc_SetFixedWidth, np, width - event->ptMouse.x + lp->m_rcItem.left - cp->m_rcItem.right + cp->m_rcItem.left, TRUE, NULL);
                                }
                            }
                            else
                                return 0;
                        }
                        else {
                            if (event->ptMouse.y - lp->m_rcItem.top > lp->m_cxyMin.cy && np->m_rcItem.bottom - lp->m_rcItem.top - event->ptMouse.y + lp->m_rcItem.top - cp->m_rcItem.bottom + cp->m_rcItem.top) {
                                ZuiInt height = np->m_rcItem.bottom - lp->m_rcItem.top;
                                if (p->type == 2) {
                                    ZuiControlCall(Proc_SetFixedHeight, lp, event->ptMouse.y - lp->m_rcItem.top, TRUE, NULL);
                                }
                                else if (p->type == 4)
                                {
                                    ZuiControlCall(Proc_SetFixedHeight, np, height - event->ptMouse.y + lp->m_rcItem.top - cp->m_rcItem.bottom + cp->m_rcItem.top, TRUE, NULL);
                                }
                            }
                            else
                                return 0;
                            //ZuiControlCall(Proc_SetFixedHeight, lp, event->ptMouse.y - lp->m_rcItem.top, NULL, NULL);
                        }
                        ZuiControlNeedUpdate(cp->m_pParent);
                    }
                }


            }
        }
                               break;
        default:
            break;
        }
    }
                       break;
    case Proc_SetAttribute: {
        if (wcscmp(Param1, L"sepside") == 0) {
            if (wcscmp(Param2, L"left") == 0) {
                p->type = 1;
            }
            else if (wcscmp(Param2, L"top") == 0) {
                p->type = 2;
            }
            else if (wcscmp(Param2, L"right") == 0) {
                p->type = 3;
            }
            else if (wcscmp(Param2, L"bottom") == 0) {
                p->type = 4;
            }
        }
        break;
    }
    case Proc_GetControlFlags: {
        return (ZuiAny)ZFLAG_SETCURSOR;
        break;
    }
    case Proc_OnPaint: {
        ZuiControlCall(Proc_OnPaintBkColor, cp, Param1, Param2, NULL);
        return 0;
        break;
    }
    default:
        break;
    }
    return p->old_call(ProcId, cp, 0, Param1, Param2, Param3);
}




