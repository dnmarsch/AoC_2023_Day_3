#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>

typedef enum : int
{
    eUp = 0,
    eDown,
    eLeft,
    eRight,
    eDiagUpLeft,
    eDiagUpRight,
    eDiagDownLeft,
    eDiagDownRight
} AdjacencyTypes;

class GearRatio
{
public:
    GearRatio()
    {}
    void calcSumPartNumbers();
    unsigned long int getSumPartNumber() const {
        return sumPartNums;
    }

private:

    struct partNumber
    {
        int pos;
        int lineNum;
        bool hasBeenAdded;
    };
    
    void readInput();
    void findSpecialChars(std::string& line, int lineNum);
    void checkForValidPartNumbers();
    void checkForPartNum(AdjacencyTypes adjType, int x, int y);
    bool canLookUp(int y);
    bool canLookDown(int y);
    bool canLookLeft(int x);
    bool canLookRight(int x);
    bool posHasNumericalDigit(int x, int y);
    void getPartNumberString(int x, int y);
    void checkPartNumAddIfNeeded(int partNum, int x, int y);
    void addEntryToMapSumPartNumber(int partNum, int x, int y);
    unsigned long int sumPartNums = 0;
    std::vector<std::string> schematic = {};
    // std::ifstream inputFile{"input.txt"};
    std::ifstream inputFile{"test.txt"};
    std::vector<std::pair<int,int>> specialChars = {};
    std::multimap<int, struct partNumber> partNums = {};
    std::vector<AdjacencyTypes> adjacentTypes = { eUp, eDown, eLeft, eRight, eDiagUpLeft, eDiagUpRight,
        eDiagDownLeft, eDiagDownRight };
    int lineWidth = 0;
    int lineNum = 0;

    std::string notNumericalDigit = "!@#$%^&*-_+=/.";
    std::string numericalDigits = "0123456789";
};

/**
 * @brief Read all lines of input from "schematic"
 * 
 */
void GearRatio::readInput()
{
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::cout << line << "\n";
        findSpecialChars(line, lineNum);
        schematic.push_back(line);
        lineNum++;
    }
    lineWidth = line.length();
    std::cout << "line length: " << lineWidth << "\n";
}

/**
 * @brief Find all special characters in line and add position to vector 
 * (. doesn't count as special char)
 * @param line string from input file
 * @param lineNum line number from input file
 */
void GearRatio::findSpecialChars(std::string& line, int lineNum)
{
    size_t foundPos = 0;
    size_t foundPosPrev = 0;

    while(foundPos != std::string::npos)
    {
        foundPos = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890.", foundPosPrev);
        if (foundPos != std::string::npos) //pos > prev??
        {
            std::cout << "Found Special Character: " << line[foundPos] << " at position: " << foundPos 
                << " on line num: " << lineNum << "\n";
            specialChars.push_back({foundPos, lineNum});
            foundPosPrev = foundPos + 1;
        }
    }
}

/**
 * @brief Check if we can look in previous line for part number
 * 
 * @param y line number
 * @return true can check line above for part numbers
 * @return false you're at the beginning of the file, 
 *  can't look another line above
 */
bool GearRatio::canLookUp(int y)
{
    return (y-1 >= 0);
}

/**
 * @brief Check if we can look in next line for part number
 * 
 * @param y line number
 * @return true can check line below for part numbers
 * @return false you're at the end of the file, can't look
 * another line below
 */
bool GearRatio::canLookDown(int y)
{
    return (y+1 <= lineNum);
}

/**
 * @brief Check if we can look left in current line for part numbers
 * 
 * @param x row position (of special character)
 * @return true can check line to left for part number
 * @return false you're at the beginning of the line, can't look left
 */
bool GearRatio::canLookLeft(int x)
{
    return (x-1 >= 0);
}

/**
 * @brief Check if we can look right in current line for part numbers
 * 
 * @param x row position (of special character)
 * @return true can check line to right for part number
 * @return false you're at the end of the line, can't look right
 */
bool GearRatio::canLookRight(int x)
{
    return (x+1 <= lineWidth);
}

/**
 * @brief Check if position has a numerical digit for a part number
 * 
 * @param x row of position to check
 * @param y line of position to check
 * @return true a part number is here & needs to be obtained
 * @return false no part number here
 */
bool GearRatio::posHasNumericalDigit(int x, int y)
{
    bool isNumericDigit = false;
    size_t foundPos = x;
    std::string line = schematic.at(y);

    foundPos = line.find_first_of("0123456789", x);
    if (foundPos == x)
    {
        std::cout << "Found Numeric Digit: " << line.at(x) << " at position: " << foundPos 
            << " on line num: " << y << "\n";
        isNumericDigit = true;
    }
    return isNumericDigit;
}



