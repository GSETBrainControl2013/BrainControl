#include <iostream>

using namespace std;

string arrMorseCode[26] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
                            "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
                            "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
                            "-.--", "--.."};
string arrMorseCodeTrans[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
                           "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

string compareMorse(string xyz);

int main()
{
    string comparison;
    cin >> comparison;
    cout << compareMorse(comparison);
    return 0;
}

string compareMorse(string xyz){
    for(int i = 0; i < 26; i++){
        if(xyz == (arrMorseCode[i])){
        return arrMorseCodeTrans[i];
        }
    }

    return "";
}
