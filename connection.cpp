#include "connection.h"

QSqlDatabase& Connection::getInstance() {
    static QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    if (!db.isOpen()) {
        db.setDatabaseName(
            "Driver={Oracle in instantclient_11_2};Dbq=localhost:1521/XE;"
            "Uid=WORKSHOP_USER;Pwd=MyPassword;"
            );
        if (!db.open()) {
            qDebug() << "Connection failed:" << db.lastError().text();
        } else {
            qDebug() << "Connected to Oracle XE!";
        }
    }
    return db;
}
