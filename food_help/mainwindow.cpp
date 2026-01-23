#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QScrollArea>
#include <QLineEdit>
#include <QListWidget>
#include <QGroupBox>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_db(DatabaseManager::instance())
{
    ui->setupUi(this);

    m_userProducts = m_db.loadUserProducts();
    m_previousIds  = m_db.loadPreviousDishes();

    resize(1024, 700);

    setStyleSheet(R"(
        QMainWindow { background-color: #f8fafc; }
        QTabWidget::pane {
            border: 1px solid #e2e8f0;
            background: #ffffff;
            border-radius: 12px;
            top: -1px;
        }
        QTabBar::tab {
            background: #e2e8f0;
            padding: 12px 28px;
            margin: 2px 4px 0 4px;
            border-top-left-radius: 10px;
            border-top-right-radius: 10px;
            font-size: 14px;
            min-width: 130px;
        }
        QTabBar::tab:selected {
            background: #a7e6a7;
            color: #1e293b;
            font-weight: bold;
        }
        QPushButton {
            background-color: #a7e6a7;
            color: #1e293b;
            border: none;
            border-radius: 14px;
            padding: 10px 24px;
            font-size: 14px;
            min-height: 44px;
        }
        QPushButton:hover { background-color: #84d984; }
        QPushButton:pressed { background-color: #6bc46b; }
        QFrame#mealCard {
            background-color: white;
            border: 1px solid #e2e8f0;
            border-radius: 14px;
        }
        QLabel#mealTitle {
            font-size: 17px;
            font-weight: bold;
            color: #1e293b;
        }
        QLabel#mealInfo {
            color: #64748b;
            font-size: 13px;
        }
        QLabel#mealImage {
            background-color: #f1f5f9;
            border-radius: 10px;
        }
        QScrollBar:vertical {
            border: none;
            background: #f1f5f9;
            width: 10px;
            margin: 0;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background: #a7e6a7;
            border-radius: 5px;
            min-height: 30px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
        QLineEdit {
            padding: 10px 14px;
            border: 1px solid #e2e8f0;
            border-radius: 10px;
            font-size: 14px;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #e2e8f0;
            border-radius: 12px;
            margin-top: 10px;
            padding-top: 15px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 10px;
        }
        QListWidget {
            border: 1px solid #e2e8f0;
            border-radius: 10px;
            background-color: white;
            padding: 5px;
        }
        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #f1f5f9;
        }
        QListWidget::item:selected {
            background-color: #e8f5e8;
        }
    )");

    setupTabs();
    generateDay();
    refreshDayTab();
}

MainWindow::~MainWindow()
{
    m_db.saveUserProducts(m_userProducts);
    m_db.savePreviousDishes(m_previousIds);
    delete ui;
}

void MainWindow::setupTabs()
{
    tabWidget = new QTabWidget(this);
    tabWidget->setDocumentMode(true);

    // Вкладка "Меню на день"
    tabDay = new QWidget();
    QVBoxLayout *mainDayLayout = new QVBoxLayout(tabDay);
    mainDayLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollDay = new QScrollArea();
    scrollDay->setWidgetResizable(true);
    scrollDay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *cardsDay = new QWidget();
    dayLayout = new QVBoxLayout(cardsDay);
    dayLayout->setContentsMargins(12, 12, 12, 12);
    dayLayout->setSpacing(12);

    Dish::MealType types[] = {Dish::BREAKFAST, Dish::LUNCH, Dish::DINNER, Dish::SNACK};
    for (auto type : types) {
        QFrame *card = createMealCard(type);
        mealCardFrames[type] = card;
        dayLayout->addWidget(card);
    }
    dayLayout->addStretch(1);

    scrollDay->setWidget(cardsDay);
    mainDayLayout->addWidget(scrollDay);

    tabWidget->addTab(tabDay, "Меню на день");

    // Вкладка "Все рецепты"
    tabAll = new QWidget();
    QVBoxLayout *mainAllLayout = new QVBoxLayout(tabAll);
    mainAllLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollAll = new QScrollArea();
    scrollAll->setWidgetResizable(true);
    scrollAll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *cardsAll = new QWidget();
    allLayout = new QVBoxLayout(cardsAll);
    allLayout->setContentsMargins(12, 12, 12, 12);
    allLayout->setSpacing(12);

    refreshAllRecipesTab();

    scrollAll->setWidget(cardsAll);
    mainAllLayout->addWidget(scrollAll);

    tabWidget->addTab(tabAll, "Все рецепты");

    // Вкладка "Из моих продуктов"
    tabMy = new QWidget();
    setupMyProductsTab(tabMy);
    tabWidget->addTab(tabMy, "Из моих продуктов");

    QWidget *central = new QWidget();
    QVBoxLayout *centralL = new QVBoxLayout(central);
    centralL->setContentsMargins(0, 0, 0, 0);
    centralL->addWidget(tabWidget);
    setCentralWidget(central);
}

