#include "slucto.h"
#include "ui_slucto.h"

SlUcto::SlUcto(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SlUcto)
{
    ui->setupUi(this);
    ui->tableView->setModel(1, &model);
}

SlUcto::~SlUcto()
{
    delete ui;
}
