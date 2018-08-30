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

void FenetrePrincipale::closeEvent(QCloseEvent *event)
{
    play = false;
    event->accept();
}

void FenetrePrincipale::configuration()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

    ui->activites->setHidden(true);
    ui->feedback->setHidden(true);
    ui->planificateur->setHidden(true);
}

void FenetrePrincipale::gestionVideo()
{
    play = !play;

    if (play) {
        this->MiseAJourImage();
    } else {
        reconnaissanceManager->setIsStopped(false);
        reconnaissanceManager = NULL;
    }
}

void FenetrePrincipale::MiseAJourImage()
{
    lectureVideo();

    while(true) {
        reconnaissanceManager->update();
        cv::Mat img = reconnaissanceManager->getCurrentFeed();

        cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
        ui->image->setPixmap(QPixmap::fromImage(QImage((unsigned char*) img.data,img.cols,img.rows,QImage::Format_RGB888)));
        ui->image->repaint();
        MiseAJourHistogramme(reconnaissanceManager->getTimePosition());
        MiseAJourProgression(reconnaissanceManager->getTimeStamp());
        MiseAJourInformations(reconnaissanceManager->getInformations());

        qApp->processEvents();
        if ((!play) || (reconnaissanceManager->getTimePosition() == 100)) {
            break;
        }
    }

    arretVideo();
}

void FenetrePrincipale::MiseAJourHistogramme(int position)
{
    ui->histogramme->setValue(position);
    ui->histogramme->repaint();
}

void FenetrePrincipale::MiseAJourProgression(string timer)
{
    ui->progression->setText(QString::fromStdString(timer));
    ui->progression->repaint();
}

