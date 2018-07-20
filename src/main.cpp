#include "fenetreprincipale.h"
#include <QApplication>
#include <QRect>
#include <QPoint>
#include <QStyle>
#include <QDesktopWidget>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int id = QFontDatabase::addApplicationFont(":/fonts/Ubuntu/Ubuntu-Regular.ttf");
    QString ubuntuRegular = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont ubuntu(ubuntuRegular, 8);
    app.setFont(ubuntu);

    FenetrePrincipale fenetre;
    fenetre.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, fenetre.size(), qApp->desktop()->availableGeometry()));
    fenetre.show();

    return app.exec();
}
