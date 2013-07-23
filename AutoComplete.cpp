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

std::vector<std::string> autoComplete(autoCompleteVars *vars, std::string input, int correctionNum) {

    suck::Vector<int> correctionIdVector;
	std::vector<std::string> corrections;

    correctionIdVector = vars->tst.prefixSearch(input.c_str()).join(vars->tst.nearSearch(input.c_str(), 4));
    int correctionIds[correctionIdVector.count()][2];

    for (unsigned j = 0; j < correctionIdVector.count();j++) {
        int tmp = correctionIdVector.get(j);
        correctionIds[j][0] = vars->daMap[vars->tst.getKey(correctionIdVector.get(j))];
        correctionIds[j][1] = tmp;
    }

    if (correctionIdVector.count() > 0) {
         std::sort(&correctionIds[0][0], &correctionIds[correctionIdVector.count()-1][0]);
    }

    for (unsigned k = 0; k < correctionNum && k < correctionIdVector.count(); k++){
         corrections.push_back(vars->tst.getKey(correctionIds[correctionIdVector.count() - k - 1][1]));
    }

    vars->daMap[input] += 1;

    return corrections;

}

void cleanupTST(autoCompleteVars *vars) {
    ofstream file (vars->fileName.c_str());

    for (int i = 0; i < vars->tst.count(); i++) {
        std::string tmp = vars->tst.getKey(i);
        file << tmp;
        file << " ";
        file << vars->daMap[tmp];
        file << "\n";
    }

    file.close();

    delete vars;
}
