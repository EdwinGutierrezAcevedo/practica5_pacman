#ifndef PUNTOS_H
#define PUNTOS_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QVector>

class Puntos : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum TipoPunto {
        NORMAL,
        POWER_UP
    };

    explicit Puntos(QGraphicsItem* parent = nullptr);
    ~Puntos();

    // Métodos principales
    void agregarPunto(int x, int y, TipoPunto tipo = NORMAL);
    bool hayPunto(int x, int y) const;
    bool eliminarPunto(int x, int y);
    int contarPuntos() const;
    void limpiarPuntos();

    // QGraphicsItem overrides
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    // Constantes
    static const int TAMANIO_NORMAL = 10;
    static const int TAMANIO_POWER_UP = 16;

private:
    struct PuntoData {
        QGraphicsEllipseItem* item;
        TipoPunto tipo;
        QPoint posicion;
    };

    QVector<PuntoData> m_puntos;
    const int m_tamanioCelda = 30;
};

#endif // PUNTOS_H
