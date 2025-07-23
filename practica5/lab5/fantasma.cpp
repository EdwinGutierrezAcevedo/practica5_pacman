#include "fantasma.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>

Fantasma::Fantasma(QColor color, Mapa* mapa, const QString& spritePath, QObject* parent)
    : QObject(parent), m_mapa(mapa), m_velocidad(3), m_currentFrame(0), m_frameBase(0) {

    m_color = color;

    // Cargar sprite sheet
    if (!m_spriteSheet.load(spritePath)) {
        qDebug() << "Error al cargar sprite sheet:" << spritePath;
        // Crear un gráfico simple como respaldo
        QPixmap pixmap(30, 30);
        pixmap.fill(m_color);
        setPixmap(pixmap);
    } else {
        // Configurar el sprite inicial
        setPixmap(m_spriteSheet.copy(0, 0, m_frameSize, m_frameSize));
    }

    // Dirección inicial aleatoria
    int dirIndex = QRandomGenerator::global()->bounded(4);
    switch (dirIndex) {
    case 0: m_direccion = QPoint(1, 0); break;  // derecha
    case 1: m_direccion = QPoint(-1, 0); break; // izquierda
    case 2: m_direccion = QPoint(0, 1); break;  // abajo
    case 3: m_direccion = QPoint(0, -1); break; // arriba
    }

    // Timer para movimiento
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Fantasma::mover);
    timer->start(200);
}

void Fantasma::mover() {
    // 30% de probabilidad de cambiar dirección
    if (QRandomGenerator::global()->bounded(100) < 30) {
        cambiarDireccion();
    }

    QPoint nuevaPos = pos().toPoint() + m_direccion * m_velocidad;

    if (validarMovimiento(nuevaPos)) {
        setPos(nuevaPos);
        actualizarSprite();
    } else {
        cambiarDireccion();
    }
}

void Fantasma::actualizarSprite() {
    // Determinar la base del frame según la dirección
    if (m_direccion.y() == -1) {
        m_frameBase = 0; // Arriba (primeros 2 frames)
    } else if (m_direccion.y() == 1) {
        m_frameBase = 2; // Abajo (siguientes 2 frames)
    } else if (m_direccion.x() == -1) {
        m_frameBase = 4; // Izquierda (siguientes 2 frames)
    } else if (m_direccion.x() == 1) {
        m_frameBase = 6; // Derecha (últimos 2 frames)
    }

    // Alternar entre los dos frames de la dirección
    m_currentFrame = (m_currentFrame + 1) % 2;
    int frameX = (m_frameBase + m_currentFrame) * m_frameSize;

    setPixmap(m_spriteSheet.copy(frameX, 0, m_frameSize, m_frameSize));
}

void Fantasma::setDireccion(const QPoint& nuevaDireccion) {
    m_direccion = nuevaDireccion;
    actualizarSprite();
}

bool Fantasma::validarMovimiento(QPoint nuevaPosicion) {
    int cellSize = m_mapa->obtenerTamanioCelda();
    int x = nuevaPosicion.x() + cellSize / 2;
    int y = nuevaPosicion.y() + cellSize / 2;
    return !m_mapa->esPared(x, y);
}

void Fantasma::cambiarDireccion() {
    QVector<QPoint> direcciones = {
        QPoint(1, 0), QPoint(-1, 0), QPoint(0, 1), QPoint(0, -1)
};

// Eliminar movimiento inverso
QPoint opuesto = QPoint(-m_direccion.x(), -m_direccion.y());
direcciones.removeAll(opuesto);

// Probar direcciones válidas
for (const QPoint& dir : direcciones) {
    QPoint testPos = pos().toPoint() + dir * m_velocidad;
    if (validarMovimiento(testPos)) {
        setDireccion(dir);
        return;
    }
}
}

QPoint Fantasma::obtenerPosicion() const {
    return pos().toPoint();
}
