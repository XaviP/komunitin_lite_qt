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

netServices::netServices(QObject *parent) : QObject(parent),
    netManager(new QNetworkAccessManager(this)), hasAccess(false)
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
            this, SLOT(get_access_reply(QNetworkReply*)), Qt::QueuedConnection);
}

void netServices::get_access_reply(QNetworkReply* postReply) {

    if(postReply->error()) {
        qDebug() << "Error: " << postReply->errorString();
        postReply->deleteLater();
        emit access_reply(true);
    }
    else {
        QString strReply = postReply->readAll();
        postReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        this->authHeaderValue = QString("Bearer " + jsonResponse.object()
                                        ["access_token"].toString()).toUtf8();
        this->hasAccess = true;
        emit access_reply(false);

    }
}

void netServices::get_accounts()
{
    QNetworkRequest networkRequest(
        QUrl(baseApiUrl + "/social/users/me?include=members"));
    prepare_request(networkRequest);

    netManager->get(networkRequest);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_accounts_reply(QNetworkReply*))); // Qt::QueuedConnection crashes.
}

void netServices::get_accounts_reply(QNetworkReply* getReply) {
    std::vector<account> accs;
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
        emit accounts_reply(true, accs);
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
            accs.push_back(account(user_id, acc["id"].toString().toStdString()));
            accs[i].member_name = acc["attributes"].toObject()["name"].
                    toString().toStdString();
            accs[i].member_image = acc["attributes"].toObject()["image"].
                    toString().toStdString();
            accs[i].account_id = acc["relationships"].toObject()["account"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            accs[i].account_code = acc["attributes"].toObject()["code"].
                    toString().toStdString();
            accs[i].account_link = acc["relationships"].toObject()["account"].
                    toObject()["links"].toObject()["related"].toString().toStdString();
            accs[i].group_id = acc["relationships"].toObject()["group"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            accs[i].group_code = accs[i].account_code.substr(0, 4);
        };
        emit accounts_reply(false, accs);
    }
}

void netServices::get_account_data(account* acc) {
    get_account_balance(acc);
//    get_account_transfers(acc);

}


void netServices::get_account_balance(account* acc) {
    current_account = acc;
    QNetworkRequest networkRequest(
        QUrl(baseApiUrl + QString::fromStdString(acc->account_link) +
             "?include=currency"));
    prepare_request(networkRequest);

    netManager->get(networkRequest);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_account_balance_reply(QNetworkReply*)));
}

void netServices::get_account_balance_reply(QNetworkReply* getReply) {
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
        emit account_balance_reply(true);
    }
    else {
        QString strReply = getReply->readAll();
        getReply->deleteLater();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        current_account->balance = jsonResponse.object()["data"].toObject()["attributes"].
                toObject()["balance"].toDouble();
        current_account->currency.id =  jsonResponse.object()["included"].toArray()[0].
                toObject()["id"].toString().toStdString();
        QJsonObject attr = jsonResponse.object()["included"].toArray()[0].
                toObject()["attributes"].toObject();
        current_account->currency.name = attr["name"].toString().toStdString();
        current_account->currency.plural = attr["namePlural"].toString().toStdString();
        current_account->currency.symbol = attr["symbol"].toString().toStdString();
        current_account->currency.decimals = attr["decimals"].toString().toInt();
        emit account_balance_reply(false);
    }
}

void netServices::get_account_transfers(account* acc) {
    current_account = acc;
    QString url = baseApiUrl + "/accounting/" +
            QString::fromStdString(acc->group_code) +
            "/transfers?filter[account]=" +
            QString::fromStdString(acc->account_id);
    QNetworkRequest networkRequest(url);
    prepare_request(networkRequest);

    netManager->get(networkRequest);
    connect(netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(get_account_transfers_reply(QNetworkReply*)));

}

void netServices::get_account_transfers_reply(QNetworkReply* getReply) {
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
        emit account_transfers_reply(true, "");
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
            current_account->transfers.push_back(transfer(trans["id"].toString().toStdString()));
            transfer* p = &current_account->transfers.back();
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
            if (p->payer_account_id == current_account->account_id) {
                p->payer_account_code = current_account->account_code;
            }
            else {unknown_accounts.push_back(p->payer_account_id);}
            p->payee_account_id = trans["relationships"].toObject()["payee"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->payee_account_id == current_account->account_id) {
                p->payee_account_code = current_account->account_code;
            }
            else {unknown_accounts.push_back(p->payee_account_id);}
            p->currency.id = trans["relationships"].toObject()["currency"].
                    toObject()["data"].toObject()["id"].toString().toStdString();
            if (p->currency.id == current_account->currency.id) {
                p->currency = current_account->currency;
            }
        }
        string comma_list;
        for(int j= 0; j < (int)unknown_accounts.size(); j++) {
            comma_list += unknown_accounts[j];
            if (unknown_accounts[j] != unknown_accounts.back()) comma_list += ",";
        }
        emit account_transfers_reply(false, comma_list);
    }

}

void netServices::get_unknown_accounts(account* acc, const string& comma_list) {
    QString url = baseApiUrl + "/social/" + QString::fromStdString(acc->group_code) +
            "/members?filter[account]=" + QString::fromStdString(comma_list);
    QNetworkReply *getReply = nullptr;
    this->get_call(url, getReply);
    if(getReply->error()) {
        qDebug() << "Error: " << getReply->errorString();
        getReply->deleteLater();
//        emit account_data_reply(true);
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
            for (int j=0; j < (int)acc->transfers.size(); j++) {
                transfer* p = &acc->transfers[j];
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
    }
}

void netServices::prepare_request(QNetworkRequest& networkRequest) {
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/vnd.api+json");
    networkRequest.setRawHeader("Authorization", this->authHeaderValue);
}

void netServices::get_call(const QString& url, QNetworkReply*& getReply) {
    QUrl getUrl = QUrl(url);
    QNetworkRequest networkRequest(getUrl);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/vnd.api+json");
    networkRequest.setRawHeader("Authorization", this->authHeaderValue);

    QEventLoop loop;
    getReply = this->netManager->get(networkRequest);
    connect(getReply, SIGNAL(finished()), &loop, SLOT(quit()), Qt::SingleShotConnection);
    loop.exec();
}
