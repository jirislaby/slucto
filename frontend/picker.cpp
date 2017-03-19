#include "picker.h"
#include "ui_picker.h"

Picker::Picker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Picker)
{
    ui->setupUi(this);
}

Picker::~Picker()
{
    delete ui;
}
