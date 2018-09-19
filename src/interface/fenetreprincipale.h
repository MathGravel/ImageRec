#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include "fenetreparametres.h"
#include <QMainWindow>
#include <QApplication>
#include <QStyle>
#include <QTranslator>
#include <QDesktopWidget>
#include "RecoManager.h"
#include <ctime>

extern Configuration parametres;

namespace Ui {
    class FenetrePrincipale;
}

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT

    public:
        explicit FenetrePrincipale(QWidget *parent = 0);
        ~FenetrePrincipale();
        void configuration();
        virtual void lectureVideo();
        virtual void arretVideo();
        bool getPlay() {return play;}
        QString getNomAction(string nom);
        QString getNomPlan(string nom);
        QString getLogo(string nom);

    protected:
        void closeEvent(QCloseEvent *event) override;


    public slots:
        void gestionVideo();
        void MiseAJourImage();
        void MiseAJourHistogramme(int position);
        void MiseAJourProgression(string timer);
        void MiseAJourInformations(std::map<std::string, std::map<std::string, std::string>> informations);
        void ouvrirFenetreParametres();
        void pleinEcranVideo();

    private:
        void loadLanguage(const QString& rLanguage);

        Ui::FenetrePrincipale *ui;
        FenetreParametres *fenetreParametres;
        RecoManager *reconnaissanceManager;
        bool play = false;

          // creates the language menu dynamically from the content of m_langPath

          QTranslator m_translator; // contains the translations for this application
          QTranslator m_translatorQt; // contains the translations for qt
          QString m_currLang; // contains the currently loaded language
          QString m_langPath; // Path of language files. This is always fixed to /languages.
          clock_t deltaTime = 0;
          unsigned int frames = 0;
          double  frameRate = 30;
          double  averageFrameTimeMilliseconds = 33.333;
          clock_t beginFrame ;
          clock_t endFrame ;

};

#endif // FENETREPRINCIPALE_H
