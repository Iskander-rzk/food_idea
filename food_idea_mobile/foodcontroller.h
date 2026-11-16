#ifndef FOODCONTROLLER_H
#define FOODCONTROLLER_H

#include "databasemanager.h"
#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QSet>

class FoodController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList today READ today NOTIFY dataChanged)
    Q_PROPERTY(QVariantList allRecipes READ allRecipes NOTIFY dataChanged)
    Q_PROPERTY(QVariantList myRecipes READ myRecipes NOTIFY dataChanged)
    Q_PROPERTY(QStringList products READ products NOTIFY dataChanged)
    Q_PROPERTY(int recipeCount READ recipeCount NOTIFY dataChanged)
    Q_PROPERTY(QVariantList excludedProducts READ excludedProducts NOTIFY dataChanged)
    Q_PROPERTY(QVariantList shoppingList READ shoppingList NOTIFY dataChanged)
    Q_PROPERTY(QVariantList favoriteRecipes READ favoriteRecipes NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap dailyTotals READ dailyTotals NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap nutritionGoals READ nutritionGoals NOTIFY dataChanged)
    Q_PROPERTY(QStringList missingForToday READ missingForToday NOTIFY dataChanged)

public:
    explicit FoodController(QObject *parent = nullptr);

    QVariantList today() const;
    QVariantList allRecipes() const;
    QVariantList myRecipes() const;
    QStringList products() const;
    int recipeCount() const;
    QVariantList excludedProducts() const;
    QVariantList shoppingList() const;
    QVariantList favoriteRecipes() const;
    QVariantMap dailyTotals() const;
    QVariantMap nutritionGoals() const;
    QStringList missingForToday() const;

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
    Q_INVOKABLE void addExcludedProduct(const QString &name, bool allergy);
    Q_INVOKABLE void removeExcludedProduct(int index);
    Q_INVOKABLE bool isExcluded(int dishId) const;
    Q_INVOKABLE void toggleFavorite(int dishId);
    Q_INVOKABLE bool isFavorite(int dishId) const;
    Q_INVOKABLE void addShoppingItem(const QString &item);
    Q_INVOKABLE void removeShoppingItem(int index);
    Q_INVOKABLE void toggleShoppingItem(int index);
    Q_INVOKABLE void addMissingToShopping();
    Q_INVOKABLE void setNutritionGoals(double calories, double protein, double fat, double carbs);

signals:
    void dataChanged();
    void messageChanged(const QString &message);

private:
    QVariantMap dishToMap(const Dish &dish, bool includeMissing = true) const;
    QStringList missingIngredients(const Dish &dish) const;
    QString missingString(const Dish &dish) const;
    Dish dishById(int id) const;
    bool dishExcluded(const Dish &dish) const;
    Dish selectDish(Dish::MealType type, const QSet<int> &avoid, int maxMissing) const;
    void saveExtras() const;
    void rememberChoice(Dish::MealType type, int id);

    DatabaseManager &m_db;
    QStringList m_products;
    QMap<Dish::MealType, Dish> m_today;
    QMap<Dish::MealType, int> m_previousIds;
    QMap<Dish::MealType, QList<int>> m_recentChoices;
    QStringList m_excludedNames;
    QList<bool> m_excludedAllergy;
    QSet<int> m_favorites;
    QStringList m_shopping;
    QSet<QString> m_shoppingDone;
    double m_goalCalories = 2000;
    double m_goalProtein = 100;
    double m_goalFat = 70;
    double m_goalCarbs = 250;
};

#endif // FOODCONTROLLER_H
