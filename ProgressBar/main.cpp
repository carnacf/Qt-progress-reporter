#include "ProgressDialog.h"
#include "Common/ProgressReporter.h"

#include <chrono>
#include <thread>
#include <iostream>

#include <QApplication>
#include <QMainwindow>
#include <QtWidgets>

#include <omp.h>

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

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            internalHAndler->increment();
        }

        mainHandler->increment();
    }
}

void startComputation()
{
    ProgressDialog dialog;
    auto& handler = dialog.getReporter();
    dialog.exec(dummyFunc, 10, 10, std::ref(handler));
}

void dummyFuncWithOmp(int nbI = 100, int nbJ = 100, const IProgressReporter& reporter = EmptyProgressReporter())
{
    auto mainHandler = reporter.startReport("Dummy", nbI);

#pragma omp parallel for
    for (int i = 0; i < nbI; i++)
    {
        if (reporter.isCanceled())
            break;

        std::string label = std::string("Dummy internal i:") + std::to_string(i);
        auto internalHAndler = reporter.startReport(label, nbJ);
        internalHAndler->setAutoClose(false);
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

void startComputationWithomp()
{
    ProgressDialog dialog;
    auto& handler = dialog.getReporter();
    dialog.exec(dummyFuncWithOmp, 10, 10, std::ref(handler));
}

void startComputationWithQuestion()
{
    ProgressDialog dialog;
    dialog.show();
    auto& reporter = dialog.getReporter();

    auto mainHandler = reporter.startReport("Main proc", 2);
    dialog.exec(dummyFunc, 2, 5, std::ref(reporter));
    mainHandler->increment();

    auto answer = QMessageBox::question(nullptr, "This is a question.", "Some event require you to answer of you want to continue.");
    if (answer != QMessageBox::StandardButton::Yes)
    {
        return;
    }

    dialog.exec(dummyFunc, 2, 5, std::ref(reporter));
    mainHandler->increment();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;
    
    QWidget* mainWidget = new QWidget(&mainWindow);
    mainWindow.setCentralWidget(mainWidget);

    QVBoxLayout* layout = new QVBoxLayout();
    mainWidget->setLayout(layout);

    QPushButton* button1 = new QPushButton("Test");
    QObject::connect(button1, &QPushButton::clicked, &startComputation);
    layout->addWidget(button1);

    QPushButton* button2 = new QPushButton("Test with omp multithreading");
    QObject::connect(button2, &QPushButton::clicked, &startComputationWithomp);
    layout->addWidget(button2);

    QPushButton* button3 = new QPushButton("Test with question");
    QObject::connect(button3, &QPushButton::clicked, &startComputationWithQuestion);
    layout->addWidget(button3);

    mainWindow.show();

    return a.exec();
}
