#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"

#include <QDebug>

FenetrePrincipale::FenetrePrincipale(QWidget *parent) : QMainWindow(parent), ui(new Ui::FenetrePrincipale)
{
    ui->setupUi(this);

    QObject::connect(ui->parametres, SIGNAL(clicked()), this, SLOT(ouvrirFenetreParametres()));
}

FenetrePrincipale::~FenetrePrincipale()
{
    delete ui;
}

void FenetrePrincipale::ouvrirFenetreParametres()
{
    fenetreParametres = new FenetreParametres(this);
    fenetreParametres->exec();
}
