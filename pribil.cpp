#include "pribil.h"
#include <algorithm>
#include <iostream>

ProfitCalculator::ProfitCalculator(double budget, double maxWeight)
    : budget(budget), maxWeight(maxWeight) {
    if (budget <= 0 || maxWeight <= 0) {
        throw std::invalid_argument("Бюджет и грузоподъёмность должны быть положительными числами.");
    }
}

void ProfitCalculator::addItem(const Item& item) {
    item.validate(); // Проверка корректности данных товара
    items.push_back(item);
}

std::vector<Item> ProfitCalculator::maximizeProfit() {
    validateInputs(); // Проверка корректности входных данных

    // Таблица для динамического программирования
    int n = items.size();
    std::vector<std::vector<double>> dp(n + 1, std::vector<double>(static_cast<int>(maxWeight) + 1, 0));
    std::vector<std::vector<bool>> keep(n + 1, std::vector<bool>(static_cast<int>(maxWeight) + 1, false));

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= maxWeight; ++w) {
            const Item& item = items[i - 1];
            int maxItems = std::min(item.availableQty, static_cast<int>(w / item.weight));
            double cost = maxItems * item.buyPrice;

            if (maxItems > 0 && cost <= budget) {
                double profit = maxItems * (item.sellPrice - item.buyPrice);
                if (profit + dp[i - 1][w] > dp[i - 1][w]) {
                    dp[i][w] = profit + dp[i - 1][w];
                    keep[i][w] = true;
                } else {
                    dp[i][w] = dp[i - 1][w];
                }
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Восстанавливаем выбранные товары
    std::vector<Item> selectedItems;
    double remainingWeight = maxWeight;
    double remainingBudget = budget;

    for (int i = n; i > 0; --i) {
        if (keep[i][static_cast<int>(remainingWeight)]) {
            const Item& item = items[i - 1];
            int maxItems = std::min(item.availableQty, static_cast<int>(remainingWeight / item.weight));
            double cost = maxItems * item.buyPrice;

            if (cost <= remainingBudget) {
                selectedItems.push_back(item);
                remainingWeight -= maxItems * item.weight;
                remainingBudget -= cost;
            }
        }
    }

    return selectedItems;
}

void ProfitCalculator::validateInputs() const {
    if (items.empty()) {
        throw std::runtime_error("Список товаров пуст. Добавьте хотя бы один товар.");
    }
    if (budget <= 0) {
        throw std::runtime_error("Бюджет должен быть положительным числом.");
    }
    if (maxWeight <= 0) {
        throw std::runtime_error("Грузоподъёмность должна быть положительным числом.");
    }
}