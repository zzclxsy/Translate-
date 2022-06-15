#include "HookHelper.h"
// 必要的静态链接库
#pragma comment  (lib,"User32.lib")
#pragma comment  (lib,"Gdi32.lib")

// 配置打印方式
HHOOK g_keyHHook;
HHOOK g_mouseHHook;
list<s_KeyCallMsg>  g_keyCallMsgList;
list<s_MouseCallMsg>  g_mouseCallMsgList;
BOOL  g_keyIsInstall=FALSE;
BOOL  g_mouseIsInstall=FALSE;
f_print g_print = NULL;
static mutex  g_keyMu; // 线程锁
static mutex  g_mouseMu; // 线程锁


void _PrivateLog(string str){
    if(g_print==NULL){
        return;
    }
    (*g_print)(str);
}


/**
 * @brief 获取虚拟键码
 * @param lParam
 * @return
 */
DWORD GetKeyCode(LPARAM lParam){
    PKBDLLHOOKSTRUCT st = (PKBDLLHOOKSTRUCT)lParam;
    return st->vkCode;
}
/**
 * @brief 获取鼠标位置
 * @param lParam
 * @return
 */
POINT GetMousePoint(LPARAM lParam){
    PMSLLHOOKSTRUCT st =(PMSLLHOOKSTRUCT)lParam;
    return st->pt;
}

/// <summary>
/// 键盘回调
/// </summary>
/// <param name="code"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK ProcForKeyBoard(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0 || code == HC_NOREMOVE) {
        // 如果代码小于零，则挂钩过程必须将消息传递给CallNextHookEx函数，而无需进一步处理，并且应返回CallNextHookEx返回的值。此参数可以是下列值之一。(来自官网手册)
        return CallNextHookEx(g_keyHHook, code, wParam, lParam);
    }
    DWORD vkCode = GetKeyCode(lParam);
    for(auto it=g_keyCallMsgList.begin();it!=g_keyCallMsgList.end();it++){
        s_KeyCallMsg callMsg = *it;
        if(callMsg.listenWParam == 0){
            // 监听任何一个回调
            callMsg.callback(e_WParam(wParam),lParam);
        }else if(callMsg.listenWParam == vkCode){
            // 监听指定的回调
            callMsg.callback(e_WParam(wParam),lParam);
        }
    }
    // 0x0200
    // 将钩子往下传
    return CallNextHookEx( g_keyHHook, code, wParam, lParam);
}

/// <summary>
/// 键盘回调
/// </summary>
/// <param name="code"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK ProcForMouseBoard(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0 || code == HC_NOREMOVE) {
        // 如果代码小于零，则挂钩过程必须将消息传递给CallNextHookEx函数，而无需进一步处理，并且应返回CallNextHookEx返回的值。此参数可以是下列值之一。(来自官网手册)
        return CallNextHookEx(g_mouseHHook, code, wParam, lParam);
    }
    for(auto it=g_mouseCallMsgList.begin();it!=g_mouseCallMsgList.end();it++){
        s_MouseCallMsg callMsg = *it;
        if(callMsg.listenWParam == 0){
            // 监听任何一个回调
            callMsg.callback(e_WParam(wParam),lParam);
        }else if(callMsg.listenWParam == wParam){
            // 监听指定的回调
            callMsg.callback(e_WParam(wParam),lParam);
        }
    }
    // 0x0200
    // 将钩子往下传
    return CallNextHookEx( g_mouseHHook, code, wParam, lParam);
}


/**
 * @brief 设置打印函数
 * @param f
 */
void SetPrintFunction(f_print f){
    g_print = f;
}

/**
 * @brief 安装钩子
 * @return
 */
BOOL InstallHook(e_HookType type) {
    BOOL flag = FALSE;
    if(type==e_HookType::KEY_BOARD){
        // 键盘回调事件
        if(!g_keyIsInstall){
            g_keyMu.lock();
            // 两次判断防止出现线程安全问题
            if(!g_keyIsInstall){
                // 【参数1】钩子的类型，这里代表键盘钩子
                // 【参数2】钩子处理的函数
                // 【参数3】如果是DLL项目得写 GetModuleHandle("dll文件名"),如果应用程序，直接写 GetModuleHandle(NULL)或者nullptr都行
                // 【参数4】线程的ID，如果是全局钩子的话，这里要填0，如果是某个线程的钩子，那就需要写线程的ID
                g_keyHHook = SetWindowsHookEx(WH_KEYBOARD_LL, ProcForKeyBoard, GetModuleHandle(NULL), 0);
                if ( g_keyHHook == NULL) {
                    // 钩子安装失败
                    _PrivateLog("全局钩子注册失败");
                    flag = FALSE;
                    g_keyIsInstall = FALSE;
                }else{
                    flag = TRUE;
                    g_keyIsInstall = TRUE;
                }
            }
            g_keyMu.unlock();
        }
    }else if(type == e_HookType::MOUSE){
        // 鼠标回调事件
        if(!g_mouseIsInstall){
            g_mouseMu.lock();
            if(!g_mouseIsInstall){
                // 【参数1】钩子的类型，这里代表键盘钩子
                // 【参数2】钩子处理的函数
                // 【参数3】如果是DLL项目得写 GetModuleHandle("dll文件名"),如果应用程序，直接写 GetModuleHandle(NULL)或者nullptr都行
                // 【参数4】线程的ID，如果是全局钩子的话，这里要填0，如果是某个线程的钩子，那就需要写线程的ID
                g_mouseHHook = SetWindowsHookEx(WH_MOUSE_LL, ProcForMouseBoard, GetModuleHandle(NULL), 0);
                if ( g_mouseHHook == NULL) {
                    // 钩子安装失败
                    _PrivateLog("全局钩子注册失败");
                    flag =  FALSE;
                    g_mouseIsInstall = FALSE;
                }else{
                    flag = TRUE;
                    g_mouseIsInstall = TRUE;
                }
            }
            g_mouseMu.unlock();
        }
    }
    return flag;
}
/**
 * @brief 添加新的回调函数
 * @param callMsg
 * @return
 */
