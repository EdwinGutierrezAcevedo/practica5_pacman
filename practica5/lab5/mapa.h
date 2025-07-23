#ifndef MAPA_H
#define MAPA_H

#include <QGraphicsItem>
#include <QVector>

class Mapa : public QGraphicsItem {
public:
    explicit Mapa(QGraphicsItem* parent = nullptr);
    ~Mapa();

    bool cargarDesdeArchivo(const QString& nombreArchivo);
    QPoint obtenerPosicionInicial() const;
    bool todosPuntosConsumidos() const;
    bool esPared(int x, int y) const;  // Método crucial para colisiones
    QRectF boundingRect() const override;
    bool eliminarPunto(int x, int y);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    int obtenerTamanioCelda() const { return m_tamanioCelda; }
    QPoint obtenerPosicionRojo() const { return m_posicionRojo; }
    QPoint obtenerPosicionAzul() const { return m_posicionAzul; }
    QPoint obtenerPosicionRosa() const { return m_posicionRosa; }
    QPoint obtenerPosicionNaranja() const { return m_posicionNaranja; }

private:
    void limpiarMapa();
    void crearPared(int fila, int columna);
    void crearPunto(int fila, int columna);

    int m_ancho;
    int m_alto;
    const int m_tamanioCelda = 30;
    QVector<QGraphicsRectItem*> m_paredes;
    QVector<QGraphicsEllipseItem*> m_puntos;
    QPoint m_posicionInicial;
    QPoint m_posicionRojo;
    QPoint m_posicionAzul;
    QPoint m_posicionRosa;
    QPoint m_posicionNaranja;
};

#endif // MAPA_H
