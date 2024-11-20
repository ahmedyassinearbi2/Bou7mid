#include "connection.h"
#include "etudiant.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QTableView>
#include <QHeaderView>
#include <QValidator>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QProgressBar>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QThread>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QPrinter>
#include <QPainter>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>


//QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    QWidget *calendarWidget = new QWidget();
    QVBoxLayout *calendarLayout = new QVBoxLayout(calendarWidget);

    QCalendarWidget *calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true); // Optional
    calendarLayout->addWidget(calendar);
    // Left navigation menu
    QVBoxLayout *menuLayout = new QVBoxLayout;
    QPushButton *buttonProducts = new QPushButton("Products");
    QPushButton *buttonSales = new QPushButton("Sales");
    QPushButton *buttonStatistics = new QPushButton("Statistics");
    QPushButton *buttonPDF = new QPushButton("Generate PDF");

    // Set button styles
    buttonProducts->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                                  "QPushButton:hover { background-color: #555500; }");
    buttonSales->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                               "QPushButton:hover { background-color: #555500; }");
    buttonStatistics->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                                    "QPushButton:hover { background-color: #555500; }");
    buttonPDF->setStyleSheet("QPushButton { background-color: #333300; color: white; font-weight: bold; border: none; padding: 10px; border-radius: 10px; }"
                             "QPushButton:hover { background-color: #555500; }");

    menuLayout->addWidget(buttonProducts);
    menuLayout->addWidget(buttonSales);
    menuLayout->addWidget(buttonStatistics);
    menuLayout->addWidget(buttonPDF);
    menuLayout->addStretch();

    QWidget *menuWidget = new QWidget(this);
    menuWidget->setLayout(menuLayout);
    menuWidget->setFixedWidth(150);
    menuWidget->setStyleSheet("background-color: #222222; border-radius: 10px;");

    // Stacked widget for dynamic content
    QStackedWidget *stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background-color: #f0f0f0; border-radius: 10px; }");

    // Product Management Section
    QWidget *productWidget = new QWidget;
    QVBoxLayout *productLayout = new QVBoxLayout;

    // Tab widget
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet("QTabWidget::pane { border: none; }"
                             "QTabBar::tab { background: #555555; color: white; padding: 10px; border-radius: 10px; }"
                             "QTabBar::tab:selected { background: #333300; }");

    // Tab: Add/Update Product
    QWidget *addUpdateTab = new QWidget;
    QFormLayout *formLayout = new QFormLayout;

    // Input fields
    lineEdit_ID = new QLineEdit(this);
    lineEdit_ProductName = new QLineEdit(this);
    lineEdit_PriceUnit = new QLineEdit(this);
    lineEdit_Quantity = new QLineEdit(this);

    // Set input styles
    lineEdit_ID->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                               "QLineEdit:focus { border: 2px solid #555500; }");
    lineEdit_ProductName->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                        "QLineEdit:focus { border: 2px solid #555500; }");
    lineEdit_PriceUnit->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                      "QLineEdit:focus { border: 2px solid #555500; }");
    lineEdit_Quantity->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                     "QLineEdit:focus { border: 2px solid #555500; }");

    // Validators
    lineEdit_ID->setValidator(new QIntValidator(1, 99999, this));
    lineEdit_PriceUnit->setValidator(new QDoubleValidator(0.01, 99999.99, 2, this));
    lineEdit_Quantity->setValidator(new QIntValidator(1, 99999, this));

    // Add fields to form layout
    formLayout->addRow("ID (Optional):", lineEdit_ID);
    formLayout->addRow("Product Name:", lineEdit_ProductName);
    formLayout->addRow("Price per Unit:", lineEdit_PriceUnit);
    formLayout->addRow("Quantity:", lineEdit_Quantity);

    // Add/Update Buttons
    pushButton_addProduct = new QPushButton("Add Product", this);
    pushButton_updateProduct = new QPushButton("Update Product", this);
    pushButton_addProduct->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                         "QPushButton:hover { background-color: #45a049; }");
    pushButton_updateProduct->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                            "QPushButton:hover { background-color: #0b7dda; }");
    formLayout->addRow(pushButton_addProduct, pushButton_updateProduct);

    addUpdateTab->setLayout(formLayout);
    tabWidget->addTab(addUpdateTab, "Add/Update Product");

    // Progress Bar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 0); // Indeterminate mode
    progressBar->setTextVisible(true);
    progressBar->setStyleSheet("QProgressBar { text-align: center; }");
    progressBar->hide(); // Hide initially
    productLayout->addWidget(progressBar); // Add to layout

    // Tab: Delete Product
    QWidget *deleteTab = new QWidget;
    QVBoxLayout *deleteLayout = new QVBoxLayout;
    lineEdit_ID_Delete = new QLineEdit(this);
    lineEdit_ID_Delete->setPlaceholderText("Enter Product ID to delete");
    lineEdit_ID_Delete->setValidator(new QIntValidator(1, 99999, this));
    lineEdit_ID_Delete->setStyleSheet("QLineEdit { padding: 10px; border: 2px solid #333300; border-radius: 10px; }"
                                      "QLineEdit:focus { border: 2px solid #555500; }");
    pushButton_deleteProduct = new QPushButton("Delete Product", this);
    pushButton_deleteProduct->setStyleSheet("QPushButton { background-color: #F44336; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                            "QPushButton:hover { background-color: #d32f2f; }");
    deleteLayout->addWidget(new QLabel("Product ID:"));
    deleteLayout->addWidget(lineEdit_ID_Delete);
    deleteLayout->addWidget(pushButton_deleteProduct);
    deleteTab->setLayout(deleteLayout);
    tabWidget->addTab(deleteTab, "Delete Product");

    // Tab: View Products
    QWidget *viewTab = new QWidget;
    QVBoxLayout *viewLayout = new QVBoxLayout;
    pushButton_viewProducts = new QPushButton("View All Products", this);
    pushButton_viewProducts->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                           "QPushButton:hover { background-color: #0b7dda; }");
    viewTable = new QTableView(this);
    viewTable->setSortingEnabled(true);
    viewTable->horizontalHeader()->setStretchLastSection(true);
    viewTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    viewLayout->addWidget(pushButton_viewProducts);
    viewLayout->addWidget(viewTable);
    viewTab->setLayout(viewLayout);
    tabWidget->addTab(viewTab, "View Products");

    // Add tab widget to product layout
    productLayout->addWidget(tabWidget);
    productWidget->setLayout(productLayout);

    // Add productWidget to stackedWidget
    stackedWidget->addWidget(productWidget);

    // Sales Section (Placeholder for now)
    QWidget *salesWidget = new QWidget;
    QVBoxLayout *salesLayout = new QVBoxLayout;
    QLabel *salesLabel = new QLabel("Sales Page (Placeholder)");
    salesLabel->setAlignment(Qt::AlignCenter);
    salesLabel->setStyleSheet("font-size: 20px; color: #333300;");
    salesLayout->addWidget(salesLabel);
    salesWidget->setLayout(salesLayout);

    // Add salesWidget to stackedWidget
    stackedWidget->addWidget(salesWidget);

    // Statistics Section
    QWidget *statisticsWidget = new QWidget;
    QVBoxLayout *statisticsLayout = new QVBoxLayout;

    // Create a chart
    QLineSeries *series = new QLineSeries();
    series->append(1, 1);
    series->append(2, 3);
    series->append(3, 2);
    series->append(4, 5);
    series->append(5, 4);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistics Curve Example");

    // Set up axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(0, 6);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(0, 6);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view and set the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    statisticsLayout->addWidget(chartView);

    statisticsWidget->setLayout(statisticsLayout);

    // Add statisticsWidget to stackedWidget
    stackedWidget->addWidget(statisticsWidget);

    // Inside the PDF Generation Section
    QWidget *pdfWidget = new QWidget;
    QVBoxLayout *pdfLayout = new QVBoxLayout;
    QLabel *pdfLabel = new QLabel("PDF Generation Page");
    pdfLabel->setAlignment(Qt::AlignCenter);
    pdfLabel->setStyleSheet("font-size: 20px; color: #333300;");
    pdfLayout->addWidget(pdfLabel);

    // Create and add the Generate PDF button
    QPushButton *pushButton_generatePDF = new QPushButton("Generate PDF", this);
    pushButton_generatePDF->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 10px; border-radius: 10px; }"
                                          "QPushButton:hover { background-color: #0b7dda; }");
    pdfLayout->addWidget(pushButton_generatePDF); // Add button to layout

    pdfWidget->setLayout(pdfLayout);

    // Add pdfWidget to stackedWidget
    stackedWidget->addWidget(pdfWidget);

    // Add menuWidget and stackedWidget to mainLayout
    mainLayout->addWidget(menuWidget);
    mainLayout->addWidget(stackedWidget);
    mainLayout->setStretch(1, 5);

    // Set central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect navigation buttons to stackedWidget
    connect(buttonProducts, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(productWidget);
    });
    connect(buttonSales, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(salesWidget);
    });
    connect(buttonStatistics, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(statisticsWidget);
    });
    connect(buttonPDF, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(pdfWidget);
    });

    // Keep existing button functionality
    connect(pushButton_addProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_addProduct_clicked);
    connect(pushButton_updateProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_updateProduct_clicked);
    connect(pushButton_deleteProduct, &QPushButton::clicked, this, &MainWindow::on_pushButton_deleteProduct_clicked);
    connect(pushButton_viewProducts, &QPushButton::clicked, this, &MainWindow::on_pushButton_viewProducts_clicked);
    connect(pushButton_generatePDF, &QPushButton::clicked, this, &MainWindow::on_pushButton_generatePDF_clicked);
    connect(calendar, &QCalendarWidget::clicked, this, &MainWindow::onCalendarDateClicked);
    calendarWidget->setLayout(calendarLayout);

    // Add the calendar widget to your stacked widget or main layout
    stackedWidget->addWidget(calendarWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::on_pushButton_addProduct_clicked()
{
    int id = lineEdit_ID->text().isEmpty() ? 0 : lineEdit_ID->text().toInt();
    QString productName = lineEdit_ProductName->text();
    double priceUnit = lineEdit_PriceUnit->text().toDouble();
    int quantity = lineEdit_Quantity->text().toInt();

    if (productName.isEmpty() || priceUnit <= 0 || quantity <= 0) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled with valid data.");
        return;
    }

    // Show the progress bar
    progressBar->show();
    progressBar->setValue(0); // Reset the progress bar

    // Using QFuture<bool>
    QFuture<bool> future = QtConcurrent::run([=]() {
        Product product(id, productName, priceUnit, quantity);
        bool success = product.addProduct(); // Assuming this function adds the product to the database
        QThread::sleep(2); // Simulate time delay for adding product
        return success;
    });

    // Handle the completion of the task
    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [this, watcher]() {
        watcher->deleteLater();
        progressBar->hide(); // Hide the progress bar

        if (watcher->result()) {
            QMessageBox::critical(this, "Error", "Failed to add the product.");

        } else {
            QMessageBox::information(this, "Success", "Product added successfully!");
        }
    });
    watcher->setFuture(future);
}

