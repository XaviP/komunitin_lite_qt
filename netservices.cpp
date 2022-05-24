#include <string>
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

QString baseApiUrl = "https://demo.integralces.net/ces/api";
const char oauth2TokenUrl[] = "https://demo.integralces.net/oauth2/token";
const char oauth2ClientId[] = "odoo-pos-komunitin";
//const char oauth2ClientPassword[] = "xxx-xxx-xxx-xxx";
const char oauth2Scope[] = "komunitin_accounting komunitin_social profile";

netServices::netServices(QObject *parent)
    : QObject(parent),
    netManager(new QNetworkAccessManager(this)),
    hasAccess(false),
    accounts(),
    index_current_acc(0),
    comma_list("")
{}

netServices::~netServices() { delete netManager; }

void netServices::get_access(const std::string& email, const std::string& password)
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
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/x-www-form-urlencoded");

    netManager->post(networkRequest, postData);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_access_reply(QNetworkReply*)), Qt::SingleShotConnection);
}

void netServices::get_access_reply(QNetworkReply* postReply) {

    if(postReply->error()) {
        qDebug() << "Error: " << postReply->errorString();
        postReply->deleteLater();
        emit error_auth();
    }
    else {
        QString strReply = postReply->readAll();
        postReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        this->authHeaderValue = QString("Bearer " + jsonResponse.object()
                                        ["access_token"].toString()).toUtf8();
        this->hasAccess = true;
        qDebug() << "going to emit has_access";
        emit has_access();
    }
}

void netServices::get_accounts() {
    QNetworkRequest networkRequest(
        QUrl(baseApiUrl + "/social/users/me?include=members"));
    prepare_request(networkRequest);

    netManager->get(networkRequest);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_accounts_reply(QNetworkReply*)), Qt::SingleShotConnection);
}

void netServices::get_accounts_reply(QNetworkReply* getReply) {
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
        emit network_error();
        }
    else {
        QString strReply = getReply->readAll();
        getReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        std::string user_id = jsonResponse.object()["data"].toObject()["id"].
                toString().toStdString();
        int sizeArray = jsonResponse.object()["included"].toArray().size();

        QJsonObject acc;
        for (int i=0; i < sizeArray; i++) {
            acc = jsonResponse.object()["included"].toArray()[i].toObject();
            accounts.push_back(account(user_id, acc["id"].toString().toStdString()));
            accounts[i].member_name = acc["attributes"].toObject()["name"].
                    toString().toStdString();
            accounts[i].member_image = acc["attributes"].toObject()["image"].
                    toString().toStdString();
            accounts[i].account_id = acc["relationships"].toObject()["account"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            accounts[i].account_code = acc["attributes"].toObject()["code"].
                    toString().toStdString();
            accounts[i].account_link = acc["relationships"].toObject()["account"].
                    toObject()["links"].toObject()["related"].toString().toStdString();
            accounts[i].group_id = acc["relationships"].toObject()["group"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            accounts[i].group_code = accounts[i].account_code.substr(0, 4);
        };
        index_current_acc = 0;
        emit has_accounts();
    }
}

void netServices::get_account_balance() {
    qDebug() << QString::fromStdString(accounts[index_current_acc].account_link);
    QNetworkRequest networkRequest(
        QUrl(QString::fromStdString(accounts[index_current_acc].account_link) +
             "?include=currency"));
    prepare_request(networkRequest);

    netManager->get(networkRequest);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_account_balance_reply(QNetworkReply*)), Qt::SingleShotConnection);
}

void netServices::get_account_balance_reply(QNetworkReply* getReply) {
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
        emit network_error();
    }
    else {
        QString strReply = getReply->readAll();
        getReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        accounts[index_current_acc].balance = jsonResponse.object()["data"].toObject()["attributes"].
                toObject()["balance"].toDouble();
        accounts[index_current_acc].currency.id =  jsonResponse.object()["included"].toArray()[0].
                toObject()["id"].toString().toStdString();
        QJsonObject attr = jsonResponse.object()["included"].toArray()[0].
                toObject()["attributes"].toObject();
        accounts[index_current_acc].currency.name = attr["name"].toString().toStdString();
        accounts[index_current_acc].currency.plural = attr["namePlural"].toString().toStdString();
        accounts[index_current_acc].currency.symbol = attr["symbol"].toString().toStdString();
        accounts[index_current_acc].currency.decimals = attr["decimals"].toString().toInt();
        emit has_balance();
    }
}

