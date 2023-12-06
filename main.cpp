#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>


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
        int row;
        bool hasBeenAdded;
    };
    
    void readInput();
    void findSpecialChars(std::string& line, int lineNum);
    void checkForValidPartNumbers();
    unsigned long int sumPartNums = 0;
    std::vector<std::string> schematic = {};
    // std::ifstream inputFile{"input.txt"};
    std::ifstream inputFile{"test.txt"};
    std::vector<std::pair<int,int>> specialChars = {};
    std::multimap<int, struct partNumber> partNums = {};
    int lineWidth = 0;
};

/**
 * @brief Read all lines of input from "schematic"
 * 
 */
void GearRatio::readInput()
{
    std::string line;
    int lineNum = 0;
    while (std::getline(inputFile, line))
    {
        std::cout << line << "\n";
        findSpecialChars(line, lineNum++);
        schematic.push_back(line);
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
            specialChars.push_back({lineNum, foundPos});
            foundPosPrev = foundPos + 1;
        }
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

        /*check for adjacent "part numbers" & sum if adjacent and not summed yet & add to multimap
        (left, right, up, down, diagonal(up & left, up & right, down & left, down & right)) */
        for(auto adjacentType : adjacentTypes) //@TODO: make an enum : int
        {
            //@TODO: must check to not exceed bounds (>0 <max_row <max_pos)
            if(partNumFound(adjacent, specCharPos, specCharRow)) //@TODO: check if adjacent chars are '.' or a numeric digit (use switch case)
            {
                //get substring (search until 0123456789 not found) & convert to int
                //@TODO: search left & right & add sub-strings together if found in both directions THEN convert to int

                //check multimap & add to sum if not currently added (not yet in map || bool flag false)

                //add to multimap
            }
            //else adjacent char is '.' or a special character so move on to next check
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