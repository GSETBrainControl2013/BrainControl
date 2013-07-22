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

#define MAX_LINE_LEN			1024
#define DIC_FILE_NAME			"dictest.txt"


template<class T>
std::ostream& operator<<(std::ostream& os,const Vector<T>& v) {
    os << "Vector{";
    bool first = true;
    for(unsigned int i=0;i<v.count();++i) {
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
    TernarySearchTree<int> tst;
	Vector< TstItem<int> > itemVector;
	Vector<int> correctionIds;
	Vector<string> corrections;

	if ( fileName ) {
		FILE * fp = fopen( fileName,"r" );
		if ( fp != NULL ) {
			char s[MAX_LINE_LEN];

			while( fgets( s, MAX_LINE_LEN, fp ) != NULL)
			{
				string key = s;
                key.trim("\r").trim("\n");//remove \r or \n
				int value = 0;
				tst.add( key.c_str(), value );
				TstItem<int> item( key, value );
				itemVector.add( item );
			}
			fclose( fp );
		}

        tst.buildBalancedTree( itemVector );
        int usageCounts[itemVector.count()];
        for (int i = 0; i < itemVector.count(); i++){
            usageCounts[i] = itemVector[i].value;
        }
        itemVector.clear();

        string input = "";
        char c = getchar();

        while (c != 'Q') {
            if (c == 'C') {
                usageCounts[tst.getItemIndex(input.c_str())]++;
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
        usageCounts[tst.getItemIndex(input.c_str())]++;

        ofstream file ("dictest.txt");

        for (int i = 0; i < tst.count(); i++) {
            file << *tst.getKey(i);
            file << " ";
            file << usageCounts[i];
        }

        file.close();
	}
}

int main()
{
	ternarySearchTreeNeighborTest();
}
