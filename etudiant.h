#ifndef ETUDIANT_H
#define ETUDIANT_H
#include "connection.h"
#include "etudiant.h"
#include "mainwindow.h"
#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>


class Product
{
public:
    Product() = default;
    Product(int id, QString productName, double priceUnit, int quantity)
        : id(id), productName(productName), priceUnit(priceUnit), quantity(quantity) {}

    // Setters
    void setId(int id) { this->id = id; }
    void setProductName(const QString &name) { this->productName = name; }
    void setPriceUnit(double price) { this->priceUnit = price; }
    void setQuantity(int quantity) { this->quantity = quantity; }

    // CRUD Operations
    bool addProduct();
    bool updateProduct();
    bool deleteProduct();
    QSqlQueryModel* getAllProducts();

private:
    int id = 0;
    QString productName;
    double priceUnit;
    int quantity = 0; // New field
};

#endif // ETUDIANT_H
