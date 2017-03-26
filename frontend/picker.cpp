#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>

#include "picker.h"
#include "ui_picker.h"

Picker::Picker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Picker)
{
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
            size(), qApp->desktop()->availableGeometry()));
}

Picker::~Picker()
{
    delete ui;
}

void Picker::printInvoice()
{
    pi.show();
    this->hide();
}

void Picker::newInvoice()
{
    su.show();
    this->hide();
}
