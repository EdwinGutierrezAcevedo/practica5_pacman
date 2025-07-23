#include "pacman.h"
#include "mapa.h"
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsView>
#include "mainwindow.h"

Pacman::Pacman() :
    m_dx(0), m_dy(0),
    m_deseadoDx(0), m_deseadoDy(0),
    m_currentFrame(0), m_frameBase(9),
    m_frameSize(30) {


    m_spriteSheet.load("pacman.png");
    setPixmap(m_spriteSheet.copy(m_frameBase * m_frameSize, 0, m_frameSize, m_frameSize));

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Pacman::mover);
    m_timer->start(150);
}

Pacman::~Pacman() {
    m_timer->stop();
    delete m_timer;
}

void Pacman::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_D: // Derecha
        m_deseadoDx = 30;
        m_deseadoDy = 0;
        m_frameBase = 9;
        break;
    case Qt::Key_A: // Izquierda
        m_deseadoDx = -30;
        m_deseadoDy = 0;
        m_frameBase = 6;
        break;
    case Qt::Key_Z: // Abajo
        m_deseadoDx = 0;
        m_deseadoDy = 30;
        m_frameBase = 3;
        break;
    case Qt::Key_S: // Arriba
        m_deseadoDx = 0;
        m_deseadoDy = -30;
        m_frameBase = 0;
        break;
    }
}

void Pacman::mover() {
    // 1. Intentar cambiar dirección si no hay colisión
    int testX = x() + m_deseadoDx;
    int testY = y() + m_deseadoDy;
    if (!chequearColision(testX, testY)) {
        m_dx = m_deseadoDx;
        m_dy = m_deseadoDy;
    }

    // 2. Mover en dirección actual
    int nuevoX = x() + m_dx;
    int nuevoY = y() + m_dy;
    if (!chequearColision(nuevoX, nuevoY)) {
        setPos(nuevoX, nuevoY);
    } else {
        // Si hay colisión, detener movimiento
        m_dx = 0;
        m_dy = 0;
    }

    // 3. Comer puntos
    QList<QGraphicsItem*> itemsColisionados = collidingItems();
    for (QGraphicsItem* item : itemsColisionados) {
        if (item->type() == QGraphicsEllipseItem::Type) { // filtra los elementos con los que choca pacman y solo toma en cuenta los puntos
            if (!scene()->views().isEmpty()) {
                QGraphicsView* view = scene()->views().first();
                if (MainWindow* mainWindow = qobject_cast<MainWindow*>(view->window())) {
                    // Actualizar puntaje
                    mainWindow->actualizarPuntaje(10);

                    // Convertir coordenadas a celdas del mapa
                    int col = item->x() / mainWindow->getMapa()->obtenerTamanioCelda();
                    int fila = item->y() / mainWindow->getMapa()->obtenerTamanioCelda();

                    // Eliminar el punto del modelo (Mapa se encargará de la escena)
                    mainWindow->getMapa()->eliminarPunto(col, fila);

                    // Verificar victoria
                    if (mainWindow->getMapa()->todosPuntosConsumidos()) {
                        mainWindow->mostrarMensaje("¡Ganaste!", Qt::green);
                        m_timer->stop();
                    }
                }
            }
        }
    }

    // 4. Animación
    m_currentFrame = (m_currentFrame + 1) % 3;
    setPixmap(m_spriteSheet.copy((m_frameBase + m_currentFrame) * m_frameSize, 0, m_frameSize, m_frameSize));

    // 5. Teletransporte horizontal con verificación de colisión
    const int m_tamanioCelda = 30;
    qreal spriteWidth = boundingRect().width();
    int col = x() / m_tamanioCelda;

    if (col >= 27 && x() > 27 * m_tamanioCelda + 5) {
        int destinoX = 0 - spriteWidth / 2;
        if (!chequearColision(destinoX, y())) {
            setX(destinoX);
        }
    }

    if (col == 0 && x() < 5) { // cerca del borde izquierdo
        int destinoX = 27 * m_tamanioCelda - spriteWidth / 2;
        if (!chequearColision(destinoX, y())) {
            setX(destinoX);
        }
    }
}

bool Pacman::chequearColision(int x, int y) {

    QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem* item : items) {
        if (Mapa* mapa = dynamic_cast<Mapa*>(item)) {
            return mapa->esPared(x, y);
        }
    }
    return false;
}
