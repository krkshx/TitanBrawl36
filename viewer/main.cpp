// titan_viewer entry point.

#include "HomeViewer.hpp"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    HomeViewer w;
    w.show();
    return app.exec();
}
