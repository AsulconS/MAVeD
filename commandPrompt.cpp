/********************************************************************************
 *                                                                              *
 * MAVeD - MilanverDatabase                                                     *
 * Copyright (c) 2020 Adrian Bedregal (AsulconS)                                *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty. In no event will the authors be held liable for any damages        *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 *    claim that you wrote the original software. If you use this software      *
 *    in a product, an acknowledgment in the product documentation would be     *
 *    appreciated but is not required.                                          *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 *    misrepresented as being the original software.                            *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 ********************************************************************************/

#include <vector>
#include <string>

#include <fstream>
#include <iostream>
#include <streambuf>

#include <unordered_map>

#define EXIT_CODE_LENGTH 5
#define MAX_ANYWORD_LENGTH 64
#define MAX_INTLITR_LENGTH 32
#define MAX_STRINGL_LENGTH 8000

#define NETWORK_PACKET_SIZE (4 * 1024)
#define BATCH_SIZE_LIMIT (65536 * NETWORK_PACKET_SIZE)

static std::unordered_map<std::string, std::pair<bool, unsigned char>> keywords =
{
    {"add",                     {true, 0}},
    {"add constraint",          {true, 1}},
    {"alter",                   {true, 0}},
    {"alter column",            {true, 1}},
    {"alter table",             {true, 1}},
    {"all",                     {true, 0}},
    {"and",                     {true, 0}},
    {"any",                     {true, 0}},
    {"as",                      {true, 0}},
    {"asc",                     {true, 0}},
    {"auto_increment",          {true, 0}},

    {"backup",                  {true, 0}},
    {"backup database",         {true, 1}},
    {"begin",                   {true, 0}},
    {"between",                 {true, 0}},
    {"by",                      {true, 0}},

    {"cascade",                 {true, 0}},
    {"case",                    {true, 0}},
    {"check",                   {true, 0}},
    {"column",                  {true, 0}},
    {"commit",                  {true, 0}},
    {"constraint",              {true, 0}},
    {"count",                   {true, 0}},
    {"create",                  {true, 0}},
    {"create database",         {true, 1}},
    {"create index",            {true, 1}},
    {"create or replace view",  {true, 3}},
    {"create table",            {true, 1}},
    {"create procedure",        {true, 1}},
    {"create unique index",     {true, 2}},
    {"create view",             {true, 1}},

    {"database",                {true, 0}},
    {"default",                 {true, 0}},
    {"delete",                  {true, 0}},
    {"desc",                    {true, 0}},
    {"distinct",                {true, 0}},
    {"drop",                    {true, 0}},
    {"drop column",             {true, 1}},
    {"drop constraint",         {true, 1}},
    {"drop database",           {true, 1}},
    {"drop default",            {true, 1}},
    {"drop index",              {true, 1}},
    {"drop table",              {true, 1}},
    {"drop view",               {true, 1}},

    {"end",                     {true, 0}},
    {"exec",                    {true, 0}},
    {"exists",                  {true, 0}},

    {"false",                   {true, 0}},
    {"foreign",                 {true, 0}},
    {"foreign key",             {true, 1}},
    {"from",                    {true, 0}},
    {"full outer join",         {true, 2}},

    {"group",                   {true, 0}},
    {"group by",                {true, 1}},

    {"having",                  {true, 0}},

    {"if",                      {true, 0}},
    {"ifnull",                  {true, 0}},
    {"in",                      {true, 0}},
    {"index",                   {true, 0}},
    {"inner",                   {true, 0}},
    {"inner join",              {true, 1}},
    {"insert into",             {true, 1}},
    {"insert into select",      {true, 2}},
    {"into",                    {true, 0}},
    {"is null",                 {true, 1}},
    {"is not null",             {true, 2}},

    {"join",                    {true, 0}},

    {"key",                     {true, 0}},

    {"left",                    {true, 0}},
    {"left join",               {true, 1}},
    {"like",                    {true, 0}},
    {"limit",                   {true, 0}},

    {"modify",                  {true, 0}},

    {"not",                     {true, 0}},
    {"not null",                {true, 1}},
    {"null",                    {true, 0}},

    {"on",                      {true, 0}},
    {"or",                      {true, 0}},
    {"order",                   {true, 0}},
    {"order by",                {true, 1}},
    {"outer",                   {true, 0}},
    {"outer join",              {true, 1}},

    {"primary",                 {true, 0}},
    {"primary key",             {true, 1}},
    {"procedure",               {true, 0}},

    {"references",              {true, 0}},
    {"replace",                 {true, 0}},
    {"restrict",                {true, 0}},
    {"right",                   {true, 0}},
    {"right join",              {true, 1}},
    {"rollback",                {true, 0}},
    {"rownum",                  {true, 0}},

    {"select",                  {true, 0}},
    {"select distinct",         {true, 1}},
    {"select into",             {true, 1}},
    {"select top",              {true, 1}},
    {"set",                     {true, 0}},
    {"sum",                     {true, 0}},

    {"table",                   {true, 0}},
    {"top",                     {true, 0}},
    {"transaction",             {true, 0}},
    {"true",                    {true, 0}},
    {"truncate",                {true, 0}},
    {"truncate table",          {true, 1}},

    {"union",                   {true, 0}},
    {"union all",               {true, 1}},
    {"unique",                  {true, 0}},
    {"update",                  {true, 0}},
    {"using",                   {true, 0}},

    {"values",                  {true, 0}},
    {"view",                    {true, 0}},

    {"where",                   {true, 0}},

    // Types

    {"char",                    {true, 0}},
    {"varchar",                 {true, 0}},
    {"binary",                  {true, 0}},
    {"varbinary",               {true, 0}},
    {"tinyblob",                {true, 0}},
    {"tinytext",                {true, 0}},
    {"text",                    {true, 0}},
    {"blob",                    {true, 0}},
    {"mediumtext",              {true, 0}},
    {"mediumblob",              {true, 0}},
    {"longtext",                {true, 0}},
    {"longblob",                {true, 0}},
    {"enum",                    {true, 0}},

    {"bit",                     {true, 0}},
    {"tinyint",                 {true, 0}},
    {"bool",                    {true, 0}},
    {"boolean",                 {true, 0}},
    {"smallint",                {true, 0}},
    {"mediumint",               {true, 0}},
    {"int",                     {true, 0}},
    {"integer",                 {true, 0}},
    {"bigint",                  {true, 0}},
    {"float",                   {true, 0}},
    {"double",                  {true, 0}},
    {"double precision",        {true, 1}},
    {"decimal",                 {true, 0}},
    {"dec",                     {true, 0}},

    {"date",                    {true, 0}},
    {"datetime",                {true, 0}},
    {"timestamp",               {true, 0}},
    {"time",                    {true, 0}},
    {"year",                    {true, 0}}
};

