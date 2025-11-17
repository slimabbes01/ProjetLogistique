#ifndef COMMANDEMANAGER_H
#define COMMANDEMANAGER_H

#include <QString>
#include <QVector>
#include <QDate>

struct Commande {
    int id;
    int clientId;
    QString description;
    double montant;
    QDate dateCommande;
    QString statut;
};

class CommandeManager {
public:
    static bool addCommande(const Commande &c);
    static bool updateCommande(const Commande &c);
    static bool deleteCommande(int id);
    static QVector<Commande> getAllCommandes();
};

#endif
