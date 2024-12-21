#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QVector>
#include <QMouseEvent>
#include <QPushButton>
#include <QColor>
#include <QLineF>
#include <QStackedWidget>
#include <QWidget>
#include <QSet>
#include <QMap>
#include <QPainter>
#include <QRandomGenerator>
#include <cmath>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // Конструктор и Деструктор
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // Перечисление типов алгоритмов
    enum AlgorithmType {
        None,                                           // Нет выбранного алгоритма
        AlgorithmA,                                     // Алгоритм A
        MinCost                                         // Алгоритм минимальных затрат
    };

    // Структура для хранения информации о точке
    struct Point {
        QPointF position;                               // Позиция точки
        QString name;                                   // Имя точки
        QVector<Point*> neighbors;                      // Соседи точки
        bool isSelected = false;                        // Флаг выбора точки
        bool DotsNotIncluded = false;                   // Флаг, исключающий точки из алгоритмов
    };

    // Графические компоненты
    QPushButton* checkAlgorithmButton;
    QPushButton* checkMinCostButton;
    QPushButton* startButton;
    QPushButton* resetButton;
    QPushButton* backButton;

    QStackedWidget* stackedWidget;
    QWidget* menuWidget;
    QWidget* algorithmWidget;

    // Данные для работы
    QVector<Point*> points;                             // Все точки
    QVector<Point*> selectedPoints;                     // Выбранные точки
    QVector<QLineF> linesToDraw;                        // Линии для отрисовки
    QVector<QLineF> returnPathLinesToDraw;              // Линии для возвращающего пути

    QSet<Point*> visitedPoints;                         // Множество посещенных точек

    qreal calculatedCost = 0;                           // Стоимость пути
    qreal returnCost = 0;                               // Стоимость обратного пути

    Point* startPoint = nullptr;                        // Стартовая точка
    Point* currentPoint = nullptr;                      // Текущая точка
    Point* endPoint = nullptr;                          // Конечная точка

    AlgorithmType currentAlgorithm = None;              // Текущий выбранный алгоритм

    bool allowRightClickReset = true;                   // Разрешение сброса через правый клик
    bool drawGrayLines = false;                         // Флаг отрисовки серых линий
    int dotClick = 0;                                   // Счетчик кликов по точке

    // Основные методы
    void generateRandomPoints();                        // Генерация случайных точек
    void connectPoints();                               // Соединение точек
    void paintEvent(QPaintEvent* event) override;       // Отрисовка событий
    void mousePressEvent(QMouseEvent* event) override;  // Обработка нажатий мыши

    // Управление алгоритмами
    void startCalculationAlgorithm();                   // Запуск расчета для выбранного алгоритма
    void startCalculationMinCost();                     // Запуск расчета для алгоритма минимальных затрат

    // Управление мышью
    void startCalculation();                            // Начало расчета
    void resetCalculation();                            // Сброс расчета
    void onCheckAlgorithmClicked();                     // Обработка нажатия кнопки для алгоритма
    void onCheckMinCostClicked();                       // Обработка нажатия кнопки для минимальных затрат
    void onBackClicked();                               // Обработка нажатия кнопки "Назад"

    // Вспомогательные методы
    bool doIntersect(const QPointF& p1,
                     const QPointF& q1,
                     const QPointF& p2,
                     const QPointF& q2);                // Проверка пересечения отрезков
    void slotUpdate();                                  // Обновление состояния
};

#endif // MAINWINDOW_H
