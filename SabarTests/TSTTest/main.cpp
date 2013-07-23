/*******************************************************************
C++ Package of  Ternary Search Tree
Copyright (C) 2006  Zheyuan Yu

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Read full GPL at http://www.gnu.org/copyleft/gpl.html

Email me at jerryy@gmail.com if you have any question or comment
WebSite: http://www.cs.dal.ca/~zyu

*************************************************************************/

/**
 * Unit test for the ternary search tree class
 * Revisions:
 * Jan 16, 2005
 * Initial creation
 */

#include "ternarySearchTree.h"
#include <iostream>
#include <fstream>
#include "vector.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <map>

#define MAX_LINE_LEN			1024
#define DIC_FILE_NAME			"dictest.txt"
int correctionNum = 3;

template<class T>
std::ostream& operator<<(std::ostream& os,const std::vector<T>& v) {
    os << "std::vector{";
    bool first = true;
    for(unsigned int i=0;i<v.size();++i) {
        if(!first) {
            os << ",";
        }
        first = false;
        os << v[i];
    }
    os << "}";
    return os;
}

void ternarySearchTreeNeighborTest() {
    const char * fileName = DIC_FILE_NAME;
    suck::TernarySearchTree<int> tst;
	suck::Vector< suck::TstItem<int> > itemVector;
	map <string, int> daMap;
	suck::Vector<int> correctionIdVector;
	std::vector<std::string> corrections;

	if ( fileName ) {
		FILE * fp = fopen( fileName,"r" );
		if ( fp != NULL ) {
			char s[MAX_LINE_LEN];
            int count = 0;
			while( fgets( s, MAX_LINE_LEN, fp ) != NULL)
			{
			    suck::string str = s;
				int value = -1;
                istringstream iss(s);
                std::string key;
                iss >> key >> value;
                tst.add( key.c_str(), value);
                suck::TstItem<int> item( suck::string(key.c_str()), value );
                itemVector.add( item );
                daMap[key] = value;
                count++;

			}
			fclose( fp );
		}

        tst.buildBalancedTree( itemVector );

        itemVector.clear();

        string input = "";
        char c = getchar();

        while (c != 'Q') {
            if (c == 'C') {
                daMap[input] += 1;
                input = "";
            }
            else {
                input += c;
                correctionIdVector = tst.prefixSearch(input.c_str()).join(tst.nearSearch(input.c_str(), 4));
                int correctionIds[correctionIdVector.count()][2];

                for (unsigned j = 0; j < correctionIdVector.count();j++) {
                    int tmp = correctionIdVector.get(j);
                    correctionIds[j][0] = daMap[tst.getKey(correctionIdVector.get(j))];
                    correctionIds[j][1] = tmp;
                }

                if (correctionIdVector.count() > 0) {
                     std::sort(&correctionIds[0][0], &correctionIds[correctionIdVector.count()-1][0]);
                }

                for (unsigned k = 0; k < correctionNum && k < correctionIdVector.count(); k++){
                     corrections.push_back(tst.getKey(correctionIds[correctionIdVector.count() - k - 1][1]));
                }

                cout << corrections;
            }
            c = getchar();
            if (c == '\n') c = getchar();

            corrections.clear();

        }
        daMap[input] += 1;

        ofstream file ("dictest.txt");

        for (int i = 0; i < tst.count(); i++) {
            string tmp = tst.getKey(i);
            file << tmp;
            file << " ";
            file << daMap[tmp];
            file << "\n";
        }

        file.close();
	}
}

int main()
{
	ternarySearchTreeNeighborTest();
}
