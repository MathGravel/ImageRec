#include "button.h"

Button::Button(QWidget *parent) : QPushButton(parent) {}

void Button::resizeEvent(QResizeEvent *)
{
    QFont f = font();
    QList<QWidget*> appWidgets = qApp->topLevelWidgets();

    for(int i = 0; i < appWidgets.size(); i++) {
        if(appWidgets.at(i)->objectName() == "FenetrePrincipale") {
            if ((appWidgets.at(i)->width() >= 1250) && (appWidgets.at(i)->height() >= 800)) {
                setIconSize(QSize(20,20));
                f.setPointSize(14);
                setFont(f);
            } else {
                setIconSize(QSize(16,16));
                f.setPointSize(10);
                setFont(f);
            }
            break;
        }
    }
}
