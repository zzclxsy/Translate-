#ifndef XUNFEITRANSLATE_H
#define XUNFEITRANSLATE_H
#include <qstring.h>
#include <QList>
#include "HttpHelper.h"
class Translate
{
public:
    typedef enum{
        HanToEnglis,
        EnglisToHan
    }E_Language;

    Translate();
    QString StartTranslate(QString text, E_Language = EnglisToHan);

private:

    typedef struct{
        QString from;
        QString to;
    }S_Direction;

    QString GetHttpBody(QString app_id, QString from, QString to, QString text);
    QMap<QString, QString> GetHttpHeader();
    QString GetRFC1123Time();
    QString GetAuthorization();
    void GetSignature(QString body);
    QString GetDestText(QByteArray data);

private:
    QList<QString> m_week;
    QList<QString> m_month;
    QString m_date;
    QString m_digest;
    QString m_sign;
    HttpHelper m_http;
    QMap<E_Language, S_Direction> m_languageDireMap;
};

#endif // XUNFEITRANSLATE_H
