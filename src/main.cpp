#include "fenetreprincipale.h"
#include <QApplication>
#include <QRect>
#include <QPoint>
#include <QStyle>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FenetrePrincipale fenetre;
    fenetre.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, fenetre.size(), qApp->desktop()->availableGeometry()));
    fenetre.show();

    return app.exec();
}
