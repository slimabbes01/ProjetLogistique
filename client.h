#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QtSql/QSqlQuery>

struct Client {
    int id;
    QString nom;
    QString prenom;
    QString adresse;
    QString telephone;
    QString email;
};

class ClientManager {
public:
    bool addClient(const Client& c);
    bool updateClient(const Client& c);
    bool deleteClient(int id);
    QVector<Client> getAllClients();
    QVector<Client> searchClients(const QMap<QString, QString>& criteria);
};

#endif
