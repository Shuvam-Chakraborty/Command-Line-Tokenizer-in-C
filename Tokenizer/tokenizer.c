#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Token_Count = 0;

// Returns 'true' if the character is a DELIMITER.
bool isDelimiter(char ch)
{
    if (ch == ' '  || ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
        ch == ','  || ch == ';' || ch == '>' || ch == '<' || ch == '=' ||
        ch == ':'  || ch == '?' || ch == '(' || ch == ')' || ch == '[' ||
        ch == ']'  || ch == '{' || ch == '}' || ch == '%' || ch == '!' ||
        ch == '&'  || ch == '|' || ch == '\n'|| ch == '\t'|| ch == '\r') // ✅ FIX: added \r
        return (true);
    return (false);
}

// Returns 'true' if the character is a SPECIAL SYMBOL.
bool isSpecialSymbol(char ch)
{
    if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}')
        return true;
    return false;
}

// Returns 'true' if the character is an OPERATOR.
bool isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '!' || ch == '%' || ch == '&' || ch == '|')
        return (true);
    return (false);
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(char* str)
{
    if (str[0] == '\0') return false; // ✅ FIX: empty string is not a valid identifier
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isDelimiter(str[0]) == true)
        return (false);
    return (true);
}

// Returns 'true' if the string is a KEYWORD.
bool isKeyword(char* str)
{
    if (   !strcmp(str, "auto")     || !strcmp(str, "double")   || !strcmp(str, "int")      || !strcmp(str, "struct")
        || !strcmp(str, "break")    || !strcmp(str, "else")     || !strcmp(str, "long")     || !strcmp(str, "switch")
        || !strcmp(str, "case")     || !strcmp(str, "enum")     || !strcmp(str, "register") || !strcmp(str, "typedef")
        || !strcmp(str, "char")     || !strcmp(str, "extern")   || !strcmp(str, "return")   || !strcmp(str, "union")
        || !strcmp(str, "continue") || !strcmp(str, "for")      || !strcmp(str, "signed")   || !strcmp(str, "void")
        || !strcmp(str, "do")       || !strcmp(str, "if")       || !strcmp(str, "static")   || !strcmp(str, "while")
        || !strcmp(str, "goto")     || !strcmp(str, "sizeof")   || !strcmp(str, "volatile") || !strcmp(str, "default")
        || !strcmp(str, "const")    || !strcmp(str, "float")    || !strcmp(str, "short")    || !strcmp(str, "unsigned"))
        return (true);
    return (false);
}

// Returns 'true' if the string is an INTEGER.
bool isInteger(char* str)
{
    int i, len = strlen(str);
    if (len == 0) return (false);
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' &&
            str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' &&
            str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
            return (false);
    }
    return (true);
}

// Returns 'true' if the string is a REAL NUMBER.
bool isRealNumber(char* str)
{
    int i, len = strlen(str);
    bool hasDecimal = false;
    if (len == 0) return false;
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' &&
            str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' &&
            str[i] != '8' && str[i] != '9' && str[i] != '.' || (str[i] == '-' && i > 0))
            return (false);
        if (str[i] == '.') hasDecimal = true;
    }
    return (hasDecimal);
}

// Extracts the SUBSTRING, skipping all whitespace characters.
char* subString(char* str, int left, int right)
{
    int i, j = 0;
    char* subStr = (char*)malloc(sizeof(char) * (right - left + 2));
    for (i = left; i <= right; i++)
    {
        // ✅ FIX: added \r to the skip list alongside \t \n and space
        if (str[i] == '\t' || str[i] == '\n' || str[i] == ' ' || str[i] == '\r') continue;
        else subStr[j++] = str[i];
    }
    subStr[j] = '\0';
    return subStr;
}

