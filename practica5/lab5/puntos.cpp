#include "puntos.h"
#include <QPainter>
#include <QDebug>

Puntos::Puntos(QGraphicsItem* parent)
    : QObject(), QGraphicsItem(parent) {

}

Puntos::~Puntos() {
    limpiarPuntos();
}


bool Puntos::eliminarPunto(int x, int y) {
    for (int i = 0; i < m_puntos.size(); ++i) {
        if (m_puntos[i].posicion.x() == x && m_puntos[i].posicion.y() == y) {
            delete m_puntos[i].item;
            m_puntos.remove(i);
            return true;
        }
    }
    return false;
}

void Puntos::limpiarPuntos() {
    for (PuntoData& pd : m_puntos) {
        delete pd.item;
    }
    m_puntos.clear();
}

QRectF Puntos::boundingRect() const {
    // Devolver un rectángulo que contenga todos los puntos
    if (m_puntos.isEmpty()) {
        return QRectF();
    }

    QRectF rect;
    for (const PuntoData& pd : m_puntos) {
        rect = rect.united(pd.item->boundingRect().translated(pd.item->pos()));
    }
    return rect;
}


void Puntos::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // Los puntos se dibujan automáticamente como items hijos
}
