#include <iostream>
#include <fstream>
#include <vector>
#include "TST.h"
#include <string>

using namespace std;

#define MAX_LINE_LEN			1024
#define DIC_FILE_NAME			"dictest.txt"

/*
void updateUsage(TST tst, string input) {
    int *value = tst.getValue(input.c_str());

    if (value != NULL) {
        tst.add(input.c_str(), *value + 1);
    }
    else {
        //optionally add key to the tree with a 0 value
        //tst.add(input.c_str(), 0);
    }
}

void writeTreeToFile(TernarySearchTree<int> tst, bool reset) {
    ofstream file ("dictest.txt");

    TstItem<int> *outputItem;

    for (int i = 0; i < tst.count(); i++) {
        outputItem = tst.getItem(i);
        file << outputItem->key;
        file << " ";
        if (reset) file << "0";
        else file << outputItem->value;
    }

    file.close();

}

vector<string> getAutoComplete(string input) {

}
*/


int main() {
  /*  const char * fileName = DIC_FILE_NAME;
    TST tst;

	if ( fileName ) {
		FILE * fp = fopen( fileName,"r" );
		if ( fp != NULL ) {
			char s[MAX_LINE_LEN];

			while( fgets( s, MAX_LINE_LEN, fp ) != NULL)
			{
				string key = s;
				key.trim("\r").trim("\n");
				itemVector.add( item );
			}
			fclose( fp );
		}

        tst.buildBalancedTree( itemVector );
        itemVector.clear();

        string input = "";
        char c = getchar();

        while (c != 'Q') {
            if (c == 'C') {
                updateUsage(tst, input);
                input = "";
            }
            else {
                input += c;
                correctionIds = tst.prefixSearch(input.c_str()).join(tst.nearSearch(input.c_str(), 2));

                for (unsigned j = 0; j < correctionIds.count();j++) {
                    int tmp = correctionIds.get(j);
                    corrections.add(tst.getKey(tmp));
                }
                cout << corrections;
            }
            c = getchar();
            if (c == '\n') c = getchar();

            corrections.clear();

        }
        updateUsage(tst, input);

        writeTreeToFile(tst, false);

*/

    TST tst;

    cout << size(tst);
    add(tst, "a", 0);
    cout << size(tst);



}