// Tokenizes one line of (comment-stripped) source code.
void parse(char* str)
{
    int left = 0, right = 0;
    int len = strlen(str);

    if (str[0] == '#' || str[0] == '\n' || str[0] == '\r') return;

    while (right <= len && left <= right)
    {
        if (str[right] == '\n' || str[right] == '\r')
        {
            right++;
            left = right;
            if (str[right] == '\0') break;
            continue;
        }

        // Handle string literals
        if (str[left] == '\"')
        {
            char subStr[256];
            int i = 0;
            subStr[i++] = str[right++];
            while (right != len && (str[right] != '\"' || !isDelimiter(str[right + 1])))
                subStr[i++] = str[right++];
            if (str[right] == '\"') subStr[i++] = str[right++];
            subStr[i] = '\0';
            left = right;
            printf("%s \t:\t STRING LITERAL\n", subStr);
            Token_Count++;
            continue;
        }

        if (isDelimiter(str[right]) == false)
            right++;

        // Single-character delimiter token
        if (isDelimiter(str[right]) == true && left == right)
        {
            if (isOperator(str[right]) == true)
            {
                if (isOperator(str[right + 1]) == true)
                {
                    if ((str[right] == '|' && str[right+1] == '|') ||
                        (str[right] == '+' && str[right+1] == '+') ||
                        (str[right] == '-' && str[right+1] == '-') ||
                        (str[right] == '>' && str[right+1] == '>') ||
                        (str[right] == '<' && str[right+1] == '<') ||
                        (str[right] == '&' && str[right+1] == '&') ||
                        (str[right+1] == '='))
                    {
                        printf("%c%c \t:\t OPERATOR\n", str[right], str[right+1]);
                        right++;
                        Token_Count++;
                    }
                    else
                    {
                        printf("%c \t:\t OPERATOR\n", str[right]);
                        Token_Count++;
                    }
                }
                else
                {
                    printf("%c \t:\t OPERATOR\n", str[right]);
                    Token_Count++;
                }
            }
            else if (isSpecialSymbol(str[right]) == true)
            {
                printf("%c \t:\t SPECIAL SYMBOL\n", str[right]);
                Token_Count++;
            }
            else
            {
                // Skip spaces, tabs, \r, \n silently
                if (!(str[right] == ' '  || str[right] == '\t' ||
                      str[right] == '\n' || str[right] == '\r' || str[right] == '\0'))
                {
                    printf("%c \t:\t SEPARATOR\n", str[right]);
                    Token_Count++;
                }
            }
            right++;
            left = right;
        }
        // Multi-character token
        else if ((isDelimiter(str[right]) == true && left != right) ||
                 (right == len && left != right))
        {
            char* subStr = subString(str, left, right - 1);

            // ✅ FIX: skip empty/whitespace-only substrings (caused by \r\n or indentation)
            if (strlen(subStr) == 0)
            {
                free(subStr);
                left = right;
                continue;
            }

            if (isKeyword(subStr) == true)
                printf("%s \t:\t KEYWORD\n", subStr);
            else if (isInteger(subStr) == true)
                printf("%s \t:\t INTEGER\n", subStr);
            else if (isRealNumber(subStr) == true)
                printf("%s \t:\t REAL NUMBER\n", subStr);
            else if (validIdentifier(subStr) == true && isDelimiter(str[right-1]) == false)
                printf("%s \t:\t VALID IDENTIFIER\n", subStr);
            else if (validIdentifier(subStr) == false && isDelimiter(str[right-1]) == false)
                printf("%s \t:\t NOT VALID IDENTIFIER\n", subStr);

            free(subStr); // ✅ FIX: always free malloc'd memory
            left = right;
            Token_Count++;
        }
    }
    return;
}

// Strips // and /* */ comments from a single line.
int process(char* str, char* newStr, int check)
{
    int j = 0, len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (check)
        {
            while (i != len && !(str[i] == '/' && str[i-1] == '*')) i++;
            if (i == len) break;
            check = 0;
        }
        else
        {
            if (str[i] != '/')
            {
                newStr[j++] = str[i];
            }
            else
            {
                if (len - i >= 2)
                {
                    if (str[i+1] == '/') break;
                    else if (len - i >= 4 && str[i+1] == '*')
                    {
                        check = 1;
                        i = i + 3;
                        while (i != len && !(str[i] == '/' && str[i-1] == '*')) i++;
                        if (i == len) break;
                        check = 0;
                    }
                    else newStr[j++] = str[i];
                }
            }
        }
    }
    newStr[j] = '\0';
    return check;
}

int main()
{
    FILE* ptr;
    ptr = fopen("test.c", "r");
    if (ptr == NULL)
    {
        printf("Error: could not open Test_Sum.cpp\n");
        return 1;
    }

    char str[256], processed_str[256];
    int check = 0;

    while (fgets(str, sizeof(str), ptr))
    {
        check = process(str, processed_str, check);
        parse(processed_str);
    }

    printf("\n\nToken Count : %d\n", Token_Count);
    fclose(ptr);
    return (0);
}