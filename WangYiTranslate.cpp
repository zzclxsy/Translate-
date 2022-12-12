#include "WangYiTranslate.h"

#define WANG_YI_URL "https://www.youdao.com/result?"
WangYiTranslate::WangYiTranslate()
{
    m_languageMap.insert(E_Language::EnglisToHan,"en");
    m_languageMap.insert(E_Language::HanToEnglis,"en");

    m_httpHeader.insert("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36 Edg/108.0.1462.42");
    m_httpHeader.insert("sec-ch-ua-platform","Windows");

}

QString WangYiTranslate::StartTranslate(QString text, E_Language lang)
{
    m_language =m_languageMap[lang];
    QString tranContent = "word=" +text + "&";
    QString language ="lang=" + m_language;
    QString url = WANG_YI_URL + tranContent + language;
    qDebug()<<url;
    QByteArray replay;
    m_http.getSyn(url,m_httpHeader,replay);
    return Translate(replay);
}

QString WangYiTranslate::Translate(QByteArray &replay)
{
    QString result = WordTranslate(replay);
    if (result.isEmpty() == false)
        return result;

    result = SentenceTranslate(replay);
    if (result.isEmpty() == false)
        return result;

    return "翻译失败";
}

QString WangYiTranslate::WordTranslate(QByteArray &replay)
{
    QRegExp rxlen("class=\"trans\" data-v-d80c723c>(.*)<");
    rxlen.setMinimal(true);
    int pos = rxlen.indexIn(replay);
    qDebug()<<"WordTranslate POS:"<<pos;
    if (pos > -1) {
        QString value = rxlen.cap(1);
        return value;
    }

    return QString();
}

QString WangYiTranslate::SentenceTranslate(QByteArray &replay)
{
    QRegExp rxlen("class=\"trans-content\" data-v-6eef0f9c>(.*)<");
    rxlen.setMinimal(true);
    int pos = rxlen.indexIn(replay);
    qDebug()<<"SentenceTranslate POS:"<<pos;
    if (pos > -1) {
        QString value = rxlen.cap(1);

        return value;
    }

    return QString();
}
