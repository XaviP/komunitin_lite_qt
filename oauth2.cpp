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

void Oauth2::check_tokens() {
    if (kSettingsP->access_token.isEmpty()) {
        hasAccess = false;
        emit new_auth();
    } else {
        if (kSettingsP->created + kSettingsP->expires_in < QDateTime::currentSecsSinceEpoch()) {
            hasAccess = false;
            refresh_tokens();
        } else {
            hasAccess = true;
            emit has_access();
        }
    }
}

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
        qDebug() << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        kSettingsP->access_token = jsonResponse.object()["access_token"].toString();
        kSettingsP->refresh_token = jsonResponse.object()["refresh_token"].toString();
        kSettingsP->created = QDateTime::currentSecsSinceEpoch();
        kSettingsP->expires_in = jsonResponse.object()["expires_in"].toInt();
        hasAccess = true;

        emit has_access();
    }
}

void Oauth2::refresh_tokens() {
    QUrlQuery query;
    query.addQueryItem("grant_type", "refresh_token");
    query.addQueryItem("refresh_token", kSettingsP->refresh_token);
    query.addQueryItem("username", kSettingsP->user_email);
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
            this, SLOT(refresh_tokens_reply(QNetworkReply*)), Qt::SingleShotConnection);
}

void Oauth2::refresh_tokens_reply(QNetworkReply* postReply) {
    if(postReply->error()) {
        qDebug() << "Error: " << postReply->errorString();
        postReply->deleteLater();
        emit error_auth();
    }
    else {
        QString strReply = postReply->readAll();
        postReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        kSettingsP->access_token = jsonResponse.object()["access_token"].toString();
        kSettingsP->refresh_token = jsonResponse.object()["refresh_token"].toString();
        kSettingsP->created = QDateTime::currentSecsSinceEpoch();
        kSettingsP->expires_in = jsonResponse.object()["expires_in"].toInt();
        hasAccess = true;

        emit has_access();
    }
}

void Oauth2::prepare_request(QNetworkRequest& networkRequest) {
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/vnd.api+json");
    networkRequest.setRawHeader("Authorization", QString("Bearer " + kSettingsP->access_token).toUtf8());
}
