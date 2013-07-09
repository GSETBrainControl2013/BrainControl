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
    if (inputExp.lowerFace == EXP_SMIRK_LEFT){
        resultString += "-";
    }
    else if (inputExp.lowerFace == EXP_SMIRK_RIGHT){
        resultString += ".";
    }
    else if (inputExp.upperFace == EXP_FURROW){
        resultString += " ";
    }
    //use right nod for spacing between words
    //space will be indicated by / character

}
