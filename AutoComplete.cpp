// AutoComplete
// AKA "Suck"

#include "AutoComplete.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


// returns struct used for autocomplete method, takes file name and max line length
autoCompleteVars * createTST(std::string fileName, int maxLineLength) {

    autoCompleteVars* vars = new autoCompleteVars;
    vars->fileName = fileName;

    FILE * fp = fopen( fileName.c_str(),"r" );
    if ( fp != NULL ) {
        char s[maxLineLength];
        int count = 0;
        while( fgets( s, maxLineLength, fp ) != NULL)
        {
            suck::string str = s;
            int value = -1;
            istringstream iss(s);
            std::string key;
            iss >> key >> value;
            vars->tst.add( key.c_str(), value);
            suck::TstItem<int> item( suck::string(key.c_str()), value );
            vars->itemVector.add( item );
            vars->daMap[key] = value;
            count++;

        }
        fclose( fp );
    }

    vars->tst.buildBalancedTree(vars->itemVector);
    vars->itemVector.clear();

    return vars;
}

void autoComplete(autoCompleteVars *vars, std::string input, int correctionNum,std::vector<std::string>&corrections) {

    suck::Vector<int> correctionIdVector;
    corrections.clear();
    corrections.reserve(correctionNum);

    correctionIdVector = vars->tst.prefixSearch(input.c_str()).join(vars->tst.nearSearch(input.c_str(), 20));

    corrections.clear();
    std::vector<std::string> keys;

    for (unsigned j = 0; j < correctionIdVector.count();j++) {
        keys.push_back(vars->tst.getKey(correctionIdVector.get(j)));
    }

    for(size_t i=0;i<correctionIdVector.count();++i) {
        if(i < correctionNum) {
            corrections.push_back(keys[i]);
        }
        for(size_t j=0;j<corrections.size();++j) {
            std::string otherKey = keys[i];
            int aUsage = vars->daMap[otherKey],
                bUsage = vars->daMap[corrections[j]];
            if(aUsage > bUsage || (aUsage == bUsage && otherKey < corrections[j])) {
                corrections[j] = otherKey;
                break;
            }
        }
    }

    // Bubble sort it!
    bool swapped = true;
    size_t size = corrections.size();
    for(size_t i=0;i<size&&swapped;++i) {
        swapped = false;
        for(size_t j=0;j+2+i<size;++j) {
            int aUsage = vars->daMap[corrections[j]],
                bUsage = vars->daMap[corrections[j+1]];
            if(aUsage < bUsage || (aUsage == bUsage && corrections[j+1] < corrections[j])) {
                std::string tmp = corrections[j];
                corrections[j] = corrections[j+1];
                corrections[j+1] = tmp;
                swapped = true;
            }
        }
    }
}

void cleanupTST(autoCompleteVars *vars) {
    ofstream file (vars->fileName.c_str());

    for (int i = 0; i < vars->tst.count(); i++) {
        std::string tmp = vars->tst.getKey(i);
        for(size_t j=0;j<tmp.length();++j) {
            file << (char)std::toupper(tmp[j]);
        }
        file << " ";
        file << vars->daMap[tmp];
        file << "\n";
    }

    file.close();

    delete vars;
}
