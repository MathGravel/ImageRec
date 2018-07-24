#include "fenetreparametres.h"
#include "ui_fenetreparametres.h"

FenetreParametres::FenetreParametres(QWidget *parent) : QDialog(parent), ui(new Ui::FenetreParametres)
{
    ui->setupUi(this);

    QObject::connect(ui->annuler, SIGNAL(clicked()), this, SLOT(fermer()));
    QObject::connect(ui->valider, SIGNAL(clicked()), this, SLOT(enregistrer()));
}

FenetreParametres::~FenetreParametres()
{
    delete ui;
}

void FenetreParametres::fermer()
{
    close();
}

void FenetreParametres::enregistrer()
{

}