std::string::size_type find_first_of_not_reverse(
    std::string const& str,
    std::string::size_type const pos,
    std::string const& chars)
{
    // assert(pos > 1);
    // assert(pos < str.size());
    if (pos < 1)
    {
        return std::string::npos;
    }

    std::string::size_type const res = str.find_last_of(chars, pos - 1) + 1;
    // return res == pos ? find_first_of_not_reverse(str, pos - 1, chars) : res ? res : std::string::npos;

    if(res == pos)
    {
        find_first_of_not_reverse(str, pos - 1, chars);
    }
    else
    {
        if(res == 0)
        {
            return 0;
        }
        else
        {
            return res;
        }
    }
}

/**
 * @brief Add entry to map & add part number to total part number
 * 
 * @param partNum part number to be added
 * @param x position of start of part number to be added
 * @param y line number of part number to be added
 */
void GearRatio::addEntryToMapSumPartNumber(int partNum, int x, int y)
{
    sumPartNums += partNum;
    std::cout << "adding to total Part number: " << partNum << "\tTotal Part Numbers: " << sumPartNums << "\n";
    partNums.insert({partNum, (struct partNumber){x, y, true}});
}
/**
 * @brief Check multimap for this part number, if not already entered,
 * add to multimap & add to total part number.
 * 
 * @param partNum value/key for multimap & to potentially add to total part number
 * @param x position of start of part number in string/line
 * @param y line number of part number in string
 */
void GearRatio::checkPartNumAddIfNeeded(int partNum, int x, int y)
{
    /* Handle checking multimap & adding to map & total sum */
    // if(partNums.size() > 0)
    // {
        if(auto it = partNums.find(partNum) == partNums.end())
        {
            addEntryToMapSumPartNumber(partNum, x, y);
        }
        else
        {
            auto range = partNums.equal_range(partNum);
            for(auto i = range.first; i != range.second; ++i)
            {
                std::cout << "multi map has an entry for: " << partNum << " this already, need to check if value at same position\n";
                if(!(x == i->second.pos && y == i->second.lineNum)) //not same position && hasn't been added yet
                {
                    addEntryToMapSumPartNumber(partNum, x, y);
                }
                // else //same position
                // {
                // }
            }
        }
    // }
    // else //immediately add first entry to multimap
    // {
    //     addEntryToMapSumPartNumber(partNum, x, y);
    // }
}
/*
467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
*/

/**
 * @brief This position has a numerical digit,
 * need to search left & right for substrings,
 * concatenate, convert to int, check multimap if already added.
 * If not already added, then add to sum for total part number calc.
 * 
 * @param x row position to search at
 * @param y line to search at
 */
void GearRatio::getPartNumberString(int x, int y)
{
    std::string line = schematic.at(y);
    std::cout << "current line #: " << y << "\t" << line << "\n";
    size_t left_pos = find_first_of_not_reverse(line, x, notNumericalDigit);
    size_t right_pos = line.find_first_not_of(numericalDigits, x);




    if(left_pos < right_pos && left_pos != std::string::npos && right_pos != std::string::npos)
    {
        std::string AsciiSubstring = line.substr(left_pos, x-left_pos) + line.substr(x, right_pos-x);
        int partNumber = std::stoi(AsciiSubstring); 
        std::cout << "Ascii substring: " << AsciiSubstring << "\tint val: " << partNumber << "\n";

        checkPartNumAddIfNeeded(partNumber, left_pos, y);
    }
    //part number is to the right (diagonal) of the special character (left doesn't matter)
    else if (right_pos < lineWidth && left_pos != std::string::npos && right_pos != std::string::npos)
    {
        std::string AsciiSubstring = line.substr(left_pos, x-left_pos) + line.substr(x, right_pos-x);
        int partNumber = std::stoi(AsciiSubstring);
        std::cout << "Ascii substring: " << AsciiSubstring << "\tint val: " << partNumber << "\n";

        checkPartNumAddIfNeeded(partNumber, left_pos, y);
    }
    //part number is to the left (diagonal) of the special character (right doesn't matter)

    else if (left_pos == 0) //part number goes all the way to end of line
    {
        std::cout << "Left pos = npos!!!!!!!!!!!!!!!!" << "\n";
        std::string AsciiSubstring = line.substr(0, x) + line.substr(x, right_pos-x);
        int partNumber = std::stoi(AsciiSubstring);
        std::cout << "Ascii substring: " << AsciiSubstring << "\tint val: " << partNumber << "\n";

        checkPartNumAddIfNeeded(partNumber, 0, y);

        // while(true);
    }
    else if (right_pos == std::string::npos) //part number goes all the way to end of line
    {
        std::cout << "Right pos = npos!!!!!!!!!!!!!!!!" << "\n";
        std::string AsciiSubstring = line.substr(left_pos, x-left_pos) + line.substr(x, lineWidth-x);
        int partNumber = std::stoi(AsciiSubstring); 
        std::cout << "Ascii substring: " << AsciiSubstring << "\tint val: " << partNumber << "\n";

        checkPartNumAddIfNeeded(partNumber, left_pos, y);
        // while(true);
    }
}