void MainWindow::setupMyProductsTab(QWidget *tab)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(tab);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(20);

    // Левый столбец - ввод продуктов
    QWidget *leftColumn = new QWidget();
    leftColumn->setFixedWidth(300);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftColumn);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(12);

    // Группа "Мои продукты"
    QGroupBox *productsGroup = new QGroupBox("Мои продукты");
    QVBoxLayout *productsLayout = new QVBoxLayout(productsGroup);
    productsLayout->setSpacing(10);

    // Поле ввода нового продукта
    QHBoxLayout *inputLayout = new QHBoxLayout();
    myProductsEdit = new QLineEdit();
    myProductsEdit->setPlaceholderText("Введите продукт...");
    inputLayout->addWidget(myProductsEdit);

    QPushButton *addButton = new QPushButton("+");
    addButton->setFixedWidth(50);
    inputLayout->addWidget(addButton);
    productsLayout->addLayout(inputLayout);

    // Список продуктов
    myProductsList = new QListWidget();
    updateProductsList();
    productsLayout->addWidget(myProductsList);

    // Кнопка удаления
    QPushButton *removeButton = new QPushButton("Удалить выбранный");
    productsLayout->addWidget(removeButton);

    leftLayout->addWidget(productsGroup);
    leftLayout->addStretch();

    // Правый столбец - рецепты
    QWidget *rightColumn = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightColumn);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(12);

    QLabel *titleLabel = new QLabel("Рецепты из ваших продуктов");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1e293b;");
    rightLayout->addWidget(titleLabel);

    QScrollArea *scrollRecipes = new QScrollArea();
    scrollRecipes->setWidgetResizable(true);
    scrollRecipes->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *cardsContainer = new QWidget();
    myRecipesLayout = new QVBoxLayout(cardsContainer);
    myRecipesLayout->setContentsMargins(0, 0, 0, 0);
    myRecipesLayout->setSpacing(12);

    refreshMyProductsTab();

    scrollRecipes->setWidget(cardsContainer);
    rightLayout->addWidget(scrollRecipes);

    // Подключаем сигналы
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addProductFromLineEdit);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeProduct);
    connect(myProductsEdit, &QLineEdit::returnPressed, this, &MainWindow::addProductFromLineEdit);

    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(rightColumn);
}

QFrame* MainWindow::createMealCard(Dish::MealType type)
{
    QFrame *card = new QFrame();
    card->setObjectName("mealCard");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QString typeName;
    switch (type) {
    case Dish::BREAKFAST: typeName = "Завтрак"; break;
    case Dish::LUNCH:     typeName = "Обед"; break;
    case Dish::DINNER:    typeName = "Ужин"; break;
    case Dish::SNACK:     typeName = "Перекус"; break;
    default:              typeName = "Неизвестно";
    }
    QLabel *title = new QLabel(typeName);
    title->setObjectName("mealTitle");
    layout->addWidget(title);

    QLabel *imageLbl = new QLabel();
    imageLbl->setObjectName("mealImage");
    imageLbl->setFixedHeight(120);
    imageLbl->setScaledContents(true);
    imageLbl->setAlignment(Qt::AlignCenter);
    layout->addWidget(imageLbl);

    QLabel *info = new QLabel("Блюдо не выбрано");
    info->setObjectName("mealInfo");
    info->setWordWrap(true);
    layout->addWidget(info);

    QHBoxLayout *btnL = new QHBoxLayout();
    btnL->addStretch();
    QPushButton *replaceBtn = new QPushButton("Заменить");
    replaceBtn->setProperty("mealType", static_cast<int>(type));
    connect(replaceBtn, &QPushButton::clicked, this, [this, type]() {
        replaceMeal(type);
    });
    btnL->addWidget(replaceBtn);
    layout->addLayout(btnL);

    return card;
}

