#include "../ui/mainwindow.h"

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

void MainWindow::slotUpdate() {
    update();
}
