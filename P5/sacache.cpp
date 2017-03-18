//
//  main.cpp
//  sacache
//
//  Created by Tony Xiao on 7/24/16.
//  Copyright © 2016 TonyXiao. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <ctype.h>
#include <fstream>
//#include <array>
#include <vector>
using namespace std;
#include <map>

// class CacheLine{};



class CacheLine
{
public:
    int data[4] ={};
    int tag;
    int dirtyBit;
    CacheLine(){
        tag = 0;
        dirtyBit = 0;
    }
};

std::vector<CacheLine> mem;
int findinCache(CacheLine* cacheLine, int tag);
void count(int counter[], int found);
void read(int found, int* counter, CacheLine* cache, ofstream *output, int tag, int offset, int set);
void write(int found, CacheLine* cache, int offset, int data, int* counter, int tag);




int findinCache(CacheLine* cacheLine, int tag)
{
    for (int found = 0; found < 6; found++)
        if (cacheLine[found].tag== tag)
            return found;
    
    return 6;
}

void count(int counter[], int found)
{
    for (int i = 0; i < 6; i++)
        counter[i]++;
    counter[found] = 0;
}

void read(int found, int* counter, CacheLine* cache, ofstream *output, int tag, int offset, int set)
{
    if (found == 6) // miss
    {
        int min = 0;
        for (int i = 1; i < 6; i++)
            if (counter[min] < counter[i])
                min = i;
        
        if (cache[min].dirtyBit)
            mem.push_back(cache[min]);
        
        for(int i =0; i< mem.size();i++)
            if(tag == mem[i].tag)
                for(int j = 0; j<4;j++)
                    cache[min].data[j] = mem[i].data[j];
        
        if(cache[min].data[offset] > 15)
            *output << hex << uppercase << (int) cache[min].data[offset];
        else
            *output << hex << uppercase << 0 << (int) cache[min].data[offset];
        
        *output << " " << 0 << " " << cache[min].dirtyBit << endl;
        
        cache[min].tag = tag;
        cache[min].dirtyBit = 0;
        count(counter, min);
    }
    else // hit
    {
        if(cache[found].data[offset] > 15)
            *output << hex << uppercase << (int) cache[found].data[offset];
        else
            *output << hex << uppercase << 0 << (int) cache[found].data[offset];
        
        *output << " " << 1 << " " << cache[found].dirtyBit << endl;
        
        count(counter, found);
    }
}

void write(int found, CacheLine* cache, int offset, int data, int* counter, int tag)
{
    if (found != 6) // hit
    {
        cache[found].data[offset] = data;
        cache[found].dirtyBit = 1;
        count(counter, found);
    }
    else // miss
    {
        
        int min = 0;
        for (int i = 1; i < 6; i++)
            if (counter[min] < counter[i])
                min = i;
        
        
        if (cache[min].dirtyBit)
            mem.push_back(cache[min]);
        
        for(int i =0; i< mem.size();i++)
            if(tag == mem[i].tag)
                for(int j = 0; j<4;j++)
                    cache[min].data[j] = mem[i].data[j];
        
        
        
        cache[min].data[offset] = data;
        cache[min].tag = tag;
        cache[min].dirtyBit = 1;
        count(counter, min);
    }
    
    
}



int main(int argc, char *argv[])
{
    ifstream source(argv[1]);
    ofstream output("sa-out.txt");
    int counter[10][6] = {};
    CacheLine cache[10][6] = {};
    
    
    char loc[4]= {};
    
// while (!source.eof())
// {


// }

    while(source >> loc)
    {
        
        int location = (int)strtol(loc, NULL, 16);
        char rORw [10] ={};
        source >> rORw;
        char dat[10] = {};
        source >> dat;
        int data = (int)strtol(dat, NULL, 16);
        int tag = location >> 2;
        int offset = location % 4;
        int set = tag % 10;
        int found = findinCache(cache[set], tag);
        
        if (rORw[0] == 'F') // write
        {
            write(found, cache[set], offset, data, counter[set], tag);
        }
        else // read
        {
            read(found, counter[set], cache[set], &output, tag, offset, set);
            // void read(int found, int** counter, CacheLine** cache, ofstream *output, int set, int tag, int offset)
        }
    }
    
    source.close();
    output.close();
    return 0;
}