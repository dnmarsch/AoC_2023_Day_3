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
    eRight
} AdjacencyTypes;

struct partNumber
{
    int pos;
    int lineNum;
    bool hasBeenAdded;
    int length;
};

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
    void readInput();
    void findPartNumbers(std::string& line, int lineNum);
    void checkForValidPartNumbers();
    void checkForAdjSpecChar(AdjacencyTypes adjType, std::pair<const int, struct partNumber> partNumEntry);
    bool canLookUp(int y);
    bool canLookDown(int y);
    bool canLookLeft(int x);
    bool canLookRight(int x);
    bool posHasSpecChar(int x, int y, int searchLength);
    void checkPartNumAddIfNeeded(int partNum, int x, int y);
    void addEntryToMapSumPartNumber(int partNum, int x, int y);
    unsigned long int sumPartNums = 0;
    std::vector<std::string> schematic = {};
    std::ifstream inputFile{"input.txt"};
    // std::ifstream inputFile{"test.txt"};
    std::multimap<int, struct partNumber> partNums = {};
    std::vector<AdjacencyTypes> adjacentTypes = { eUp, eDown, eLeft, eRight };
    int lineWidth = 0;
    int lineNum = 0;

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
        lineWidth = line.length();
        // std::cout << line << "\n";
        findPartNumbers(line, lineNum);
        schematic.push_back(line);
        lineNum++;
    }
    // std::cout << "line length: " << lineWidth << "\n";
}

/**
 * @brief Find all special characters in line and add position to vector 
 * (. doesn't count as special char)
 * @param line string from input file
 * @param lineNum line number from input file
 */
