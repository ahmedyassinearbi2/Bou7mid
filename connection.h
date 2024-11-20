#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "etudiant.h"
#include "mainwindow.h"
class Connection
{
    QSqlDatabase db;
public:
    Connection();
    bool createconnect();
};

#endif // CONNECTION_H
