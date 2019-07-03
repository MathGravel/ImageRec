#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"
#include "NotifyMessageBox.h"

const int ACTUAL_INFO=9;

FenetrePrincipale::FenetrePrincipale(QWidget *parent) : QMainWindow(parent), ui(new Ui::FenetrePrincipale)
{
    ui->setupUi(this);

    QObject::connect(ui->application, SIGNAL(clicked()), this, SLOT(gestionVideo()));
    QObject::connect(ui->parametres, SIGNAL(clicked()), this, SLOT(ouvrirFenetreParametres()));
    QObject::connect(ui->pleinEcran, SIGNAL(clicked()), this, SLOT(pleinEcranVideo()));
    this->reconnaissanceManager = NULL;

}

FenetrePrincipale::~FenetrePrincipale()
{
    delete ui;
}

void FenetrePrincipale::closeEvent(QCloseEvent *event)
{
    play = false;
    event->accept();

#ifdef USE_KITCHEN
    //this->reconnaissanceManager->reset();
#endif
    if (reconnaissanceManager != NULL) {
        delete reconnaissanceManager;
        reconnaissanceManager = NULL;
    }
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
    }
}

void FenetrePrincipale::MiseAJourImage()
{
    lectureVideo();
    int frame= 0;
    while(true) {
        qApp->processEvents();

        if(reconnaissanceManager == NULL)
            break;
        clock_t b = clock();
        reconnaissanceManager->update();
        clock_t e = clock();
#ifndef USE_KITCHEN_DIST
        //std::cout << to_string(((e - b)/(double)CLOCKS_PER_SEC)*1000.0 ) << std::endl;
#endif
#if USE_KITCHEN
        reconnaissanceManager->trace->addFrameCount(((e - b)/(double)CLOCKS_PER_SEC)*1000.0);
#endif
        cv::Mat img = reconnaissanceManager->getCurrentFeed();
        deltaTime += e - b;
        cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
        ui->image->setPixmap(QPixmap::fromImage(QImage((unsigned char*) img.data,img.cols,img.rows,QImage::Format_RGB888)));
        ui->image->repaint();
        MiseAJourHistogramme(reconnaissanceManager->getTimePosition());
        MiseAJourProgression(reconnaissanceManager->getTimeStamp());
 /*      if(frame<ACTUAL_INFO)
        {
            MiseAJourInformations(reconnaissanceManager->getInformations());
           frame=0;
        }
        else {
            frame++;
        }
*/
        /*if (reconnaissanceManager->getTimePosition() == 100)
            play = false;*/
MiseAJourInformations(reconnaissanceManager->getInformations());
        if ((!play) ) {
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
        ui->actionsPrecedentesTitre->setText(tr("Actions précédentes"));
        ui->actionsPrecedentes1->setHidden(false);
        ui->actionPrecedente1Label->setText(getNomAction(informations["actionPrecedente1"]["nom"]));
        ui->actionPrecedente1Logo->setPixmap(QPixmap(getLogo(informations["actionPrecedente1"]["nom"])));
        ui->actionPrecedente1Pourcentage->setText(QString::fromStdString(informations["actionPrecedente1"]["pourcentage"]) + " %");
    } else {
        ui->actionsPrecedentesTitre->setText(tr("Action précédente"));
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
    QString actionActuelleName = QString::fromStdString(informations["actionActuelle"]["nom"]);
    //std::cout<<"2 : : "<<actionActuelleName<<std::endl;
    ui->actionActuelle1Label->setText(actionActuelleName);
    ui->actionActuelle1Logo->setPixmap(QPixmap(getLogo(informations["actionActuelle"]["nom"])));
    ui->actionActuelle1Pourcentage->setText(QString::fromStdString(informations["actionActuelle"]["pourcentage"]) + " %");

    if (informations["planCourant1"]["nom"] != "NA") {
        ui->plansCourants1Label->setText(getNomAction(informations["planCourant1"]["nom"]));
        ui->plansCourants1Logo->setPixmap(QPixmap(getLogo(informations["planCourant1"]["nom"])));
        ui->plansCourants1Pourcentage->setText(QString::fromStdString(informations["planCourant1"]["pourcentage"]) + " %");
    } else {
        ui->plansCourants1Pourcentage->setText("100.00 %");
    }

    if (informations["planCourant2"]["nom"] != "NA") {
        ui->plansCourants2->setHidden(false);
        ui->plansCourants2Label->setText(getNomAction(informations["planCourant2"]["nom"]));
        ui->plansCourants2Logo->setPixmap(QPixmap(getLogo(informations["planCourant2"]["nom"])));
        ui->plansCourants2Pourcentage->setText(QString::fromStdString(informations["planCourant2"]["pourcentage"]) + " %");
    } else {
        ui->plansCourants2->setHidden(true);
    }

    if (informations["planCourant3"]["nom"] != "NA") {
        ui->plansCourants3->setHidden(false);
        ui->plansCourants3Label->setText(getNomAction(informations["planCourant3"]["nom"]));
        ui->plansCourants3Logo->setPixmap(QPixmap(getLogo(informations["planCourant3"]["nom"])));
        ui->plansCourants3Pourcentage->setText(QString::fromStdString(informations["planCourant3"]["pourcentage"]) + " %");
    } else {
        ui->plansCourants3->setHidden(true);
    }

    if (informations["planCourant3"]["nom"] + informations["planCourant2"]["nom"] == "NANA") {
        ui->plansCourantsTitre->setText(tr("Plan courant"));
    } else {
        ui->plansCourantsTitre->setText(tr("Plans courants"));
    }

    if (informations["actionSuivante1"]["nom"] != "NA") {
        ui->actionsSuivantes1Pourcentage->setHidden(false);
        ui->actionsSuivantes1Label->setText(getNomAction(informations["actionSuivante1"]["nom"]));
        ui->actionsSuivantes1Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante1"]["nom"])));
        ui->actionsSuivantes1Pourcentage->setText(QString::fromStdString(informations["actionSuivante1"]["pourcentage"]) + " %");
    }

    if (informations["actionSuivante2"]["nom"] != "NA") {
        ui->actionsSuivantes2->setHidden(false);
        ui->actionsSuivantes2Label->setText(getNomAction(informations["actionSuivante2"]["nom"]));
        ui->actionsSuivantes2Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante2"]["nom"])));
        ui->actionsSuivantes2Pourcentage->setText(QString::fromStdString(informations["actionSuivante2"]["pourcentage"]) + " %");
    } else {
        ui->actionsSuivantes2->setHidden(true);
    }

    if (informations["actionSuivante3"]["nom"] != "NA") {
        ui->actionsSuivantes3->setHidden(false);
        ui->actionsSuivantes3Label->setText(getNomAction(informations["actionSuivante3"]["nom"]));
        ui->actionsSuivantes3Logo->setPixmap(QPixmap(getLogo(informations["actionSuivante3"]["nom"])));
        ui->actionsSuivantes3Pourcentage->setText(QString::fromStdString(informations["actionSuivante3"]["pourcentage"]) + " %");
    } else {
        ui->actionsSuivantes3->setHidden(true);
    }

    if (informations["actionSuivante2"]["nom"] + informations["actionSuivante3"]["nom"] == "NANA") {
        if (informations["planCourant3"]["nom"] + informations["planCourant2"]["nom"] == "NANA") {
            ui->actionsSuivantes1Pourcentage->setHidden(true);
            ui->actionsSuivantes1Logo->setPixmap(QPixmap(":/logos/check.png"));
            ui->actionsSuivantes1Label->setText(tr("Aucune action attendue"));

        }
        ui->actionsSuivantesTitre->setText(tr("Action suivante"));
    } else {
        ui->actionsSuivantesTitre->setText(tr("Actions suivantes"));
    }
    if(informations["errorPlan"]["erreur"]!="Fine"){
        NotifyMessageBox::showMessage(QString::fromStdString(informations["errorPlan"]["erreur"]),
                  QFont("Segoe UI", 12),
                  1500, // time interval to destroy after
                  this);
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
    if (nom == "coffeemaking") { return tr("Préparation de café"); }
    if (nom == "chocomaking") { return tr("Préparation de chocolat"); }
    if (nom == "teakettle") { return tr("Bouilloire"); }
    if (nom == "water") { return tr("Pichet d'eau"); }
    if (nom == "mug") { return tr("Tasse"); }
    if (nom == "coffee") { return tr("Café"); }
    if (nom == "pot") { return tr("Pot"); }
    if (nom == "coffeemaker") { return tr("Machine à café"); }
    if (nom == "milk") { return tr("Lait"); }
    if (nom == "choco") { return tr("Chocolat en poudre"); }
    if (nom == "tea") { return tr("Thé"); }
    if (nom == "egg") { return tr("Oeuf"); }
    if (nom == "red") { return tr("Rouge"); }
    if (nom == "blue") { return tr("Bleu"); }
    if (nom == "yellow") { return tr("Jaune"); }
    if (nom == "green") { return tr("Vert"); }
    if (nom == "orange") { return tr("Orange"); }
    if (nom == "redA") { return tr("Rouge-2"); }
    if (nom == "blueA") { return tr("Bleu-2"); }
    if (nom == "yellowA") { return tr("Jaune-2"); }
    if (nom == "greenA") { return tr("Vert-2"); }
    if (nom == "orangeA") { return tr("Orange-2"); }

    return tr("Action indéfinie");
}

QString FenetrePrincipale::getNomPlan(string nom)
{
    if (nom == "teamaking") { return tr("Préparation de thé"); }
    if (nom == "coffeemaking") { return tr("Préparation de café"); }
    if (nom == "chocomaking") { return tr("Préparation de chocolat"); }
    if (nom == "GoalA") { return tr("Plan A"); }
    if (nom == "GoalB") { return tr("Plan B"); }
    if (nom == "GoalC") { return tr("Plan C"); }
    if (nom == "GoalD") { return tr("Plan D"); }
    if (nom == "GoalE") { return tr("Plan E"); }


    return tr("Plan indéfini");
}

QString FenetrePrincipale::getLogo(string nom)
{
    if (nom == "teamaking") { return ":/logos/teamaking.png"; }
    if (nom == "coffeemaking") { return ":/logos/coffeemaking.png"; }
    if (nom == "chocomaking") { return ":/logos/chocomaking.png"; }
    if (nom == "teakettle") { return ":/logos/teakettle.png"; }
    if (nom == "water") { return ":/logos/pitcher.png"; }
    if (nom == "mug") { return ":/logos/mug.png"; }
    if (nom == "coffee") { return ":/logos/coffee.png"; }
    if (nom == "pot") { return ":/logos/pot.png"; }
    if (nom == "coffeemaker") { return ":/logos/coffemaker.png"; }
    if (nom == "milk") { return ":/logos/milk.png"; }
    if (nom == "choco") { return ":/logos/chocolate.png"; }
    if (nom == "tea") { return ":/logos/tea.png"; }
    if (nom == "egg") { return ":/logos/egg.errorPlan_noEndpng"; }

    return ":/logos/question.png";
}

void FenetrePrincipale::ouvrirFenetreParametres()
{
    play = false;
    //arretVideo();
    fenetreParametres = new FenetreParametres(this);
    fenetreParametres->exec();
    this->loadLanguage(QString::fromStdString(parametres.getParametre("langue")));
}

void FenetrePrincipale::loadLanguage(const QString &rLanguage) {


    if (rLanguage.toStdString() == "Français" || rLanguage.toStdString() == "French" ) {
        qApp->removeTranslator(&m_translator);
    } else {
        QString path = QApplication::applicationDirPath();
        path.append("/lang_en");
        m_translator.load(path);
        qApp->installTranslator(&m_translator);
    }
    ui->retranslateUi(this);

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
    beginFrame = clock();
    ui->informations->setHidden(true);
    ui->actionsPrecedentes->setHidden(true);
    ui->activites->setHidden(false);
    ui->feedback->setHidden(false);
    ui->planificateur->setHidden(false);

    reconnaissanceManager = new RecoManager(parametres.getParametres());
    reconnaissanceManager->setIsStopped(true);
    reconnaissanceManager->start_thread();
    ui->application->setText(tr("  Arrêter l'acquisition de la vidéo"));
    ui->application->setIcon(QIcon(":/logos/stop.png"));
    ui->application->repaint();

}

double clockToMilliseconds(clock_t ticks){
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return (ticks/(double)CLOCKS_PER_SEC)*1000.0;
}

void FenetrePrincipale::arretVideo()
{
    endFrame = clock();
    if (reconnaissanceManager != NULL) {
     reconnaissanceManager->setIsStopped(false);
#if USE_KITCHEN
     this->reconnaissanceManager->trace->addTotalCount(beginFrame,endFrame);
#endif
     delete reconnaissanceManager;
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
    ui->application->setText(tr("  Lancer l'acquisition de la vidéo"));
    ui->application->setIcon(QIcon(":/logos/play.png"));
    ui->application->repaint();
}