BOOL AddKeyBoardHookCallBack(s_KeyCallMsg callMsg){
    BOOL flag = InstallHook(e_HookType::KEY_BOARD);
    if(!flag){
        // 安装钩子失败
        return FALSE;
    }
    // 安装成功
    g_keyCallMsgList.push_back(callMsg);
    return TRUE;
}
/**
 * @brief 添加新的回调函数
 * @param callMsg
 * @return
 */
BOOL AddMouseHookCallBack(s_MouseCallMsg callMsg){
    BOOL flag = InstallHook(e_HookType::MOUSE);
    if(!flag){
        // 安装钩子失败
        return FALSE;
    }
    // 安装成功
    g_mouseCallMsgList.push_back(callMsg);
    return TRUE;
}

/**
 * @brief 卸载钩子
 * @param name 钩子名称
 * @param type 钩子类型
 * @return
 */
BOOL UninstallHook(string name,e_HookType type) {
    BOOL unFlag = FALSE;
    switch(type){
    case e_HookType::KEY_BOARD:
        // 卸载键盘钩子
        g_keyMu.lock();
        if(g_keyIsInstall){
            size_t size = g_keyCallMsgList.size();
            if(size==1){
                unFlag = UnhookWindowsHookEx( g_keyHHook);
                if(unFlag){
                    g_keyCallMsgList.clear();
                }
                g_keyIsInstall = unFlag ? FALSE : TRUE;
            }else if(size>1){
                // 找到要卸载的钩子的名称
                for(auto it=g_keyCallMsgList.begin();it!=g_keyCallMsgList.end();it++){
                    s_KeyCallMsg callMsg = *it;
                    if(callMsg.name == name){
                        g_keyCallMsgList.erase(it);
                        break;
                    }
                }
                unFlag=TRUE;
            }
        }else{
            // 钩子已经被卸载
            unFlag = TRUE;
        }
        g_keyMu.unlock();
        return unFlag;
    case e_HookType::MOUSE:
        // 卸载鼠标钩子
        g_mouseMu.lock();
        if(g_mouseIsInstall){
            size_t size = g_mouseCallMsgList.size();
            if(size==1){
                unFlag = UnhookWindowsHookEx(g_mouseHHook);
                if(unFlag){
                   g_mouseCallMsgList.clear();
                }
                g_mouseIsInstall = unFlag ? FALSE : TRUE;
            }else if(size>1){
                for(auto it=g_mouseCallMsgList.begin();it!=g_mouseCallMsgList.end();it++){
                    s_MouseCallMsg callMsg = *it;
                    if(callMsg.name == name){
                        g_mouseCallMsgList.erase(it);
                        break;
                    }
                }
                unFlag=TRUE;
            }
        }else{
            // 钩子已经被卸载
            unFlag = TRUE;
        }
        g_mouseMu.unlock();
        return unFlag;
     default:
        return TRUE;
    }
}

/**
 * @brief 卸载所有的钩子
 * @param name 钩子名称
 * @param type 钩子类型
 * @return
 */
BOOL UninstallAllHook(e_HookType type) {
    BOOL unFlag = FALSE;
    switch(type){
    case e_HookType::KEY_BOARD:
        // 卸载键盘钩子
        g_keyMu.lock();
        if(g_keyIsInstall){
            unFlag = UnhookWindowsHookEx( g_keyHHook);
            if(unFlag){
                g_keyCallMsgList.clear();
            }
            g_keyIsInstall = unFlag ? FALSE : TRUE;
        }else{
            // 钩子已经被卸载
            unFlag = TRUE;
        }
        g_keyMu.unlock();
        return unFlag;
    case e_HookType::MOUSE:
        // 卸载鼠标钩子
        g_mouseMu.lock();
        if(g_mouseIsInstall){
            unFlag = UnhookWindowsHookEx(g_mouseHHook);
            if(unFlag){
               g_mouseCallMsgList.clear();
            }
            g_mouseIsInstall = unFlag ? FALSE : TRUE;
        }else{
            // 钩子已经被卸载
            unFlag = TRUE;
        }
        g_mouseMu.unlock();
        return unFlag;
     default:
        return TRUE;
    }
}
