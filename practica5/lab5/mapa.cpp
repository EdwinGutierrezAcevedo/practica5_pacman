#include "mapa.h"
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>
#include <QDebug>

Mapa::Mapa(QGraphicsItem* parent) :
    QGraphicsItem(parent),
    m_ancho(0),
    m_alto(0)
{

}

Mapa::~Mapa() {
    limpiarMapa();
}

bool Mapa::cargarDesdeArchivo(const QString& nombreArchivo) {
    limpiarMapa();

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error al abrir el archivo del mapa:" << nombreArchivo;
        return false;
    }

    QTextStream in(&archivo);
    QStringList lineas;

    // Primera pasada: calcular dimensiones
    while (!in.atEnd()) {
        QString linea = in.readLine();
        lineas.append(linea);
        m_ancho = qMax(m_ancho, linea.length());
    }
    m_alto = lineas.size();

    // Segunda pasada: crear objetos
    for (int fila = 0; fila < lineas.size(); ++fila) {
        const QString& linea = lineas[fila];
        for (int col = 0; col < linea.length(); ++col) {
            QChar c = linea[col];
            int x = col * m_tamanioCelda;
            int y = fila * m_tamanioCelda;

            switch (c.toLatin1()) {
            case '#':
                crearPared(fila, col);
                break;
            case '.':
                crearPunto(fila, col);
                break;
            case 'P':
                m_posicionInicial = QPoint(x, y);
                break;
            case 'R': // Fantasma Rojo
                m_posicionRojo = QPoint(x, y);
                break;
            case 'B': // Fantasma Azul
                m_posicionAzul = QPoint(x, y);
                break;
            case 'S': // Fantasma Rosa (Pink)
                m_posicionRosa = QPoint(x, y);
                break;
            case 'O': // Fantasma Naranja (Yellow/Clyde)
                m_posicionNaranja = QPoint(x, y);
                break;
            default:
                break; // Espacios vacíos
            }
        }
    }
    return true;
}

void Mapa::crearPared(int fila, int columna) {
    auto* pared = new QGraphicsRectItem(0, 0, m_tamanioCelda, m_tamanioCelda, this);
    pared->setPos(columna * m_tamanioCelda, fila * m_tamanioCelda);
    pared->setBrush(Qt::blue);
    pared->setPen(Qt::NoPen);
    m_paredes.append(pared);
}

void Mapa::crearPunto(int fila, int columna) {
    auto* punto = new QGraphicsEllipseItem(10, 10, 10, 10, this);
    punto->setPos(columna * m_tamanioCelda + 5, fila * m_tamanioCelda + 5);
    punto->setBrush(Qt::darkYellow);
    punto->setPen(Qt::NoPen);
    m_puntos.append(punto);
}

bool Mapa::esPared(int x, int y) const {
    // Convertir coordenadas de píxeles a celdas
    int col = x / m_tamanioCelda;
    int fila = y / m_tamanioCelda;

    // Verificar límites del mapa
    if (fila < 0 || fila >= m_alto || col < 0 || col >= m_ancho) {
        return true; // Fuera de límites = pared
    }

    // Buscar pared en la posición calculada
    for (const auto& pared : m_paredes) {
        if (pared->pos() == QPointF(col * m_tamanioCelda, fila * m_tamanioCelda)) {
            return true;
        }
    }
    return false;
}

bool Mapa::todosPuntosConsumidos() const {
    // Verifica si quedan puntos en el vector
    return m_puntos.empty();
}

QRectF Mapa::boundingRect() const {
    return QRectF(0, 0, m_ancho * m_tamanioCelda, m_alto * m_tamanioCelda);
}

void Mapa::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Mapa::limpiarMapa() {
    // Eliminar paredes
    for (auto* pared : m_paredes) {
        if (pared && scene()) { // Verificar que el item y la escena existan
            scene()->removeItem(pared);
        }
        delete pared;
    }
    m_paredes.clear();

    // Eliminar puntos
    for (auto* punto : m_puntos) {
        if (punto && scene()) {
            scene()->removeItem(punto);
        }
        delete punto;
    }
    m_puntos.clear();
}

QPoint Mapa::obtenerPosicionInicial() const {
    return m_posicionInicial;
}

bool Mapa::eliminarPunto(int x, int y) {
    for (int i = 0; i < m_puntos.size(); ++i) {
        QPointF pos = m_puntos[i]->pos();
        int puntoX = pos.x() / m_tamanioCelda;
        int puntoY = pos.y() / m_tamanioCelda;

        if (puntoX == x && puntoY == y) {
            delete m_puntos[i];
            m_puntos.remove(i);
            return true;
        }
    }
    return false;
}
