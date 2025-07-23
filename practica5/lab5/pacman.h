#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Pacman : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Pacman();
    ~Pacman();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void mover();

private:
    int m_dx, m_dy;
    int m_deseadoDx, m_deseadoDy;
    QTimer* m_timer;
    QPixmap m_spriteSheet;
    int m_currentFrame;
    int m_frameBase;
    const int m_frameSize = 30;

    bool chequearColision(int x, int y);
    void cruzarPantalla();
    QTimer* timer;
signals:
    void powerUpComido();
};

#endif // PACMAN_H
