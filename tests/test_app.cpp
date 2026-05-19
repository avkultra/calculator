#include <gtest/gtest.h>
#include "../src/app.h"
#include <string>

class AppTest : public ::testing::Test
{
  protected:
    app::CApplication app;

    int runWithArgs(const std::string& jsonArg)
    {
        char* argv[] =
        {
            const_cast<char*>("calculator"),
            const_cast<char*>(jsonArg.c_str())
        };
        return app.run(2, argv);
    }
};

// Тест сложение
TEST_F(AppTest, AdditionReturnsZero)
{
    std::string json = R"({"operation":"+","operand1":10,"operand2":5})";
    EXPECT_EQ(runWithArgs(json), 0);
}

// Тест вычитание
TEST_F(AppTest, SubtractionReturnsZero)
{
    std::string json = R"({"operation":"-","operand1":20,"operand2":3})";
    EXPECT_EQ(runWithArgs(json), 0);
}

// Тест умножение
TEST_F(AppTest, MultiplicationReturnsZero)
{
    std::string json = R"({"operation":"*","operand1":6,"operand2":7})";
    EXPECT_EQ(runWithArgs(json), 0);
}

// Тест деление
TEST_F(AppTest, DivisionReturnsZero)
{
    std::string json = R"({"operation":"/","operand1":100,"operand2":4})";
    EXPECT_EQ(runWithArgs(json), 0);
}

// Тест степень
TEST_F(AppTest, PowerReturnsZero)
{
    std::string json = R"({"operation":"^","operand1":2,"operand2":8})";
    EXPECT_EQ(runWithArgs(json), 0);
}

// Тест факториал
TEST_F(AppTest, FactorialReturnsZero)
{
    std::string json = R"({"operation":"!","operand":5})";
    EXPECT_EQ(runWithArgs(json), 0);
}

// Тест  деление на ноль
TEST_F(AppTest, DivisionByZeroReturnsError)
{
    std::string json = R"({"operation":"/","operand1":10,"operand2":0})";
    EXPECT_EQ(runWithArgs(json), 1);
}

// Тест невалидный JSON
TEST_F(AppTest, InvalidJsonReturnsError)
{
    std::string json = "invalid json";
    EXPECT_EQ(runWithArgs(json), 1);
}

// Тест неизвестная операция
TEST_F(AppTest, UnknownOperationReturnsError)
{
    std::string json = R"({"operation":"%","operand1":10,"operand2":5})";
    EXPECT_EQ(runWithArgs(json), 1);
}

// Тест отрицательный факториал
TEST_F(AppTest, NegativeFactorialReturnsError)
{
    std::string json = R"({"operation":"!","operand":-5})";
    EXPECT_EQ(runWithArgs(json), 1);
}

// Тест отсутствуют аргументы
TEST_F(AppTest, NoArgumentsReturnsError)
{
    char* argv[] = {const_cast<char*>("calculator")};
    EXPECT_EQ(app.run(1, argv), 1);
}

// Тест отсутствует operand1
TEST_F(AppTest, MissingOperand1ReturnsError)
{
    std::string json = R"({"operation":"+","operand2":5})";
    EXPECT_EQ(runWithArgs(json), 1);
}

// Тест факториал 0
TEST_F(AppTest, FactorialOfZero)
{
    std::string json = R"({"operation":"!","operand":0})";
    EXPECT_EQ(runWithArgs(json), 0);
}
