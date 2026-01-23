#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databasemanager.h"
#include "dish.h"

#include <QTabWidget>
#include <QMap>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QList>
#include <QListWidget>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_generateDayButton_clicked();
    void on_addDishButton_clicked();
    void on_showAllButton_clicked();

    void on_replaceBreakfast_clicked();
    void on_replaceLunch_clicked();
    void on_replaceDinner_clicked();
    void on_replaceSnack_clicked();

    void addProductFromLineEdit();
    void removeProduct();
    void updateProductsList();

private:
    void setupTabs();
    QFrame* createMealCard(Dish::MealType type);
    QFrame* createRecipeCard(const Dish &dish);
    void updateMealCard(Dish::MealType type, const Dish &dish);
    void refreshDayTab();
    void generateDay();
    void replaceMeal(Dish::MealType type);
    QString getMissingString(const Dish &dish) const;
    void refreshAllRecipesTab();
    void refreshMyProductsTab();
    void addToDayMenu(int dishId);
    void setupMyProductsTab(QWidget *tab);  // ← Новый метод для настройки вкладки

    Ui::MainWindow *ui;
    DatabaseManager &m_db;

    QTabWidget *tabWidget = nullptr;
    QWidget *tabDay = nullptr;
    QWidget *tabAll = nullptr;
    QWidget *tabMy = nullptr;
    QVBoxLayout *dayLayout = nullptr;
    QVBoxLayout *allLayout = nullptr;
    QVBoxLayout *myRecipesLayout = nullptr;  // ← Layout для рецептов во вкладке "Из моих продуктов"
    QMap<Dish::MealType, QFrame*> mealCardFrames;

    // Widgets для вкладки "Из моих продуктов"
    QListWidget *myProductsList = nullptr;
    QLineEdit *myProductsEdit = nullptr;

    QStringList m_userProducts;
    QMap<Dish::MealType, Dish> m_today;
    QMap<Dish::MealType, int> m_previousIds;

};

#endif // MAINWINDOW_H
