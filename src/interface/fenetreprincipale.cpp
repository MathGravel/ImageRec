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
    reconnaissanceManager->update();
}

FenetrePrincipale::~FenetrePrincipale()
{
    delete ui;
}

void FenetrePrincipale::configuration()
{
    // Chargement langue
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void FenetrePrincipale::gestionVideo()
{
    this->MiseAJourImage();
}

void FenetrePrincipale::MiseAJourImage()
{
    reconnaissanceManager->update();
    cv::Mat img = reconnaissanceManager->getCurrentFeed();
    cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
    ui->image->setPixmap(QPixmap::fromImage(QImage((unsigned char*) img.data,img.cols,img.rows,QImage::Format_RGB888)));
    ui->image->repaint();
    qApp->processEvents();
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
