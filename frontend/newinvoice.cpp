#include <QApplication>
#include <QDebug>
#include <QScreen>

#include "newinvoice.h"
#include "printinvoice.h"
#include "ui_newinvoice.h"

NewInvoice::NewInvoice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewInvoice)
{
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
	    size(), qApp->primaryScreen()->availableGeometry()));

    buttonsEnable();
    setupModels();
}

void NewInvoice::setupModels()
{
    rcvModel.setTable("receiver");
    rcvModel.setSort(rcvModel.fieldIndex("name"), Qt::AscendingOrder);
    rcvModel.select();
    rcvModel.setHeaderData(0, Qt::Horizontal, "IČ");
    rcvModel.setHeaderData(1, Qt::Horizontal, "Název");
    rcvModel.setHeaderData(2, Qt::Horizontal, "Ulice");
    rcvModel.setHeaderData(3, Qt::Horizontal, "Město");
    rcvModel.setHeaderData(4, Qt::Horizontal, "PSČ");
    ui->rcvView->setModel(&rcvModel);
    ui->rcvView->resizeColumnsToContents();

    itmModel.setTable("item");
    itmModel.select();
    itmModel.setSort(1, Qt::AscendingOrder);
    itmModel.setHeaderData(1, Qt::Horizontal, "Název");
    itmModel.setHeaderData(2, Qt::Horizontal, "Cena");
    const int first = 3;
    itmModel.setFirst(first);
    itmModel.insertColumn(first);
    itmModel.setHeaderData(first, Qt::Horizontal, "Počet");
    itmModel.insertColumn(first + 1);
    itmModel.setHeaderData(first + 1, Qt::Horizontal, "Poznámka");
    ui->itmView->setColumnHidden(0, true);
    ui->itmView->setModel(&itmModel);
    for (int c = 0; c <= first; c++)
        ui->itmView->resizeColumnToContents(c);
    ui->itmView->setColumnWidth(first, ui->itmView->columnWidth(first) + 20);

    relModel.setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    relModel.setTable("invoice_item");
    relModel.setRelation(relModel.fieldIndex("id_item"),
                         QSqlRelation("item", "id", "name"));
    relModel.setFilter(QString("id_invoice = %1").arg(-1));
    relModel.select();
    relModel.setHeaderData(0, Qt::Horizontal, "ID");
    relModel.setHeaderData(1, Qt::Horizontal, "Název");
    relModel.setHeaderData(2, Qt::Horizontal, "Počet");
    relModel.setHeaderData(3, Qt::Horizontal, "Cena");
    relModel.setHeaderData(4, Qt::Horizontal, "Poznámka");
    ui->sumItemView->setModel(&relModel);
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
        QDate curr = QDate::currentDate();
        ui->issEdit->setDate(curr);
        ui->dueEdit->setDate(curr.addDays(14));

        const QItemSelectionModel *selModel = ui->rcvView->selectionModel();
        if (!selModel || !selModel->selectedRows().count())
            return;
        int row = selModel->selectedRows()[0].row();
        const QSqlRecord rec = rcvModel.record(row);
        ui->ICEdit->setText(rec.value("ic").toString());
        ui->nameEdit->setText(rec.value("name").toString());
        ui->cityEdit->setText(rec.value("city").toString());

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
        ui->createInvButton->setEnabled(true);
    } else {
        relModel.revertAll();
        ui->createInvButton->setEnabled(false);
//      relModel.setFilter(QString());
    }
}

void NewInvoice::prev()
{
    int idx = ui->tabWidget->currentIndex();
    ui->tabWidget->setCurrentIndex(idx - 1);
    buttonsEnable();
}

void NewInvoice::next()
{
    int idx = ui->tabWidget->currentIndex();
    ui->tabWidget->setCurrentIndex(idx + 1);
    buttonsEnable();
}

void NewInvoice::newRcv()
{
    if (!rcvModel.insertRow(rcvModel.rowCount()))
        qWarning() << "cannot create row" << rcvModel.lastError();
}

void NewInvoice::newItm()
{
    if (!itmModel.insertRow(itmModel.rowCount()))
        qWarning() << "cannot create row" << itmModel.lastError();
}

static void setFilter(QSqlTableModel &model, const QString &str)
{
    if (str.length() && !str.contains('\'')) {
        /* SQL injection */
        model.setFilter(QString("name LIKE '\%%1\%'").arg(str));
    } else
        model.setFilter(QString());
}

void NewInvoice::filterRcv(QString str)
{
    if (ui->tabWidget->currentWidget() == ui->tab_rcv)
        setFilter(rcvModel, str);
    else
        setFilter(itmModel, str);
}

void NewInvoice::createInv()
{
    relModel.submitAll();
    QSqlTableModel invoice;
    invoice.setTable("invoice");
    QSqlRecord rec = invoice.record();
    rec.setValue("id", ui->invNOBox->value());
    rec.setValue("receiver", ui->ICEdit->text().toInt());
    rec.setValue("issuance", ui->issEdit->dateTime().toSecsSinceEpoch());
    rec.setValue("due", ui->dueEdit->dateTime().toSecsSinceEpoch());
    qDebug() << rec;
    if (invoice.insertRecord(-1, rec))
        ui->printInvButton->setEnabled(true);
    else
        qWarning() << "cannot insert invoice" << invoice.lastError();
}

void NewInvoice::printInv()
{
    PrintInvoice::printInvoice(ui->invNOBox->value());
}