QFrame* MainWindow::createRecipeCard(const Dish &dish)
{
    QFrame *card = new QFrame();
    card->setObjectName("mealCard");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QLabel *title = new QLabel(dish.getTypeString() + ": " + dish.getName());
    title->setObjectName("mealTitle");
    layout->addWidget(title);

    QLabel *imageLbl = new QLabel();
    imageLbl->setObjectName("mealImage");
    imageLbl->setFixedHeight(120);
    imageLbl->setScaledContents(true);
    imageLbl->setAlignment(Qt::AlignCenter);
    layout->addWidget(imageLbl);

    // Фото только если есть
    if (!dish.getImagePath().isEmpty() && QFile::exists(dish.getImagePath())) {
        QPixmap pix(dish.getImagePath());
        imageLbl->setPixmap(pix.scaled(imageLbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLbl->setVisible(true);
    } else {
        imageLbl->setVisible(false);
    }

    QLabel *info = new QLabel(QString(
                                  "Калории: %1 ккал | БЖУ: %2 / %3 / %4 г\n"
                                  "Недостающие: %5\n\n"
                                  "%6")
                                  .arg(dish.getCalories())
                                  .arg(dish.getProtein()).arg(dish.getFat()).arg(dish.getCarbs())
                                  .arg(getMissingString(dish))
                                  .arg(dish.getRecipe().left(120) + (dish.getRecipe().size() > 120 ? "..." : "")));
    info->setObjectName("mealInfo");
    info->setWordWrap(true);
    layout->addWidget(info);

    // Добавляем кнопку "Добавить в меню дня"
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton *addToDayBtn = new QPushButton("Добавить в меню дня");
    addToDayBtn->setProperty("dishId", dish.getId());
    connect(addToDayBtn, &QPushButton::clicked, this, [this, dish]() {
        addToDayMenu(dish.getId());
    });
    btnLayout->addWidget(addToDayBtn);

    layout->addLayout(btnLayout);

    return card;
}

void MainWindow::updateMealCard(Dish::MealType type, const Dish &dish)
{
    if (!mealCardFrames.contains(type)) return;

    QFrame *card = mealCardFrames[type];
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(card->layout());
    if (!layout) return;

    QLabel *title = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
    QLabel *imageLbl = qobject_cast<QLabel*>(layout->itemAt(1)->widget());
    QLabel *info = qobject_cast<QLabel*>(layout->itemAt(2)->widget());

    if (title) title->setText(dish.getTypeString() + ": " + dish.getName());

    if (imageLbl) {
        if (!dish.getImagePath().isEmpty() && QFile::exists(dish.getImagePath())) {
            QPixmap pix(dish.getImagePath());
            imageLbl->setPixmap(pix.scaled(imageLbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLbl->setVisible(true);
        } else {
            imageLbl->setVisible(false);
        }
    }

    if (info) {
        QString txt = QString(
                          "Калории: %1 ккал\n"
                          "БЖУ: %2 / %3 / %4 г\n"
                          "Недостающие: %5\n\n"
                          "%6")
                          .arg(dish.getCalories())
                          .arg(dish.getProtein())
                          .arg(dish.getFat())
                          .arg(dish.getCarbs())
                          .arg(getMissingString(dish))
                          .arg(dish.getRecipe().left(120) + (dish.getRecipe().size() > 120 ? "..." : ""));
        info->setText(txt);
    }
}

void MainWindow::refreshDayTab()
{
    for (auto it = m_today.constBegin(); it != m_today.constEnd(); ++it) {
        updateMealCard(it.key(), it.value());
    }
}

void MainWindow::refreshAllRecipesTab()
{
    if (!allLayout) return;

    // Очищаем layout вкладки "Все рецепты"
    QLayoutItem* item;
    while ((item = allLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
            delete item->widget();
        }
        delete item;
    }

    QList<Dish> allDishes = m_db.getAllDishes();

    // Добавляем кнопку "Добавить новый рецепт" в начало
    QFrame *addCard = new QFrame();
    addCard->setObjectName("mealCard");
    addCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout *addLayout = new QVBoxLayout(addCard);
    addLayout->setContentsMargins(12, 12, 12, 12);
    addLayout->setSpacing(8);

    QLabel *addTitle = new QLabel("Добавить новый рецепт");
    addTitle->setObjectName("mealTitle");
    addTitle->setAlignment(Qt::AlignCenter);
    addLayout->addWidget(addTitle);

    QLabel *addImage = new QLabel("+");
    addImage->setObjectName("mealImage");
    addImage->setFixedHeight(120);
    addImage->setAlignment(Qt::AlignCenter);
    addImage->setStyleSheet("background-color: #e8f5e8; font-size: 48px; color: #4CAF50;");
    addLayout->addWidget(addImage);

    QPushButton *addRecipeBtn = new QPushButton("+ Добавить свой рецепт");
    addRecipeBtn->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 15px;");
    connect(addRecipeBtn, &QPushButton::clicked, this, &MainWindow::on_addDishButton_clicked);
    addLayout->addWidget(addRecipeBtn);

    allLayout->addWidget(addCard);

    // Добавляем все существующие рецепты
    for (const Dish &d : allDishes) {
        QFrame *card = createRecipeCard(d);
        allLayout->addWidget(card);
    }

    allLayout->addStretch(1);
}

void MainWindow::refreshMyProductsTab()
{
    if (!myRecipesLayout) return;

    // Очищаем layout с рецептами
    QLayoutItem* item;
    while ((item = myRecipesLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
            delete item->widget();
        }
        delete item;
    }

    QList<Dish> allDishes = m_db.getAllDishes();

    if (m_userProducts.isEmpty()) {
        QLabel *hintLabel = new QLabel("Добавьте продукты в список слева, чтобы увидеть подходящие рецепты");
        hintLabel->setAlignment(Qt::AlignCenter);
        hintLabel->setStyleSheet("color: #64748b; font-size: 14px; padding: 40px;");
        myRecipesLayout->addWidget(hintLabel);
    } else {
        bool foundAny = false;

        for (const Dish &d : allDishes) {
            int missingCount = 0;

            for (const QString &ing : d.getIngredients()) {
                QString norm = ing.trimmed().toLower();
                bool has = false;
                for (const QString &p : m_userProducts) {
                    QString productNorm = p.trimmed().toLower();
                    if (productNorm.contains(norm) || norm.contains(productNorm)) {
                        has = true;
                        break;
                    }
                }
                if (!has) {
                    ++missingCount;
                }
            }

            // Показываем рецепты, где недостает не более 2 ингредиентов
            if (missingCount <= 2) {
                QFrame *card = createRecipeCard(d);
                myRecipesLayout->addWidget(card);
                foundAny = true;
            }
        }

        if (!foundAny) {
            QLabel *noRecipesLabel = new QLabel("Нет рецептов для ваших продуктов.\nПопробуйте добавить больше продуктов или изменить названия.");
            noRecipesLabel->setAlignment(Qt::AlignCenter);
            noRecipesLabel->setStyleSheet("color: #64748b; font-size: 14px; padding: 40px;");
            myRecipesLayout->addWidget(noRecipesLabel);
        }
    }

    myRecipesLayout->addStretch(1);
}

void MainWindow::generateDay()
{
    QSet<int> forbidden;
    for (int id : m_previousIds.values()) forbidden.insert(id);

    m_today[Dish::BREAKFAST] = m_db.getBestDishForType(Dish::BREAKFAST, m_userProducts, forbidden);
    m_today[Dish::LUNCH]     = m_db.getBestDishForType(Dish::LUNCH,     m_userProducts, forbidden);
    m_today[Dish::DINNER]    = m_db.getBestDishForType(Dish::DINNER,    m_userProducts, forbidden);
    m_today[Dish::SNACK]     = m_db.getBestDishForType(Dish::SNACK,     m_userProducts, forbidden);

    m_previousIds.clear();
    for (auto it = m_today.constBegin(); it != m_today.constEnd(); ++it) {
        if (it.value().getId() != -1)
            m_previousIds[it.key()] = it.value().getId();
    }

    refreshDayTab();
}

void MainWindow::replaceMeal(Dish::MealType type)
{
    QSet<int> forbidden;
    if (m_today.contains(type) && m_today[type].getId() != -1)
        forbidden.insert(m_today[type].getId());

    Dish newDish = m_db.getBestDishForType(type, m_userProducts, forbidden, 3);
    if (newDish.getId() == -1) {
        QMessageBox::information(this, "Замена", "Не удалось найти другое подходящее блюдо");
        return;
    }

    m_today[type] = newDish;
    refreshDayTab();
}

void MainWindow::addToDayMenu(int dishId)
{
    // Находим блюдо по ID
    QList<Dish> allDishes = m_db.getAllDishes();
    Dish selectedDish;

    for (const Dish &d : allDishes) {
        if (d.getId() == dishId) {
            selectedDish = d;
            break;
        }
    }

    if (selectedDish.getId() == -1) {
        QMessageBox::warning(this, "Ошибка", "Блюдо не найдено");
        return;
    }

    // Заменяем блюдо этого типа в меню на день
    Dish::MealType type = selectedDish.getType();
    m_today[type] = selectedDish;

    // Обновляем ID в истории
    m_previousIds[type] = dishId;

    // Обновляем отображение
    refreshDayTab();

    QMessageBox::information(this, "Успех",
                             QString("Блюдо \"%1\" добавлено в меню на день (%2)")
                                 .arg(selectedDish.getName())
                                 .arg(selectedDish.getTypeString()));

    // Переключаемся на вкладку меню на день
    tabWidget->setCurrentIndex(0);
}

QString MainWindow::getMissingString(const Dish &dish) const
{
    QStringList missing;
    for (const QString &ing : dish.getIngredients()) {
        QString norm = ing.trimmed().toLower();
        bool found = false;
        for (const QString &p : m_userProducts) {
            if (p.trimmed().toLower().contains(norm) || norm.contains(p.trimmed().toLower())) {
                found = true;
                break;
            }
        }
        if (!found) missing << ing;
    }
    return missing.isEmpty() ? "всё есть" : missing.join(", ");
}

void MainWindow::addProductFromLineEdit()
{
    QString prod = myProductsEdit->text().trimmed();
    if (!prod.isEmpty()) {
        m_userProducts << prod;
        updateProductsList();
        refreshMyProductsTab();
        myProductsEdit->clear();
        myProductsEdit->setFocus();

        // Также обновляем меню на день
        generateDay();
    }
}

void MainWindow::removeProduct()
{
    int row = myProductsList->currentRow();
    if (row >= 0) {
        m_userProducts.removeAt(row);
        updateProductsList();
        refreshMyProductsTab();
        generateDay();
    }
}

void MainWindow::updateProductsList()
{
    if (myProductsList) {
        myProductsList->clear();
        myProductsList->addItems(m_userProducts);
    }
}

void MainWindow::on_generateDayButton_clicked()
{
    generateDay();
}

void MainWindow::on_addDishButton_clicked()
{
    QString name = QInputDialog::getText(this, "Новое блюдо", "Название:");
    if (name.isEmpty()) return;

    QStringList typesStr = {"Завтрак", "Обед", "Ужин", "Перекус"};
    QString typeStr = QInputDialog::getItem(this, "Тип", "Выберите тип:", typesStr, 0, false);
    Dish::MealType type;
    if (typeStr == "Завтрак") type = Dish::BREAKFAST;
    else if (typeStr == "Обед") type = Dish::LUNCH;
    else if (typeStr == "Ужин") type = Dish::DINNER;
    else type = Dish::SNACK;

    QString recipe = QInputDialog::getMultiLineText(this, "Рецепт", "Рецепт пошагово:");

    double cal = QInputDialog::getDouble(this, "Калории", "Калории:", 300, 0, 2000, 0);
    double p = QInputDialog::getDouble(this, "Белки", "Белки (г):", 20, 0, 200, 1);
    double f = QInputDialog::getDouble(this, "Жиры", "Жиры (г):", 10, 0, 200, 1);
    double c = QInputDialog::getDouble(this, "Углеводы", "Углеводы (г):", 40, 0, 200, 1);

    QString ingsStr = QInputDialog::getText(this, "Ингредиенты", "Через запятую:");
    QStringList ingredients = ingsStr.split(",", Qt::SkipEmptyParts);
    for (auto &s : ingredients) s = s.trimmed();

    QString imgPath = QFileDialog::getOpenFileName(this, "Фото блюда (необязательно)", "",
                                                   "Изображения (*.png *.jpg *.jpeg *.bmp)");
    if (imgPath.isEmpty()) imgPath = "";

    Dish newDish(-1, name, type, recipe, cal, p, f, c, ingredients, imgPath);

    if (m_db.addDish(newDish)) {
        QMessageBox::information(this, "Успех", "Блюдо добавлено!");
        // Обновляем все вкладки
        refreshAllRecipesTab();
        refreshMyProductsTab();
        refreshDayTab();
    }
}

void MainWindow::on_showAllButton_clicked()
{
    QList<Dish> dishes = m_db.getAllDishes();
    QString text = "Все блюда:\n\n";
    for (const Dish &d : dishes) {
        text += d.getTypeString() + " | " + d.getName() + " (" + QString::number(d.getCalories()) + " ккал)\n";
    }
    QMessageBox::information(this, "Все блюда", text.isEmpty() ? "Блюд пока нет" : text);
}

void MainWindow::on_replaceBreakfast_clicked() { replaceMeal(Dish::BREAKFAST); }
void MainWindow::on_replaceLunch_clicked()     { replaceMeal(Dish::LUNCH); }
void MainWindow::on_replaceDinner_clicked()    { replaceMeal(Dish::DINNER); }
void MainWindow::on_replaceSnack_clicked()     { replaceMeal(Dish::SNACK); }
