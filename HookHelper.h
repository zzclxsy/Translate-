#ifndef HOOKHELPER_H
#define HOOKHELPER_H

#include <iostream>
#include <Windows.h>
#include <string>
#include <list>
#include <mutex>
#include <functional>
using namespace std;

enum class e_WParam;
typedef std::function<void(e_WParam wParam, LPARAM lParam)>  f_keyEvent;
//typedef void (*f_keyEvent )(e_WParam wParam, LPARAM lParam);
typedef void (*f_print)(string str);
// 键盘消息
struct s_KeyCallMsg{
    string name;
    WPARAM listenWParam;
    f_keyEvent callback;
};
// 鼠标消息
struct s_MouseCallMsg{
    string name;
    WPARAM listenWParam;
    f_keyEvent callback;
};
// HOOK的种类
enum class e_HookType{
    MOUSE,
    KEY_BOARD
};
// 触发的事件
enum class e_WParam{
    // 按键
    KEYDOWN=0x0100,
    KEYUP=0x0101,
    SYSKEYDOWN=0x0104,
    SYSKEYUP=0x0105,
    // 鼠标
    LBUTTONUP=0x0202,
    LBUTTONDOWN=0x0201,
    RBUTTONDOWN=0x0204,
    RBUTTONUP=0x0205,
    // 滚动事件
    MOUSEHWHEEL=0x020E,
    MOUSEWHEEL=0x020A,
    // 鼠标移动
    MOUSEMOVE=0x0200
};
/**
 * @brief 获取虚拟键码
 * @param lParam
 * @return
 */
DWORD GetKeyCode(LPARAM lParam);
/**
 * @brief 获取鼠标位置
 * @param lParam
 * @return
 */
POINT GetMousePoint(LPARAM lParam);
/**
 * @brief 设置打印函数
 * @param f
 */
void SetPrintFunction(f_print f);
/**
 * @brief 添加新的键盘回调函数
 * @param callMsg
 * @return
 */
BOOL AddKeyBoardHookCallBack(s_KeyCallMsg callMsg);
/**
 * @brief 添加新的鼠标回调函数
 * @param callMsg
 * @return
 */
BOOL AddMouseHookCallBack(s_MouseCallMsg callMsg);
/**
 * @brief 卸载钩子
 * @param name 钩子名称
 * @param type 钩子类型
 * @return
 */
BOOL UninstallHook(string name,e_HookType type);
/**
 * @brief 卸载所有的钩子
 * @param type 钩子类型
 * @return
 */
BOOL UninstallAllHook(e_HookType type) ;

#endif // HOOKHELPER_H
