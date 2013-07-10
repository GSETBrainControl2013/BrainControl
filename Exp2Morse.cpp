#include <iostream>
using namespace std;

void translateExp(Expression inputExp);
string resultString;

int main(){
    Expression inputExp;
    cin >> inputExp;
    translateExp(inputExp);
    cout << resultString;
    return 0;
}

void translateExp(Expression inputExp){
    if (inputExp == Expression::LSMIRK){
        resultString += "-";
    }
    else if (inputExp == Expression::RSMIRK){
        resultString += ".";
    }
    else if (inputExp == Expression::FURROW){
        resultString += " ";
    }
    else if (inputExp == Expression::SHAKE_RIGHT) {
        resultString += "/";
    }
}
