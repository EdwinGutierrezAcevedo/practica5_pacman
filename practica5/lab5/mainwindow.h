#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "fantasma.h"

//class Fantasma;
class Mapa;
class Pacman;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void cargarNivel(const QString& nombreArchivo);
    void iniciarJuego();
    Mapa* getMapa() const { return m_mapa; }
    void mostrarMensaje(const QString& mensaje, const QColor& color);

public slots:
    void actualizarPuntaje(int puntos);
    void verificarColisiones();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene* m_escena;
    Mapa* m_mapa;
    Pacman* m_pacman;
    QGraphicsTextItem* m_textoPuntaje;
    int m_puntaje;
    QList<Fantasma*> fantasmas;
    QTimer* m_colisionTimer;


    void configurarEscena();
    void limpiarEscena();
    void ajustarVista();
};

#endif // MAINWINDOW_H
