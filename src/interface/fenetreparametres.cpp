#include "fenetreparametres.h"
#include "ui_fenetreparametres.h"

FenetreParametres::FenetreParametres(QWidget *parent) : QDialog(parent), ui(new Ui::FenetreParametres)
{
    ui->setupUi(this);

    chargementParametres();

    QObject::connect(ui->annuler, SIGNAL(clicked()), this, SLOT(fermer()));
    QObject::connect(ui->valider, SIGNAL(clicked()), this, SLOT(enregistrer()));
    QObject::connect(ui->sourceChoix, SIGNAL(currentIndexChanged(QString)), this, SLOT(sourceChoix(QString)));
    QObject::connect(ui->message, SIGNAL(clicked()), this, SLOT(message()));
    QObject::connect(ui->actionsChoix, SIGNAL(currentIndexChanged(QString)), this, SLOT(actionsChoix(QString)));
    QObject::connect(ui->objetsChoix, SIGNAL(currentIndexChanged(QString)), this, SLOT(objetsChoix(QString)));
    QObject::connect(ui->mainsChoix, SIGNAL(currentIndexChanged(QString)), this, SLOT(mainsChoix(QString)));
    QObject::connect(ui->traitementChoix, SIGNAL(currentIndexChanged(QString)), this, SLOT(traitementChoix(QString)));
    QObject::connect(ui->langueChoix, SIGNAL(currentIndexChanged(QString)), this, SLOT(langueChoix(QString)));
}

FenetreParametres::~FenetreParametres()
{
    delete ui;
}

void FenetreParametres::chargementParametres()
{
    parametresTemporaires["sourceType"] = QString::fromStdString(parametres.getParametre("sourceType"));
    ui->sourceChoix->setCurrentText(parametresTemporaires["sourceType"]);
    parametresTemporaires["sourceChemin"] = QString::fromStdString(parametres.getParametre("sourceChemin"));
    ui->message->setText(parametresTemporaires["sourceChemin"]);
    parametresTemporaires["affichageActions"] = QString::fromStdString(parametres.getParametre("affichageActions"));
    ui->actionsChoix->setCurrentText(parametresTemporaires["affichageActions"]);
    parametresTemporaires["affichageObjets"] = QString::fromStdString(parametres.getParametre("affichageObjets"));
    ui->objetsChoix->setCurrentText(parametresTemporaires["affichageObjets"]);
    parametresTemporaires["affichageMains"] = QString::fromStdString(parametres.getParametre("affichageMains"));
    ui->mainsChoix->setCurrentText(parametresTemporaires["affichageMains"]);
    parametresTemporaires["precisionTraitement"] = QString::fromStdString(parametres.getParametre("precisionTraitement"));
    ui->traitementChoix->setCurrentText(parametresTemporaires["precisionTraitement"]);
    parametresTemporaires["langue"] = QString::fromStdString(parametres.getParametre("langue"));
    ui->langueChoix->setCurrentText(parametresTemporaires["langue"]);
}

void FenetreParametres::sauvegardeParametres()
{
    parametres.setParametre("sourceType" , parametresTemporaires["sourceType"].toStdString());
    parametres.setParametre("sourceChemin" , parametresTemporaires["sourceChemin"].toStdString());
    parametres.setParametre("affichageActions" , parametresTemporaires["affichageActions"].toStdString());
    parametres.setParametre("affichageObjets" , parametresTemporaires["affichageObjets"].toStdString());
    parametres.setParametre("affichageMains" , parametresTemporaires["affichageMains"].toStdString());
    parametres.setParametre("precisionTraitement" , parametresTemporaires["precisionTraitement"].toStdString());
    parametres.setParametre("langue" , parametresTemporaires["langue"].toStdString());
}

void FenetreParametres::selectionVideo()
{
    QString dossier = QFileDialog::getExistingDirectory();
    if (dossier != "") {
        ui->message->setText(dossier);
        parametresTemporaires["sourceChemin"] = dossier;
    } else {
        ui->message->setText("Attention ! Aucun dossier n'est sélectionné...");
    }
}

void FenetreParametres::fermer()
{
    close();
}

void FenetreParametres::enregistrer()
{
    if (parametresTemporaires["langue"] != QString::fromStdString(parametres.getParametre("langue"))) {
        // Changement langue
    }

    sauvegardeParametres();
    parametres.sauvegardeParametres();
    close();
}

void FenetreParametres::sourceChoix(QString valeur)
{
    if (valeur == "Vidéo locale") {
        selectionVideo();
    } else {
        ui->message->setText("");
    }

    parametresTemporaires["sourceType"] = valeur;
}

void FenetreParametres::message()
{
    selectionVideo();
}

void FenetreParametres::actionsChoix(QString valeur)
{
    parametresTemporaires["affichageActions"] = valeur;
}

void FenetreParametres::objetsChoix(QString valeur)
{
    parametresTemporaires["affichageObjets"] = valeur;
}

void FenetreParametres::mainsChoix(QString valeur)
{
    parametresTemporaires["affichageMains"] = valeur;
}

void FenetreParametres::traitementChoix(QString valeur)
{
    parametresTemporaires["precisionTraitement"] = valeur;
}

void FenetreParametres::langueChoix(QString valeur)
{
    parametresTemporaires["langue"] = valeur;
}
