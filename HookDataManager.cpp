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
    message("选中待翻译文本,按住Ctrl的同时，再连续按2下C键进行翻译");
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
    }

        break;
    case e_WParam::KEYDOWN:
    {
        if (keyCode == KEY_CTRL)
        {
            if (!m_keyCtrlPress)
            {
                m_keyCtrlPress = true;
                m_keyCPress = 0;
            }
        }

        if (m_keyCtrlPress)
        {
            if (keyCode == KEY_C)
                m_keyCPress++;

            if (m_keyCPress == 2)
            {
                qDebug()<<"开始翻译";
                translate();
                m_keyCPress = 0;
            }
        }
    }
        break;
    case e_WParam::SYSKEYDOWN:
        break;
    case e_WParam::SYSKEYUP:
        break;
    }
}

void HookDataManager::translate()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasText())
    {
        //将图片数据转为QImage
        message(m_translate.StartTranslate(mimeData->text()));
    }
}

