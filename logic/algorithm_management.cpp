#include "../ui/mainwindow.h"

void MainWindow::startCalculationAlgorithm() {
    if (selectedPoints.size() != dotClick) {
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

    Node* startNode = new Node{ startPoint, nullptr, 0, heuristic(startPoint, endPoint) };
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
                Node* neighborNode = new Node{ neighbor, currentNode, tentativeGCost, heuristic(neighbor, endPoint) };
                openList.append(neighborNode);
            }
            else if (tentativeGCost < (*it)->gCost) {
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
                else {neighbor->DotsNotIncluded = true;}
            }
        }
    }

    linesToDraw = pathLines;
    calculatedCost = totalCost;

    update();
}

void MainWindow::startCalculationMinCost() {
    if (selectedPoints.size() != dotClick) {
        return;
    }

    Point* startPoint = selectedPoints[0];
    QVector<Point*> visited;
    QVector<QLineF> pathLines;
    qreal totalCost = 0;
    QVector<Point*> reverseVisited;

    struct PathStep {
        Point* point;
        Point* previousPoint;
        qreal cost;
    };

    QVector<PathStep> pathStack;

    Point* currentPoint = startPoint;
    visited.append(currentPoint);

    qDebug() << "Start Point:" << currentPoint->name;

    while (visited.size() < points.size()) {
        qreal minCost = std::numeric_limits<qreal>::max();
        Point* nextPoint = nullptr;

        for (Point* neighbor : currentPoint->neighbors) {
            if (!visited.contains(neighbor)) {
                qreal cost = QLineF(currentPoint->position, neighbor->position).length();
                if (cost < minCost) {
                    minCost = cost;
                    nextPoint = neighbor;
                }
            }
        }

        if (nextPoint) {
            qDebug() << "Connecting:" << currentPoint->name << "->" << nextPoint->name << " Cost:" << minCost;
            pathLines.append(QLineF(currentPoint->position, nextPoint->position));
            totalCost += minCost;
            pathStack.append({currentPoint, nextPoint, minCost});
            visited.append(nextPoint);
            currentPoint = nextPoint;
        } else {
            if (pathStack.isEmpty()) {
                qDebug() << "No more paths available. Exiting.";
                break;
            }

            PathStep lastStep = pathStack.takeLast();
            currentPoint = lastStep.point;
            totalCost += lastStep.cost;
        }
    }

    endPoint = currentPoint;

    qreal returnCostTotal = 0;
    while (!pathStack.isEmpty()) {
        PathStep step = pathStack.takeLast();
        reverseVisited.append(step.point);
        returnCostTotal += step.cost;
    }

    returnCost = returnCostTotal;

    visitedPoints.clear();
    for (Point* point : reverseVisited) {
        visitedPoints.insert(point);
    }

    linesToDraw = pathLines;
    calculatedCost = totalCost;

    qDebug() << "Total Cost:" << totalCost;
    qDebug() << "Return Cost:" << returnCost;

    update();
}