static std::unordered_map<char, bool> separators =
{
    {'(', true},
    {')', true},
    {'{', true},
    {'}', true},
    {'.', true},
    {',', true},
    {';', true}
};

static std::unordered_map<std::string, bool> operators =
{
    {"=", true},
    {"!", true},
    {"+", true},
    {"-", true},
    {"*", true},
    {"/", true},
    {"<", true},
    {">", true},
    {"==", true},
    {"!=", true},
    {"//", true},
    {"<=", true},
    {">=", true},
    {"<>", true}
};

char cWordBuffer[MAX_ANYWORD_LENGTH];
char cIntlBuffer[MAX_INTLITR_LENGTH];
char cStrlBuffer[MAX_STRINGL_LENGTH];

bool isWhitespace(char c)
{
    return c == ' ' || c == '\n' || c == '\r';
}

bool hasExitCode(const std::string& str)
{
    return (str[0] == 'e') && (str[1] == 'x') && (str[2] == 'i') && (str[3] == 't') && (str[4] == '\0');
}

void toLower(char* str)
{
    while(*str != '\0')
    {
        if((*str - 'A') * ('Z' - *str) >= 0)
        {
            *str += 32;
        }
        ++str;
    }
}

void postWord(unsigned char& w, unsigned char& i, bool& wd, bool& op, bool& il, std::vector<std::string>& expression,
                                                                                std::vector<std::string>& tExpression,
                                                                                std::vector<std::string>& tIdentifiers,
                                                                                std::vector<std::string>& tKeywords,
                                                                                std::vector<std::string>& tOperators,
                                                                                std::vector<std::string>& tLiterals)
{
    if(wd)
    {
        cWordBuffer[w] = '\0';
        toLower(cWordBuffer);
        if(keywords[cWordBuffer].first)
        {
            expression.push_back(cWordBuffer);
            tExpression.push_back("KW");
            tKeywords.push_back(cWordBuffer);
        }
        else
        {
            expression.push_back(cWordBuffer);
            tExpression.push_back("ID");
            tIdentifiers.push_back(cWordBuffer);
        }
        wd = false;
        w = 0;
    }
    else if(op)
    {
        cWordBuffer[w] = '\0';
        expression.push_back(cWordBuffer);
        tExpression.push_back("OP");
        tOperators.push_back(cWordBuffer);
        op = false;
        w = 0;
    }
    else if(il)
    {
        cIntlBuffer[i] = '\0';
        expression.push_back(cIntlBuffer);
        tExpression.push_back("IL");
        tLiterals.push_back(cIntlBuffer);
        il = false;
        i = 0;
    }
}

