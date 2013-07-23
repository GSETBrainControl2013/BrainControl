#ifndef AUTOCOMPLETE_H_INCLUDED
#define AUTOCOMPLETE_H_INCLUDED

#include "vector.h"
#include "ternarySearchTree.h"
#include <map>
#include <vector>
#include <string>

struct autoCompleteVars {
    std::string fileName;
    suck::TernarySearchTree<int> tst;
	suck::Vector< suck::TstItem<int> > itemVector;
	map <string, int> daMap;
};

autoCompleteVars * createTST(std::string filename, int maxFileLength);

void autoComplete(autoCompleteVars *vars, std::string input, int correctionNum,std::vector<std::string>& corrections);

void cleanupTST(autoCompleteVars *vars);

#endif // AUTOCOMPLETE_H_INCLUDED
