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
    rcvModel.setSort(rcvModel.fieldIndex("name"), Qt::AscendingOrder);
    rcvModel.select();
    itmModel.setTable("item");
    itmModel.select();
    relModel.setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    relModel.setTable("invoice_item");
    relModel.setRelation(relModel.fieldIndex("id_item"),
                         QSqlRelation("item", "id", "name"));
    relModel.setFilter(QString("id_invoice = %1").arg(-1));
    relModel.select();

    rcvModel.setHeaderData(0, Qt::Horizontal, "IČ");
    rcvModel.setHeaderData(1, Qt::Horizontal, "Název");
    rcvModel.setHeaderData(2, Qt::Horizontal, "Ulice");
    rcvModel.setHeaderData(3, Qt::Horizontal, "Město");
    rcvModel.setHeaderData(4, Qt::Horizontal, "PSČ");

    rcvModel.setSort(1, Qt::AscendingOrder);
    itmModel.setHeaderData(1, Qt::Horizontal, "Název");
    itmModel.setHeaderData(2, Qt::Horizontal, "Cena");

    const int first = 3;
    itmModel.setFirst(first);
    itmModel.insertColumn(first);
    itmModel.setHeaderData(first, Qt::Horizontal, "Počet");
    itmModel.insertColumn(first + 1);
    itmModel.setHeaderData(first + 1, Qt::Horizontal, "Poznámka");

    relModel.setHeaderData(0, Qt::Horizontal, "ID");
    relModel.setHeaderData(1, Qt::Horizontal, "Název");
    relModel.setHeaderData(2, Qt::Horizontal, "Počet");
    relModel.setHeaderData(3, Qt::Horizontal, "Cena");
    relModel.setHeaderData(4, Qt::Horizontal, "Poznámka");

/*    sumItmModel.setHeaderData(0, Qt::Horizontal, "Název");
    sumItmModel.setHeaderData(1, Qt::Horizontal, "Cena");
    sumItmModel.setHeaderData(2, Qt::Horizontal, "Počet");
    sumItmModel.setHeaderData(3, Qt::Horizontal, "Poznámka");
*/
    ui->rcvView->setModel(&rcvModel);
    ui->itmView->setModel(&itmModel);
    ui->itmView->setColumnHidden(0, true);
    //ui->sumItemView->setModel(&sumItmModel);
 //   ui->sumItemView->setModel(&itmModel);
    ui->sumItemView->setModel(&relModel);

    ui->rcvView->resizeColumnsToContents();
    for (int c = 0; c <= first; c++)
        ui->itmView->resizeColumnToContents(c);
    ui->itmView->setColumnWidth(first, ui->itmView->columnWidth(first) + 20);
}

NewInvoice::~NewInvoice()
{
    delete ui;
}

void NewInvoice::copyToVS()
{
    ui->invVSEdit->setText(QString::number(ui->invNOBox->value()));
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
        int row = selModel->selectedRows()[0].row();
        const QSqlRecord rec = rcvModel.record(row);
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
        query.exec();
        while (query.next()) {
            id = query.value(0).toInt();
        }
        ui->invNOBox->setValue(id);
        ui->invVSEdit->setText(QString::number(id));

        relModel.setFilter(QString("id_invoice = %1").arg(id));

        QVector<int> cnts = itmModel.getCounts();
        for (int i = 0; i < cnts.count(); i++)  {
            if (!cnts[i])
                continue;

            QSqlRecord itemRec = itmModel.record(i);
            QSqlRecord rec = relModel.record();
            rec.setValue("id_invoice", id);
            rec.setValue("id_item", itemRec.value("id").toInt());
            rec.setValue("name", itemRec.value("id").toString());
            rec.setValue("count", cnts[i]);
            rec.setValue("price", itemRec.value("price").toDouble());
            rec.setValue("note", itmModel.getComment(i));
            if (!relModel.insertRecord(-1, rec)) {
                qWarning() << relModel.lastError();
                break;
            }
        }
        ui->sumItemView->resizeColumnsToContents();
        ui->sumItemView->setColumnWidth(2, ui->sumItemView->columnWidth(2) + 20);
    } else {
        relModel.revertAll();
//      relModel.setFilter(QString());
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
