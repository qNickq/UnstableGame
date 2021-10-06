#include "keygraphicview.h"

KeyGraphicView::KeyGraphicView(QWidget *parent)
    : QGraphicsView(parent)
{

    /* Немного поднастроим отображение виджета и его содержимого */
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
    this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    /* Также зададим минимальные размеры виджета
     * */
    this->setMinimumHeight(100);
    this->setMinimumWidth(100);

    scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    this->setScene(scene);          // Устанавливаем сцену в виджет

    group_1 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов
    group_2 = new QGraphicsItemGroup(); // Инициализируем вторую группу элементов

    scene->addItem(group_1);            // Добавляем первую группу в сцену
    scene->addItem(group_2);            // Добавляем вторую группу в сцену

    timer = new QTimer();               // Инициализируем Таймер
    //timer->setSingleShot(true);
    // Подключаем СЛОТ для отрисовки к таймеру
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    timer->start(50);                   // Стартуем таймер на 50 миллисекунд

    // Update Temporary timer
    QTimer* timerUpdate = new QTimer();
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    timerUpdate->start(100);

    // Timer for updating difficult
    QTimer* timerDifficult = new QTimer();
    connect(timerDifficult, SIGNAL(timeout()), this, SLOT(slotDifficultKeyBoxTimer()));
    timerDifficult->start(1000);

    // Timer for generating new keyBoxes
    timerGenerate = new QTimer();
    connect(timerGenerate, SIGNAL(timeout()), this, SLOT(slotGenerateKeyBoxTimer()));
    timerGenerate->start(this->currentGenerateTimeout);

    // Timer for updating positions of keyBoxes
    QTimer* timerKeyBox = new QTimer();
    connect(timerKeyBox, SIGNAL(timeout()), this, SLOT(slotKeyBoxTimer()));
    timerKeyBox->start(100);

    this->keyBoxes.reserve(10);

}

KeyGraphicView::~KeyGraphicView()
{

}

void KeyGraphicView::slotUpdateTimer()
{
    this->color = (this->color == Qt::red ? Qt::blue : Qt::red);
}


void KeyGraphicView::slotGenerateKeyBoxTimer()
{
    auto pFoo = std::make_unique<KeyBox>(startPositions.first, startPositions.second, this->currentSpeed);
    this->keyBoxes.push_back(std::move(pFoo));
}

void KeyGraphicView::slotDifficultKeyBoxTimer()
{

    if(difficult.find(seconds) != difficult.end())
    {
        auto dif = difficult.at(seconds);
        currentSpeed = dif.speed;
        currentGenerateTimeout = dif.generateTimeout;
        timerGenerate->setInterval(this->currentGenerateTimeout);
    }
    seconds++;
}

void KeyGraphicView::slotKeyBoxTimer()
{
    for(auto& keyBox: keyBoxes)
    {
        if(keyBox)
        {
            keyBox->updateSpeed();
        }

    }

    keyBoxes.erase(std::remove_if(keyBoxes.begin(), keyBoxes.end(),
                           [=](const std::unique_ptr<KeyBox>& keyBox) { return keyBox->getY() > this->maxKeyBoxY; }), keyBoxes.end());
}

void KeyGraphicView::slotAlarmTimer()
{
    /* Удаляем все элементы со сцены,
     * если они есть перед новой отрисовкой
     * */
    this->deleteItemsFromGroup(group_1);
    this->deleteItemsFromGroup(group_2);

    int width = this->width();      // определяем ширину нашего виджета
    int height = this->height();    // определяем высоту нашего виджета

    /* Устанавливаем размер сцены по размеру виджета
     * Первая координата - это левый верхний угол,
     * а Вторая - это правый нижний угол
     * */
    scene->setSceneRect(0,0,width,height);

    /* Приступаем к отрисовке произвольной картинки
     * */
    QPen penBlack(Qt::black); // Задаём чёрную кисть
    QPen penRed(this->color);   // Задаём красную кисть

    /* Нарисуем черный прямоугольник
     * */
    group_1->addToGroup(scene->addLine(20,20, width - 20, 20, penBlack));
    group_1->addToGroup(scene->addLine(width - 20, 20, width - 20, height -20, penBlack));
    group_1->addToGroup(scene->addLine(width - 20, height -20, 20, height -20, penBlack));
    group_1->addToGroup(scene->addLine(20, height -20, 20, 20, penBlack));

    /* Нарисуем красный квадрат
     * */
    int sideOfSquare = (height > width) ? (width - 60) : (height - 60);
    int centerOfWidget_X = width/2;
    int centerOfWidget_Y = height/2;

    group_2->addToGroup(scene->addLine(centerOfWidget_X - (sideOfSquare/2),
                                       centerOfWidget_Y - (sideOfSquare/2),
                                       centerOfWidget_X + (sideOfSquare/2),
                                       centerOfWidget_Y - (sideOfSquare/2),
                                       penRed));

    group_2->addToGroup(scene->addLine(centerOfWidget_X + (sideOfSquare/2),
                                       centerOfWidget_Y - (sideOfSquare/2),
                                       centerOfWidget_X + (sideOfSquare/2),
                                       centerOfWidget_Y + (sideOfSquare/2),
                                       penRed));

    group_2->addToGroup(scene->addLine(centerOfWidget_X + (sideOfSquare/2),
                                       centerOfWidget_Y + (sideOfSquare/2),
                                       centerOfWidget_X - (sideOfSquare/2),
                                       centerOfWidget_Y + (sideOfSquare/2),
                                       penRed));

    group_2->addToGroup(scene->addLine(centerOfWidget_X - (sideOfSquare/2),
                                       centerOfWidget_Y + (sideOfSquare/2),
                                       centerOfWidget_X - (sideOfSquare/2),
                                       centerOfWidget_Y - (sideOfSquare/2),
                                       penRed));
}

/* Этим методом перехватываем событие изменения размера виджет
 * */
void KeyGraphicView::resizeEvent(QResizeEvent *event)
{
    timer->start(50);   // Как только событие произошло стартуем таймер для отрисовки
    QGraphicsView::resizeEvent(event);  // Запускаем событие родителького класса
}


/* Метод для удаления всех элементов из группы
 * */
void KeyGraphicView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    /* Перебираем все элементы сцены, и если они принадлежат группе,
     * переданной в метод, то удаляем их
     * */
    foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
    }
}
