#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Random Points in Circle");
    setFixedSize(800, 600);

    menuWidget = new QWidget(this);
    algorithmWidget = new QWidget(this);
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(menuWidget);
    stackedWidget->addWidget(algorithmWidget);

    setCentralWidget(stackedWidget);

    checkAlgorithmButton = new QPushButton("Проверка Алгоритма A*", menuWidget);
    checkAlgorithmButton->setFixedSize(180, 40);
    connect(checkAlgorithmButton, &QPushButton::clicked, this, &MainWindow::onCheckAlgorithmClicked);

    checkMinCostButton = new QPushButton("Минимальная стоимость", menuWidget);
    checkMinCostButton->setFixedSize(180, 40);
    connect(checkMinCostButton, &QPushButton::clicked, this, &MainWindow::onCheckMinCostClicked);

    QVBoxLayout* menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->addWidget(checkAlgorithmButton);
    menuLayout->setAlignment(checkAlgorithmButton, Qt::AlignCenter);
    menuLayout->addWidget(checkMinCostButton);
    menuLayout->setAlignment(checkMinCostButton, Qt::AlignCenter);
    menuWidget->setLayout(menuLayout);

    startButton = new QPushButton("Начать расчёт", algorithmWidget);
    resetButton = new QPushButton("Сбросить", algorithmWidget);
    backButton = new QPushButton("Вернуться назад", algorithmWidget);

    startButton->setEnabled(false);
    startButton->setFixedSize(100, 40);
    resetButton->setFixedSize(100, 40);
    backButton->setFixedSize(100, 40);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startCalculation);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetCalculation);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);

    QVBoxLayout* algorithmLayout = new QVBoxLayout(algorithmWidget);
    algorithmLayout->addWidget(startButton);
    algorithmLayout->addWidget(resetButton);
    algorithmLayout->addWidget(backButton);
    algorithmWidget->setLayout(algorithmLayout);
}

MainWindow::~MainWindow() {
    for (auto point : points) {
        delete point;
    }
    points.clear();
}
