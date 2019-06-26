/**
 * \file      fenetreprincipale.h
 * \author    Mathieu Gravel
 * \version   1.0
 * \date      13 June 2019
 * \brief     Create the main interface
 * \details   Positions every layout of main interface and call other others functions
 */

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
        
		/**
		* \fn      explicit FenetrePrincipale(QWidget *parent = 0)
		* \brief   Constructor of class FenetrePrincipale
		* \details Create the window according to file : fenetreprincipale.ui
		* 		   Associating button application, parameters, pleinEcran to functions
		* \param   parent	the parent's adress of this program
		*/
        explicit FenetrePrincipale(QWidget *parent = 0);
		
		/**
		* \fn ~FenetrePrincipale()
		* \brief Destructor of class FenetrePrincipale
		*/
        ~FenetrePrincipale();
		
		/**
		* \fn configuration()
		* \brief Configure layout of the main window
		* \details Positionning layouts and hiding some                  
		*/
        void configuration();
		
		/**
		* \fn lectureVideo()
		* \brief  
		* \details    
		*/
        virtual void lectureVideo();
		
		/**
		* \fn arretVideo()
		* \brief  
		* \details    
		*/
        virtual void arretVideo();
		
		// Getters
        bool getPlay() {return play;}
        QString getNomAction(string nom);
        QString getNomPlan(string nom);
        QString getLogo(string nom);

    protected:
        /**
		* \fn arretVideo()
		* \brief  
		* \details                     
		* \param   
		*/
        void closeEvent(QCloseEvent *event) override;


    public slots:
	
		/**
		* \fn gestionVideo()
		* \brief Start or Stop to capture video
		* \details if video capture is on, stop it (stop the loop in \a MiseAJourImage)
		*	  	   else start it (start the loop in \a MiseAJourImage
		*/
        void gestionVideo();
		
		/**
		* \fn MiseAJourImage()
		* \brief  when video start loop and update image (according to processor speed)
		* \details 
		*/
        void MiseAJourImage();
		
		/**
		* \fn MiseAJourHistogramme()
		* \brief  
		* \details             
		* \param   
		*/
        void MiseAJourHistogramme(int position);
		
		/**
		* \fn MiseAJourProgression()
		* \brief  
		* \details  
		* \param
		*/
        void MiseAJourProgression(string timer);
		
		/**
		* \fn MiseAJourInformations()
		* \brief  
		* \details  
		* \param
		*/
        void MiseAJourInformations(std::map<std::string, std::map<std::string, std::string>> informations);
		
		/**
		* \fn ouvrirFenetreParametres()
		* \brief  
		* \details
		*/
        void ouvrirFenetreParametres();
		
		/**
		* \fn pleinEcranVideo()
		* \brief  
		* \details
		*/
        void pleinEcranVideo();

    private:
	
		/**
		* \fn loadLanguage()
		* \brief  
		* \details           
		* \param
		*/
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
