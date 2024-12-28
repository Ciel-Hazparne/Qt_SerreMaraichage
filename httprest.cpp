#include "httprest.h"

HttpRest::HttpRest():
    dataBuffer(new QByteArray)

{

}

void HttpRest::requeteRestPOST(QString url_str, QString data)
{
    QByteArray message(data.toStdString().c_str());
    QByteArray postDataSize = QByteArray::number(message.size());
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Envoie POST");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", postDataSize);
    reply = qnam.post(request, message);
    connect(reply, &QIODevice::readyRead, this, &HttpRest::httpReadyRead);
    connect(reply, &QNetworkReply::finished, this, &HttpRest::httpFinished);
}

void HttpRest::httpReadyRead()
{
    dataBuffer->append(reply->readAll());
}

void HttpRest::httpFinished()
{
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug()<< "Pas d'erreur ";
        qDebug()<< reply->rawHeader("Content-Type");
    }
    else
        qDebug()<< "Erreur: " << reply->error();

    reply->deleteLater();
    reply = Q_NULLPTR;
}

QNetworkReply* HttpRest::getReply()
{
    return reply;
}

QByteArray* HttpRest::getDataBuffer()
{
    return dataBuffer;
}

HttpRest::~HttpRest()
{

}
