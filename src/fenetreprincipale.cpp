#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"
#include <QDebug>

FenetrePrincipale::FenetrePrincipale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FenetrePrincipale)
{
    ui->setupUi(this);
}

FenetrePrincipale::~FenetrePrincipale()
{
    delete ui;
}
