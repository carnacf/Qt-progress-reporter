#include "ProgressDialog.h"
#include "Common/ProgressReporter.h"

#include <chrono>
#include <thread>

#include <QApplication>
#include <QMainwindow>
#include <QtWidgets>

void dummyFunc(int nbI = 100, int nbJ = 100, const IProgressReporter& reporter = EmptyProgressReporter())
{
    auto mainHandler = reporter.startReport("Dummy", nbI);
    for (int i = 0; i < nbI; i++)
    {
        if (reporter.isCanceled())
            break;

        std::string label = std::string("Dummy internal i:") + std::to_string(i);
        auto internalHAndler = reporter.startReport(label, nbJ);
        for (int j = 0; j < nbJ; j++)
        {
            if (reporter.isCanceled())
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            internalHAndler->increment();
        }

        mainHandler->increment();
    }
}

void startComputation()
{
    ProgressDialog dialog;
    auto& handler = dialog.getReporter();
    //dummyFunc(10, 10, handler);
    dialog.exec(dummyFunc, 10, 10, std::ref(handler));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;
    
    QPushButton* button = new QPushButton("Start");
    QObject::connect(button, &QPushButton::clicked, &startComputation);

    mainWindow.setCentralWidget(button);
    mainWindow.show();

    return a.exec();
}
