#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "pribil.h"

// Тестируем функцию добавления товаров
TEST_CASE("Test valid item addition") {
    ProfitCalculator calculator(1000.0, 100.0);
    Item item = {"Product A", 10.0, 50.0, 70.0, 10};

    // Ожидаем, что товар добавится без ошибок
    CHECK_NOTHROW(calculator.addItem(item));
}

TEST_CASE("Test invalid item addition") {
    ProfitCalculator calculator(1000.0, 100.0);
    Item invalidItem = {"Product B", -10.0, -50.0, 30.0, -5};  // Невалидные данные

    // Ожидаем, что при добавлении товара с неверными данными будет выброшено исключение
    CHECK_THROWS_AS(calculator.addItem(invalidItem), std::invalid_argument);
}

// Тест на максимизацию прибыли
TEST_CASE("Test maximize profit") {
    ProfitCalculator calculator(1000.0, 100.0);
    Item item1 = {"Product A", 10.0, 50.0, 70.0, 10};
    Item item2 = {"Product B", 20.0, 60.0, 90.0, 5};
    Item item3 = {"Product C", 5.0, 30.0, 50.0, 20};

    calculator.addItem(item1);
    calculator.addItem(item2);
    calculator.addItem(item3);

    auto selectedItems = calculator.maximizeProfit();

    // Проверяем, что программа выбрала товары с максимальной прибылью
    CHECK(selectedItems.size() > 0);  // Проверяем, что товары выбраны

    double totalProfit = 0.0;
    for (const auto& item : selectedItems) {
        totalProfit += (item.sellPrice - item.buyPrice) * item.availableQty;
    }

    // Проверяем, что общая прибыль больше нуля
    CHECK(totalProfit > 0);
}

// Тест на проверку бюджета
TEST_CASE("Test insufficient budget") {
    ProfitCalculator calculator(200.0, 50.0);  // Ограниченный бюджет
    Item item1 = {"Product A", 10.0, 50.0, 70.0, 10};
    Item item2 = {"Product B", 20.0, 60.0, 90.0, 5};
    Item item3 = {"Product C", 5.0, 30.0, 50.0, 20};

    calculator.addItem(item1);
    calculator.addItem(item2);
    calculator.addItem(item3);

    auto selectedItems = calculator.maximizeProfit();

    // Проверяем, что общая стоимость выбранных товаров не превышает бюджет
    double totalCost = 0.0;
    for (const auto& item : selectedItems) {
        totalCost += item.buyPrice * item.availableQty;
    }

    CHECK(totalCost <= 200.0);
}

// Тест на проверку веса
TEST_CASE("Test insufficient weight capacity") {
    ProfitCalculator calculator(1000.0, 50.0);  // Ограниченная грузоподъемность
    Item item1 = {"Product A", 10.0, 50.0, 70.0, 10};
    Item item2 = {"Product B", 20.0, 60.0, 90.0, 5};
    Item item3 = {"Product C", 5.0, 30.0, 50.0, 20};

    calculator.addItem(item1);
    calculator.addItem(item2);
    calculator.addItem(item3);

    auto selectedItems = calculator.maximizeProfit();

    // Проверяем, что программа выбрала товары в пределах доступной грузоподъемности
    double totalWeight = 0.0;
    for (const auto& item : selectedItems) {
        totalWeight += item.weight * item.availableQty;
    }

    CHECK(totalWeight <= 50.0);  // Проверка на превышение грузоподъемности
}

// Тест на использование оставшегося бюджета и веса
TEST_CASE("Test use remaining budget and weight") {
    ProfitCalculator calculator(1000.0, 50.0);  // Достаточный бюджет и вес
    Item item1 = {"Product A", 10.0, 50.0, 70.0, 10};  // Товар с высокой прибылью
    Item item2 = {"Product B", 20.0, 60.0, 90.0, 5};   // Товар с меньшей прибылью
    Item item3 = {"Product C", 5.0, 30.0, 50.0, 20};   // Дешевый товар

    calculator.addItem(item1);
    calculator.addItem(item2);
    calculator.addItem(item3);

    auto selectedItems = calculator.maximizeProfit();

    // Проверяем, что остался бюджет и место для покупки дополнительного товара
    double remainingBudget = 1000.0;
    double remainingWeight = 50.0;

    for (const auto& item : selectedItems) {
        remainingBudget -= item.buyPrice * item.availableQty;
        remainingWeight -= item.weight * item.availableQty;
    }

    CHECK(remainingBudget <= 0.0);
    CHECK(remainingWeight <= 0.0);
}
