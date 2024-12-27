#include <iostream>
#include <limits>
#include "pribil.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double getValidDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0) {
            std::cout << "error\n";
            clearInput();
        } else {
            return value;
        }
    }
}

int getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0) {
            std::cout << "error\n";
            clearInput();
        } else {
            return value;
        }
    }
}

double getValidSellPrice(double buyPrice) {
    double sellPrice;
    while (true) {
        std::cout << "Enter the resale price of the item (must be greater than the wholesale price): ";
        std::cin >> sellPrice;
        if (std::cin.fail() || sellPrice <= buyPrice) {
            std::cout << "error: resale price cannot be less than or equal to wholesale price\n";
            clearInput();
        } else {
            return sellPrice;
        }
    }
}

int main() {
    try {
        double budget = getValidDouble("enter available budget: ");
        double maxWeight = getValidDouble("enter maximum load capacity of truck: ");
        int numItems = getValidInt("enter quantity of available goods: ");

        ProfitCalculator calculator(budget, maxWeight);

        for (int i = 0; i < numItems; ++i) {
            Item item;
            std::cout << "\nТовар #" << i + 1 << "\n";
            std::cout << "enter product name: ";
            std::cin >> item.name;

            item.weight = getValidDouble("enter the weight of the product: ");
            item.buyPrice = getValidDouble("enter the wholesale price of the product: ");
            item.sellPrice = getValidSellPrice(item.buyPrice);  // Используем новую функцию
            item.availableQty = getValidInt("enter the available quantity of the product: ");

            calculator.addItem(item);
        }

        auto selectedItems = calculator.maximizeProfit();

        std::cout << "\nselected products:\n";
        for (const auto& item : selectedItems) {
            std::cout << "product: " << item.name
                      << ", quantity: " << item.availableQty
                      << ", profit: " << (item.sellPrice - item.buyPrice) * item.availableQty << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
    }

    return 0;
}
