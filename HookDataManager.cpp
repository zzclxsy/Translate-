#include "HookDataManager.h"
#include <QDebug>
#include <functional>
#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>
HookDataManager::HookDataManager(QObject *parent):QObject(parent)
{
    m_keyCtrlPress = false;
    m_keyCPress = 0;
}

HookDataManager::~HookDataManager()
{
    uninstallKeyBoardHook();
}

void HookDataManager::registerKeyBoardHook()
{
    // 设置传送信息
    s_KeyCallMsg callMsg;
    callMsg.name = "keyCall";
    callMsg.callback = std::bind(&HookDataManager::keyEvent,this,std::placeholders::_1,std::placeholders::_2);
    callMsg.listenWParam = 0;

    BOOL flag = AddKeyBoardHookCallBack(callMsg);
    if (!flag)
        message("键盘钩子设置失败");
    message("键盘钩子设置成功");
}

void HookDataManager::uninstallKeyBoardHook()
{
    UninstallAllHook(e_HookType::KEY_BOARD);
}

void HookDataManager::keyEvent(e_WParam wParam, LPARAM lParam)
{
    DWORD keyCode;
    keyCode = GetKeyCode(lParam);
    switch(wParam){
    case e_WParam::KEYUP:
    {
        if (keyCode == KEY_CTRL)
            m_keyCtrlPress = false;

        if (m_keyCtrlPress)
        {
            if (keyCode == KEY_C)
                m_keyCPress++;

            if (m_keyCPress == 2)
            {
                getClipBoard();
                m_keyCPress = 0;
            }
        }
    }

        break;
    case e_WParam::KEYDOWN:
    {
        if (keyCode == KEY_CTRL)
        {
            m_keyCtrlPress = true;
            m_keyCPress = 0;
        }
    }
        break;
    case e_WParam::SYSKEYDOWN:
        message("alt按键按下") ;
        break;
    case e_WParam::SYSKEYUP:
        message("alt按键松开") ;
        break;
    }
}

void HookDataManager::getClipBoard()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasText())
    {
        //将图片数据转为QImage
        message(mimeData->text());
    }
}
