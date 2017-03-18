//
//  main.cpp
//  dmcache
//
//  Created by Tony Xiao on 7/23/16.
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

class CacheLine{
public:
    int tag;
    int set;
    int location;
    int dirtyBit;
    int word[8] = {};
    
    CacheLine()
    {
        tag = 0;
        set = 0;
        location = 0;
        dirtyBit = 0;
    }
};
void write();
void read();
void copy();
void copyLine(int location, int set, int tag, int offset, CacheLine* cache, int data);
void write(CacheLine *cache, int set, int location, int tag, int offset, int data, int*track);
void read(CacheLine *cache, int set, int tag, int data, ostream *output);



void copyLine(int location, int set, int tag, int offset, CacheLine* cache, int data)
{
            cache[set].location = location;
        //cout << location;
        cache[set].tag  = tag;
        // cout << tag;
        cache[set].set = set;
        cache[set].dirtyBit = 1;
        // cout<< data;
        cache[set].word[offset] = data;

}

void write();
void read();
std::vector<CacheLine> mainmem;


void write(CacheLine *cache, int set, int location,
           int tag, int offset, int data, int*track)
{
    
    if(track[set] == 0) // miss write to cache
    {
       copyLine(location, set, tag, offset, cache, data);
        track[set] = 1;
    }else if(cache[set].tag == tag)
    {
        cache[set].word[offset] = data;
        cache[set].dirtyBit = 1;
        track[set] = 1;
    }else
    {
        mainmem.push_back(cache[set]);
        copyLine(location, set, tag, offset, cache, data);
    }
    return;
}

void read(CacheLine *cache, int set, int tag,
          int data, ostream *output)
{
    
    
    if(cache[set].tag == tag)// in cache
    {
        if(data > 15)
            *output << uppercase << hex << data;
        else
            *output << 0 << uppercase << hex << data;
        *output << " " << 1 << " " << cache[set].dirtyBit << endl;
        
        // cout <<data;
        // if(data > 15)
        //     *output << uppercase << hex << data;
        // else
        //     *output << 0 << uppercase << hex << data;
        // *output << " " << 1 << " " << cache[set].dirtyBit << endl;
        
    }else // not in cache
    {
        int prevDirty = cache[set].dirtyBit;
        // cout << prevDirty;
        mainmem.push_back(cache[set]);

        for(unsigned int i = 0; i < mainmem.size(); i++)
        {
            if(mainmem[i].tag == tag)
                cache[set] = mainmem[i];
        }        
        // cout << data;
        cache[set].dirtyBit = 0;
        
        if(data > 15)
            *output << uppercase << hex << data;
        else
            *output << 0 << uppercase << hex << data;
        
        *output << " " << 0 << " " << prevDirty << endl;
        // cout << data
        // if(data > 15)
        //     *output << uppercase << hex << data;
        // else
        //     *output << 0 << uppercase << hex << data;
        
        // *output << " " << 0 << " " << prevDirty << endl;
    }
}


//std::array<CacheLine,65536> mainmem; // 2^16

//void addToMem(CacheLine line)
//{
//    for(int i =0; i<65536;i++)
//        if(mainmem[i] == null)
//            mainmem = line;
//}


int main(int argc, const char * argv[]) {
    CacheLine cache[32];
    int track[32]= {};
    std::string input;
    std::ifstream source(argv[1]);
    ofstream output("dm-out.txt");
    //    cout << argv[1];
    // while(1)
    // {
    char loc[4] = {};
    while (source >> loc)
    {
        // char loc[10]= {};
        // source >> loc;
        //        cout << loc;
        char rORw[10] ={};
        source >> rORw;
        char dat[10]= {};
        source >> dat;
        //        cout << dat;
        //        cout << rORw;
        int location = (int)strtol(loc, NULL, 16);
        int tag = location >> 8; // 11111111, 8
        int set = (location &0xFF) >> 3;
        int offset = location & 0x7; // 111, 3
        int data = (int) strtol(dat, NULL, 16);
        
        if(rORw[0] == 'F') // write
        {
            write(cache, set, location, tag, offset, data, track);
        }
        
        else // read
        {
            read(cache, set, tag, data, &output);
        }
        
    }
    // }
    output.close();
    source.close();
    //    CacheLine->clear();
    return 0;
}
