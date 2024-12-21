#include "../ui/mainwindow.h"

void MainWindow::generateRandomPoints() {
    points.clear();

    int radius = std::min(width(), height()) / 2 - 10;
    QPoint center(width() / 2, height() / 2);

    int attempts = 0;
    int pointIndex = 0;
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
            points.push_back(new Point{ pos, QString("Point %1").arg(pointIndex++) });
        }

        attempts++;
    }

    connectPoints();
}

void MainWindow::connectPoints() {
    for (auto point : points) {
        QVector<std::pair<qreal, Point*>> distances;

        for (auto other : points) {
            if (point != other) {
                qreal distance = QLineF(point->position, other->position).length();
                distances.append({ distance, other });
            }
        }

        std::sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        int numNeighbors = QRandomGenerator::global()->bounded(2, 7);

        for (int i = 0; i < numNeighbors; ++i) {
            Point* neighbor = distances[i].second;

            if (!point->neighbors.contains(neighbor)) {
                point->neighbors.append(neighbor);
            }

            if (!neighbor->neighbors.contains(point)) {
                neighbor->neighbors.append(point);
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!points.isEmpty()) {

        QPoint center(width() / 2, height() / 2);
        int radius = std::min(width(), height()) / 2 - 10;

        painter.setPen(QPen(Qt::black));
        painter.drawEllipse(center, radius, radius);

        int pointRadius = 5;

        QPen linePen(drawGrayLines ? Qt::gray : Qt::black, 1);
        painter.setPen(linePen);

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

        for (int i = 0; i < points.size(); ++i) {
            const auto& point = points[i];

            if (point == startPoint) {
                painter.setBrush(Qt::green);
            }
            else if (point == endPoint) {
                painter.setBrush(Qt::green);
            }
            else if (point->isSelected) {
                painter.setBrush(Qt::green);
            }
            else if (point->DotsNotIncluded) {
                painter.setBrush(Qt::black);
            }
            else if (visitedPoints.contains(point)) {
                painter.setBrush(Qt::yellow);
            }
            else {
                painter.setBrush(Qt::red);
            }

            painter.setPen(QPen(Qt::blue, 2));
            painter.drawEllipse(point->position, pointRadius, pointRadius);

            painter.setPen(QPen(Qt::black));
            painter.drawText(point->position + QPointF(10, -10), QString::number(i));
        }

        if (calculatedCost > 0) {
            QString totalCostText = QString("Расчётная цена всего пути: %1 RUB").arg(calculatedCost, 0, 'f', 2);
            painter.setPen(QPen(Qt::black));
            painter.drawText(rect().adjusted(10, 10, -10, -10), Qt::AlignRight | Qt::AlignTop, totalCostText);
        }

        if (returnCost > 0) {
            QString returnCostText = QString("Расчётная цена обратного пути: %1 RUB").arg(returnCost, 0, 'f', 2);
            painter.setPen(QPen(Qt::black));
            painter.drawText(rect().adjusted(10, 30, -10, -10), Qt::AlignRight | Qt::AlignTop, returnCostText);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();

        if (QLineF(clickPos, QPoint(width() / 2, height() / 2)).length() <= std::min(width(), height()) / 2) {
            for (auto& point : points) {
                if (QLineF(clickPos, point->position).length() <= 10) {
                    if (selectedPoints.size() < dotClick) {
                        point->isSelected = true;
                        selectedPoints.append(point);

                        if (selectedPoints.size() == dotClick) {
                            startButton->setEnabled(true);
                        }
                    }
                    break;
                }
            }
        }
        update();
    }
    else if (event->button() == Qt::RightButton) {
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
