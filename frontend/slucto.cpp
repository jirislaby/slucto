#include "slucto.h"
#include "ui_slucto.h"

SlUcto::SlUcto(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SlUcto)
{
    ui->setupUi(this);
    rcvModel.setTable("prijemce");
    rcvModel.select();
    itmModel.setTable("polozka");
    itmModel.select();
    ui->rcvView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->rcvView->setModel(&rcvModel);
    ui->itmView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->itmView->setModel(&itmModel);
}

SlUcto::~SlUcto()
{
    delete ui;
}
