CONFIG   += c++11
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET   = reconnaissance-plans-activites
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        main.cpp \
        interface/fenetreprincipale.cpp \
        interface/fenetreparametres.cpp \
        interface/image.cpp \
        interface/button.cpp \
        interface/video.cpp \
        interface/label.cpp \
        interface/configuration.cpp

HEADERS += \
        interface/fenetreprincipale.h \
        interface/fenetreparametres.h \
        interface/image.h \
        interface/button.h \
        interface/video.h \
        interface/label.h \
        interface/configuration.h

FORMS   += \
        interface/fenetreprincipale.ui \
        interface/fenetreparametres.ui

DISTFILES +=

RESOURCES += \
        ressources/images.qrc \
        ressources/icones.qrc \
        ressources/fonts.qrc
