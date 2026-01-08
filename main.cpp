#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>

using namespace std;

static const std::unordered_set<std::string_view> CPP_KEYWORDS = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto",
    "bitand", "bitor", "bool", "break",
    "case", "catch", "char", "char8_t", "char16_t", "char32_t",
    "class", "compl", "concept", "const", "consteval",
    "constexpr", "constinit", "const_cast", "continue",
    "co_await", "co_return", "co_yield",
    "decltype", "default", "delete", "do", "double", "dynamic_cast",
    "else", "enum", "explicit", "export", "extern",
    "false", "float", "for", "friend",
    "goto",
    "if", "inline", "int",
    "long",
    "mutable", "module",
    "namespace", "new", "noexcept", "not", "not_eq", "nullptr",
    "operator", "or", "or_eq",
    "private", "protected", "public",
    "register", "reinterpret_cast", "requires", "return",
    "short", "signed", "sizeof", "static", "static_assert",
    "static_cast", "struct", "switch",
    "template", "this", "thread_local", "throw", "true",
    "try", "typedef", "typeid", "typename",
    "union", "unsigned", "using",
    "virtual", "void", "volatile",
    "wchar_t", "while",
    "xor", "xor_eq"};

static const std::unordered_set<std::string_view> CPP_OPERATORS = {"+=", "-=", "*=", "/=", "==", "=", "+", "-", "/", "*"};
static const std::unordered_set<std::string_view> CPP_SPECIAL_SYMBOLS = {";", ",", ".", "(", ")", "{", "}"};

enum class TokenType
{
    Operator,
    Separator,
    Keyword,
    Literal,
    Identifier,
    Unknown
};

string printType(TokenType type)
{
    switch (type)
    {
    case TokenType::Operator:
        return "OPERATOR";
    case TokenType::Separator:
        return "SEPARATOR";
    case TokenType::Keyword:
        return "KEYWORD";
    case TokenType::Literal:
        return "LITERAL";
    case TokenType::Identifier:
        return "IDENTIFIER";
    }
    return "UNKNOWN";
}

struct Token
{
    TokenType type;
    string lexeme;

    void printToken()
    {
        cout << "Type: " << printType(type) << " Data: " << lexeme << '\n';
    }
};

bool isNumber(const string &str)
{
    for (char c : str)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

vector<Token> Lexer(vector<string> &Lines)
{
    vector<Token> TokenVector;

    for (auto _token : Lines)
    {
        if (CPP_KEYWORDS.find(_token) != CPP_KEYWORDS.end())
        {
            TokenVector.push_back(Token{TokenType::Keyword, _token});
            continue;
        }
        else if (CPP_OPERATORS.find(_token) != CPP_OPERATORS.end())
        {
            TokenVector.push_back(Token{TokenType::Operator, _token});
            continue;
        }
        else if (CPP_SPECIAL_SYMBOLS.find(_token) != CPP_SPECIAL_SYMBOLS.end())
        {
            TokenVector.push_back(Token{TokenType::Separator, _token});
            continue;
        }
        else if ((_token[0] == '"' && _token[_token.size() - 1] == '"') || isNumber(_token))
        {
            TokenVector.push_back(Token{TokenType::Literal, _token});
            continue;
        }
        else if (isalpha(_token[0]))
        {
            TokenVector.push_back(Token{TokenType::Identifier, _token});
            continue;
        }
        else
        {
            TokenVector.push_back(Token{TokenType::Unknown, _token});
        }
    }

    return TokenVector;
}

vector<string> removeWhitespace(vector<string> &lines)
{
    vector<string> noWhitespaceVector;

    for (string line : lines)
    {
        if (line.size() < 1)
            continue;
        else if (line[0] == '/' && line[1] == '/')
            continue;

        string holderString;

        int iterator = 0;
        while (iterator < line.size())
        {
            if (line[iterator] == ' ' || line[iterator] == '\n' || line[iterator] == '\t' || line[iterator] == '\r')
            {
                if (!holderString.empty())
                    noWhitespaceVector.push_back(holderString);

                holderString.clear();
                iterator++;
                continue;
            }
            else if (line[iterator] == ',' || line[iterator] == ';' || line[iterator] == '.' || line[iterator] == '{' || line[iterator] == '}' || line[iterator] == '(' || line[iterator] == ')')
            {
                if (!holderString.empty())
                {
                    noWhitespaceVector.push_back(holderString);
                    holderString.clear();
                }

                holderString += line[iterator];
                noWhitespaceVector.push_back(holderString);
                holderString.clear();
                iterator++;
                continue;
            }
            else if (line[iterator] == '"') // Count everything inside quotes as one token
            {
                holderString += line[iterator];
                iterator++;
                while (iterator < line.size() && line[iterator] != '"')
                {
                    holderString += line[iterator];
                    iterator++;
                }
                noWhitespaceVector.push_back(holderString);
                holderString.clear();
                continue;
            }

            holderString += line[iterator];
            iterator++;
        }

        if (!holderString.empty())
            noWhitespaceVector.push_back(holderString);
    }

    return noWhitespaceVector;
}

vector<string> getLines(const string &inputFileName)
{
    ifstream FILE;
    FILE.open(inputFileName);

    vector<string> lineVector;

    if (FILE.is_open())
    {
        string line;

        while (getline(FILE, line))
        {
            lineVector.push_back(line);
        }

        FILE.close();
    }

    return lineVector;
}

int main(int argc, char *argv[])
{
    const string fileName = argv[1];
    vector<string> Lines = getLines(fileName);
    vector<string> noWhitespace = removeWhitespace(Lines);
    vector<Token> parsed = Lexer(noWhitespace);

    for (auto _token : parsed)
    {
        _token.printToken();
        cout << '\n';
    }

    return 0;
}