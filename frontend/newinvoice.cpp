#include <QDebug>

#include "newinvoice.h"
#include "ui_newinvoice.h"

NewInvoice::NewInvoice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewInvoice)
{
    ui->setupUi(this);

    buttonsEnable();

    rcvModel.setTable("receiver");
    rcvModel.select();
    itmModel.setTable("item");
    itmModel.select();

    rcvModel.setHeaderData(0, Qt::Horizontal, "IČ");
    rcvModel.setHeaderData(1, Qt::Horizontal, "Název");
    rcvModel.setHeaderData(2, Qt::Horizontal, "Ulice");
    rcvModel.setHeaderData(3, Qt::Horizontal, "Město");

    itmModel.setHeaderData(0, Qt::Horizontal, "ID");
    itmModel.setHeaderData(1, Qt::Horizontal, "Název");
    itmModel.setHeaderData(2, Qt::Horizontal, "Cena");

    itmModel.insertColumn(3);
    itmModel.setHeaderData(3, Qt::Horizontal, "Počet");
    itmModel.insertColumn(4);
    itmModel.setHeaderData(4, Qt::Horizontal, "Poznámka");

    ui->rcvView->setModel(&rcvModel);
    ui->itmView->setModel(&itmModel);
}

NewInvoice::~NewInvoice()
{
    delete ui;
}

void NewInvoice::buttonsEnable()
{
    int idx = ui->tabWidget->currentIndex();

    ui->backButton->setDisabled(idx == 0);
    ui->fwdButton->setDisabled(idx == ui->tabWidget->count() - 1);
}

void NewInvoice::currentChanged()
{
    buttonsEnable();
}

void NewInvoice::prev()
{
    int idx = ui->tabWidget->currentIndex();
    qDebug() << __func__ << idx;
    ui->tabWidget->setCurrentIndex(idx - 1);
    buttonsEnable();
}

void NewInvoice::next()
{
    int idx = ui->tabWidget->currentIndex();
    qDebug() << __func__ << idx;
    ui->tabWidget->setCurrentIndex(idx + 1);
    buttonsEnable();
}