/**
 * @brief Check adjacency type of this special character for a part number & handle
 * finding the entire part number & summing it (if not already summed via multimap)
 * 
 * @param adjType AdjacencyTypes that we're currently checking
 * @param x special character row position
 * @param y special character line position
 * @return true part number found at this adjacency
 * @return false no part number found
 */
void GearRatio::checkForPartNum(AdjacencyTypes adjType, int x, int y)
{
    switch(adjType)
    {
        case eUp:
            if(canLookUp(y) && posHasNumericalDigit(x, y-1))
            {
                std::cout << "Looking up at pos: " << x << "line to look at: " << y-1 << "\n";
                getPartNumberString(x, y-1);
            }
            break;
        case eDown:
            if(canLookDown(y))
            {
                std::cout << "Looking down at pos: " << x << "line to look at: " << y+1 << "\n";
                getPartNumberString(x, y+1);
            }
            break;
        case eLeft:
            if(canLookLeft(x))
            {
                std::cout << "Looking left at pos: " << x-1 << "line to look at: " << y << "\n";
                getPartNumberString(x-1, y);
            }
            break;
        case eRight:
            if(canLookRight(x))
            {
                std::cout << "Looking right at pos: " << x+1 << "line to look at: " << y << "\n";
                getPartNumberString(x+1, y);
            }
            break;
        case eDiagUpLeft:
            if(canLookUp(y) && canLookLeft(x))
            {
                std::cout << "Looking Up & left at pos: " << x-1 << "line to look at: " << y-1 << "\n";
                getPartNumberString(x-1, y-1);
            }
            break;
        case eDiagUpRight:
            if(canLookUp(y) && canLookRight(x))
            {
                std::cout << "Looking Up & right at pos: " << x+1 << "line to look at: " << y-1 << "\n";
                getPartNumberString(x+1, y-1);
            }
            break;
        case eDiagDownLeft:
            if(canLookDown(y) && canLookLeft(x))
            {
                std::cout << "Looking Down & left at pos: " << x-1 << "line to look at: " << y+1 << "\n";
                getPartNumberString(x-1, y+1);
            }
            break;
        case eDiagDownRight:
            if(canLookDown(y) && canLookRight(x))
            {
                std::cout << "Looking Down & right at pos: " << x+1 << "line to look at: " << y+1 << "\n";
                getPartNumberString(x+1, y+1);
            }
            break;

        default:
            throw std::invalid_argument("Not an Adjacency Type");
    }
}

/**
 * @brief Iterate through special characters vector.
 * Check for valid (adjacent) "part numbers" & add to sum
 * (only do once for each as there could be multiple character adjacent to one part number).
 * 
 */
void GearRatio::checkForValidPartNumbers()
{

    for(auto it = specialChars.begin(); it != specialChars.end(); it++)
    {
        auto specCharPos = it->first;
        auto specCharRow = it->second;
        std::cout << "special char found at pos: " << specCharPos << "\tline num: " << specCharRow << "\n";
    }
    for(auto it = specialChars.begin(); it != specialChars.end(); it++)
    {
        auto specCharPos = it->first;
        auto specCharRow = it->second;

        /*check for adjacent "part numbers" & sum if adjacent and not summed yet & add to multimap
        (left, right, up, down, diagonal(up & left, up & right, down & left, down & right)) */
        for(auto adjacentType : adjacentTypes)
        {
            checkForPartNum(adjacentType, specCharPos, specCharRow);
        }
    }
}

/**
 * @brief Read input file & find valid part numbers adjacent to special characters
 * 
 */
void GearRatio::calcSumPartNumbers()
{
    readInput();

    checkForValidPartNumbers();
}

int main() {
    GearRatio gondolaLift;

    gondolaLift.calcSumPartNumbers();

    auto sumPartNums = gondolaLift.getSumPartNumber();
    std::cout << "FINAL VALUE: " << sumPartNums;

    return 0;
}