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

typedef enum : int
{
    eLeftDir = 0,
    eRightDir,
    eBothDir
} Directions;

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
    void getPartNumberString(int x, int y, Directions direction);
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

std::string::size_type find_first_of_not_reverse(std::string const& line, std::string::size_type const pos)
{
    //go backwards through !isDigit(line.at(pos)) & return pos
    std::cout << "searching through line: " << line << "\n";
    auto i = pos;
    for(i = pos; i >= 0; i--)
    {
        if(!isdigit(line.at(i)))
        {
            i+=1; //don't include the not digit character
            return i;
        }
        else
        {
            std::cout << "digit: " << line.at(i) << "\ti: " << i << "\n";
        }
        if(i == 0) break;
    }
    if(isdigit(line.at(0))) return 0;
    else return std::string::npos; //no digit character found in search
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
        }
    }
}

/**
 * @brief This position has a numerical digit,
 * need to search left & right for substrings,
 * concatenate, convert to int, check multimap if already added.
 * If not already added, then add to sum for total part number calc.
 * 
 * @param x row position to search at
 * @param y line to search at
 */
void GearRatio::getPartNumberString(int x, int y, Directions direction)
{
    bool isStringToEOL = false;
    std::string leftSubstring;
    std::string rightSubstring;

    std::string line = schematic.at(y);
    std::cout << "current line #: " << y << "\t" << line << "\n";
    size_t left_pos = find_first_of_not_reverse(line, x);
    size_t right_pos = line.find_first_not_of(numericalDigits, x);

    if(direction == eLeftDir || direction == eBothDir)
    {
        if(left_pos != std::string::npos && left_pos < x)
        {
            if(direction == eLeftDir)
                leftSubstring =  line.substr(left_pos, x-left_pos+1);
            else
                leftSubstring =  line.substr(left_pos, x-left_pos); //no+1 bc right substring includes it
        }
    }

    if(direction == eRightDir || direction == eBothDir)
    {
        //either no numeric digit found OR numeric digits all the way to end of line
        if(right_pos == std::string::npos)
        {
            if(isdigit(line.at(lineWidth-1))) //digit at end of line
            {
                rightSubstring = line.substr(x, lineWidth-x);
            }
        }
        else if(right_pos != std::string::npos)
        {
            rightSubstring = line.substr(x, right_pos-x);
        }
    }

    std::string AsciiSubstring = leftSubstring + rightSubstring;
    if(AsciiSubstring.size() > 0)
    {
        for(auto i=0; i<AsciiSubstring.size(); i++)
        {
            if(!isdigit(AsciiSubstring.at(i)))
            {
                std::cout << "ascii string: " << AsciiSubstring << "\n";
                while(true); //for testing
            }
        }
        int partNumber = std::stoi(AsciiSubstring);
        std::cout << "Ascii substring: " << AsciiSubstring << "\tint val: " << partNumber << "\n";

        if(left_pos != std::string::npos)
        {
            checkPartNumAddIfNeeded(partNumber, left_pos, y); //left substring start is beginning of string
        }
        else
        {
            checkPartNumAddIfNeeded(partNumber, x, y); //right substring start is beginning of string
        }
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
                getPartNumberString(x, y-1, eBothDir);
            }
            break;
        case eDown:
            if(canLookDown(y))
            {
                std::cout << "Looking down at pos: " << x << "line to look at: " << y+1 << "\n";
                getPartNumberString(x, y+1, eBothDir);
            }
            break;
        case eLeft:
            if(canLookLeft(x))
            {
                std::cout << "Looking left at pos: " << x-1 << "line to look at: " << y << "\n";
                getPartNumberString(x-1, y, eLeftDir);
            }
            break;
        case eRight:
            if(canLookRight(x))
            {
                std::cout << "Looking right at pos: " << x+1 << "line to look at: " << y << "\n";
                getPartNumberString(x+1, y, eRightDir);
            }
            break;
        case eDiagUpLeft:
            if(canLookUp(y) && canLookLeft(x))
            {
                std::cout << "Looking Up & left at pos: " << x-1 << "line to look at: " << y-1 << "\n";
                getPartNumberString(x-1, y-1, eBothDir);
            }
            break;
        case eDiagUpRight:
            if(canLookUp(y) && canLookRight(x))
            {
                std::cout << "Looking Up & right at pos: " << x+1 << "line to look at: " << y-1 << "\n";
                getPartNumberString(x+1, y-1, eBothDir);
            }
            break;
        case eDiagDownLeft:
            if(canLookDown(y) && canLookLeft(x))
            {
                std::cout << "Looking Down & left at pos: " << x-1 << "line to look at: " << y+1 << "\n";
                getPartNumberString(x-1, y+1, eBothDir);
            }
            break;
        case eDiagDownRight:
            if(canLookDown(y) && canLookRight(x))
            {
                std::cout << "Looking Down & right at pos: " << x+1 << "line to look at: " << y+1 << "\n";
                getPartNumberString(x+1, y+1, eBothDir);
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
    // for(auto it = specialChars.begin(); it != specialChars.end(); it++)
    // {
    //     auto specCharPos = it->first;
    //     auto specCharRow = it->second;
    //     std::cout << "special char found at pos: " << specCharPos << "\tline num: " << specCharRow << "\n";
    // }
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

    for(auto it = partNums.begin(); it != partNums.end(); it++)
    {
        std::cout << "part num: " << it->first << "\t at x,y: " << it->second.pos << " " << it->second.lineNum << "\n";
    }
}

int main() {
    GearRatio gondolaLift;

    gondolaLift.calcSumPartNumbers();

    auto sumPartNums = gondolaLift.getSumPartNumber();
    std::cout << "FINAL VALUE: " << sumPartNums;

    return 0;
}