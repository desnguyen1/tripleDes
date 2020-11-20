#include "keyScheduling.h"

#include <iostream>
#include <string>

using std::endl;
using std::cin;
using std::cout;
using std::string;


int pc_one[56] = {56, 48, 40, 32, 24, 16, 8,
                  0, 57, 49, 41, 33, 25, 17,
                  9,	1, 58, 50, 42, 34, 26,
                  18, 10, 2, 59, 51, 43, 35,
                  62, 54, 46, 38, 30, 22 ,14,
                  6, 61, 53, 45, 37, 29, 21,
                  13, 5, 60, 52, 44, 36, 28,
                  20, 12, 4, 27, 19, 11, 3};

int pc_two[48] = {13, 16, 10, 23, 0, 4,
                  2, 27, 14, 5, 20, 9,
                  22,	18, 11,	3, 25, 7,
                  15, 6, 26, 19, 12, 1,
                  40,	51,	30, 36, 46, 54,
                  29, 39,	50,	44,	32,	47,
                  43,	48,	38,	55,	33,	52,
                  45,	41,	49,	35,	28,	31};

//get key
string getKey(){
    string key;
    cout<<"\nSecret Key must be 8 characters with no spaces (ASCII)";
    cout<<"\nEnter a Secret key: ";
    cin>>key;
    return key;
}

string leftShift(string key){
    string output="";
    for(int i = 1; i< 28;i++){
        //output[0] = key[1]
        output += key[i];
    }
    output += key[0];
    return output;
}

string rightShift(string key){
    string output="";
    //ouput[0] = last bit in key
    output += key[key.length()-1];
    for(int i =0; i<27;i++){
        //output[1]=key[0]
        output += key[i];
    }
    return output;
}

void generateSubKeys(string key, string subKey[]){
    int loc, roundNum;
    string pcOneOut="", pcTwoOut="";
    string c="", d="", combined="";
    //pc_one

    for(int i=0; i<64; i++){
        loc = pc_one[i];
        pcOneOut += key[loc];
    }
    //cout<<"\npcOneOut: "<<pcOneOut;

    //split
    c = pcOneOut.substr(0, 28);
    d = pcOneOut.substr(28, 28);

    //16 rounds to produce 16 subKeys
    for(roundNum =1; roundNum<=16;roundNum++ ) {
        //cout<<"\nround Number #"<<roundNum;
        pcTwoOut = "";

        //shift one for all rounds
        c=leftShift(c);
        d=leftShift(d);

        //shift again for rounds that are not 1,2,9,16
        if (roundNum != 1 && roundNum != 2 && roundNum != 9 && roundNum != 16) {
            c=leftShift(c);
            d=leftShift(d);
        }
        //combine left and d
        combined = c + d;

        //test to see if bits shifted correctly
        //cout<<"\nc: "<<left;
        //cout<<"\nd: "<<d;
        //cout<<"\ncombined: "<<combined;

        //pc_two
        for(int i =0; i<48;i++){
            loc = pc_two[i];
            pcTwoOut += combined[loc];
        }

        //store subkeys for each round
        subKey[roundNum-1] = pcTwoOut;
        //cout<<"\nRound #"<<roundNum;
        //cout<<"\ntest subkey: "<<subKey[roundNum-1];
    }

}

void generateSubKeysDecryption(string key, string subKey[]){
    int loc, roundNum=16, num=16;
    string pcOneOut="", pcTwoOut="";
    string c="", d="", combined="";
    //pc_one

    for(int i=0; i<64; i++){
        loc = pc_one[i];
        pcOneOut += key[loc];
    }

    //split
    c = pcOneOut.substr(0, 28);
    d = pcOneOut.substr(28, 28);

    for(roundNum=1; roundNum<=16; roundNum++){
        pcTwoOut="";
        if(roundNum != 1){
            c=rightShift(c);
            d=rightShift(d);
            //shift right twice if round is not 2,9,16
            if(roundNum != 2 && roundNum != 9 && roundNum != 16){
                c=rightShift(c);
                d=rightShift(d);
            }
        }
        combined = c+d;
        //pc_two
        for(int i =0; i<48;i++){
            loc = pc_two[i];
            pcTwoOut += combined[loc];
        }
        //print k16 to k1
        //so k16 will be k1
        subKey[roundNum-1] = pcTwoOut;
        //cout<<"\nRound #"<<roundNum;
        //cout<<"\ntest subkey: "<<subKey[roundNum-1];
    }
}

