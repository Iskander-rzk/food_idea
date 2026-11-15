#ifndef FOODCONTROLLER_H
#define FOODCONTROLLER_H

#include "databasemanager.h"
#include <QObject>
#include <QVariantList>
#include <QVariantMap>

class FoodController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList today READ today NOTIFY dataChanged)
    Q_PROPERTY(QVariantList allRecipes READ allRecipes NOTIFY dataChanged)
    Q_PROPERTY(QVariantList myRecipes READ myRecipes NOTIFY dataChanged)
    Q_PROPERTY(QStringList products READ products NOTIFY dataChanged)
    Q_PROPERTY(int recipeCount READ recipeCount NOTIFY dataChanged)

public:
    explicit FoodController(QObject *parent = nullptr);

    QVariantList today() const;
    QVariantList allRecipes() const;
    QVariantList myRecipes() const;
    QStringList products() const;
    int recipeCount() const;

    Q_INVOKABLE void generateDay();
    Q_INVOKABLE void replaceMeal(int mealType);
    Q_INVOKABLE void addToDayMenu(int dishId);
    Q_INVOKABLE void addProduct(const QString &product);
    Q_INVOKABLE void removeProduct(int index);
    Q_INVOKABLE bool addDish(const QString &name,
                             int mealType,
                             const QString &recipe,
                             double calories,
                             double protein,
                             double fat,
                             double carbs,
                             const QString &ingredients,
                             const QString &imagePath);
    Q_INVOKABLE QVariantMap recipeById(int id) const;

signals:
    void dataChanged();
    void messageChanged(const QString &message);

private:
    QVariantMap dishToMap(const Dish &dish, bool includeMissing = true) const;
    QStringList missingIngredients(const Dish &dish) const;
    QString missingString(const Dish &dish) const;
    Dish dishById(int id) const;

    DatabaseManager &m_db;
    QStringList m_products;
    QMap<Dish::MealType, Dish> m_today;
    QMap<Dish::MealType, int> m_previousIds;
};

#endif // FOODCONTROLLER_H
