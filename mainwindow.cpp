#include "mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <cmath>
#include <QVector>
#include <QTimer>
#include <QLineF>
#include <QPointF>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Random Points in Circle");
    setFixedSize(800, 600);

    startButton = new QPushButton("Начать расчёт", this);
    resetButton = new QPushButton("Сбросить", this);

    startButton->setEnabled(false);

    startButton->setFixedSize(100, 40);
    resetButton->setFixedSize(100, 40);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startCalculation);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetCalculation);

    QWidget* buttonPanel = new QWidget(this);
    QVBoxLayout* buttonLayout = new QVBoxLayout(buttonPanel);

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(resetButton);

    buttonPanel->setLayout(buttonLayout);
    buttonPanel->setFixedWidth(180);

    setCentralWidget(buttonPanel);

    generateRandomPoints();
}

MainWindow::~MainWindow() {
    for (auto point : points) {
        delete point;
    }
    points.clear();
}

void MainWindow::startCalculation() {
    if (selectedPoints.size() != 2) {
        return;
    }

    allowRightClickReset = false;

    Point* startPoint = selectedPoints[0];
    Point* endPoint = selectedPoints[1];

    struct Node {
        Point* point;
        Node* parent;
        qreal gCost;
        qreal hCost;
        qreal fCost() const { return gCost + hCost; }
    };

    auto heuristic = [](const Point* a, const Point* b) -> qreal {
        return QLineF(a->position, b->position).length();
    };

    QVector<Node*> openList;
    QVector<Point*> closedList;

    Node* startNode = new Node{startPoint, nullptr, 0, heuristic(startPoint, endPoint)};
    openList.append(startNode);

    Node* currentNode = nullptr;

    while (!openList.isEmpty()) {
        std::sort(openList.begin(), openList.end(), [](Node* a, Node* b) {
            return a->fCost() < b->fCost();
        });

        currentNode = openList.takeFirst();
        closedList.append(currentNode->point);

        if (currentNode->point == endPoint) {
            break;
        }

        for (Point* neighbor : currentNode->point->neighbors) {
            if (closedList.contains(neighbor)) {
                continue;
            }

            qreal tentativeGCost = currentNode->gCost + QLineF(currentNode->point->position, neighbor->position).length();

            auto it = std::find_if(openList.begin(), openList.end(), [neighbor](Node* node) {
                return node->point == neighbor;
            });

            if (it == openList.end()) {
                Node* neighborNode = new Node{neighbor, currentNode, tentativeGCost, heuristic(neighbor, endPoint)};
                openList.append(neighborNode);
            } else if (tentativeGCost < (*it)->gCost) {
                (*it)->gCost = tentativeGCost;
                (*it)->parent = currentNode;
            }
        }
    }

    if (currentNode->point != endPoint) {
        qDebug() << "Path not found!";
        return;
    }

    QVector<QLineF> pathLines;
    qreal totalCost = 0;

    while (currentNode->parent != nullptr) {
        pathLines.append(QLineF(currentNode->point->position, currentNode->parent->point->position));
        totalCost += QLineF(currentNode->point->position, currentNode->parent->point->position).length() * 10;
        currentNode = currentNode->parent;
    }

    for (auto& point : points) {
        for (auto& neighbor : point->neighbors) {
            neighbor->isSelected = false;
        }
    }

    for (const QLineF& line : pathLines) {
        for (auto& point : points) {
            for (auto& neighbor : point->neighbors) {
                if ((line.p1() == point->position && line.p2() == neighbor->position) ||
                    (line.p2() == point->position && line.p1() == neighbor->position)) {
                    neighbor->isSelected = true;
                }
            }
        }
    }

    linesToDraw = pathLines;
    calculatedCost = totalCost;

    update();
}

void MainWindow::resetCalculation() {
    selectedPoints.clear();
    for (auto& point : points) {
        point->isSelected = false;
    }

    linesToDraw.clear();
    calculatedCost = 0;

    startButton->setEnabled(false);
    allowRightClickReset = true;
    update();
}

