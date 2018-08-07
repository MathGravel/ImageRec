#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"

FenetrePrincipale::FenetrePrincipale(QWidget *parent) : QMainWindow(parent), ui(new Ui::FenetrePrincipale)
{
    ui->setupUi(this);

    QObject::connect(ui->application, SIGNAL(clicked()), this, SLOT(gestionVideo()));
    QObject::connect(ui->parametres, SIGNAL(clicked()), this, SLOT(ouvrirFenetreParametres()));
    QObject::connect(ui->pleinEcran, SIGNAL(clicked()), this, SLOT(pleinEcranVideo()));

    map <string, string> parametres;
    parametres["affichageActions"] = "Zone, nom et pourcentage";
    parametres["affichageObjets"] = "Zone";
    parametres["affichageMains"] = "Zone et poucentage";
    parametres["precisionTraitement"] = "75 %";
    parametres["Show Name"] = "1";
    parametres["Show Zone"] = "1";
    parametres["Show Percentage"] = "1";
    parametres["langue"] = "Français";
    parametres["sourceType"] = "Local";
    parametres["sourceChemin"] = "/home/baptiste/Vidéos/Boiled.mkv";
    parametres["sourceCheminDepth"] = "/home/baptiste/Vidéos/BoiledDepth.mkv";

    reconnaissanceManager = new RecoManager(parametres);
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
    // Chargement langue
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void FenetrePrincipale::gestionVideo()
{
    play = !play;

    if (play) {
        this->MiseAJourImage();
    }
}

void FenetrePrincipale::MiseAJourImage()
{
    ui->application->setText("  Arrêter l'acquisition de la vidéo");
    ui->application->setIcon(QIcon(":/logos/stop.png"));
    ui->application->repaint();

    while(true) {
        reconnaissanceManager->update();
        cv::Mat img = reconnaissanceManager->getCurrentFeed();
        cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
        ui->image->setPixmap(QPixmap::fromImage(QImage((unsigned char*) img.data,img.cols,img.rows,QImage::Format_RGB888)));
        ui->image->repaint();
        qApp->processEvents();
        if (!play) {
            break;
        }
    }

    ui->image->setPixmap(QPixmap());
    ui->image->clear();
    ui->application->setText("  Lancer l'acquisition de la vidéo");
    ui->application->setIcon(QIcon(":/logos/play.png"));
    ui->application->repaint();
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
