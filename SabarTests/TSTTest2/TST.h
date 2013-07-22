#ifndef TST_H_INCLUDED
#define TST_H_INCLUDED
#include <string>

struct Node {

    static int N;

    char current;
    bool wordend;
    Node* left;
    Node* mid;
    Node* right;
    int value;
    Node(char c='@', int v=0, Node* l=NULL, Node* m=NULL, Node* r=NULL,bool end=false)
    {
        wordend=end;
        current=c;
        value = v;
        left=l;
        mid=m;
        right=r;
    }
};

typedef struct Node* TST;

int size(TST t);
bool contains(TST t, std::string key);
TST add(TST t, std::string s, int i);



#endif // TST_H_INCLUDED