void MainWindow::slotUpdate() {
    update();
}

void MainWindow::generateRandomPoints() {
    points.clear();

    int radius = std::min(width(), height()) / 2 - 10;
    QPoint center(width() / 2, height() / 2);

    int attempts = 0;
    while (points.size() < 100 && attempts < 1000) {
        float angle = QRandomGenerator::global()->bounded(2.0 * M_PI);
        float distance = QRandomGenerator::global()->bounded(radius - 10);

        QPointF pos(center.x() + distance * cos(angle), center.y() + distance * sin(angle));

        bool valid = true;
        for (const auto& point : points) {
            if (QLineF(pos, point->position).length() < 20) {
                valid = false;
                break;
            }
        }

        if (valid) {
            points.push_back(new Point{pos});
        }

        attempts++;
    }

    connectPoints();
}

bool MainWindow::doIntersect(const QPointF& p1, const QPointF& q1, const QPointF& p2, const QPointF& q2) {
    auto orientation = [](const QPointF& p, const QPointF& q, const QPointF& r) -> int {
        qreal val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
        if (val == 0) return 0;
        return (val > 0) ? 1 : 2;
    };

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    return (o1 != o2 && o3 != o4);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint center(width() / 2, height() / 2);
    int radius = std::min(width(), height()) / 2 - 10;

    painter.setPen(QPen(Qt::black));
    painter.drawEllipse(center, radius, radius);

    int pointRadius = 5;

    painter.setPen(QPen(Qt::black, 1));
    for (const auto& point : points) {
        for (const auto& neighbor : point->neighbors) {
            QLineF line(point->position, neighbor->position);
            painter.drawLine(line);
        }
    }

    painter.setPen(QPen(Qt::blue, 2));
    for (const auto& line : linesToDraw) {
        painter.drawLine(line);
    }

    for (const auto& point : points) {
        if (point->isSelected) {
            painter.setBrush(Qt::green);
        } else if (point->neighbors.isEmpty()) {
            painter.setBrush(Qt::black);
        } else {
            painter.setBrush(Qt::red);
        }
        painter.drawEllipse(point->position, pointRadius, pointRadius);
    }

    if (calculatedCost > 0) {
        QString costText = QString("Расчётная цена пути: %1 RUB").arg(calculatedCost, 0, 'f', 2);
        painter.setPen(QPen(Qt::black));
        painter.drawText(rect().adjusted(10, 10, -10, -10), Qt::AlignRight | Qt::AlignTop, costText);
    }
}

void MainWindow::connectPoints() {
    for (auto point : points) {
        QVector<std::pair<qreal, Point*>> distances;
        for (auto other : points) {
            if (point != other) {
                qreal distance = QLineF(point->position, other->position).length();
                distances.append({distance, other});
            }
        }

        std::sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        int numNeighbors = QRandomGenerator::global()->bounded(2, 7);

        for (int i = 0; i < numNeighbors; ++i) {
            if (std::find(point->neighbors.begin(), point->neighbors.end(), distances[i].second) == point->neighbors.end()) {
                point->neighbors.append(distances[i].second);
                distances[i].second->neighbors.append(point);
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();

        if (QLineF(clickPos, QPoint(width() / 2, height() / 2)).length() <= std::min(width(), height()) / 2) {
            for (auto& point : points) {
                if (QLineF(clickPos, point->position).length() <= 10) {
                    if (selectedPoints.size() < 2) {
                        point->isSelected = true;
                        selectedPoints.append(point);

                        if (selectedPoints.size() == 2) {
                            startButton->setEnabled(true);
                        }
                    }
                    break;
                }
            }
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        if (allowRightClickReset) {
            selectedPoints.clear();
            for (auto& point : points) {
                point->isSelected = false;
            }
            startButton->setEnabled(false);
            update();
        }
    }
}
