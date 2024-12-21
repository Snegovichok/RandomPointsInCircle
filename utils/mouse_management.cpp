#include "../ui/mainwindow.h"

void MainWindow::startCalculation() {
    drawGrayLines = true;
    if (currentAlgorithm == AlgorithmA) {
        startCalculationAlgorithm();
    }
    else if (currentAlgorithm == MinCost) {
        startCalculationMinCost();
    }
}

void MainWindow::resetCalculation() {
    drawGrayLines = false;
    selectedPoints.clear();
    for (auto& point : points) {
        point->isSelected = false;
        point->DotsNotIncluded = false;
    }

    linesToDraw.clear();
    calculatedCost = 0;
    returnCost = 0;
    endPoint = nullptr;
    visitedPoints.clear();

    startButton->setEnabled(false);
    allowRightClickReset = true;
    update();
}

void MainWindow::onCheckAlgorithmClicked() {
    currentAlgorithm = AlgorithmA;
    dotClick = 2;
    stackedWidget->setCurrentWidget(algorithmWidget);
    generateRandomPoints();
}

void MainWindow::onCheckMinCostClicked() {
    currentAlgorithm = MinCost;
    dotClick = 1;
    stackedWidget->setCurrentWidget(algorithmWidget);
    generateRandomPoints();
}

void MainWindow::onBackClicked() {
    drawGrayLines = false;
    points.clear();
    linesToDraw.clear();

    resetCalculation();
    stackedWidget->setCurrentWidget(menuWidget);
}
