#ifndef HOOKDATAMANAGER_H
#define HOOKDATAMANAGER_H
#include <QObject>
#include "HookHelper.h"
#include "XunFeiTranslate.h"
#include "ITranslate.h"
#define KEY_CTRL 162
#define KEY_C 67
class HookDataManager:public QObject
{
    Q_OBJECT
public:
    HookDataManager(QObject *parent = nullptr);
    ~HookDataManager();
    Q_INVOKABLE void registerKeyBoardHook();
    Q_INVOKABLE void uninstallKeyBoardHook();

private:
    void keyEvent(e_WParam wParam, LPARAM lParam);
    void mouseEvent(e_WParam wParam, LPARAM lParam);
    void translate();
    QString getTranslate(QString data);

private:
    bool m_keyCtrlPress;
    int m_keyCPress;
    ITranslate *mp_translate;
signals:
    void message(QString mess);
    void clipDataChanged();
};

#endif // HOOKDATAMANAGER_H
