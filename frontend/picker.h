#ifndef PICKER_H
#define PICKER_H

#include <QWidget>

namespace Ui {
class Picker;
}

class Picker : public QWidget
{
    Q_OBJECT

public:
    explicit Picker(QWidget *parent = 0);
    ~Picker();

private:
    Ui::Picker *ui;
};

#endif // PICKER_H
