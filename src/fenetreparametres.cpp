#include "fenetreparametres.h"
#include "ui_fenetreparametres.h"

FenetreParametres::FenetreParametres(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FenetreParametres)
{
    ui->setupUi(this);
}

FenetreParametres::~FenetreParametres()
{
    delete ui;
}