void FenetrePrincipale::MiseAJourInformations(std::map<std::string, std::map<std::string, std::string>> informations)
{
    if ((informations["actionPrecedente1"]["nom"] == "NA") && (informations["actionPrecedente2"]["nom"] == "NA")) {
        ui->actionsPrecedentes->setHidden(true);
    } else {
        ui->actionsPrecedentes->setHidden(false);
    }

    if (informations["actionPrecedente1"]["nom"] != "NA") {
        ui->actionsPrecedentesTitre->setText("Actions précédentes");
        ui->actionsPrecedentes1->setHidden(false);
        ui->actionPrecedente1Label->setText(getNomAction(informations["actionPrecedente1"]["nom"]));
        ui->actionPrecedente1Logo->setPixmap(QPixmap(getLogo(informations["actionPrecedente1"]["nom"])));
        ui->actionPrecedente1Pourcentage->setText(QString::fromStdString(informations["actionPrecedente1"]["pourcentage"]) + " %");
    } else {
        ui->actionsPrecedentesTitre->setText("Action précédente");
        ui->actionsPrecedentes1->setHidden(true);
    }

    if (informations["actionPrecedente2"]["nom"] != "NA") {
        ui->actionsPrecedentes2->setHidden(false);
        ui->actionsPrecedentes2Label->setText(getNomAction(informations["actionPrecedente2"]["nom"]));
        ui->actionsPrecedentes2Logo->setPixmap(QPixmap(getLogo(informations["actionPrecedente2"]["nom"])));
        ui->actionsPrecedentes2Pourcentage->setText(QString::fromStdString(informations["actionPrecedente2"]["pourcentage"]) + " %");
    } else {
        ui->actionsPrecedentes2->setHidden(true);
    }


    ui->actionActuelle1Label->setText(getNomAction(informations["actionActuelle"]["nom"]));
    ui->actionActuelle1Logo->setPixmap(QPixmap(getLogo(informations["actionActuelle"]["nom"])));
    ui->actionActuelle1Pourcentage->setText(QString::fromStdString(informations["actionActuelle"]["pourcentage"]) + " %");

    if (informations["planCourant1"]["nom"] != "NA") {
        ui->plansCourants1Label->setText(getNomAction(informations["planCourant1"]["nom"]));
        ui->plansCourants1Logo->setPixmap(QPixmap(getLogo(informations["planCourant1"]["nom"])));
        ui->plansCourants1Pourcentage->setText(QString::fromStdString(informations["planCourant1"]["pourcentage"]) + " %");
    }

    if (informations["planCourant2"]["nom"] != "NA") {
        ui->plansCourants2Label->setText(getNomAction(informations["planCourant2"]["nom"]));
        ui->plansCourants2Logo->setPixmap(QPixmap(getLogo(informations["planCourant2"]["nom"])));
        ui->plansCourants2Pourcentage->setText(QString::fromStdString(informations["planCourant2"]["pourcentage"]) + " %");
    } else {
        ui->plansCourants2->setHidden(true);
    }
    if (informations["planCourant3"]["nom"] != "NA") {
        ui->plansCourants3Label->setText(getNomAction(informations["planCourant3"]["nom"]));
        ui->plansCourants3Logo->setPixmap(QPixmap(getLogo(informations["planCourant3"]["nom"])));
        ui->plansCourants3Pourcentage->setText(QString::fromStdString(informations["planCourant3"]["pourcentage"]) + " %");
    } else {
        ui->plansCourants3->setHidden(true);
    }

    if (informations["planCourant3"]["nom"] + informations["planCourant2"]["nom"] == "NANA")
        ui->plansCourantsTitre->setText("Plan courant");

    if (informations["actionSuivante1"]["nom"] != "NA") {
        ui->actionsSuivantes1Label->setText(getNomAction(informations["actionSuivante1"]["nom"]));
        ui->actionsSuivantes1Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante1"]["nom"])));
        ui->actionsSuivantes1Pourcentage->setText(QString::fromStdString(informations["actionSuivante1"]["pourcentage"]) + " %");
    }
    if (informations["actionSuivante2"]["nom"] != "NA") {
        ui->actionsSuivantes2Label->setText(getNomAction(informations["actionSuivante2"]["nom"]));
        ui->actionsSuivantes2Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante2"]["nom"])));
        ui->actionsSuivantes2Pourcentage->setText(QString::fromStdString(informations["actionSuivante2"]["pourcentage"]) + " %");
    }
    if (informations["actionSuivante3"]["nom"] != "NA") {
        ui->actionsSuivantes3Label->setText(getNomAction(informations["actionSuivante3"]["nom"]));
        ui->actionsSuivantes3Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante3"]["nom"])));
        ui->actionsSuivantes3Pourcentage->setText(QString::fromStdString(informations["actionSuivante3"]["pourcentage"]) + " %");
    }

    /*
    ui->plansCourants1Label->setText(getNomPlan(informations["planCourant1"]["nom"]));
    ui->plansCourants1Logo->setPixmap(QPixmap(getLogo(informations["planCourant1"]["nom"])));
    ui->plansCourants1Pourcentage->setText(QString::fromStdString(informations["planCourant1"]["pourcentage"]) + " %");

    ui->plansCourants2Label->setText(getNomPlan(informations["planCourant2"]["nom"]));
    ui->plansCourants2Logo->setPixmap(QPixmap(getLogo(informations["planCourant2"]["nom"])));
    ui->plansCourants2Pourcentage->setText(QString::fromStdString(informations["planCourant2"]["pourcentage"]) + " %");

    ui->plansCourants3Label->setText(getNomPlan(informations["planCourant3"]["nom"]));
    ui->plansCourants3Logo->setPixmap(QPixmap(getLogo(informations["planCourant3"]["nom"])));
    ui->plansCourants3Pourcentage->setText(QString::fromStdString(informations["planCourant3"]["pourcentage"]) + " %");

    */
    /*
    ui->actionsSuivantes1Label->setText(getNomAction(informations["actionSuivante1"]["nom"]));
    ui->actionsSuivantes1Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante1"]["nom"])));
    ui->actionsSuivantes1Pourcentage->setText(QString::fromStdString(informations["actionSuivante1"]["pourcentage"]) + " %");

    ui->actionsSuivantes2Label->setText(getNomAction(informations["actionSuivante2"]["nom"]));
    ui->actionsSuivantes2Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante2"]["nom"])));
    ui->actionsSuivantes2Pourcentage->setText(QString::fromStdString(informations["actionSuivante2"]["pourcentage"]) + " %");

    ui->actionsSuivantes3Label->setText(getNomAction(informations["actionSuivante3"]["nom"]));
    ui->actionsSuivantes3Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante3"]["nom"])));
    ui->actionsSuivantes3Pourcentage->setText(QString::fromStdString(informations["actionSuivante3"]["pourcentage"]) + " %");
    */
}

