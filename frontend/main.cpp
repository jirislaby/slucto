#include "newinvoice.h"
#include "picker.h"
#include "printinvoice.h"
#include <QApplication>
#include <QtSql>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir confDir(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation)[0]);
    QString confFile(confDir.absoluteFilePath("slucto.db"));
    if (!QFile::exists(confFile)) {
        qCritical("database file not found at " + confFile.toLatin1());
        return EXIT_FAILURE;
    }
    qDebug() << "using database at " << confFile;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(confFile);
    bool ok = db.open();
    if (!ok)
        return EXIT_FAILURE;

#if 0
    query.exec("SELECT * FROM prijemce");
    while (query.next()) {
        qDebug() << query.value(0);
        qDebug() << query.value(1).toString();
    }
#endif

    Picker p;
    p.show();

    return a.exec();
}
