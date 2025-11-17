#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

class Connection {
public:
    static QSqlDatabase& getInstance(); // Declaration only
};

#endif
