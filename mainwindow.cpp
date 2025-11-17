#include "mainwindow.h"
#include "connection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QSplitter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    tabWidget(nullptr),
    tabClients(nullptr),
    tabCommandes(nullptr),
    tabStatistics(nullptr),
    tabStatisticsCommandes(nullptr),
    tabStatisticsClients(nullptr),
    tableViewClients(nullptr),
    tableViewCommandes(nullptr),
    modelClients(nullptr),
    modelCommandes(nullptr)
{
    QSqlDatabase &db = Connection::getInstance();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB Error", "Database not connected!");
        return;
    }

    setupUI();
    loadClients();
    loadCommandes();
    loadClientComboBox();
    // Only setup statistics after models are loaded
    if (modelClients && modelCommandes)
        setupStatisticsTabs();

    if(tableViewClients->model())
        connect(tableViewClients->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                this, &MainWindow::populateClientForm);

    if(tableViewCommandes->model())
        connect(tableViewCommandes->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                this, &MainWindow::populateCommandeForm);


    // --- Connect Client Buttons ---
    connect(pushButtonAddClient, &QPushButton::clicked, this, &MainWindow::addClient);
    connect(pushButtonUpdateClient, &QPushButton::clicked, this, &MainWindow::updateClient);
    connect(pushButtonDeleteClient, &QPushButton::clicked, this, &MainWindow::deleteClient);
    connect(pushButtonSearchClient, &QPushButton::clicked, this, &MainWindow::searchClient);
    connect(pushButtonExportClientsPdf, &QPushButton::clicked, this, &MainWindow::onExportClientsPdfClicked);

    // --- Connect Commande Buttons ---
    connect(pushButtonAddCommande, &QPushButton::clicked, this, &MainWindow::addCommande);
    connect(pushButtonUpdateCommande, &QPushButton::clicked, this, &MainWindow::updateCommande);
    connect(pushButtonDeleteCommande, &QPushButton::clicked, this, &MainWindow::deleteCommande);
    connect(pushButtonSearchCommande, &QPushButton::clicked, this, &MainWindow::searchCommande);
    connect(pushButtonExportCommandesPdf, &QPushButton::clicked, this, &MainWindow::onExportCommandesPdfClicked);

    // --- Auto-fill forms ---
    connect(tableViewClients->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::populateClientForm);
    connect(tableViewCommandes->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::populateCommandeForm);

    // Enable sorting
    tableViewClients->setSortingEnabled(true);
    tableViewCommandes->setSortingEnabled(true);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // ------------------- Clients Tab -------------------
    tabClients = new QWidget();
    QVBoxLayout *vLayoutClients = new QVBoxLayout(tabClients);

    tableViewClients = new QTableView();
    vLayoutClients->addWidget(tableViewClients);

    QFormLayout *formClients = new QFormLayout();
    lineEditNom = new QLineEdit(); lineEditPrenom = new QLineEdit();
    lineEditAdresse = new QLineEdit(); lineEditTelephone = new QLineEdit(); lineEditEmail = new QLineEdit();
    formClients->addRow("Nom:", lineEditNom);
    formClients->addRow("Prenom:", lineEditPrenom);
    formClients->addRow("Adresse:", lineEditAdresse);
    formClients->addRow("Telephone:", lineEditTelephone);
    formClients->addRow("Email:", lineEditEmail);
    vLayoutClients->addLayout(formClients);

    QHBoxLayout *hLayoutButtonsClients = new QHBoxLayout();
    pushButtonAddClient = new QPushButton("Add");
    pushButtonUpdateClient = new QPushButton("Update");
    pushButtonDeleteClient = new QPushButton("Delete");
    pushButtonSearchClient = new QPushButton("Search");
    pushButtonExportClientsPdf = new QPushButton("Export PDF");

    hLayoutButtonsClients->addWidget(pushButtonAddClient);
    hLayoutButtonsClients->addWidget(pushButtonUpdateClient);
    hLayoutButtonsClients->addWidget(pushButtonDeleteClient);
    hLayoutButtonsClients->addWidget(pushButtonSearchClient);
    hLayoutButtonsClients->addWidget(pushButtonExportClientsPdf);
    vLayoutClients->addLayout(hLayoutButtonsClients);

    tabWidget->addTab(tabClients, "Clients");

    // ------------------- Commandes Tab -------------------
    tabCommandes = new QWidget();
    QVBoxLayout *vLayoutCommandes = new QVBoxLayout(tabCommandes);

    tableViewCommandes = new QTableView();
    vLayoutCommandes->addWidget(tableViewCommandes);

    QFormLayout *formCommandes = new QFormLayout();
    comboBoxClient = new QComboBox();
    lineEditDescription = new QLineEdit();
    lineEditMontant = new QLineEdit();
    dateEditCommande = new QDateEdit(QDate::currentDate());
    comboBoxStatut = new QComboBox();
    comboBoxStatut->addItems({"EN_COURS","LIVREE","ANNULEE"});

    formCommandes->addRow("Client:", comboBoxClient);
    formCommandes->addRow("Description:", lineEditDescription);
    formCommandes->addRow("Montant:", lineEditMontant);
    formCommandes->addRow("Date:", dateEditCommande);
    formCommandes->addRow("Statut:", comboBoxStatut);
    vLayoutCommandes->addLayout(formCommandes);

    QHBoxLayout *hLayoutButtonsCommandes = new QHBoxLayout();
    pushButtonAddCommande = new QPushButton("Add");
    pushButtonUpdateCommande = new QPushButton("Update");
    pushButtonDeleteCommande = new QPushButton("Delete");
    pushButtonSearchCommande = new QPushButton("Search");
    pushButtonExportCommandesPdf = new QPushButton("Export PDF");

    hLayoutButtonsCommandes->addWidget(pushButtonAddCommande);
    hLayoutButtonsCommandes->addWidget(pushButtonUpdateCommande);
    hLayoutButtonsCommandes->addWidget(pushButtonDeleteCommande);
    hLayoutButtonsCommandes->addWidget(pushButtonSearchCommande);
    hLayoutButtonsCommandes->addWidget(pushButtonExportCommandesPdf);
    vLayoutCommandes->addLayout(hLayoutButtonsCommandes);

    tabWidget->addTab(tabCommandes, "Commandes");
}

