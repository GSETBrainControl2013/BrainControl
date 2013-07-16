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
#include "vector.h"
#include <time.h>

#define MAX_LINE_LEN			1024
#define DIC_FILE_NAME			"dic.txt"

void ternarySearchTreeUnitTest()
{
	const char * fileName = DIC_FILE_NAME;
	TernarySearchTree<string> tst;

	/**
     *	read in dictionary file, build the tree
	 * the file format should be one of follwings
	 * 1. word<tab>description\n
	 * 2. word\n
	 */
	Vector<string> newKeyVector;
	Vector<string> newValueVector;
	Vector< TstItem<string> > itemVector;
	if ( fileName )
	{

		/* get token string from input file */
		FILE * fp = fopen( fileName,"r" );
		if ( fp != NULL )
		{
			char s[MAX_LINE_LEN];

			while( fgets( s, MAX_LINE_LEN, fp ) != NULL)
			{
				string key = s;
				/*int index = str.indexOf("\t");
				string key = str.substring( 0, index );
				key.trim();
				*/
				key.trim("\r").trim("\n");//remove \r or \n
				string value = "value of " + key;
				value.trim("\r");//remove \r or \n
				value.trim("\n");
				tst.add( key.c_str(), value );
				TstItem<String> item( key, value );
				itemVector.add( item );
			}
			fclose( fp );
		}
	}

	// test for searching

	for ( int i =0;i<tst.count();i++)
	{
		const char * str = tst.getKey( i );
		if ( tst.contains( str ) )
		{	// key is found, display key and value
			string * value = tst.getValue( str );
			cout<< "key/value: "<< str <<" - "<< value->c_str() <<endl;
		}
		else
		{
			cout<< "key: " << str << " not found!"<<endl;
		}
	}

	cout<<"total "<<tst.count()<<" items in the tree"<<endl;

	// test build a balanced ternary search tree.
	tst.buildBalancedTree( itemVector );
	itemVector.clear();


	// Get sorted item list
	Vector<int> sortedVector = tst.getSortedItemIndexes();
	for ( unsigned i=0; i < sortedVector.count(); i++)
	{
		int index = sortedVector[i];
		const char * key = tst.getKey( index );
		string * value = tst.getValue( index ) ;
		cout<<"key/value "<< key <<" - "<<value->c_str()<<endl;
	}
	// Partial matching search

	string key("?a?a?a");
	//string key("wo*");
	Vector<int> pmVector = tst.partialMatchSearch( key.c_str() );

	for (unsigned j=0;j<pmVector.count();j++)
	{
		cout<<"\tmatching: "<< tst.getKey( pmVector[j] ) <<" -  "<< tst.getValue( pmVector[j] )->c_str()<<endl;
	}
	cout<<"total matches "<< (int)pmVector.count()<<endl;

	// Near neighbor search

	string s("jerry");
	Vector<int> nearVector = tst.nearSearch( s.c_str(), 1 );

	for (unsigned j=0;j<nearVector.count();j++)
	{
		cout<<"\tmatching: "<< tst.getKey( nearVector[j]) << " -  " << tst.getValue( nearVector[j] )->c_str()<<endl;
	}
	cout<<"total matches "<<(int)nearVector.count()<<endl;

}

template<class T>
std::ostream& operator<<(std::ostream& os,const Vector<T>& v) {
    os << "Vector{";
    bool first = true;
    for(int i=0;i<v.count();++i) {
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

	int i = 0;

	if ( fileName ) {
		FILE * fp = fopen( fileName,"r" );
		if ( fp != NULL ) {
			char s[MAX_LINE_LEN];

			while( fgets( s, MAX_LINE_LEN, fp ) != NULL)
			{
				string key = s;
				key.trim("\r").trim("\n");//remove \r or \n
				int value = i;
				TstItem<int> item( key, value );
				itemVector.add( item );
				i++;
			}
			fclose( fp );
		}

        tst.buildBalancedTree( itemVector );
        itemVector.clear();

        string input = "";

        while ((input += getchar()) != "Q") {

            if (input == "C") {
                input = getchar();
            }
            else {
                if (input.length() > 5) {
                    correctionIds = tst.prefixSearch(input.c_str());
                }
                else {
                    correctionIds = tst.prefixSearch(input.c_str());
                }
                for (unsigned j = 0; j < correctionIds.count();j++) {
                    int tmp = correctionIds.get(j);
                    corrections.add(tst.getKey(tmp));
                }
                cout << corrections;

                correctionIds.clear();
                corrections.clear();
            }
        }

	}
}

int main()
{
	struct tm *newtime;
	time_t aclock;

	time( &aclock );   // Get time in seconds
	newtime = localtime( &aclock );   // Convert time to struct tm form
	// Print local time as a string
	printf( "Starting datetime: %s", asctime( newtime ) );


	// Unit Test for Ternary Search Tree
	//ternarySearchTreeUnitTest();

	// Construct tree and while loop for input, display neighbors
	ternarySearchTreeNeighborTest();

	time( &aclock );   // Get time in seconds
	newtime = localtime( &aclock );   // Convert time to struct tm form
	// Print local time as a string
	printf( "Ending datetime: %s", asctime( newtime ) );

}
