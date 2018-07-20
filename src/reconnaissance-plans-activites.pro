QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET   = reconnaissance-plans-activites
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        fenetreprincipale.cpp \
        main.cpp \
        image.cpp \
        button.cpp \
    video.cpp \
    label.cpp \
    fenetreparametres.cpp

HEADERS += \
        fenetreprincipale.h \
        image.h \
        button.h \
    video.h \
    label.h \
    fenetreparametres.h

FORMS   += \
        fenetreprincipale.ui \
    fenetreparametres.ui

DISTFILES +=

RESOURCES += \
        images.qrc \
        icones.qrc \
        fonts.qrc
