#include <QApplication>
#include "MainWindow.h"
#include "WelcomeDialog.h"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    app.setApplicationName("MatrixEdit");
    app.setOrganizationName("MatrixCut");
    app.setStyle("Fusion");

    WelcomeDialog welcome;
    if (welcome.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return app.exec();
    }
    return 0;
}
