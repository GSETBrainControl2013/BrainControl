#include "TST.h"

int size(TST t) {
    return *t::N;
}
/*
bool contains(TST t, std::string key) {
    return t.get(key) != NULL;
}
*/

TST add(TST t, std::string s, int i) {

    if (t == NULL) {
        TST temp = new TST(s[i],NULL,NULL,NULL,false);
        t=temp;
    }

    if (s[i] < t->current) {
        add(t->left,s,i);
    }
    else if (s[i] > t->current) {
        add(t->right,s,i);
    }
    else
    {
        if ( i + 1 == s.length()) {
            t->wordend = true;
        }
        else
        {
            add(t->mid,s,i+1);
        }
    }

    *t::N++;
    return t;
}
