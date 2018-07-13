QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET   = reconnaissance-plans-activites
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        src/fenetreprincipale.cpp \
    src/main.cpp

HEADERS += \
        src/fenetreprincipale.h

FORMS   += \
        src/fenetreprincipale.ui