// Function to handle updating a product
void MainWindow::on_pushButton_updateProduct_clicked()
{
    int id = lineEdit_ID->text().toInt();
    QString productName = lineEdit_ProductName->text();
    double priceUnit = lineEdit_PriceUnit->text().toDouble();
    int quantity = lineEdit_Quantity->text().toInt();

    if (id <= 0 || productName.isEmpty() || priceUnit <= 0 || quantity < 0) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled with valid data.");
        return;
    }

    // Show the progress bar
    progressBar->show();
    progressBar->setValue(0); // Reset the progress bar

    // Using QFuture<bool> for update
    QFuture<bool> future = QtConcurrent::run([=]() {
        Product product(id, productName, priceUnit, quantity);
        bool success = product.updateProduct(); // Assuming this function updates the product in the database
        QThread::sleep(2); // Simulate time delay for updating product
        return success;
    });

    // Handle the completion of the task
    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [this, watcher]() {
        watcher->deleteLater();
        progressBar->hide(); // Hide the progress bar

        if (watcher->result()) {
            QMessageBox::critical(this, "Error", "Failed to update the product.");

        } else {
            QMessageBox::information(this, "Success", "Product updated successfully!");
        }
    });
    watcher->setFuture(future);
}

// Function to handle deleting a product
void MainWindow::on_pushButton_deleteProduct_clicked()
{
    int id = lineEdit_ID_Delete->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid ID.");
        return;
    }

    // Show the progress bar
    progressBar->show();
    progressBar->setValue(0); // Reset the progress bar

    // Using QFuture<bool> for delete
    QFuture<bool> future = QtConcurrent::run([=]() {
        Product product;
        product.setId(id);
        bool success = product.deleteProduct(); // Assuming this function deletes the product from the database
        QThread::sleep(2); // Simulate time delay for deleting product
        return success;
    });

    // Handle the completion of the task
    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [this, watcher]() {
        watcher->deleteLater();
        progressBar->hide(); // Hide the progress bar

        if (watcher->result()) {
            QMessageBox::critical(this, "Error", "Failed to delete the product.");

        } else {
            QMessageBox::information(this, "Success", "Product deleted successfully!");
        }
    });
    watcher->setFuture(future);
}

