#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "dish.h"
#include <QList>
#include <QMap>
#include <QSet>

class DatabaseManager
{
public:
    static DatabaseManager &instance();

    QList<Dish> getAllDishes() const;
    Dish getBestDishForType(Dish::MealType type,
                            const QStringList &availableProducts,
                            const QSet<int> &forbiddenIds,
                            int maxMissing = 2) const;
    bool addDish(const Dish &dish);

    void savePreviousDishes(const QMap<Dish::MealType, int> &previousIds);
    QMap<Dish::MealType, int> loadPreviousDishes() const;

    void saveUserProducts(const QStringList &products);
    QStringList loadUserProducts() const;

private:
    DatabaseManager();
    ~DatabaseManager() = default;

    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    void initializeData();
    void loadCustomDishes();
    void saveCustomDishes() const;

    QList<Dish> m_dishes;
    int m_nextId = 1;
};

#endif // DATABASEMANAGER_H
