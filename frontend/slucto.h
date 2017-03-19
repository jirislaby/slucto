#ifndef SLUCTO_H
#define SLUCTO_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class SlUcto;
}

class SlUcto : public QMainWindow
{
    Q_OBJECT

public:
    explicit SlUcto(QWidget *parent = 0);
    ~SlUcto();

private:
    Ui::SlUcto *ui;
    QSqlTableModel rcvModel;
    QSqlTableModel itmModel;
};

#endif // SLUCTO_H
