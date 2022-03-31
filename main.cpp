#include "ctre.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <cctype>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <llvm/ADT/StringSwitch.h>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>
#include <variant>

namespace Constants {
enum Type {
  NONE,
  NUMBER_INT,
  NUMBER_FLOAT,
  BRACE_ENCLOSED,
  SQUARE_ENCLOSED,
  FIGURE_ENCLOSED,
  STATEMENT,
  NAME,
  OPERATION,
  NUMBER_TYPE,
  ERROR
};
struct NONE_T {};
enum Character {
  DIGIT,
  OPERATOR,
  LETTER,
  BRACKET,
  SQUARE_BRACKET,
  FIGURE_BRACKET,
  ERR,
  SKIP,
  STATEMENT_END
};
enum Operator { ASSIGNMENT, LOGICAL, BINARY, UNARY, END, MALFORMED };
const std::string Name(Constants::Type type) {
  using namespace Constants;
  switch (type) {
  case NONE:
    return "NONE";
  case NUMBER_INT:
    return "NUMBER_INT";
  case NUMBER_FLOAT:
    return "NUMBER_FLOAT";
  case BRACE_ENCLOSED:
    return "BRACE_ENCLOSED";
  case SQUARE_ENCLOSED:
    return "SQUARE_ENCLOSED";
  case FIGURE_ENCLOSED:
    return "FIGURE_ENCLOSED";
  case STATEMENT:
    return "STATEMENT";
  case NAME:
    return "NAME";
  case OPERATION:
    return "OPERATION";
  case NUMBER_TYPE:
    return "NUMBER_TYPE";
  case ERROR:
    return "ERROR";
  }
  return "NONE";
}
const std::string Name(Constants::Operator type) {
  using namespace Constants;
  switch (type) {
  case ASSIGNMENT:
    return "_ASSIGNMENT";
  case LOGICAL:
    return "_LOGICAL";
  case BINARY:
    return "_BINARY";
  case UNARY:
    return "_UNARY";
  case MALFORMED:
    return "_MALFORMED";
  case END:
    return "_END";
  }
  return "_MALFORMED";
}
} // namespace Constants

struct Token {
  std::string_view str;
  Constants::Type type;
};

struct Operation {
  inline static const std::string operators_starting = "+-/*,^:<>=%|&!";
  inline static const std::vector<std::string> operators = {
      "+=", "-=", "*=", "/=", "%=", "=", "||", "&&", "==", "!=", "<",
      ">",  "<=", ">=", "+",  "-",  "%", "*",  "/",  "^",  "++", "--"};
  std::string op;
  Constants::Operator type;
  Operation() = default;
  Operation(std::string set_op, Constants::Operator set_type) {
    op = set_op;
    type = set_type;
  }
  Operation(Token token) {
    op = std::string{token.str};
    type = Operation::Set_Type(token.str);
  }
  static bool Is_Operation(const char c) {
    return operators_starting.find(c) != std::string::npos;
  }
  static Constants::Operator Set_Type(std::string_view operation) {
    if (operation.empty()) {
      return Constants::END;
    }
    if (auto res = std::find(operators.begin(), operators.end(), operation);
        res != operators.end()) {
      int pos = res - operators.begin();
      if (pos <= 5) {
        return Constants::ASSIGNMENT;
      } else if (pos <= 13) {
        return Constants::LOGICAL;
      } else if (pos <= 19) {
        return Constants::BINARY;
      } else {
        return Constants::UNARY;
      }
    } else {
      return Constants::MALFORMED;
    }
  }
  int8_t Priority() const {
    return llvm::StringSwitch<int8_t>(op)
        .Cases("+=", "-=", "*=", "/=", "%=", "=", 2)
        .Case("||", 3)
        .Case("&&", 4)
        .Cases("==", "!=", 5)
        .Cases("<", ">", "<=", ">=", 6)
        .Cases("+", "-", 7)
        .Cases("%", "*", "/", 8)
        .Case("^", 9)
        .Cases("++", "--", 10)
        .Default(11);
  }
  template <class T> T Execute(auto left_value, auto right_value) {
    return llvm::StringSwitch<T>(op)
        .Case("||", (int)left_value || (int)right_value)
        .Case("&&", (int)left_value && (int)right_value)
        .Case("==", (int)left_value == (int)right_value)
        .Case("!=", (int)left_value != (int)right_value)
        .Case("<", left_value < right_value)
        .Case(">", left_value > right_value)
        .Case("<=", left_value <= right_value)
        .Case(">=", left_value >= right_value)
        .Case("+", left_value + right_value)
        .Case("-", left_value - right_value)
        .Case("%", (int)left_value % (int)right_value)
        .Case("*", left_value * right_value)
        .Case("/", left_value / right_value)
        .Case("^", left_value = pow(left_value, right_value))
        .Default(left_value);
  }
};

