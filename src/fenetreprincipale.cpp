#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"

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

void FenetrePrincipale::configuration()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void FenetrePrincipale::gestionVideo()
{
    // Lancement / arrêt de la vidéo !
}

void FenetrePrincipale::MiseAJourImage()
{
    // Changement de l'image du lecteur !
}

void FenetrePrincipale::MiseAJourHistogramme()
{
    // Mise à jour de l'histogramme !
}

void FenetrePrincipale::MiseAJourProgression()
{
    // Mise à jour du timer !
}

void FenetrePrincipale::MiseAJourInformations()
{
    // Mise à jour des inforamtions !
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