void netServices::get_account_transfers() {
    QString url = baseApiUrl + "/accounting/" +
            QString::fromStdString(accounts[index_current_acc].group_code) +
            "/transfers?filter[account]=" +
            QString::fromStdString(accounts[index_current_acc].account_id);
    QNetworkRequest networkRequest(url);
    prepare_request(networkRequest);

    netManager->get(networkRequest);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_account_transfers_reply(QNetworkReply*)), Qt::SingleShotConnection);

}

void netServices::get_account_transfers_reply(QNetworkReply* getReply) {
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
        emit network_error();
    }
    else {
        QString strReply = getReply->readAll();
        getReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        int sizeArray = jsonResponse.object()["data"].toArray().size();

        QJsonObject trans;
        std::vector<string> unknown_accounts;
        for (int i=0; i < sizeArray; i++) {
            trans = jsonResponse.object()["data"].toArray()[i].toObject();
            accounts[index_current_acc].transfers.push_back(transfer(trans["id"].toString().toStdString()));
            transfer* p = &accounts[index_current_acc].transfers.back();
            p->amount = trans["attributes"].toObject()["amount"].toInt();
            p->meta = trans["attributes"].toObject()["meta"].
                    toString().toStdString();
            p->state = trans["attributes"].toObject()["state"].
                    toString().toStdString();
            p->created = trans["attributes"].toObject()["created"].
                    toString().toStdString();
            p->updated = trans["attributes"].toObject()["updated"].
                    toString().toStdString();
            p->payer_account_id = trans["relationships"].toObject()["payer"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->payer_account_id == accounts[index_current_acc].account_id) {
                p->payer_account_code = accounts[index_current_acc].account_code;
            }
            else {unknown_accounts.push_back(p->payer_account_id);}
            p->payee_account_id = trans["relationships"].toObject()["payee"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->payee_account_id == accounts[index_current_acc].account_id) {
                p->payee_account_code = accounts[index_current_acc].account_code;
            }
            else {unknown_accounts.push_back(p->payee_account_id);}
            p->currency.id = trans["relationships"].toObject()["currency"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->currency.id == accounts[index_current_acc].currency.id) {
                p->currency = accounts[index_current_acc].currency;
            }
        }
        for(int j= 0; j < (int)unknown_accounts.size(); j++) {
            comma_list += unknown_accounts[j];
            if (unknown_accounts[j] != unknown_accounts.back()) comma_list += ",";
        }
        emit has_transfers();
    }

}

void netServices::get_unknown_accounts() {
    QString url = baseApiUrl + "/social/" + QString::fromStdString(accounts[index_current_acc].group_code) +
            "/members?filter[account]=" + QString::fromStdString(comma_list);
    QNetworkRequest networkRequest(url);
    prepare_request(networkRequest);

    netManager->get(networkRequest);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_unknown_accounts_reply(QNetworkReply*)), Qt::SingleShotConnection);
}

void netServices::get_unknown_accounts_reply(QNetworkReply* getReply) {
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
        emit network_error();
    }
    else {
        QString strReply = getReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        int sizeArray = jsonResponse.object()["data"].toArray().size();
        QJsonObject da;
        for (int i=0; i < sizeArray; i++) {
            da = jsonResponse.object()["data"].toArray()[i].toObject();
            string id = da["relationships"].toObject()["account"].toObject()
                    ["data"].toObject()["id"].toString().toStdString();
            for (int j=0; j < (int)accounts[index_current_acc].transfers.size(); j++) {
                transfer* p = &accounts[index_current_acc].transfers[j];
                if (id == p->payer_account_id) {
                    p->payer_account_code = da["attributes"].toObject()["code"].
                            toString().toStdString();
                } else {
                    if (id == p->payee_account_id) {
                        p->payee_account_code = da["attributes"].toObject()["code"].
                            toString().toStdString();
                    }
                }
            }
        }
        emit has_all_data();
    }
}

void netServices::prepare_request(QNetworkRequest& networkRequest) {
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/vnd.api+json");
    networkRequest.setRawHeader("Authorization", this->authHeaderValue);
}
