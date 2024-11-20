#include "connection.h"
#include "etudiant.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QDebug>

// Add a new product
// Add a new product
bool Product::addProduct()
{
    if (quantity <= 0) {
        qDebug() << "Error: Quantity must be greater than 0.";
        return false;
    }

    if (id == 0) {
        QSqlQuery query;
        query.prepare("SELECT MAX(id) FROM Products");
        if (query.exec() && query.next()) {
            int lastId = query.value(0).toInt();
            id = lastId + 1;
        } else {
            id = 1;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Products (id, product_name, price_unit, quantity) "
                  "VALUES (:id, :product_name, :price_unit, :quantity)");
    query.bindValue(":id", id);
    query.bindValue(":product_name", productName);
    query.bindValue(":price_unit", priceUnit);
    query.bindValue(":quantity", quantity);

    return query.exec();
}



// Update an existing product
bool Product::updateProduct()
{
    if (quantity <= 0) {
        qDebug() << "Error: Quantity must be greater than 0.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE Products SET product_name = :product_name, price_unit = :price_unit, quantity = :quantity WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":product_name", productName);
    query.bindValue(":price_unit", priceUnit);
    query.bindValue(":quantity", quantity);

    return query.exec();
}


// Delete a product
bool Product::deleteProduct()
{
    QSqlQuery query;
    query.prepare("DELETE FROM Products WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();
}

QSqlQueryModel* Product::getAllProducts()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT id, product_name, price_unit, quantity FROM Products ORDER BY product_name ASC");

    if (!query.exec()) {
        qDebug() << "Query Execution Error:" << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    return model;
}
