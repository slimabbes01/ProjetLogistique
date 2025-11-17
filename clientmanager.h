#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QString>
#include <QVector>
#include <QMap>

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
    static bool addClient(const Client &c);
    static bool updateClient(const Client &c);
    static bool deleteClient(int id);
    static QVector<Client> getAllClients();
    static QMap<int, QString> getClientNames(); // pour comboBox
};

#endif
