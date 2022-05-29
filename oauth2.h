#ifndef OAUTH2_H
#define OAUTH2_H

#include <QObject>
#include <QNetworkReply>

struct komunitin_settings
{
    QString user_email = "";
    QString access_token = "";
    QString refresh_token = "";
};

class Oauth2 : public QObject {
    Q_OBJECT

private:
    QNetworkAccessManager *netM;
    QString access_token;
    QString refresh_token;

public:
    Oauth2(QNetworkAccessManager*, QObject *parent = nullptr);
    bool hasAccess;
    komunitin_settings* kSettingsP;
    void prepare_request(QNetworkRequest&);

private slots:
    void get_access_reply(QNetworkReply*);

public slots:
    void get_access(const std::string&, const std::string&);

signals:
    void has_access();
    void error_auth();
};

#endif // OAUTH2_H