// =========================
// Load Data
// =========================
void MainWindow::loadClients() {
    modelClients = new QSqlTableModel(this, Connection::getInstance());
    modelClients->setTable("CLIENT");
    modelClients->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelClients->select();
    tableViewClients->setModel(modelClients);
    tableViewClients->resizeColumnsToContents();
}

void MainWindow::loadCommandes() {
    modelCommandes = new QSqlTableModel(this, Connection::getInstance());
    modelCommandes->setTable("COMMANDE");
    modelCommandes->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelCommandes->select();
    tableViewCommandes->setModel(modelCommandes);
    tableViewCommandes->resizeColumnsToContents();
}

void MainWindow::loadClientComboBox() {
    comboBoxClient->clear();
    QSqlQuery query(Connection::getInstance());
    if(query.exec("SELECT ID_CLIENT, NOM || ' ' || PRENOM AS FULLNAME FROM CLIENT")) {
        while(query.next()) {
            comboBoxClient->addItem(query.value("FULLNAME").toString(), query.value("ID_CLIENT"));
        }
    }
}

// =========================
// Validation
// =========================
bool MainWindow::validateClientInputs() {
    if (lineEditNom->text().isEmpty() || lineEditPrenom->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Nom and Prenom are required!");
        return false;
    }
    return true;
}

bool MainWindow::validateCommandeInputs() {
    if (comboBoxClient->currentIndex() < 0 || lineEditDescription->text().isEmpty() || lineEditMontant->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Client, Description and Montant are required!");
        return false;
    }
    return true;
}

