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
    lectureVideo();

    while(true) {
        reconnaissanceManager->update();
        cv::Mat img = reconnaissanceManager->getCurrentFeed();
        cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
        ui->image->setPixmap(QPixmap::fromImage(QImage((unsigned char*) img.data,img.cols,img.rows,QImage::Format_RGB888)));
        ui->image->repaint();
        MiseAJourHistogramme(reconnaissanceManager->getTimePosition());
        MiseAJourProgression(reconnaissanceManager->getTimeStamp());
        qApp->processEvents();
        if (!play) {
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

void FenetrePrincipale::MiseAJourInformations()
{
    // Mise à jour des inforamtions !
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
    reconnaissanceManager = new RecoManager(parametres.getParametres());
    ui->application->setText("  Arrêter l'acquisition de la vidéo");
    ui->application->setIcon(QIcon(":/logos/stop.png"));
    ui->application->repaint();
}

void FenetrePrincipale::arretVideo()
{
    reconnaissanceManager = NULL;
    MiseAJourHistogramme(0);
    MiseAJourProgression("0:00 / 0:00");
    ui->image->setPixmap(QPixmap());
    ui->image->clear();
    ui->application->setText("  Lancer l'acquisition de la vidéo");
    ui->application->setIcon(QIcon(":/logos/play.png"));
    ui->application->repaint();
}
