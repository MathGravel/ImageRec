#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"
#include <QDebug>

FenetrePrincipale::FenetrePrincipale(QWidget *parent) : QMainWindow(parent), ui(new Ui::FenetrePrincipale)
{
    ui->setupUi(this);

    QObject::connect(ui->application, SIGNAL(clicked()), this, SLOT(gestionVideo()));
    QObject::connect(ui->parametres, SIGNAL(clicked()), this, SLOT(ouvrirFenetreParametres()));
    QObject::connect(ui->pleinEcran, SIGNAL(clicked()), this, SLOT(pleinEcranVideo()));
}

FenetrePrincipale::~FenetrePrincipale()
{
    delete ui;
}

void FenetrePrincipale::configuration(QApplication *app)
{
    int id = QFontDatabase::addApplicationFont(":/fonts/Ubuntu/Ubuntu-Regular.ttf");
    QString ubuntuRegular = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont ubuntu(ubuntuRegular, 8);
    app->setFont(ubuntu);

    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void FenetrePrincipale::gestionVideo()
{
    // Lancement / arrêt de la vidéo !
}

void FenetrePrincipale::ouvrirFenetreParametres()
{
    fenetreParametres = new FenetreParametres(this);
    fenetreParametres->exec();
}

void FenetrePrincipale::pleinEcranVideo()
{
    // Plein écran !
}