// Function to handle viewing all products
void MainWindow::on_pushButton_viewProducts_clicked()
{
    Product product;
    QSqlQueryModel *model = product.getAllProducts(); // Assuming this function retrieves all products

    if (model) {
        viewTable->setModel(model);
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "Product Name");
        model->setHeaderData(2, Qt::Horizontal, "Price per Unit");
        model->setHeaderData(3, Qt::Horizontal, "Quantity");
    } else {
        QMessageBox::critical(this, "Error", "Failed to retrieve products.");
    }
}
void MainWindow::on_pushButton_generatePDF_clicked()
{
    // Create a printer object
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("output.pdf"); // Specify the output file name

    // Start painting on the printer
    QPainter painter(&printer);
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Error", "Failed to open PDF for writing.");
        return;
    }

    // Set up your document
    painter.drawText(100, 100, "Hello, PDF!"); // Example content
    painter.drawText(100, 120, "This is a simple PDF document generated using Qt.");

    // You can add more content here, like images or formatted text
    // painter.drawImage(QPoint(100, 150), myImage); // Example for adding an image

    painter.end(); // Finish painting

    // Notify user of success
    QMessageBox::information(this, "Success", "PDF generated successfully!");
}
void MainWindow::onCalendarDateClicked(const QDate &date)
{
    // Display a message box with the selected date
    QMessageBox::information(this, "Date Selected", "You selected: " + date.toString());
}

