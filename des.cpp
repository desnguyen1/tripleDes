#include "des.h"

#include <iostream>
#include <string>

using std::endl;
using std::cin;
using std::cout;
using std::string;

int IP[64] = {57, 49, 41, 33, 25, 17, 9, 1,
              59, 51, 43, 35, 27, 19, 11, 3,
              61, 53, 45, 37, 29, 21, 13, 5,
              63, 55, 47, 39, 31, 23, 15, 7,
              56, 48, 40, 32, 24, 16, 8, 0,
              58, 50, 42, 34, 26, 18, 10, 2,
              60, 52, 44, 36, 28, 20, 12, 4,
              62, 54, 46, 38, 30, 22, 14, 6};

int expansionBox[48]={31, 0, 1,	2, 3, 4,
                      3, 4, 5, 6,	7, 8,
                      7, 8, 9, 10, 11, 12,
                      11, 12,	13,	14,	15,	16,
                      15,	16,	17,	18,	19,	20,
                      19,	20,	21,	22,	23,	24,
                      23,	24,	25,	26,	27,	28,
                      27,	28,	29,	30,	31,	0};

int sBoxes [8][4][16] = {
        //s1
        {14, 4, 13,	1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
        },
        //s2
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
        },
        //s3
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15,	1,
                13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14,	7,
                1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
        },
        //s4
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
        },
        //s5
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
        },
        //s6
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
        },
        //s7
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
        },
        //s8
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
        }
};
int permutation[32] = {
        15,	6, 19, 20, 28, 11, 27, 16,
        0, 14, 22, 25, 4, 17, 30, 9,
        1, 7, 23, 13, 31, 26, 2, 8,
        18, 12, 29,	5, 21, 10, 3, 24
};
int fp[64]={
        39,	7, 47, 15, 55, 23, 63, 31,
        38,	6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34,	2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25,
        32,	0, 40, 8, 48, 16, 56, 24
};
//addition mod 2
string xorFunc(string str1, string str2){
    string xorStr="";
    for(int i =0; i<str1.size(); i++){
        //1 xor 1 or 0 xor 0
        if((str1[i] + str2[i]) % 2 == 0){
            xorStr.push_back('0');
        }
            //1 xor 0
        else{
            xorStr.push_back('1');
        }
    }
    return xorStr;
}
int binary_to_num(string binary){
    int num=0, power=0;
    int i = binary.length()-1;
    while(i>=0){
        //if 1 multiply 2^power
        if(binary[i] == '1'){
            num += pow(2, power);
        }
        //increase power of 2 as we got right to left
        power++;
        i--;
    }
    return num;

}
string num_to_binary(int num){
    string str="";
    while(num != 0){
        if(num % 2 ==0 ){
            str.push_back('0');
        }
        else{
            str.push_back('1');
        }
        num /= 2;
    }
    //each sBlock requires output of 4 bits
    while (str.length() < 4){
        str = "0" + str;
    }
    return str;
}

string des(string text, string subkeys[]){
    string output="";
    string ipOut="", expOut="", xorOut="", sBoxOut="", fFuncOut="";
    string left="", right="", nextRight="", combine="";
    int loc, rounds;

    //start with IP
    for(int i=0; i<64; i++){
        loc = IP[i];
        ipOut += text[loc];
    }
    //split
    left = ipOut.substr(0,32);
    right = ipOut.substr(32,32);

    for(rounds=1; rounds <= 16; rounds++) {
        //fFunction
        //fFunction: expansion
        for (int i = 0; i < 48; i++) {
            loc = expansionBox[i];
            expOut += right[loc];
        }

        //fFunction: xor
        xorOut = xorFunc(expOut, subkeys[rounds - 1]);

        //fFunction: sBoxes
        string sBoxBits[8][6]={};
        //split into blocks of 6bits (48/6=8)
        int k =0;
        for(int i =0; i<8; i++){
            for(int j = 0; j<6; j++){
                sBoxBits[i][j] = xorOut[k];
                k++;
                //cout<<"\nsBoxBits: "<<sBoxBits[i][j];
            }
        }
        //for each sBlock
        string rowNum, columnNum;
        int value[8];
        for(int i = 0; i<8; i++) {
            rowNum = "";
            columnNum="";
            int row = 0;
            int column = 0;
            int r = 0, c = 1;

            //get row number
            for (int j = 0; j < 2; j++) {
                rowNum += sBoxBits[i][r];
                r = 5;
            }
            for (int k = 0; k < 4; k++) {
                columnNum += sBoxBits[i][c];
                c++;
            }
            row = binary_to_num(rowNum);
            //cout<<"\nrow #"<<row;
            column = binary_to_num(columnNum);
            //cout<<"\ncolumn #"<<column;
            value[i] = sBoxes[i][row][column];
            /*for(int x=0; x<8;x++){
                cout<<"value: "<<value[i];
            */
            sBoxOut += num_to_binary(value[i]);
        }
        //fFunction: permutation
        for(int i =0; i<32;i++){
            loc = permutation[i];
            fFuncOut += sBoxOut[loc];
        }

        //xor fFunction output with left
        nextRight= xorFunc(left, fFuncOut);

        //next left is prev right
        left=right;
        right = nextRight;

    }//end of rounds loop

    //swap one last time
    string temp="";
    temp = left;
    left = right;
    right = temp;

    //combine for final permutation
    combine = left+right;

    //final permutation
    for(int i=0; i<64; i++){
        loc = fp[i];
        output += combine[loc];
    }

    return output;
}