bool tokenize(const std::string& str)
{
    if(str.empty())
    {
        return true;
    }

    cWordBuffer[0] = '\0';
    bool wd {false}; // word flag
    bool op {false}; // operator flag
    bool il {false}; // integer literal flag
    bool sl {false}; // string literal flag
    bool cm {false}; // commentary flag

    char slDelimiter; // string literal delimiter [' "]

    std::vector<std::string> expression {};
    std::vector<std::string> tExpression {};

    std::vector<std::string> tIdentifiers {};
    std::vector<std::string> tKeywords {};
    std::vector<char> tSeparators {};

    std::vector<std::string> tOperators {};
    std::vector<std::string> tLiterals {};

    unsigned int t {0};     // tokenizable string index counter
    unsigned char w {0};    // word buffer index counter
    unsigned char i {0};    // integer literal buffer index counter
    unsigned short s {0};   // string literal buffer index counter
    while(str[t] != '\0')
    {
        if(cm) // If it's being commented, ignore and pass
        {
            if(str[t] == '\n' || str[t] == '\r') // Stop commentary mode at \n or \r
            {
                cm = false;
            }
            ++t;
            continue;
        }
        else if(sl) // If it's being inserted any string literal, write to its buffer and pass
        {
            if(str[t] == slDelimiter) // Stop string literal mode at its delimiter [' "]
            {
                cStrlBuffer[s] = '\0';
                expression.push_back(cStrlBuffer);
                tExpression.push_back("SL");
                tLiterals.push_back(cStrlBuffer);
                sl = false;
                s = 0;
            }
            else
            {
                cStrlBuffer[s] = str[t];
                ++s;
            }
            ++t;
            continue;
        }

        if(isWhitespace(str[t]))
        {
            postWord(w, i, wd, op, il, expression, tExpression, tIdentifiers, tKeywords, tOperators, tLiterals);
        }
        else if(separators[str[t]])
        {
            postWord(w, i, wd, op, il, expression, tExpression, tIdentifiers, tKeywords, tOperators, tLiterals);
            expression.push_back({str[t]});
            tExpression.push_back("SEP");
            tSeparators.push_back(str[t]);
        }
        else if(operators[{str[t]}])
        {
            if(op)
            {
                cWordBuffer[w] = str[t];
                cWordBuffer[w + 1] = '\0';
                if(operators[cWordBuffer])
                {
                    expression.push_back(cWordBuffer);
                    tExpression.push_back("OP");
                    tOperators.push_back(cWordBuffer);
                }
                else
                {
                    std::cerr << "Invalid Operator at expression" << std::endl;
                }
                op = false;
                w = 0;
            }
            else
            {
                postWord(w, i, wd, op, il, expression, tExpression, tIdentifiers, tKeywords, tOperators, tLiterals);
                cWordBuffer[w] = str[t];
                op = true;
                ++w;
            }
        }
        else if(str[t] == '$')
        {
            cm = true;
        }
        else
        {
            if(!wd && !il)
            {
                postWord(w, i, wd, op, il, expression, tExpression, tIdentifiers, tKeywords, tOperators, tLiterals);
                if((str[t] - '0') * ('9' - str[t]) >= 0)
                {
                    il = true;
                }
                else if(str[t] == '\'' || str[t] == '\"')
                {
                    slDelimiter = str[t];
                    sl = true;
                    ++t;
                    continue;
                }
                else
                {
                    wd = true;
                }
            }

            if(wd)
            {
                cWordBuffer[w] = str[t];
                ++w;
            }
            else if(il)
            {
                cIntlBuffer[i] = str[t];
                ++i;
            }
        }
        ++t;
    }
    postWord(w, i, wd, op, il, expression, tExpression, tIdentifiers, tKeywords, tOperators, tLiterals);

    if(expression.size() == 1)
    {
        if(expression[0] == "exit")
        {
            return false;
        }
        else if(expression[0] == "help")
        {
            std::cout << "Type \"src\" to load a query from a SQL file." << std::endl;
            std::cout << "There's not too much help yet :(" << std::endl;
        }
        else if(expression[0] == "copyright")
        {
            std::cout << "/******************************************************************************** \n";
            std::cout << " * MAVeD - MilanverDatabase (Ice Bucket) | Command Prompt v0.3a 2020            * \n";
            std::cout << " * Copyright (c) 2020 Adrian Bedregal (AsulconS)                                * \n";
            std::cout << " ********************************************************************************/\n";
        }
        else if(expression[0] == "license")
        {
            std::cout << "/******************************************************************************** \n";
            std::cout << " *                                                                              * \n";
            std::cout << " * MAVeD - MilanverDatabase (Ice Bucket) | Command Prompt v0.3a 2020            * \n";
            std::cout << " * Copyright (c) 2020 Adrian Bedregal (AsulconS)                                * \n";
            std::cout << " *                                                                              * \n";
            std::cout << " * This software is provided 'as-is', without any express or implied            * \n";
            std::cout << " * warranty. In no event will the authors be held liable for any damages        * \n";
            std::cout << " * arising from the use of this software.                                       * \n";
            std::cout << " *                                                                              * \n";
            std::cout << " * Permission is granted to anyone to use this software for any purpose,        * \n";
            std::cout << " * including commercial applications, and to alter it and redistribute it       * \n";
            std::cout << " * freely, subject to the following restrictions:                               * \n";
            std::cout << " *                                                                              * \n";
            std::cout << " * 1. The origin of this software must not be misrepresented; you must not      * \n";
            std::cout << " *    claim that you wrote the original software. If you use this software      * \n";
            std::cout << " *    in a product, an acknowledgment in the product documentation would be     * \n";
            std::cout << " *    appreciated but is not required.                                          * \n";
            std::cout << " * 2. Altered source versions must be plainly marked as such, and must not be   * \n";
            std::cout << " *    misrepresented as being the original software.                            * \n";
            std::cout << " * 3. This notice may not be removed or altered from any source distribution.   * \n";
            std::cout << " *                                                                              * \n";
            std::cout << " ********************************************************************************/\n";
        }
        else if(expression[0] == "src")
        {
            std::string path {};
            std::cout << "Type the path to the SQL source file: ";
            std::cin >> path;

            std::ifstream t(path);
            std::string src((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

            return tokenize(src);
        }
        else
        {
            std::cerr << "Invalid core function call" << std::endl;
        }
        
        return true;
    }

    std::cout << "Tokens:\n\n";

    std::cout << "Identifiers: [ ";
    for(const std::string& s : tIdentifiers)
    {
        std::cout << '\"' << s << '\"' << ' ';
    }
    std::cout << ']';
    std::cout << std::endl;

    std::cout << "Keywords: [ ";
    for(const std::string& s : tKeywords)
    {
        std::cout << '\"' << s << '\"' << ' ';
    }
    std::cout << ']';
    std::cout << std::endl;

    std::cout << "Separators: [ ";
    for(const char& s : tSeparators)
    {
        std::cout << '\"' << s << '\"' << ' ';
    }
    std::cout << ']';
    std::cout << std::endl;

    std::cout << "Operators: [ ";
    for(const std::string& s : tOperators)
    {
        std::cout << '\"' << s << '\"' << ' ';
    }
    std::cout << ']';
    std::cout << std::endl;

    std::cout << "Literals: [ ";
    for(const std::string& s : tLiterals)
    {
        std::cout << '\"' << s << '\"' << ' ';
    }
    std::cout << ']';
    std::cout << std::endl;

    std::cout << "Expression: [ ";
    for(const std::string& s : expression)
    {
        std::cout << s << ' ';
    }
    std::cout << ']';
    std::cout << std::endl;

    std::cout << "Tokenized Expression: [ ";
    for(const std::string& s : tExpression)
    {
        std::cout << s << ' ';
    }
    std::cout << ']';
    std::cout << '\n' << std::endl;

    return true;
}

int main()
{
    std::string str {};
    str.reserve(EXIT_CODE_LENGTH);

    std::cout << "/******************************************************************************** \n";
    std::cout << " * MAVeD - MilanverDatabase (Ice Bucket) | Command Prompt v0.3a 2020            * \n";
    std::cout << " * Copyright (c) 2020 Adrian Bedregal (AsulconS)                                * \n";
    std::cout << " ********************************************************************************/\n";
    std::cout << " Type \"help\", \"copyright\" or \"license\" for more information.\n";
    std::cout << " Type \"exit\" to stop the Command Prompt safely.\n";
    std::cout << ">> ";
    std::getline(std::cin, str);
    while(tokenize(str))
    {
        std::cout << ">> ";
        std::getline(std::cin, str);
    }

    return 0;
}
