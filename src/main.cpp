#include <QApplication>
#include <QFile>
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Connectify");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Team05-BSE2B");

    // Load global QSS stylesheet
    QFile styleFile(":/resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    MainWindow window;
    window.show();

    return app.exec();
}
