#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "pribil.h"

TEST_CASE("Item validation") {
    Item item;

    SUBCASE("Valid item") {
        item = {"Product1", 10.0, 5.0, 8.0, 10};
        CHECK_NOTHROW(item.validate());
    }

    SUBCASE("Invalid weight") {
        item = {"Product1", -10.0, 5.0, 8.0, 10};
        CHECK_THROWS_AS(item.validate(), std::invalid_argument);
    }

    SUBCASE("Invalid buy price") {
        item = {"Product1", 10.0, -5.0, 8.0, 10};
        CHECK_THROWS_AS(item.validate(), std::invalid_argument);
    }

    SUBCASE("Invalid sell price") {
        item = {"Product1", 10.0, 5.0, -8.0, 10};
        CHECK_THROWS_AS(item.validate(), std::invalid_argument);
    }

    SUBCASE("Invalid available quantity") {
        item = {"Product1", 10.0, 5.0, 8.0, -10};
        CHECK_THROWS_AS(item.validate(), std::invalid_argument);
    }

    SUBCASE("Buy price higher than sell price") {
        item = {"Product1", 10.0, 10.0, 8.0, 10};
        CHECK_THROWS_AS(item.validate(), std::logic_error);
    }
}

TEST_CASE("ProfitCalculator constructor validation") {
    CHECK_NOTHROW(ProfitCalculator(100.0, 50.0));
    CHECK_THROWS_AS(ProfitCalculator(-100.0, 50.0), std::invalid_argument);
    CHECK_THROWS_AS(ProfitCalculator(100.0, -50.0), std::invalid_argument);
    CHECK_THROWS_AS(ProfitCalculator(0.0, 50.0), std::invalid_argument);
}

TEST_CASE("Adding items to ProfitCalculator") {
    ProfitCalculator calculator(100.0, 50.0);

    SUBCASE("Valid item") {
        Item item = {"Product1", 10.0, 5.0, 8.0, 10};
        CHECK_NOTHROW(calculator.addItem(item));
    }

    SUBCASE("Invalid item") {
        Item item = {"Product1", 10.0, 5.0, 3.0, 10}; // Buy price > Sell price
        CHECK_THROWS_AS(calculator.addItem(item), std::logic_error);
    }
}

TEST_CASE("ProfitCalculator maximizeProfit") {
    ProfitCalculator calculator(100.0, 50.0);

    SUBCASE("Single item fits constraints") {
        Item item = {"Product1", 10.0, 5.0, 8.0, 10};
        calculator.addItem(item);
        auto result = calculator.maximizeProfit();

        REQUIRE(result.size() == 1);
        CHECK(result[0].name == "Product1");
        CHECK(result[0].availableQty == 10);
    }

    SUBCASE("Multiple items, budget and weight constraints") {
        calculator.addItem({"Product1", 10.0, 5.0, 8.0, 5});
        calculator.addItem({"Product2", 5.0, 3.0, 6.0, 10});
        auto result = calculator.maximizeProfit();

        double totalWeight = 0.0;
        double totalProfit = 0.0;

        for (const auto& item : result) {
            totalWeight += item.weight * item.availableQty;
            totalProfit += (item.sellPrice - item.buyPrice) * item.availableQty;
        }

        CHECK(totalWeight <= 50.0);
        CHECK(totalProfit > 0.0);
    }

    SUBCASE("No items available") {
        CHECK_THROWS_AS(calculator.maximizeProfit(), std::runtime_error);
    }
}
