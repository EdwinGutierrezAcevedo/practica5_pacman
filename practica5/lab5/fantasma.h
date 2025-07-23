#ifndef FANTASMA_H
#define FANTASMA_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QColor>
#include "mapa.h"

class Fantasma : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Fantasma(QColor color, Mapa* mapa, const QString& spritePath, QObject* parent = nullptr);

    void mover();
    QPoint obtenerPosicion() const;
    QTimer* timer;
    void setDireccion(const QPoint& nuevaDireccion);


private:
    void cambiarDireccion();
    bool validarMovimiento(QPoint nuevaPosicion);
    void actualizarSprite();

    QPoint m_direccion;
    int m_velocidad;
    Mapa* m_mapa;
    QColor m_color;
    QPixmap m_spriteSheet;
    int m_currentFrame;
    int m_frameBase;
    const int m_frameSize = 30; // Tamaño de cada sprite (ajustar según tus imágenes)
};

#endif // FANTASMA_H
