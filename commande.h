#ifndef COMMANDE_H
#define COMMANDE_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QtSql/QSqlQuery>

struct Commande {
    int id;
    int clientId;
    QString dateCommande;
    QString description;
    double montant;
    QString statut;
};

class CommandeManager {
public:
    bool addCommande(const Commande& c);
    bool updateCommande(const Commande& c);
    bool deleteCommande(int id);
    QVector<Commande> getAllCommandes();
    QVector<Commande> searchCommandes(const QMap<QString, QString>& criteria);
};

#endif
