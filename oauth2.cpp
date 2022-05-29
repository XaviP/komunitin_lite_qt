#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

#include "oauth2.h"

const char oauth2TokenUrl[] = "https://demo.integralces.net/oauth2/token";
const char oauth2ClientId[] = "odoo-pos-komunitin";
//const char oauth2ClientPassword[] = "xxx-xxx-xxx-xxx";
const char oauth2Scope[] = "komunitin_accounting komunitin_social profile";

Oauth2::Oauth2(QNetworkAccessManager* netManagerP, QObject *parent)
    : QObject(parent),
      netM(netManagerP),
      kSettingsP(),
      hasAccess(false)
{}

void Oauth2::get_access(const std::string& email, const std::string& password)
{
    kSettingsP->user_email = QString::fromStdString(email);

    QUrlQuery query;
    query.addQueryItem("grant_type","password");
    query.addQueryItem("username", QString::fromStdString(email));
    query.addQueryItem("password", QString::fromStdString(password));
    query.addQueryItem("client_id", oauth2ClientId);
//    query.addQueryItem("client_secret", oauth2ClientPassword);
    query.addQueryItem("scope", oauth2Scope);

    QByteArray postData;
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    QUrl tokenUrl = QUrl(oauth2TokenUrl);
    QNetworkRequest networkRequest(tokenUrl);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/x-www-form-urlencoded");

    netM->post(networkRequest, postData);
    connect(netM, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_access_reply(QNetworkReply*)), Qt::SingleShotConnection);
}

void Oauth2::get_access_reply(QNetworkReply* postReply) {

    if(postReply->error()) {
        qDebug() << "Error: " << postReply->errorString();
        postReply->deleteLater();
        emit error_auth();
    }
    else {
        QString strReply = postReply->readAll();
        postReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        access_token = jsonResponse.object()["access_token"].toString();
        refresh_token = jsonResponse.object()["refresh_token"].toString();
        kSettingsP->access_token = access_token;
        kSettingsP->refresh_token = refresh_token;
        hasAccess = true;

        emit has_access();
    }
}

void Oauth2::prepare_request(QNetworkRequest& networkRequest) {
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/vnd.api+json");
    networkRequest.setRawHeader("Authorization", QString("Bearer " + access_token).toUtf8());
}
