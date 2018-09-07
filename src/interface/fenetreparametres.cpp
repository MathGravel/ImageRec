#include "fenetreparametres.h"
#include "ui_fenetreparametres.h"

FenetreParametres::FenetreParametres(QWidget *parent) : QDialog(parent), ui(new Ui::FenetreParametres)
{
    ui->setupUi(this);

    chargementParametres();

    QObject::connect(ui->annuler, SIGNAL(clicked()), this, SLOT(fermer()));
    QObject::connect(ui->valider, SIGNAL(clicked()), this, SLOT(enregistrer()));
    QObject::connect(ui->sourceChoix, SIGNAL(currentIndexChanged(QString)), this, SLOT(sourceChoix(QString)));
    QObject::connect(ui->messageCouleur, SIGNAL(clicked()), this, SLOT(messageCouleur()));
    QObject::connect(ui->messageProfondeur, SIGNAL(clicked()), this, SLOT(messageProfondeur()));
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
    parametresTemporaires["sourceCheminProfondeur"] = QString::fromStdString(parametres.getParametre("sourceCheminProfondeur"));
    if (parametresTemporaires["sourceType"] == "Vidéo locale") {
        ui->messageCouleur->setText("<font style=\"font-weight:600;\">Couleur : </font>" + parametresTemporaires["sourceChemin"]);
        ui->messageProfondeur->setText("<font style=\"font-weight:600;\">Profondeur : </font>" + parametresTemporaires["sourceCheminProfondeur"]);
    } else {
        ui->messageCouleur->setText("");
        ui->messageProfondeur->setText("");
    }
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
    parametres.setParametre("sourceCheminProfondeur" , parametresTemporaires["sourceCheminProfondeur"].toStdString());
    parametres.setParametre("affichageActions" , parametresTemporaires["affichageActions"].toStdString());
    parametres.setParametre("affichageObjets" , parametresTemporaires["affichageObjets"].toStdString());
    parametres.setParametre("affichageMains" , parametresTemporaires["affichageMains"].toStdString());
    parametres.setParametre("precisionTraitement" , parametresTemporaires["precisionTraitement"].toStdString());
    parametres.setParametre("langue" , parametresTemporaires["langue"].toStdString());
}

void FenetreParametres::selectionVideoCouleur()
{
    QString video = QFileDialog::getOpenFileName();
    if (video != "") {
        ui->messageCouleur->setText("<font style=\"font-weight:600;\">Couleur : </font>" + video);
        parametresTemporaires["sourceChemin"] = video;
    } else {
        if (parametresTemporaires["sourceCheminProfondeur"] != "") {
            ui->messageCouleur->setText(tr("<font style=\"font-weight:600;\">Couleur : </font>") + parametresTemporaires["sourceCheminProfondeur"]);
        } else {
            ui->messageCouleur->setText(tr("<font style=\"font-weight:600;\">Couleur : </font>Attention, aucune vidéo n'est sélectionné."));
        }
    }
}

void FenetreParametres::selectionVideoProfondeur()
{
    QString video = QFileDialog::getOpenFileName();
    if (video != "") {
        ui->messageProfondeur->setText("<font style=\"font-weight:600;\">Profondeur : </font>" + video);
        parametresTemporaires["sourceCheminProfondeur"] = video;
    } else {
        if (parametresTemporaires["sourceCheminProfondeur"] != "") {
            ui->messageProfondeur->setText(tr("<font style=\"font-weight:600;\">Profondeur : </font>") + parametresTemporaires["sourceCheminProfondeur"]);
        } else {
            ui->messageProfondeur->setText(tr("<font style=\"font-weight:600;\">Profondeur : </font>Attention, aucune vidéo n'est sélectionné."));
        }
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
        ui->messageCouleur->setText(tr("<font style=\"font-weight:600;\">Couleur : </font>") + parametresTemporaires["sourceChemin"]);
        ui->messageProfondeur->setText(tr("<font style=\"font-weight:600;\">Profondeur : </font>") + parametresTemporaires["sourceCheminProfondeur"]);
    } else {
        ui->messageCouleur->setText("");
        ui->messageProfondeur->setText("");
    }

    parametresTemporaires["sourceType"] = valeur;
}

void FenetreParametres::messageCouleur()
{
    selectionVideoCouleur();
}

void FenetreParametres::messageProfondeur()
{
    selectionVideoProfondeur();
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