void GearRatio::findPartNumbers(std::string& line, int lineNum)
{
    size_t foundPos = 0;
    size_t foundPosPrev = 0;

    while(foundPos != std::string::npos)
    {
        foundPos = line.find_first_of(numericalDigits, foundPosPrev);
        if (foundPos != std::string::npos)
        {
            std::cout << "Found number: " << line[foundPos] << " at position: " << (int)foundPos 
                << " on line num: " << lineNum << "\n";
            size_t endPartNum = line.find_first_not_of(numericalDigits, foundPos);

            std::string partNumber(line, foundPos, endPartNum - foundPos);
            int foundPartNum = std::stoi(partNumber);
            std::cout << "Ascii substring: " << partNumber << "\tint val: " << foundPartNum << "\n";

            
            
            if(endPartNum == std::string::npos) //end of string/line
            {
                std::cout << "LENGTH: " << (int)(lineWidth-foundPos) << "\n";
                partNums.insert({foundPartNum, (struct partNumber){(int)foundPos, lineNum, false, (int)(lineWidth-foundPos)}});
                break;
            }
            else
            {
                std::cout << "LENGTH: " << (int)(endPartNum-foundPos) << "\n";
                partNums.insert({foundPartNum, (struct partNumber){(int)foundPos, lineNum, false, (int)(endPartNum-foundPos)}});
                foundPosPrev = foundPos + endPartNum;
            }
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
    return (y+1 <= (lineNum-1));
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
    return (x+1 <= (lineWidth-1));
}

/**
 * @brief Check if position has a numerical digit for a part number
 * 
 * @param x row of position to check
 * @param y line of position to check
 * @return true a part number is here & needs to be obtained
 * @return false no part number here
 */
bool GearRatio::posHasSpecChar(int x, int y, int searchLength)
{
    bool isSpecChar = false;
    size_t foundPos = x;
    std::string line = schematic.at(y);

    std::string searchIn = line.substr(x, searchLength);
    std::cout << "search substring: " << searchIn << "\tsearchLength: " << searchLength << "\n";
    foundPos = searchIn.find_first_not_of("0123456789.");

    if (foundPos != std::string::npos)
    {
        std::cout << "Found special character: " << searchIn.at(foundPos) << " at position: " << foundPos 
            << " on line num: " << y << "\n";
        isSpecChar = true;
    }
    return isSpecChar;
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
        std::cout << "entry not in multimap!!!!\n\n\n";
        while(true); //error, halt program to inspect call stack
    }
    else
    {
        auto range = partNums.equal_range(partNum);
        for(auto i = range.first; i != range.second; ++i)
        {
            std::cout << "map X: " << i->second.pos << "\tmap Y: " << i->second.lineNum << "\n";
            if((x == i->second.pos && y == i->second.lineNum) && !i->second.hasBeenAdded) //not same position && hasn't been added yet
            {
                std::cout << "not in same position\n";
                i->second.hasBeenAdded = true;
                addEntryToMapSumPartNumber(partNum, x, y);
            }
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
void GearRatio::checkForAdjSpecChar(AdjacencyTypes adjType, std::pair<const int,struct partNumber> partNumEntry)
{
    int startPos, endPos;
    auto curPartNum = partNumEntry.first;
    auto x = partNumEntry.second.pos;
    auto y = partNumEntry.second.lineNum;
    auto length = partNumEntry.second.length;

    switch(adjType)
    {
        case eUp:
            if(canLookUp(y))
            {
                std::cout << "Looking up at pos: " << x << "line to look at: " << y-1 << "\n";
                if(canLookLeft(x)) startPos = x-1;
                else startPos = x; //at beginning of line
                if(canLookRight(x+length)) endPos = x+length; //can look to right for special char
                else endPos = x+length-1; //at end of line
                
                if(posHasSpecChar(startPos, y-1, endPos-startPos+1))
                {
                   checkPartNumAddIfNeeded(curPartNum, x, y);
                }
            }
            break;
        case eDown:
            if(canLookDown(y))
            {
                std::cout << "Looking down at pos: " << x << "line to look at: " << y+1 << "\n";
                if(canLookLeft(x)) startPos = x-1;
                else startPos = x; //at beginning of line
                if(canLookRight(x+length)) endPos = x+length; //can look to right for special char
                else endPos = x+length-1; //at end of line
                
                if(posHasSpecChar(startPos, y+1, endPos-startPos+1))
                {
                    checkPartNumAddIfNeeded(curPartNum, x, y);
                }
            }
            break;
        case eLeft:
            if(canLookLeft(x))
            {
                std::cout << "Looking left at pos: " << x-1 << "line to look at: " << y << "\n";
                if(posHasSpecChar(x-1, y, 1))
                {
                   checkPartNumAddIfNeeded(curPartNum, x, y);
                }
            }
            break;
        case eRight:
            if(canLookRight(x))
            {
                std::cout << "Looking right at pos: " << x+1 << "line to look at: " << y << "\n";
                if(posHasSpecChar(x+length, y, 1))
                {
                   checkPartNumAddIfNeeded(curPartNum, x, y);
                }
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
    // for(auto it = partNums.begin(); it != partNums.end(); it++)
    for(auto entry : partNums)
    {
        auto curPartNum = entry.first;
        auto partNumX = entry.second.pos;
        auto partNumY = entry.second.lineNum;

        std::cout << "part number: " << curPartNum << "\tX: " << partNumX << "\tY: " << partNumY << "\n";

        /*check for adjacent "part numbers" & sum if adjacent and not summed yet & add to multimap
        (left, right, up, down, diagonal(up & left, up & right, down & left, down & right)) */
        for(auto adjacentType : adjacentTypes)
        {
            checkForAdjSpecChar(adjacentType, entry);
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

    for(auto it = partNums.begin(); it != partNums.end(); it++)
    {
        std::cout << "part num: " << it->first << "\t at x,y: " << it->second.pos << " " << it->second.lineNum << "\n";
    }

    checkForValidPartNumbers();
}

int main() {
    GearRatio gondolaLift;

    gondolaLift.calcSumPartNumbers();

    auto sumPartNums = gondolaLift.getSumPartNumber();
    std::cout << "FINAL VALUE: " << sumPartNums;

    while(true); //don't close terminal with result

    return 0;
}