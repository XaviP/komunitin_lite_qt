#include<string>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>

#include "netservices.h"
#include "account.h"
#include "transfer.h"

const QString baseApiUrl = "https://demo.integralces.net/ces/api";
const char oauth2TokenUrl[] = "https://demo.integralces.net/oauth2/token";
const char oauth2ClientId[] = "odoo-pos-komunitin";
//const char oauth2ClientPassword[] = "xxx-xxx-xxx-xxx";
const char oauth2Scope[] = "komunitin_accounting komunitin_social profile";

netServices::netServices(QObject *parent) : QObject(parent),
    netManager(new QNetworkAccessManager(this)), hasAccess(false)
{}

void netServices::get_access(std::string email, std::string password)
{
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
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QEventLoop loop;
    QNetworkReply *postReply = this->netManager->post(networkRequest,postData);
    connect(postReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(postReply->error()) {qDebug() << "Error: " << postReply->errorString();}
    else {
        QString strReply = postReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        this->authHeaderValue = QString("Bearer " + jsonResponse.object()["access_token"].toString()).toUtf8();
        this->hasAccess = true;
        qDebug() << strReply;
    }
}

void netServices::get_accounts(std::vector<account>& accs)
{
    QNetworkReply *getReply = nullptr;
    this->get_call(baseApiUrl + "/social/users/me?include=members", getReply);
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
    }
    else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(getReply->readAll()).toUtf8());
        std::string user_id = jsonResponse.object()["data"].toObject()["id"].toString().toStdString();
        int sizeArray = jsonResponse.object()["included"].toArray().size();

        QJsonObject acc;
        for (int i=0; i < sizeArray; i++) {
            acc = jsonResponse.object()["included"].toArray()[i].toObject();
            accs.push_back(account(user_id, acc["id"].toString().toStdString()));
            accs[i].member_name = acc["attributes"].toObject()["name"].toString().toStdString();
            accs[i].member_image = acc["attributes"].toObject()["image"].toString().toStdString();
            accs[i].account_id = acc["relationships"].toObject()["account"].toObject()["data"].toObject()["id"].toString().toStdString();
            accs[i].account_code = acc["attributes"].toObject()["code"].toString().toStdString();
            accs[i].account_link = acc["relationships"].toObject()["account"].toObject()["links"].toObject()["related"].toString().toStdString();
            accs[i].group_id = acc["relationships"].toObject()["group"].toObject()["data"].toObject()["id"].toString().toStdString();
            accs[i].group_code = accs[i].account_code.substr(0, 4);
        };
    }
}

void netServices::get_account_balance(account* acc) {
    QNetworkReply *getReply = nullptr;
        this->get_call(QString::fromStdString(acc->account_link) + "?include=currency", getReply);
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
    }
    else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(getReply->readAll()).toUtf8());
        acc->balance = jsonResponse.object()["data"].toObject()["attributes"].toObject()["balance"].toDouble();
        acc->currency.id =  jsonResponse.object()["included"].toArray()[0].toObject()["id"].toString().toStdString();
        QJsonObject attr = jsonResponse.object()["included"].toArray()[0].toObject()["attributes"].toObject();
        acc->currency.name = attr["name"].toString().toStdString();
        acc->currency.plural = attr["namePlural"].toString().toStdString();
        acc->currency.symbol = attr["symbol"].toString().toStdString();
        acc->currency.decimals = attr["decimals"].toString().toInt();
    }
}

void netServices::get_account_transfers(account* acc) {
    QString url = baseApiUrl + "/accounting/" + QString::fromStdString(acc->group_code) +
            "/transfers?filter[account]=" + QString::fromStdString(acc->account_id);
    QNetworkReply *getReply = nullptr;
    this->get_call(url, getReply);
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
    }
    else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(getReply->readAll()).toUtf8());
        int sizeArray = jsonResponse.object()["data"].toArray().size();

        QJsonObject trans;
        for (int i=0; i < sizeArray; i++) {
            trans = jsonResponse.object()["data"].toArray()[i].toObject();
            acc->transfers.push_back(transfer(trans["id"].toString().toStdString()));
            transfer* p = &acc->transfers.back();
            p->amount = trans["attributes"].toObject()["amount"].toInt();
            p->meta = trans["attributes"].toObject()["meta"].toString().toStdString();
            p->state = trans["attributes"].toObject()["state"].toString().toStdString();
            p->created = trans["attributes"].toObject()["created"].toString().toStdString();
            p->updated = trans["attributes"].toObject()["updated"].toString().toStdString();
            p->payer_account_id = trans["relationships"].toObject()["payer"].toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->payer_account_id == acc->account_id) {
                p->payer_account_code = acc->account_code;
            }
            p->payee_account_id = trans["relationships"].toObject()["payee"].toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->payee_account_id == acc->account_id) {
                p->payee_account_code = acc->account_code;
            }
            p->currency.id = trans["relationships"].toObject()["currency"].toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->currency.id == acc->currency.id) {
                p->currency = acc->currency;
            }
        }
    }

}

void netServices::get_call(QString url, QNetworkReply*& getReply) {
    QUrl getUrl = QUrl(url);
    qDebug() << getUrl;
    QNetworkRequest networkRequest(getUrl);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/vnd.api+json");
    networkRequest.setRawHeader("Authorization", this->authHeaderValue);

    QEventLoop loop;
    getReply = this->netManager->get(networkRequest);
    connect(getReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}