QString FenetrePrincipale::getNomAction(string nom)
{
    if (nom == "teamaking") { return tr("Préparation de thé"); }
    if (nom == "coffemaking") { return tr("Préparation de café"); }
    if (nom == "chocomaking") { return tr("Préparation de chocolat"); }
    if (nom == "teakettle") { return tr("Bouilloire"); }
    if (nom == "water") { return tr("Pichet d'eau"); }
    if (nom == "mug") { return tr("Tasse"); }
    if (nom == "coffe") { return tr("Café"); }
    if (nom == "pot") { return tr("Pot"); }
    if (nom == "coffemaker") { return tr("Machine à café"); }
    if (nom == "milk") { return tr("Lait"); }
    if (nom == "choco") { return tr("Chocolat en poudre"); }
    if (nom == "tea") { return tr("Thé"); }
    if (nom == "egg") { return tr("Oeuf"); }

    return tr("Action indéfinie");
}

QString FenetrePrincipale::getNomPlan(string nom)
{
    if (nom == "teamaking") { return tr("Préparation de thé"); }
    if (nom == "coffemaking") { return tr("Préparation de café"); }
    if (nom == "chocomaking") { return tr("Préparation de chocolat"); }

    return tr("Plan indéfini");
}

QString FenetrePrincipale::getLogo(string nom)
{
    if (nom == "teamaking") { return ":/logos/teamaking.png"; }
    if (nom == "coffemaking") { return ":/logos/coffeemaking.png"; }
    if (nom == "chocomaking") { return ":/logos/chocomaking.png"; }
    if (nom == "teakettle") { return ":/logos/teakettle.png"; }
    if (nom == "water") { return ":/logos/pitcher.png"; }
    if (nom == "mug") { return ":/logos/mug.png"; }
    if (nom == "coffe") { return ":/logos/coffee.png"; }
    if (nom == "pot") { return ":/logos/pot.png"; }
    if (nom == "coffemaker") { return ":/logos/coffemaker.png"; }
    if (nom == "milk") { return ":/logos/milk.png"; }
    if (nom == "choco") { return ":/logos/chocolate.png"; }
    if (nom == "tea") { return ":/logos/tea.png"; }
    if (nom == "egg") { return ":/logos/egg.png"; }

    return ":/logos/question.png";
}

void FenetrePrincipale::ouvrirFenetreParametres()
{
    play = false;
    arretVideo();
    fenetreParametres = new FenetreParametres(this);
    fenetreParametres->exec();
}

void FenetrePrincipale::pleinEcranVideo()
{
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
}

void FenetrePrincipale::lectureVideo()
{
    ui->informations->setHidden(true);
    ui->actionsPrecedentes->setHidden(true);
    ui->activites->setHidden(false);
    ui->feedback->setHidden(false);
    ui->planificateur->setHidden(false);

    reconnaissanceManager = new RecoManager(parametres.getParametres());
    reconnaissanceManager->setIsStopped(true);
    reconnaissanceManager->start_thread();
    ui->application->setText("  Arrêter l'acquisition de la vidéo");
    ui->application->setIcon(QIcon(":/logos/stop.png"));
    ui->application->repaint();
}

void FenetrePrincipale::arretVideo()
{
    if (play) {
        reconnaissanceManager->setIsStopped(false);
        reconnaissanceManager = NULL;
    }

    ui->activites->setHidden(true);
    ui->feedback->setHidden(true);
    ui->planificateur->setHidden(true);
    ui->informations->setHidden(false);

    MiseAJourHistogramme(0);
    MiseAJourProgression("0:00 / 0:00");
    ui->image->setPixmap(QPixmap());
    ui->image->clear();
    ui->application->setText("  Lancer l'acquisition de la vidéo");
    ui->application->setIcon(QIcon(":/logos/play.png"));
    ui->application->repaint();
}
