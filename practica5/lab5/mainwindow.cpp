#include "mainwindow.h"
#include "mapa.h"
#include "pacman.h"
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_escena(new QGraphicsScene(this)),
    m_mapa(nullptr),
    m_pacman(nullptr),
    m_puntaje(0),
    m_textoPuntaje(new QGraphicsTextItem())
{
    // Configuración inicial
    setWindowTitle("Pacman Qt");
    resize(1000, 800);

    // Configurar vista gráfica
    QGraphicsView* view = new QGraphicsView(m_escena, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setBackgroundBrush(Qt::black);
    setCentralWidget(view);


    // Configurar texto de puntaje
    m_textoPuntaje->setPlainText("Puntaje: 0");
    m_textoPuntaje->setDefaultTextColor(Qt::yellow);
    m_textoPuntaje->setFont(QFont("Arial", 16));
    m_textoPuntaje->setPos(10,0);
    m_textoPuntaje->setZValue(1000);
    m_escena->addItem(m_textoPuntaje);


    cargarNivel("mapa.txt");
    // Timer para detección de colisiones
    m_colisionTimer = new QTimer(this);
    connect(m_colisionTimer, &QTimer::timeout, this, &MainWindow::verificarColisiones);
    m_colisionTimer->start(100); // Verificar cada 100ms
}

MainWindow::~MainWindow() {
    limpiarEscena();
}

void MainWindow::cargarNivel(const QString& nombreArchivo) {
    limpiarEscena();

    m_mapa = new Mapa();
    if (!m_mapa->cargarDesdeArchivo(nombreArchivo)) {
        QMessageBox::critical(this, "Error", "No se pudo cargar el mapa");
        delete m_mapa;
        m_mapa = nullptr;
        return;
    }

    m_escena->addItem(m_mapa);
    m_escena->setSceneRect(m_mapa->boundingRect());

    // Crear Pacman en la posición inicial
    m_pacman = new Pacman();
    m_pacman->setPos(m_mapa->obtenerPosicionInicial());
    m_escena->addItem(m_pacman);
    m_pacman->setFocus();

    // Resetear puntaje
    m_puntaje = 0;
    actualizarPuntaje(0);
    // Crear fantasmas
    if (m_mapa) {
        // Rojo (Blinky)
        Fantasma* rojo = new Fantasma(Qt::red, m_mapa, "fantasma_rojo.png");
        rojo->setPos(m_mapa->obtenerPosicionRojo());
        m_escena->addItem(rojo);
        fantasmas.append(rojo);

        // Azul (Inky)
        Fantasma* azul = new Fantasma(Qt::cyan, m_mapa, "fantasma_azul.png");
        azul->setPos(m_mapa->obtenerPosicionAzul());
        m_escena->addItem(azul);
        fantasmas.append(azul);

        // Rosa (Pinky)
        Fantasma* rosa = new Fantasma(QColor(255, 184, 255), m_mapa, "fantasma_rosa.png");
        rosa->setPos(m_mapa->obtenerPosicionRosa());
        m_escena->addItem(rosa);
        fantasmas.append(rosa);

        // Naranja (Clyde)
        Fantasma* naranja = new Fantasma(QColor(255, 184, 82), m_mapa, "fantasma_naranja.png");
        naranja->setPos(m_mapa->obtenerPosicionNaranja());
        m_escena->addItem(naranja);
        fantasmas.append(naranja);
    }
}

void MainWindow::iniciarJuego() {
    if (!m_mapa) {
        cargarNivel("mapa.txt"); // Ruta por defecto
    }
}

void MainWindow::actualizarPuntaje(int puntos) {
    m_puntaje += puntos;
    m_textoPuntaje->setPlainText("Puntaje: " + QString::number(m_puntaje));

    // Verificar victoria
    if (m_mapa && m_mapa->todosPuntosConsumidos()) {
        QGraphicsTextItem* mensaje = new QGraphicsTextItem("¡Ganaste!");
        mensaje->setDefaultTextColor(Qt::green);
        mensaje->setFont(QFont("Arial", 32, QFont::Bold));
        mensaje->setPos(m_escena->width()/2 - 100, m_escena->height()/2 - 50);
        m_escena->addItem(mensaje);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    //ajustarVista();
}

void MainWindow::ajustarVista() {
    if (m_escena && !m_escena->views().isEmpty()) {
        QGraphicsView* view = m_escena->views().first();
        view->fitInView(m_escena->sceneRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::limpiarEscena() {
    if (m_pacman) {
        m_escena->removeItem(m_pacman);
        delete m_pacman;
        m_pacman = nullptr;
    }

    if (m_mapa) {
        m_escena->removeItem(m_mapa);
        delete m_mapa;
        m_mapa = nullptr;
    }

    m_puntaje = 0;

    // Limpiar fantasmas
    for (Fantasma* fantasma : fantasmas) {
        m_escena->removeItem(fantasma);
        delete fantasma;
    }
    fantasmas.clear();
}



void MainWindow::mostrarMensaje(const QString& mensaje, const QColor& color) {
    QGraphicsTextItem* texto = new QGraphicsTextItem(mensaje);
    texto->setDefaultTextColor(color);
    texto->setFont(QFont("Arial", 32, QFont::Bold));
    texto->setPos(m_escena->width()/2 - texto->boundingRect().width()/2,
                  m_escena->height()/2 - texto->boundingRect().height()/2);
    m_escena->addItem(texto);
}


void MainWindow::verificarColisiones() {
    if (!m_pacman) return;

    for (Fantasma* fantasma : fantasmas) {
        // Detección simple de colisión por proximidad
        QPointF diff = m_pacman->pos() - fantasma->pos();
        qreal distancia = std::sqrt(diff.x() * diff.x() + diff.y() * diff.y());

        if (distancia < 20) { // Umbral de colisión
            mostrarMensaje("¡Has perdido!", Qt::red);

            // Detener el juego
            m_pacman->setEnabled(false);
            for (Fantasma* f : fantasmas) {
                f->timer->stop();
            }

            return;
        }
    }
}
