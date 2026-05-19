#include <gtest/gtest.h>
#include "command_parser.h"
#include <stdexcept>

class CommandParserTest : public ::testing::Test
{
  protected:
    app::CCommandParser parser;
};

TEST_F(CommandParserTest, ParseValidAdditionCommand)
{
    std::string json = R"({"operation":"+","operand1":10,"operand2":5})";
    auto cmd = parser.parse(json);

    EXPECT_EQ(cmd.m_eOperation, app::TCalculationCommand::eOperation::ADD);
    EXPECT_EQ(cmd.m_iOprnd1, 10);
    EXPECT_EQ(cmd.m_iOprnd2, 5);
}

TEST_F(CommandParserTest, ParseValidSubtractionCommand)
{
    std::string json = R"({"operation":"-","operand1":20,"operand2":3})";
    auto cmd = parser.parse(json);

    EXPECT_EQ(cmd.m_eOperation, app::TCalculationCommand::eOperation::SUB);
    EXPECT_EQ(cmd.m_iOprnd1, 20);
    EXPECT_EQ(cmd.m_iOprnd2, 3);
}

TEST_F(CommandParserTest, ParseValidMultiplicationCommand)
{
    std::string json = R"({"operation":"multiply","operand1":6,"operand2":7})";
    auto cmd = parser.parse(json);

    EXPECT_EQ(cmd.m_eOperation, app::TCalculationCommand::eOperation::MUL);
    EXPECT_EQ(cmd.m_iOprnd1, 6);
    EXPECT_EQ(cmd.m_iOprnd2, 7);
}

TEST_F(CommandParserTest, ParseValidDivisionCommand)
{
    std::string json = R"({"operation":"divide","operand1":100,"operand2":4})";
    auto cmd = parser.parse(json);

    EXPECT_EQ(cmd.m_eOperation, app::TCalculationCommand::eOperation::DIV);
    EXPECT_EQ(cmd.m_iOprnd1, 100);
    EXPECT_EQ(cmd.m_iOprnd2, 4);
}

TEST_F(CommandParserTest, ParseValidPowerCommand)
{
    std::string json = R"({"operation":"^","operand1":2,"operand2":8})";
    auto cmd = parser.parse(json);

    EXPECT_EQ(cmd.m_eOperation, app::TCalculationCommand::eOperation::POW);
    EXPECT_EQ(cmd.m_iOprnd1, 2);
    EXPECT_EQ(cmd.m_iOprnd2, 8);
}

TEST_F(CommandParserTest, ParseValidFactorialCommand)
{
    std::string json = R"({"operation":"!","operand":5})";
    auto cmd = parser.parse(json);

    EXPECT_EQ(cmd.m_eOperation, app::TCalculationCommand::eOperation::FACT);
    EXPECT_EQ(cmd.m_iOprnd1, 5);
}

TEST_F(CommandParserTest, MissingOperationFieldThrows)
{
    std::string json = R"({"operand1":10,"operand2":5})";
    EXPECT_THROW(parser.parse(json), std::invalid_argument);
}

TEST_F(CommandParserTest, UnknownOperationThrows)
{
    std::string json = R"({"operation":"%","operand1":10,"operand2":5})";
    EXPECT_THROW(parser.parse(json), std::invalid_argument);
}

TEST_F(CommandParserTest, MissingOperand1Throws)
{
    std::string json = R"({"operation":"+","operand2":5})";
    EXPECT_THROW(parser.parse(json), std::invalid_argument);
}

TEST_F(CommandParserTest, MissingOperand2Throws)
{
    std::string json = R"({"operation":"+","operand1":10})";
    EXPECT_THROW(parser.parse(json), std::invalid_argument);
}

TEST_F(CommandParserTest, InvalidJsonThrows)
{
    std::string json = "invalid json";
    EXPECT_THROW(parser.parse(json), std::runtime_error);
}
