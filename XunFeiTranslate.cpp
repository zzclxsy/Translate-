#include "XunFeiTranslate.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QDebug>
#include <QMessageAuthenticationCode>
#define API_KEY "a81d7a247f6bdd912b46b9d7ba85b2e0"
#define APPID "5f40bedb"
#define APISecret "514bca1388c6bc0dc1e95699cd00a486"
#define HOST "itrans.xfyun.cn"
#define RequestUri "/v2/its"
#define HttpMethod  "POST"
#define Algorithm   "hmac-sha256"
#define HttpProto  "HTTP/1.1"

Translate::Translate()
{
    m_languageDireMap.insert(E_Language::EnglisToHan,S_Direction{"en","cn"});
    m_languageDireMap.insert(E_Language::HanToEnglis,S_Direction{"cn","en"});
}

QString Translate::StartTranslate(QString text, E_Language language)
{
    m_date = GetRFC1123Time();
    S_Direction direct = m_languageDireMap[language];
    QString body = GetHttpBody(APPID, direct.from, direct.to, text);
    GetSignature(body);
    QMap<QString, QString> header = GetHttpHeader();
    QByteArray replyData;
    if (m_http.postSyn("https://itrans.xfyun.cn/v2/its",header,body.toUtf8(),replyData))
    {
        return GetDestText(replyData);
    }

    return QString();
}

QString Translate::GetHttpBody(QString app_id, QString from, QString to, QString text)
{
    QJsonObject root;
    QJsonObject common;
    QJsonObject business;
    QJsonObject data;
    common["app_id"] = app_id;
    business["from"] = from;
    business["to"] = to;
    data["text"] = QString(text.toUtf8().toBase64());

    root["common"] = common;
    root["business"] = business;
    root["data"] = data;

    QJsonDocument JsonDoc;
    JsonDoc.setObject(root);
    QByteArray  jsonStr = JsonDoc.toJson(QJsonDocument::Compact);
    QString strJson(jsonStr);
    qDebug()<<"Translate::GetHttpBody"<<strJson.toStdString().c_str();
    return strJson;
}

QMap<QString, QString> Translate::GetHttpHeader()
{
    QMap<QString, QString> header;

    header["Content-Type"] = "application/json";
    header["Accept"] = "application/json";
    header["Host"] =  HOST;
    header["Date"] = m_date;
    header["Authorization"] = GetAuthorization();
    header["Digest"] = m_digest;
    return header;
}

QString Translate::GetRFC1123Time()
{
    QLocale locale = QLocale::English;
    QString format = "ddd,dd MMM yyyy hh:mm:ss";
    QString currentTime =locale.toString(QDateTime::currentDateTimeUtc(), format) + " GMT";
    return currentTime;
}

QString Translate::GetAuthorization()
{
    QString author = QString("api_key=\"%1\", algorithm=\"hmac-sha256\", headers=\"host date request-line digest\",signature=\"%2\"").arg(API_KEY).arg(m_sign);
    return author;
}

void Translate::GetSignature(QString body)
{
    QCryptographicHash SHA256(QCryptographicHash::Sha256);
    SHA256.addData(body.toLatin1().data());
    m_digest = QString("SHA-256=" +SHA256.result().toBase64());

    QString signatureStr;
    signatureStr = "host: " + QString(HOST)  + "\n";
    signatureStr += "date: " + m_date + "\n";
    signatureStr += QString(HttpMethod) + " " + QString(RequestUri)  + " " + QString(HttpProto) + "\n";
    signatureStr += "digest: " + m_digest;
    m_sign = QMessageAuthenticationCode::hash(signatureStr.toUtf8(),APISecret, QCryptographicHash::Sha256).toBase64();
}

QString Translate::GetDestText(QByteArray data)
{
    QJsonDocument document;
    QJsonParseError parseJsonErr;
    document = QJsonDocument::fromJson(data, &parseJsonErr);

    do
    {
        if(parseJsonErr.error != QJsonParseError::NoError)
            break;

        if(!document.isObject())
            break;

        QJsonObject obj = document.object();
        if (!obj.contains("data"))
            break;

        QJsonObject data = obj.value("data").toObject();
        if (!data.contains("result"))
            break;

        QJsonObject result = data.value("result").toObject();
        if (!result.contains("trans_result"))
            break;
        QJsonObject trans_result = result.value("trans_result").toObject();

        if (!trans_result.contains("dst"))
            break;
        QString value  = trans_result.value("dst").toString();

        return value;

    } while(0);

    return QString();
}