Constants::Character Starting_at(const char c) {
  if (c == ' ') {
    return Constants::SKIP;
  } else if (std::isdigit(c)) {
    return Constants::DIGIT;
  } else if (Operation::Is_Operation(c)) {
    return Constants::OPERATOR;
  } else if (c == '(') {
    return Constants::BRACKET;
  } else if (c == '[') {
    return Constants::SQUARE_BRACKET;
  } else if (c == '{') {
    return Constants::FIGURE_BRACKET;
  } else if (std::isalpha(c)) {
    return Constants::LETTER;
  } else if (c == ';') {
    return Constants::STATEMENT_END;
  }
  return Constants::ERR;
}

static constexpr auto float_pattern = ctll::fixed_string{"[0-9]+[.][0-9]+"};
static constexpr auto int_pattern = ctll::fixed_string{"[0-9]+"};

Constants::Type Number_Type(std::string_view s) noexcept {
  if (ctre::match<float_pattern>(s)) {
    return Constants::NUMBER_FLOAT;
  } else if (ctre::match<int_pattern>(s)) {
    return Constants::NUMBER_INT;
  } else {
    return Constants::NONE;
  }
}

std::size_t Traverse_brackets(std::string_view s, char bracket_open,
                              char bracket_closed) {
  int sum = 0, z = 1;
  for (std::size_t k = 1; 1; k++) {
    if (s[k] == bracket_closed && !sum) {
      z = k;
      return k + 1;
    } else if (s[k] == bracket_closed && sum) {
      sum--;
    }
    if (s[k] == bracket_open) {
      sum++;
    }
  }
  return z;
}

Token Pick_out(const std::string_view &str) {
  Constants::Type type = Constants::NONE;
  std::string_view token;
  std::size_t k = 0;
  if (auto statement_end = std::find(str.begin(), str.end(), ';');
      statement_end != str.end() &&
      std::find(str.begin(), str.end(), '{') > statement_end) {
    type = Constants::STATEMENT;
    token = {str.data(), statement_end - str.begin() + 1};
  } else {
    switch (Starting_at(str[0])) {
    case Constants::SKIP:
      type = Constants::NONE;
      token = {str.data(), 1};
      break;
    case Constants::DIGIT: {
      bool zero_dot = true, one_dot = false;
      while (std::isdigit(str[k]) || str[k] == '.') {
        if (str[k] == '.' && zero_dot) {
          zero_dot = false;
          one_dot = true;
        } else if (str[k] == '.' && one_dot) {
          type = Constants::ERROR;
          break;
        }
        k++;
      }
    }
      token = {str.data(), k};
      type = Number_Type(token);
      break;
    case Constants::OPERATOR:
      if (Operation::Is_Operation(str[k + 1])) {
        k = 2;
      } else
        k = 1;
      token = {str.data(), k};
      type = Constants::OPERATION;
      break;
    case Constants::STATEMENT_END:
      token = {str.data(), 1};
      type = Constants::STATEMENT;
      break;
    case Constants::BRACKET:
      token = {str.data(), Traverse_brackets(str, '(', ')')};
      type = Constants::BRACE_ENCLOSED;
      break;
    case Constants::SQUARE_BRACKET:
      token = {str.data(), Traverse_brackets(str, '[', ']')};
      type = Constants::SQUARE_ENCLOSED;
      break;
    case Constants::FIGURE_BRACKET:
      token = {str.data(), Traverse_brackets(str, '{', '}')};
      type = Constants::FIGURE_ENCLOSED;
      break;
    case Constants::LETTER:
      while (std::isalpha(str[k])) {
        k++;
      }
      token = {str.data(), k};
      type = Constants::NAME;
      break;
    case Constants::ERR:
      type = Constants::ERROR;
      break;
    }
  }
  return {token, type};
}

