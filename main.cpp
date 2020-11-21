#include <iostream>
#include <string>
//for bit to ascii conversion
#include <sstream>
#include <bitset>
#include "des.h"
#include "keyScheduling.h"

using std::endl;
using std::cin;
using std::cout;
using std::string;

//function prototypes
string binaryConversion();
string getPlaintext();
void get64BitBlock();
void to_ascii();

string binaryConversion(string key){
    int asciiValue;
    string reverseBinary = "";
    string binaryKey = "";
    //convert to ASCII first
    for(int i =0; i<=key.length()-1; i++) {
        reverseBinary = "";
        asciiValue = int(key[i]);
        //std::cout << "\nAscii Value: " << asciiValue;
        //ASCII to Binary
        while (asciiValue > 0) {
            if(asciiValue % 2 == 1){
                reverseBinary.push_back('1');
            }
            else{
                reverseBinary.push_back('0');
            }
            asciiValue /= 2;
        }
        std::reverse(reverseBinary.begin(), reverseBinary.end());
        //add extra 0's to make each char 8 bits
        //fix while loop to make bit correct
        while(reverseBinary.length() != 8){
            int numZeros = 8 - reverseBinary.length();
            string zeros="";
            for(int i =0; i<numZeros; i++) {
                zeros.push_back('0');
            }
            reverseBinary = zeros + reverseBinary;
        }

        //concatenate to make the proper binary Key, otherwise it will be in reverse order
        binaryKey += reverseBinary;

    }

    return binaryKey;
}

void to_ascii(string str, string& output){
    std::stringstream sstream(str);
    while(sstream.good()){
        std::bitset<8>bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output +=c;
    }
}

void get64BitBlock(string& text, string& bitBlock, int& len){
    string totalPlaintext="";
    if(text.length() > 64){
        bitBlock = text.substr(0,64);
        text.erase(0,64);
        len -= 64;
        totalPlaintext += bitBlock;
    }
    else if(text.length() <= 64){
        int numZeros = 64 - text.length();
        string zeros="";
        for(int i =0;i < numZeros;i++){
            zeros.push_back('0');
        }

        bitBlock = zeros+text;
        totalPlaintext += bitBlock;
        len = 0;
    }

}

string getPlaintext(){
    string plaintext;
    cout<<"\nEnter text to decrypt: ";
    std::getline(cin, plaintext);
    return plaintext;
}

int main() {
    string key, keyPart[3]; //keyParts is the 192-bit key split into 3 64-bit keys
    string plaintext="",ciphertext="", plaintextCopy="";
    string plainFromCiph = ""; //gets plaintext from decryption
    string asciiPlain="", asciiCiph=""; //plaintext/ciphertext in ASCII
    string bitBlock="";
    string subKeys[16];
    int len;

    plaintext = getPlaintext();
    plaintext=binaryConversion(plaintext);
    cout<<"\nplaintext in binary: "<<plaintext;
    plaintextCopy = plaintext;
    //cout<<"\ncopy plain: "<<plaintextCopy;
    cout<<"\nplaintext length: "<<plaintext.length()<<endl;
    key = getKey();
    while (key.length() != 24) {
        key = getKey();
    }
    key=binaryConversion(key);
    cout<<"\n"<<key;
    for(int i = 0; i<3; i++) {
        //avoids duplication
        ciphertext = "";

        keyPart[i] = key.substr(0, 64);
        key = key.erase(0, 64);
        //cout << "\nkeyPart[" << i << "]: " << keyPart[i];

        //key scheduling
        generateSubKeys(keyPart[i],subKeys);
        len = plaintextCopy.length();

        //encryption
        cout << "\nBeginning encryption.....";
        while(len > 0) {
            //cout<<"\nplain copy len: "<<len;
            //get 64 bit blocks of plaintext
            get64BitBlock(plaintextCopy, bitBlock, len);
            //cout<<"\nplain copy len: "<<len;

            ciphertext += des(bitBlock, subKeys);
            //cout<<"\ncipher: "<<ciphertext;
        }
        cout<<"\nCiphertext: "<<ciphertext;
        cout<<"\nlength of ciphertext:"<<ciphertext.length()<<endl;

        //testing
        plaintextCopy = ciphertext;
    }

    //decryption
    for(int i = 2; i>=0; i--){
        bitBlock = "";

        //avoids duplication
        plainFromCiph = "";

        cout<<"\nStarting decryption of ciphertext......";

        //generate subkeys in reverse order
        generateSubKeysDecryption(key, subKeys);
        len = ciphertext.length();
        while(len>0){
            //cout<<"\ncipher length: "<<len;

            //get 64bit blocks
            get64BitBlock(ciphertext,bitBlock, len);

            plainFromCiph += des(bitBlock, subKeys);
        }
        cout<<"\nPlaintext: "<<plainFromCiph;
        cout<<"\nlength of binary plaintext:"<<plainFromCiph.length();

        ciphertext = plainFromCiph;
    }

    //convert to ascii
    to_ascii(plainFromCiph, asciiPlain);
    cout<<"\nplaintext ASCII: "<<asciiPlain<<endl;

    return 0;
}

//TODO: get 24 character key and split into 3 key parts

//secretkejkdseridncdskekf