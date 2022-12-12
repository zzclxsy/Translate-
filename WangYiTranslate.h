#ifndef WANGYITRANSLATE_H
#define WANGYITRANSLATE_H
#include "HttpHelper.h"
#include "ITranslate.h"
#include <QMap>
class WangYiTranslate : public ITranslate
{
public:
    WangYiTranslate();
    virtual QString StartTranslate(QString text, E_Language = EnglisToHan) override;

private:
    QString Translate(QByteArray &replay);
    QString WordTranslate(QByteArray &replay);
    QString SentenceTranslate(QByteArray &replay);
private:
    QString m_language;
    QMap<E_Language,QString> m_languageMap;
    HttpHelper m_http;
    QMap<QString ,QString> m_httpHeader;
};

#endif // WANGYITRANSLATE_H