// =========================
// Clients CRUD
// =========================
void MainWindow::addClient() {
    if (!validateClientInputs()) return;

    QSqlQuery query(Connection::getInstance());
    query.prepare("INSERT INTO CLIENT (NOM, PRENOM, ADRESSE, TELEPHONE, EMAIL) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(lineEditNom->text());
    query.addBindValue(lineEditPrenom->text());
    query.addBindValue(lineEditAdresse->text());
    query.addBindValue(lineEditTelephone->text());
    query.addBindValue(lineEditEmail->text());

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    modelClients->select();
    loadClientComboBox();
}

void MainWindow::updateClient() {
    if (!validateClientInputs()) return;
    QModelIndex idx = tableViewClients->currentIndex();
    if (!idx.isValid()) return;
    int id = modelClients->data(modelClients->index(idx.row(),0)).toInt();

    QSqlQuery query(Connection::getInstance());
    query.prepare("UPDATE CLIENT SET NOM=?, PRENOM=?, ADRESSE=?, TELEPHONE=?, EMAIL=? WHERE ID_CLIENT=?");
    query.addBindValue(lineEditNom->text());
    query.addBindValue(lineEditPrenom->text());
    query.addBindValue(lineEditAdresse->text());
    query.addBindValue(lineEditTelephone->text());
    query.addBindValue(lineEditEmail->text());
    query.addBindValue(id);

    if(!query.exec()) {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    modelClients->select();
    loadClientComboBox();
}

void MainWindow::deleteClient() {
    QModelIndex idx = tableViewClients->currentIndex();
    if (!idx.isValid()) return;
    int id = modelClients->data(modelClients->index(idx.row(),0)).toInt();

    QSqlQuery query(Connection::getInstance());
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT=?");
    query.addBindValue(id);

    if(!query.exec()) {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    modelClients->select();
    loadClientComboBox();
}

void MainWindow::searchClient() {
    QString nom = lineEditNom->text();
    modelClients->setFilter(QString("NOM LIKE '%%1%'").arg(nom));
    modelClients->select();
}

void MainWindow::populateClientForm(const QModelIndex &current, const QModelIndex &) {
    if(!current.isValid()) return;

    lineEditNom->setText(modelClients->data(modelClients->index(current.row(),1)).toString());
    lineEditPrenom->setText(modelClients->data(modelClients->index(current.row(),2)).toString());
    lineEditAdresse->setText(modelClients->data(modelClients->index(current.row(),3)).toString());
    lineEditTelephone->setText(modelClients->data(modelClients->index(current.row(),4)).toString());
    lineEditEmail->setText(modelClients->data(modelClients->index(current.row(),5)).toString());
}

// =========================
// Commandes CRUD
// =========================
void MainWindow::addCommande() {
    if(!validateCommandeInputs()) return;

    int clientId = comboBoxClient->currentData().toInt();
    QSqlQuery query(Connection::getInstance());
    query.prepare("INSERT INTO COMMANDE (ID_CLIENT, DESCRIPTION, MONTANT, DATE_COMMANDE, STATUT) VALUES (?,?,?,?,?)");
    query.addBindValue(clientId);
    query.addBindValue(lineEditDescription->text());
    query.addBindValue(lineEditMontant->text().toDouble());
    query.addBindValue(dateEditCommande->date());
    query.addBindValue(comboBoxStatut->currentText());

    if(!query.exec()) {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    modelCommandes->select();
}

void MainWindow::updateCommande() {
    QModelIndex idx = tableViewCommandes->currentIndex();
    if(!idx.isValid()) return;
    int id = modelCommandes->data(modelCommandes->index(idx.row(),0)).toInt();
    int clientId = comboBoxClient->currentData().toInt();

    QSqlQuery query(Connection::getInstance());
    query.prepare("UPDATE COMMANDE SET ID_CLIENT=?, DESCRIPTION=?, MONTANT=?, DATE_COMMANDE=?, STATUT=? WHERE ID_COMMANDE=?");
    query.addBindValue(clientId);
    query.addBindValue(lineEditDescription->text());
    query.addBindValue(lineEditMontant->text().toDouble());
    query.addBindValue(dateEditCommande->date());
    query.addBindValue(comboBoxStatut->currentText());
    query.addBindValue(id);

    if(!query.exec()) {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    modelCommandes->select();
}

void MainWindow::deleteCommande() {
    QModelIndex idx = tableViewCommandes->currentIndex();
    if(!idx.isValid()) return;
    int id = modelCommandes->data(modelCommandes->index(idx.row(),0)).toInt();

    QSqlQuery query(Connection::getInstance());
    query.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE=?");
    query.addBindValue(id);

    if(!query.exec()) {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    modelCommandes->select();
}

void MainWindow::searchCommande() {
    QString desc = lineEditDescription->text();
    modelCommandes->setFilter(QString("DESCRIPTION LIKE '%%1%'").arg(desc));
    modelCommandes->select();
}

void MainWindow::populateCommandeForm(const QModelIndex &current, const QModelIndex &) {
    if(!current.isValid()) return;

    comboBoxClient->setCurrentText(modelCommandes->data(modelCommandes->index(current.row(),1)).toString());
    lineEditDescription->setText(modelCommandes->data(modelCommandes->index(current.row(),2)).toString());
    lineEditMontant->setText(modelCommandes->data(modelCommandes->index(current.row(),3)).toString());
    dateEditCommande->setDate(modelCommandes->data(modelCommandes->index(current.row(),4)).toDate());
    comboBoxStatut->setCurrentText(modelCommandes->data(modelCommandes->index(current.row(),5)).toString());
}

// =========================
// Export PDF
// =========================
void MainWindow::exportClientsToPdf(const QString &filename) {
    if (modelClients->rowCount() == 0) return;

    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&writer);
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    int margin = 20;
    int y = margin;

    // Draw table headers
    int colCount = modelClients->columnCount();
    int pageWidth = writer.width() - 2 * margin;
    int colWidth = pageWidth / colCount;

    QFontMetrics fm(font);
    for(int j=0;j<colCount;++j) {
        QString header = modelClients->headerData(j, Qt::Horizontal).toString();
        painter.drawText(margin + j*colWidth, y, header);
    }
    y += fm.height() + 5;

    // Draw table rows
    for(int i=0;i<modelClients->rowCount();++i){
        for(int j=0;j<colCount;++j){
            QString text = modelClients->data(modelClients->index(i,j)).toString();
            painter.drawText(margin + j*colWidth, y, text);
        }
        y += fm.height() + 5;

        // Check page overflow
        if (y > writer.height() - margin) {
            writer.newPage();
            y = margin;
        }
    }

    painter.end();
}


void MainWindow::exportCommandesToPdf(const QString &filename) {
    if (modelCommandes->rowCount() == 0) return;

    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&writer);
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    int margin = 20;
    int y = margin;

    // Draw table headers
    int colCount = modelCommandes->columnCount();
    int pageWidth = writer.width() - 2 * margin;
    int colWidth = pageWidth / colCount;

    QFontMetrics fm(font);
    for(int j=0;j<colCount;++j) {
        QString header = modelCommandes->headerData(j, Qt::Horizontal).toString();
        painter.drawText(margin + j*colWidth, y, header);
    }
    y += fm.height() + 5;

    // Draw table rows
    for(int i=0;i<modelCommandes->rowCount();++i){
        for(int j=0;j<colCount;++j){
            QString text = modelCommandes->data(modelCommandes->index(i,j)).toString();
            painter.drawText(margin + j*colWidth, y, text);
        }
        y += fm.height() + 5;

        // Check page overflow
        if (y > writer.height() - margin) {
            writer.newPage();
            y = margin;
        }
    }

    painter.end();
}

void MainWindow::onExportClientsPdfClicked() {
    QString fileName = QFileDialog::getSaveFileName(this,"Save PDF","","PDF Files (*.pdf)");
    if(!fileName.isEmpty()) exportClientsToPdf(fileName);
}

void MainWindow::onExportCommandesPdfClicked() {
    QString fileName = QFileDialog::getSaveFileName(this,"Save PDF","","PDF Files (*.pdf)");
    if(!fileName.isEmpty()) exportCommandesToPdf(fileName);
}

// =========================
// Statistics Tab
// =========================



void MainWindow::setupStatisticsTabs() {
    // --- Remove old Commandes tab safely ---
    if(tabStatisticsCommandes) {
        int idx = tabWidget->indexOf(tabStatisticsCommandes);
        if(idx != -1) tabWidget->removeTab(idx);
        delete tabStatisticsCommandes;
        tabStatisticsCommandes = nullptr;
    }

    // --- Remove old Clients tab safely ---
    if(tabStatisticsClients) {
        int idx = tabWidget->indexOf(tabStatisticsClients);
        if(idx != -1) tabWidget->removeTab(idx);
        delete tabStatisticsClients;
        tabStatisticsClients = nullptr;
    }

    // ===============================
    // Splitter to hold both charts
    // ===============================
    tabStatistics = new QWidget();
    QHBoxLayout *layoutMain = new QHBoxLayout(tabStatistics);
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // ===============================
    // Commandes Statistics
    // ===============================
    tabStatisticsCommandes = new QWidget();
    QVBoxLayout *layoutCommandes = new QVBoxLayout(tabStatisticsCommandes);

    QPieSeries *seriesCommandes = new QPieSeries();
    int countEnCours = 0, countLivree = 0, countAnnulee = 0;

    if(modelCommandes && modelCommandes->rowCount() > 0) {
        for(int i = 0; i < modelCommandes->rowCount(); ++i) {
            QString statut = modelCommandes->data(modelCommandes->index(i, 5)).toString();
            if(statut == "EN_COURS") countEnCours++;
            else if(statut == "LIVREE") countLivree++;
            else if(statut == "ANNULEE") countAnnulee++;
        }
    }

    seriesCommandes->append("EN_COURS", countEnCours);
    seriesCommandes->append("LIVREE", countLivree);
    seriesCommandes->append("ANNULEE", countAnnulee);

    QChart *chartCommandes = new QChart();
    chartCommandes->addSeries(seriesCommandes);
    chartCommandes->setTitle("Commandes Status");
    chartCommandes->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartViewCommandes = new QChartView(chartCommandes);
    chartViewCommandes->setRenderHint(QPainter::Antialiasing);

    layoutCommandes->addWidget(chartViewCommandes);
    tabStatisticsCommandes->setLayout(layoutCommandes);

    // ===============================
    // Clients Statistics
    // ===============================
    tabStatisticsClients = new QWidget();
    QVBoxLayout *layoutClients = new QVBoxLayout(tabStatisticsClients);

    QPieSeries *seriesClients = new QPieSeries();

    int totalClients = modelClients ? modelClients->rowCount() : 0;
    int clientsWithCommandes = 0;
    int clientsWithoutCommandes = 0;
    int maxCommandes = 0;

    QMap<int,int> clientCommandesCount;
    QSqlQuery query(Connection::getInstance());
    if(query.exec("SELECT ID_CLIENT, COUNT(*) AS nbr FROM COMMANDE GROUP BY ID_CLIENT")) {
        while(query.next()) {
            int clientId = query.value("ID_CLIENT").toInt();
            int nbr = query.value("nbr").toInt();
            clientCommandesCount[clientId] = nbr;
            if(nbr > maxCommandes) maxCommandes = nbr;
        }
    }

    // Count clients with commandes (excluding max) and without commandes
    for(int i = 0; i < totalClients; ++i) {
        int clientId = modelClients->data(modelClients->index(i, 0)).toInt();
        int nbr = clientCommandesCount.value(clientId, 0);

        if(nbr > 0 && nbr < maxCommandes) clientsWithCommandes++;
        else if(nbr == 0) clientsWithoutCommandes++;
        // clients with maxCommandes are ignored
    }

    // Add pie slices safely
    seriesClients->append("Clients with Commandes", clientsWithCommandes);
    seriesClients->append("Clients with No Commandes", clientsWithoutCommandes);

    QChart *chartClients = new QChart();
    chartClients->addSeries(seriesClients);
    chartClients->setTitle("Clients Statistics");
    chartClients->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartViewClients = new QChartView(chartClients);
    chartViewClients->setRenderHint(QPainter::Antialiasing);

    layoutClients->addWidget(chartViewClients);
    tabStatisticsClients->setLayout(layoutClients);

    // ===============================
    // Add charts to splitter
    // ===============================
    splitter->addWidget(tabStatisticsCommandes);
    splitter->addWidget(tabStatisticsClients);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    layoutMain->addWidget(splitter);
    tabStatistics->setLayout(layoutMain);

    // Add to main tab widget
    tabWidget->addTab(tabStatistics, "Statistics");
}
