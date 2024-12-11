#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QVector>
#include <QMouseEvent>
#include <QPushButton>
#include <QColor>
#include <QLineF>

/**
 * @class MainWindow
 * @brief Основное окно приложения для работы с точками и расчётом пути.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Конструктор основного окна.
     * @param parent Родительский виджет.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор основного окна.
     */
    ~MainWindow();

private:
    /**
     * @brief Генерирует случайные точки внутри окружности.
     */
    void generateRandomPoints();

    /**
     * @brief Подключает точки друг к другу, формируя связи.
     */
    void connectPoints();

    /**
     * @brief Отрисовывает элементы окна.
     * @param event Событие перерисовки.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Обновляет интерфейс (перерисовка окна).
     */
    void slotUpdate();

    /**
     * @brief Обрабатывает нажатия кнопок мыши.
     * @param event Событие мыши.
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief Начинает расчёт пути между двумя выбранными точками.
     */
    void startCalculation();

    /**
     * @brief Сбрасывает текущее состояние (выбор точек, расчёт пути).
     */
    void resetCalculation();

    /**
     * @brief Проверяет, пересекаются ли два отрезка.
     * @param p1 Первая точка первого отрезка.
     * @param q1 Вторая точка первого отрезка.
     * @param p2 Первая точка второго отрезка.
     * @param q2 Вторая точка второго отрезка.
     * @return true, если отрезки пересекаются; false в противном случае.
     */
    bool doIntersect(const QPointF& p1, const QPointF& q1, const QPointF& p2, const QPointF& q2);

    /**
     * @brief Флаг, разрешающий сброс правой кнопкой мыши.
     *        Отключается после начала расчёта пути.
     */
    bool allowRightClickReset = true;

    /**
     * @struct Point
     * @brief Структура, представляющая точку на экране.
     */
    struct Point {
        QPointF position;              ///< Позиция точки.
        QVector<Point*> neighbors;     ///< Соседи точки.
        bool isSelected = false;       ///< Флаг, указывающий, выбрана ли точка.
    };

    QVector<Point*> points;            ///< Список всех точек.
    QVector<Point*> selectedPoints;    ///< Список выбранных пользователем точек.

    QVector<QLineF> linesToDraw;       ///< Линии, которые нужно отрисовать (результат расчёта).

    qreal calculatedCost = 0;          ///< Общая стоимость пути (расчётная).

    QPushButton* startButton;          ///< Кнопка для запуска расчёта пути.
    QPushButton* resetButton;          ///< Кнопка для сброса состояния.
};

#endif // MAINWINDOW_H
