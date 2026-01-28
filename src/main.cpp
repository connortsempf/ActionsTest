#include <QApplication>
#include <QWidget>


int main(int argc, char *argv[]) {
    QApplication TestApp(argc, argv);
    QWidget window;
    window.setWindowTitle("Test App");
    window.resize(250, 400);
    window.show();
    return TestApp.exec();
}
