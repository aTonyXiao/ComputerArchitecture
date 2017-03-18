//
//  main.cpp
//
//  Created by Alan Sin on 24/7/2016.
//  Copyright © 2016 Alan Sin. All rights reserved.
//

#include <fstream>
#include <cstring>
#include <iostream>
using namespace std;

void ReadInput(char* Name){
    string Pages[16];
    string OutputRec[4];
    bool OutputUsed[4];
    int RandomTemp;
    for (int i = 0; i <= 3; i++){
        OutputRec[i] = "notused";
        OutputUsed[i] = false;
    }
    ifstream Input;
    Input.open(Name);
    string temp;
    for (int i = 0; i <= 15; i++){
        getline(Input, temp);
        temp.resize(5);
        Pages[i] = temp;
    }
    int clockInt = 0;

    ofstream Output;
    Output.open("vm-out.txt");
    while(getline(Input, temp)){
        bool flag = false;
        RandomTemp = temp.length() - 3;
        temp.resize(RandomTemp);
        RandomTemp = RandomTemp + 3;
        temp.resize(RandomTemp, '0');
        for (int i = 0; i <= 3; i++){
            if (OutputRec[i] == "notused"){
                OutputRec[i] = temp;
                flag = true;
                OutputUsed[i] = true;
                break;
            }
            if (OutputRec[i] == temp){
                flag = true;
                OutputUsed[i] = true;
                break;
            }
        }
        if (flag == false){
            while (OutputUsed[clockInt] == true){
                OutputUsed[clockInt] = false;
                //cout << OutputRec[0] << " " << OutputRec[1] << " " << OutputRec[2] << " " << OutputRec[3] << " " << "clock:" << clockInt << " " << temp << endl;
                if (clockInt == 3) clockInt = 0;
                else clockInt++;

            }
            OutputRec[clockInt] = temp;
            OutputUsed[clockInt] = true;
                if (clockInt == 3) clockInt = 0;
                else clockInt++;
        }
        Output << OutputRec[0];
        if (OutputRec[1] != "notused") Output << " " << OutputRec[1];
        if (OutputRec[2] != "notused") Output << " " << OutputRec[2];
        if (OutputRec[3] != "notused") Output << " " << OutputRec[3];
        Output << endl;
        //cout << OutputUsed[0] << " " << OutputUsed[1] << " " << OutputUsed[2] << " " << OutputUsed[3] << endl;
        // if (clockInt == 3) clockInt = 0;
        // else clockInt++;
        
    }
    Output.close();
}

int main(int argc, char* argv[]){
    ReadInput(argv[1]);
}