std::vector<Token> Tokenize_string(const std::string_view &elem) {
  std::vector<Token> tokens;
  std::size_t k = 0;
  while (elem.length() - k > 0) {
    auto token = Pick_out({elem.data() + k, elem.length() - k});
    if (token.type == Constants::ERROR) {
      std::cout << "syntactical error" << token.str << std::endl;
      break;
    }
    k += token.str.length();
    if (token.type != Constants::NONE)
      tokens.push_back(token);
  }
  return tokens;
}

struct Logical_token {
  struct Statement {
    std::vector<Logical_token> val;
  };

  struct Brace_enclosed {
    std::vector<Logical_token> val;
    Constants::Type type;
    void printt(bool typ = true) const {
      char brace[2];
      switch (type) {
        using namespace Constants;
      case BRACE_ENCLOSED:
        brace[0] = '(';
        brace[1] = ')';
        break;
      case SQUARE_ENCLOSED:
        brace[0] = '[';
        brace[1] = ']';
        break;
      case FIGURE_ENCLOSED:
        brace[0] = '{';
        brace[1] = '}';
        break;
      default:
        brace[0] = '|';
        brace[1] = '|';
        break;
      }
      std::cout << (typ ? Name(type) : "") << brace[0];
      for (auto &token : val) {
        token.print(type);
      }
      std::cout << brace[1] << (typ ? Name(type) : "");
    }
    void printt_type() const {
      char brace[2];
      switch (type) {
        using namespace Constants;
      case BRACE_ENCLOSED:
        brace[0] = '(';
        brace[1] = ')';
        break;
      case SQUARE_ENCLOSED:
        brace[0] = '[';
        brace[1] = ']';
        break;
      case FIGURE_ENCLOSED:
        brace[0] = '{';
        brace[1] = '}';
        break;
      default:
        brace[0] = '|';
        brace[1] = '|';
        break;
      }
      std::cout << Name(type) << brace[0] << " ";
      for (auto &token : val) {
        token.print_type();
      }
      std::cout << brace[1] << " ";
    }
    std::variant<int, double> Collapse() {
      auto temp = val;
      for (int i = 0; i < temp.size(); ++i) {
        if (temp[i].Type() == Constants::BRACE_ENCLOSED) {
          temp[i] = Number_type{temp[i].Get_val<Brace_enclosed>().Collapse(),
                                temp[i + 1].Get_val<Operation>()};
        }
        auto &&left_value = temp[i].Get_val<Number_type>();
        if (left_value.op.type == Constants::END) {
          return left_value.val;
        }
        auto &&right_value = temp[i + 1].Get_val<Number_type>();
        if (Can_Merge(left_value, right_value)) {
          left_value = Number_type::Execute(left_value, right_value);
          temp.erase(val.begin() + i + 1);
          i = 0;
        }
      }
      val = temp;
    }
  };
  struct Function_call {
    std::string name;
    Brace_enclosed val;
  };
  struct Number_type {
    std::variant<int, double> val;
    Operation op;
    void print(bool type = true) const {
      std::cout << "[[";
      switch (val.index()) {
      case 0:
        std::cout << std::get<int>(val) << "]"
                  << (type ? Constants::Name(Constants::NUMBER_INT) : "");
        break;
      case 1:
        std::cout << std::get<double>(val) << "]"
                  << (type ? Constants::Name(Constants::NUMBER_FLOAT) : "");
        break;
      }
      std::cout << "][" << op.op << "]"
                << (type ? Name(Constants::OPERATION) : "")
                << (type ? Name(op.type) : "") << "]]! ";
    }
    static Number_type Execute(Number_type left_value,
                               Number_type right_value) {
      if ((!left_value.val.index()) && (!right_value.val.index()))
        return Number_type{
            left_value.op.Execute<int>(std::get<int>(left_value.val),
                                       std::get<int>(right_value.val)),
            right_value.op};
      else {
        double left_val, right_val;
        if (left_value.val.index()) {
          left_val = std::get<int>(left_value.val) * 1.0;
        } else {
          left_val = std::get<double>(left_value.val);
        }
        if (right_value.val.index()) {
          right_val = std::get<int>(right_value.val) * 1.0;
        } else {
          right_val = std::get<double>(right_value.val);
        }
        return Number_type{left_value.op.Execute<int>(left_val, right_val),
                           right_value.op};
      }
    }
  };
  static bool Can_Merge(Number_type first, Number_type second) {
    return first.op.Priority() >= second.op.Priority();
  }
  template <class T> T Get_val() const { return std::get<T>(value); }

