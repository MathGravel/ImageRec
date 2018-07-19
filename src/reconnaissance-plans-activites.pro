QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET   = reconnaissance-plans-activites
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        fenetreprincipale.cpp \
        main.cpp \
    image.cpp \
    frame.cpp

HEADERS += \
        fenetreprincipale.h \
    image.h \
    frame.h

FORMS   += \
        fenetreprincipale.ui

DISTFILES +=

RESOURCES += \
    images.qrc \
    icones.qrc \
    fonts.qrc
