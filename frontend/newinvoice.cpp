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
    rcvModel.setHeaderData(4, Qt::Horizontal, "PSČ");

    itmModel.setHeaderData(0, Qt::Horizontal, "ID");
    itmModel.setHeaderData(1, Qt::Horizontal, "Název");
    itmModel.setHeaderData(2, Qt::Horizontal, "Cena");

    itmModel.insertColumn(3);
    itmModel.setHeaderData(3, Qt::Horizontal, "Počet");
    itmModel.insertColumn(4);
    itmModel.setHeaderData(4, Qt::Horizontal, "Poznámka");

    ui->rcvView->setModel(&rcvModel);
    ui->itmView->setModel(&itmModel);

    ui->rcvView->resizeColumnsToContents();
    for (int c = 0; c < 4; c++)
        ui->itmView->resizeColumnToContents(c);
    ui->itmView->setColumnWidth(3, ui->itmView->columnWidth(3) + 20);
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

    if (ui->tabWidget->currentWidget() == ui->tab_sum) {
        const QItemSelectionModel *selModel = ui->rcvView->selectionModel();
        if (!selModel || !selModel->selectedRows().count())
            return;
        const QModelIndex idx = selModel->selectedRows()[0];
        const QSqlTableModel *tabModel =
                dynamic_cast<const QSqlTableModel *>(idx.model());
        const QSqlRecord &rec = tabModel->record(idx.row());
        ui->ICEdit->setText(rec.value("ic").toString());
        ui->nameEdit->setText(rec.value("name").toString());
        ui->cityEdit->setText(rec.value("city").toString());

        QDate curr = QDate::currentDate();
        ui->issEdit->setDate(curr);
        ui->dueEdit->setDate(curr.addDays(14));

        QSqlQuery query;
        query.prepare("SELECT id + 1 FROM invoice WHERE id BETWEEN :min "
                        "AND :max ORDER BY id DESC LIMIT 1");
        int id = curr.year() * 100000 + curr.month() * 1000 + 1;
        query.bindValue(":min", id);
        query.bindValue(":max", id + 998);
        qDebug() << id << id + 998 << query.lastError();
        query.exec();
        while (query.next()) {
            id = query.value(0).toInt();
        }
        qDebug() << id;
        ui->invNOEdit->setText(QString::number(id));
        ui->invVSEdit->setText(QString::number(id));
    }
}

void NewInvoice::prev()
{
    int idx = ui->tabWidget->currentIndex();
//  qDebug() << __func__ << idx;
    ui->tabWidget->setCurrentIndex(idx - 1);
    buttonsEnable();
}

void NewInvoice::next()
{
    int idx = ui->tabWidget->currentIndex();
//  qDebug() << __func__ << idx;
    ui->tabWidget->setCurrentIndex(idx + 1);
    buttonsEnable();
}