  std::variant<Constants::NONE_T, int, double, Brace_enclosed, Statement,
               std::string, Operation, Number_type>
      value;
  Logical_token(Number_type num_token) { value = num_token; }
  Logical_token(Token num_token, Token op_token) {
    Number_type res_token;
    switch (num_token.type) {
    case Constants::NUMBER_INT: {
      int result;
      std::from_chars(num_token.str.begin(), num_token.str.end(), result);
      res_token.val = result;
    } break;
    case Constants::NUMBER_FLOAT: {
      std::string str = {num_token.str.begin(), num_token.str.end()};
      res_token.val = std::stod(str);
    } break;
    }
    res_token.op =
        Operation{std::string{op_token.str}, Operation::Set_Type(op_token.str)};
    value = res_token;
  }
  Logical_token(Token token) {
    switch (token.type) {
    case Constants::NONE:
      value = Constants::NONE_T{};
      break;
    case Constants::NUMBER_INT: {
      int result;
      std::from_chars(token.str.begin(), token.str.end(), result);
      value = result;
    } break;
    case Constants::NUMBER_FLOAT: {
      std::string str = {token.str.begin(), token.str.end()};
      value = std::stod(str);
    } break;
    case Constants::BRACE_ENCLOSED:
    case Constants::SQUARE_ENCLOSED:
    case Constants::FIGURE_ENCLOSED: {
      std::string_view elem{token.str.data() + 1, token.str.size() - 2};
      std::vector<Token> tokens = Tokenize_string(elem);
      tokens.push_back({"", Constants::OPERATION});
      std::vector<Logical_token> temp;
      auto &logic_tokens = temp;
      /*for (auto &tkn : tokens) {
        logic_tokens.emplace_back(tkn);
      }
      */
      /*Token hold = {"", Constants::NONE};
      std::size_t k = 0;
      for (auto &tkn : tokens) {
        k++;
        if(tkn.type == Constants::BRACE_ENCLOSED){
          auto braces_token = Logical_token(tkn);
          auto braces = braces_token.Get_val<Brace_enclosed>();
          Number_type{braces.Collapse(),""};
        }


        if (k != tokens.size() && (tkn.type == Constants::NUMBER_INT ||
                                   tkn.type == Constants::NUMBER_FLOAT)) {
          hold = tkn;
        } else if (k == tokens.size() &&
                   (tkn.type == Constants::NUMBER_INT ||
                    tkn.type == Constants::NUMBER_FLOAT)) {
          logic_tokens.emplace_back(
              Logical_token(tkn, Token{"", Constants::NONE}));
        } else if (tkn.type == Constants::OPERATION &&
                   hold.type != Constants::NONE) {
          logic_tokens.emplace_back(Logical_token(hold, tkn));
        } else {
          logic_tokens.emplace_back(tkn);
        }
      }*/
      for (int k = 0; k < tokens.size(); k += 2) {
        auto &tkn = tokens[k];
        if (tkn.type == Constants::BRACE_ENCLOSED) {
          auto braces_token = Logical_token(tkn);
          auto braces = braces_token.Get_val<Brace_enclosed>();
          logic_tokens.emplace_back(
              Number_type{braces.Collapse(), Operation{tokens[k + 1]}});
        } else {
          if ((tkn.type == Constants::NUMBER_INT ||
               tkn.type == Constants::NUMBER_FLOAT)) {
            logic_tokens.emplace_back(Logical_token{tkn, tokens[k + 1]});
          }
        }
      }
      auto temp_braces = Brace_enclosed{temp, token.type};
      value = temp_braces;
    } break;
    case Constants::NAME:
      value = std::string{token.str};
      break;
    case Constants::OPERATION:
      value = Operation{token};
      break;
    case Constants::STATEMENT: {
      std::string_view elem{token.str.data(), token.str.size() - 1};
      std::vector<Token> tokens = Tokenize_string(elem);
      std::vector<Logical_token> temp;
      auto &logic_tokens = temp;
      Token hold = {"", Constants::NONE};
      std::size_t k = 0;
      for (auto &tkn : tokens) {
        k++;
        if (k != tokens.size() && (tkn.type == Constants::NUMBER_INT ||
                                   tkn.type == Constants::NUMBER_FLOAT)) {
          hold = tkn;
        } else if (k == tokens.size() &&
                   (tkn.type == Constants::NUMBER_INT ||
                    tkn.type == Constants::NUMBER_FLOAT)) {
          logic_tokens.emplace_back(
              Logical_token(tkn, Token{"", Constants::NONE}));
        } else if (tkn.type == Constants::OPERATION &&
                   hold.type != Constants::NONE) {
          logic_tokens.emplace_back(Logical_token(hold, tkn));
        } else {
          logic_tokens.emplace_back(tkn);
        }
      }
      value = Statement{temp};

    } break;
    }
  }

