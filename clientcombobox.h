#ifndef CLIENTCOMBOBOX_H
#define CLIENTCOMBOBOX_H

#include <QComboBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class ClientComboBox : public QComboBox {
    Q_OBJECT
public:
    ClientComboBox(QWidget *parent = nullptr) : QComboBox(parent) {
        refresh();
    }

    // Load clients from DB
    void refresh() {
        clear();
        QSqlQuery query("SELECT ID_CLIENT, NOM || ' ' || PRENOM AS FULLNAME FROM CLIENT ORDER BY NOM, PRENOM", QSqlDatabase::database());
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            addItem(name, id);
        }
    }

    int currentClientId() const {
        return currentData().toInt();
    }
};

#endif // CLIENTCOMBOBOX_H

