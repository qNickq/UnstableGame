#ifndef KEYGRAPHICVIEW_H
#define KEYGRAPHICVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>

#include <QDebug>

#include <memory>
#include <vector>
#include <map>

class KeyBox
{

    // TODO
    // Sprite
    // QEvent::KeyPress
    quint16 speed; // pixels by one iteration
    quint16 x;
    quint16 y;
    bool state = 0; // 0 means notPressed, 1 means Pressed
public:
    KeyBox(const int& _x, const int& _y, const int& _speed): x(_x), y(_y), speed(_speed)
    {
        qDebug() << "KeyBox generated: " << this ;
    };
    void updateSpeed()
    {
        qDebug() << this << this->y << '\n';
        this->y += speed;
    };
    quint16 getY()
    {
        return this->y;
    }
    ~KeyBox()
    {
        // TODO: notify Game class about KeyBox erasing with its state
        qDebug() << "KeyBox " << this << " with state " << state << " was erased\n" ;
    };
};



// Расширяем класс QGraphicsView
class KeyGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit  KeyGraphicView(QWidget *parent = 0);
    ~ KeyGraphicView();

signals:

private slots:
    void slotAlarmTimer();  /* слот для обработчика переполнения таймера
                             * в нём будет производиться перерисовка
                             * виджета
                             * */
    void slotUpdateTimer();  /* слот для обработчика переполнения таймера
                             * в нём будет производиться перерисовка
                             * виджета
                             * */
    void slotGenerateKeyBoxTimer();

    void slotDifficultKeyBoxTimer();
    void slotKeyBoxTimer();

private:
    QColor color = Qt::red;
    QGraphicsScene      *scene;     // Объявляем сцену для отрисовки
    QGraphicsItemGroup  *group_1;   // Объявляем первую группу элементов
    QGraphicsItemGroup  *group_2;   // Объявляем вторую группу элементов

    // Queue of available KeyBoxes
    std::pair<quint16, quint16> startPositions = {0, 0}; // first means x, second means y
    quint16 maxKeyBoxY = 100;
    std::vector<std::unique_ptr<KeyBox>> keyBoxes;
    struct difficultParams
    {
        quint16 speed;
        quint16 generateTimeout;
    };

    const std::map<quint16,difficultParams> difficult = {{0,{1,1000}}}; // means pair seconds:speed

    // Difficult properties
    quint64 seconds = 0;
    quint16 currentSpeed = difficult.at(0).speed;
    quint16 currentGenerateTimeout = difficult.at(0).generateTimeout;

    QTimer* timerGenerate;

    /* Таймер для задержки отрисовки.
     * Дело в том, что при создании окна и виджета
     * необходимо некоторое время, чтобы родительский слой
     * развернулся, чтобы принимать от него адекватные параметры
     * ширины и высоты
     * */
    QTimer              *timer;

private:
    /* Перегружаем событие изменения размера окна,
     * чтобы перехватывать его
     * */
    void resizeEvent(QResizeEvent *event);
    /* Метод для удаления всех элементов
     * из группы элементов
     * */
    void deleteItemsFromGroup(QGraphicsItemGroup *group_1);
};

#endif // KEYGRAPHICVIEW_H
