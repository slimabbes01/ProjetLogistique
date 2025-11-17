#include "client.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool ClientManager::addClient(const Client& c) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM, PRENOM, ADRESSE, TELEPHONE, EMAIL) "
                  "VALUES (CLIENT_SEQ.NEXTVAL, :nom, :prenom, :adresse, :telephone, :email)");
    query.bindValue(":nom", c.nom);
    query.bindValue(":prenom", c.prenom);
    query.bindValue(":adresse", c.adresse);
    query.bindValue(":telephone", c.telephone);
    query.bindValue(":email", c.email);
    if (!query.exec()) {
        qDebug() << "Add client error:" << query.lastError().text();
        return false;
    }
    return true;
}

// Implement updateClient, deleteClient, getAllClients, searchClients similarly
