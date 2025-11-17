#include "CommandeManager.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool CommandeManager::addCommande(const Commande &c) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("INSERT INTO COMMANDE (ID_CLIENT, DESCRIPTION, MONTANT, DATE_COMMANDE, STATUT) VALUES (?,?,?,?,?)");
    query.addBindValue(c.clientId);
    query.addBindValue(c.description);
    query.addBindValue(c.montant);
    query.addBindValue(c.dateCommande);
    query.addBindValue(c.statut);
    return query.exec();
}

bool CommandeManager::updateCommande(const Commande &c) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("UPDATE COMMANDE SET ID_CLIENT=?, DESCRIPTION=?, MONTANT=?, DATE_COMMANDE=?, STATUT=? WHERE ID_COMMANDE=?");
    query.addBindValue(c.clientId);
    query.addBindValue(c.description);
    query.addBindValue(c.montant);
    query.addBindValue(c.dateCommande);
    query.addBindValue(c.statut);
    query.addBindValue(c.id);
    return query.exec();
}

bool CommandeManager::deleteCommande(int id) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE=?");
    query.addBindValue(id);
    return query.exec();
}

QVector<Commande> CommandeManager::getAllCommandes() {
    QVector<Commande> commandes;
    QSqlQuery query(Connection::getInstance());
    if(query.exec("SELECT * FROM COMMANDE")) {
        while(query.next()) {
            commandes.push_back({
                query.value("ID_COMMANDE").toInt(),
                query.value("ID_CLIENT").toInt(),
                query.value("DESCRIPTION").toString(),
                query.value("MONTANT").toDouble(),
                query.value("DATE_COMMANDE").toDate(),
                query.value("STATUT").toString()
            });
        }
    }
    return commandes;
}
