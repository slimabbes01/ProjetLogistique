#include "commande.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool CommandeManager::addCommande(const Commande& c) {
    QSqlQuery query(Connection::getInstance());
    query.prepare("INSERT INTO COMMANDE (ID_COMMANDE, ID_CLIENT, DATE_COMMANDE, DESCRIPTION, MONTANT, STATUT) "
                  "VALUES (COMMANDE_SEQ.NEXTVAL, :idClient, TO_DATE(:dateCommande, 'YYYY-MM-DD'), :description, :montant, :statut)");
    query.bindValue(":idClient", c.clientId);
    query.bindValue(":dateCommande", c.dateCommande);
    query.bindValue(":description", c.description);
    query.bindValue(":montant", c.montant);
    query.bindValue(":statut", c.statut);

    if (!query.exec()) {
        qDebug() << "Add commande error:" << query.lastError().text();
        return false;
    }
    return true;
}

// Implement updateCommande, deleteCommande, getAllCommandes, searchCommandes similarly