  Logical_token(std::vector<Logical_token> statement) {
    value = Statement{statement};
  }

  Logical_token() {}
  Constants::Type Type() const {
    switch (value.index()) {
    case 0:
      return Constants::NONE;
    case 1:
      return Constants::NUMBER_INT;
    case 2:
      return Constants::NUMBER_FLOAT;
    case 3:
      return std::get<Brace_enclosed>(value).type;
    case 4:
      return Constants::STATEMENT;
    case 5:
      return Constants::NAME;
    case 6:
      return Constants::OPERATION;
    case 7:
      return Constants::NUMBER_TYPE;
    }
  }
  // template
  void print(bool type = true) const {
    using namespace Constants;
    switch (value.index()) {
    case 0:
      std::cout << (type ? Name(NONE) : "") << "";
      break;
    case 1:
      std::cout << "[" << std::get<int>(value) << "]"
                << (type ? Name(NUMBER_INT) : "") << "";
      break;
    case 2:
      std::cout << "[" << std::get<double>(value) << "]"
                << (type ? Name(NUMBER_FLOAT) : "") << "";
      break;
    case 3:
      std::get<Brace_enclosed>(value).printt();
      break;
    case 4:
      std::cout << (type ? Name(STATEMENT) : "") << "|";
      for (auto &token : std::get<Statement>(value).val) {
        token.print(type);
      }
      std::cout << ";|\n" << (type ? Name(STATEMENT) : "");
      break;
    case 5:
      std::cout << "[" << std::get<std::string>(value) << "]"
                << (type ? Name(NAME) : "") << "";
      break;

    case 6:
      std::cout << "[" << std::get<Operation>(value).op << "]"
                << (type ? Name(OPERATION) : "")
                << (type ? Name(std::get<Operation>(value).type) : "") << "";
      break;
    case 7:
      std::get<Number_type>(value).print(type);
      break;
    }
  }
  void print_type() const {
    using namespace Constants;
    switch (value.index()) {
    case 0:
      std::cout << Name(NONE) << " ";
      break;
    case 1:
      std::cout << Name(NUMBER_INT) << " ";
      break;
    case 2:
      std::cout << Name(NUMBER_FLOAT) << " ";
      break;
    case 3:
      std::get<Brace_enclosed>(value).printt_type();
      break;
    case 4:
      std::cout << Name(STATEMENT) << " |";
      for (auto &token : std::get<Statement>(value).val) {
        token.print_type();
      }
      std::cout << "|;\n";
      break;
    case 5:
      std::cout << std::get<std::string>(value) << " ";
      break;
    case 6:
      std::cout << Name(OPERATION) << Name(std::get<Operation>(value).type)
                << " ";
      break;
    case 7:
      std::get<Number_type>(value).print();
      break;
    }
  }
};

std::string get_statements() {
  std::string command;
  std::ifstream t("c:/Users/miste/Desktop/compiler/test.txt");
  t.seekg(0, std::ios::end);
  command.reserve(t.tellg());
  t.seekg(0, std::ios::beg);

  std::stringstream strStream;
  strStream << t.rdbuf();
  command = strStream.str();
  boost::erase_all(command, "\r\n");
  boost::erase_all(command, "\n");
  boost::erase_all(command, "  ");
  return command;
}

int main() {
  auto command = get_statements();
  command += ";";

  Logical_token result(Token{std::string_view{command.begin(), command.end()},
                             Constants::STATEMENT});
  result.print(false);
  std::cout << "\n\n";
  result.print_type();
  return 0;
}