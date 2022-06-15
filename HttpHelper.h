#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
class HttpHelper : public QObject
{
    Q_OBJECT
public:
    explicit HttpHelper(QObject* parent = nullptr);

    /** \fn postSyn
     *
     *	 post请求
     *
     *	\param url :网址
     *	\param headerdata :请求头数据
     *  \param requestData:请求数据
     *  \param replydata:返回数据
     */
     bool postSyn(QString url, QMap<QString, QString> headerdata, QByteArray requestData, QByteArray& replydata);

    /** \fn getSyn
     *
     *	 get请求
     *
     *	\param url :网址
     *	\param headerdata :请求头数据
     *  \param replydata:返回数据
     */
    bool getSyn(QString url, QMap<QString, QString> headerdata, QByteArray& replydata);
signals:

public slots:
};
#endif // HTTPHELPER_H
