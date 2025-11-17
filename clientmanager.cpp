#include "ClientManager.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool ClientManager::addClient(const Client &c) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("INSERT INTO CLIENT (NOM, PRENOM, ADRESSE, TELEPHONE, EMAIL) VALUES (?,?,?,?,?)");
    query.addBindValue(c.nom);
    query.addBindValue(c.prenom);
    query.addBindValue(c.adresse);
    query.addBindValue(c.telephone);
    query.addBindValue(c.email);
    return query.exec();
}

bool ClientManager::updateClient(const Client &c) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("UPDATE CLIENT SET NOM=?, PRENOM=?, ADRESSE=?, TELEPHONE=?, EMAIL=? WHERE ID_CLIENT=?");
    query.addBindValue(c.nom);
    query.addBindValue(c.prenom);
    query.addBindValue(c.adresse);
    query.addBindValue(c.telephone);
    query.addBindValue(c.email);
    query.addBindValue(c.id);
    return query.exec();
}

bool ClientManager::deleteClient(int id) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT=?");
    query.addBindValue(id);
    return query.exec();
}

QVector<Client> ClientManager::getAllClients() {
    QVector<Client> clients;
    QSqlQuery query(Connection::getInstance());
    if(query.exec("SELECT * FROM CLIENT")) {
        while(query.next()) {
            clients.push_back({ query.value("ID_CLIENT").toInt(),
                               query.value("NOM").toString(),
                               query.value("PRENOM").toString(),
                               query.value("ADRESSE").toString(),
                               query.value("TELEPHONE").toString(),
                               query.value("EMAIL").toString() });
        }
    }
    return clients;
}

QMap<int, QString> ClientManager::getClientNames() {
    QMap<int, QString> map;
    QSqlQuery query(Connection::getInstance());
    if(query.exec("SELECT ID_CLIENT, NOM || ' ' || PRENOM AS FULLNAME FROM CLIENT")) {
        while(query.next())
            map[query.value("ID_CLIENT").toInt()] = query.value("FULLNAME").toString();
    }
    return map;
}
