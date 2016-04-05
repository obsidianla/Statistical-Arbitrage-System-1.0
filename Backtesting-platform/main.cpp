//
//  main.cpp
//  SAS
//
//  Created by Bowen Zhi on 3/30/16.
//  Copyright © 2016 Bowen Zhi. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

string Trim(string& str)
{
    str.erase(0,str.find_first_not_of(" \t\r\n"));
    
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    
    return str;
}

int main()
{
    ifstream fin("/Users/bowenzhi/Documents/sta_arb/test1.csv");
    
    string line;
    vector<string> date;
    vector<string> stock1;
    vector<string> stock2;
    while (getline(fin, line)) {
        //cout << line << endl;
        
        istringstream sin(line);
        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
        }
        
        string d = Trim(fields[0]);
        date.push_back(d);
        string s1 = Trim(fields[1]);
        stock1.push_back(s1);
        
        string s2 = Trim(fields[2]);
        stock2.push_back(s2);
        cout << d << "\t" << s1 << "\t" << s2 << endl;
    }
    /*for (vector<string>::iterator it=stock1.begin(); it != stock1.end(); it++) {
        cout << ' '<< *it;
        cout << '\n';
    }
    for (vector<string>::iterator it=stock2.begin(); it != stock2.end(); it++) {
        cout << ' '<< *it;
        cout << '\n';
    }
    for (vector<string>::iterator it=date.begin(); it != date.end(); it++) {
        cout << ' '<< *it;
        cout << '\n';
    }*/
    
    vector<float> spread;
    vector<float> stockOne;
    vector<float> stockTwo;
    float beta = 7.603;
    for (int i = 1; i < stock1.size();++i) {
        float stc1 = stof(stock1[i]);
        float stc2 = stof(stock2[i]);
        
        stockOne.push_back(stc1);
        stockTwo.push_back(stc2);
        spread.push_back(stc2 - beta * stc1);
        
    }
    for (int i = 1; i < stock1.size();++i) {
        cout << "stock1: " << stockOne[i] << ' ' << "stock2: " << stockTwo[i] <<' ' << "spread: "<< spread[i-1];
        //cout << stc1;
        cout << '\n';
    }
    // calculate the mean of spread
    float sum = 0;
    float mean;
    for (int i = 0; i < spread.size();++i) {
        sum += spread[i];
        //cout << stc1;
    }
    mean = sum/spread.size();
    cout << "The mean of spread is "<<mean <<endl;
    //calculate the variance of spread
    float sum_variance = 0;
    float variance;
    float std_deviation;
    for (int i = 0; i < spread.size();++i) {
        sum_variance += (spread[i] - mean)*(spread[i] - mean);
        //cout << stc1;
    }
    variance = sum_variance/spread.size();
    std_deviation = sqrt(variance);
    cout << "The variacne of spread is "<<variance <<endl;
    cout << "The standard deviation of spread is "<<std_deviation <<endl;
    
    // calculate the Z score
    vector<float> Z_score;
    for (int i = 0; i < spread.size();++i) {
        float p = (spread[i]-mean)/std_deviation;
        Z_score.push_back(p);
        cout << p;
        cout << '\n';
    }
    // transaction test
    int share1 = 0, share2 = 0;
    float totalMoney = 1000000;
    reverse(spread.begin(), spread.end());
    reverse(stockOne.begin(),stockOne.end());
    reverse(stockTwo.begin(), stockTwo.end());
    for(int i = 0; i < spread.size(); ++i){
        if ( Z_score[i] > 1.5 && share1 == 0 && share2 == 0 ){
            share1 = int(0.6 * totalMoney/stockOne[i]);
            share2 = - share1/beta;
            float shortMoney = share2 * stockTwo[i];
            totalMoney = 0.4 * totalMoney - shortMoney;
        }
        else if ( Z_score[i] < -2.0 && share1 == 0 && share2 == 0 ){
            share2 = int(0.6 * totalMoney/stockTwo[i]);
            share1 = - share2 * beta;
            float shortMoney = share1 * stockOne[i];
            totalMoney = 0.4 * totalMoney - shortMoney;
        }
        else if ( Z_score[i] > -0.9 && Z_score[i] < 0.8 && share1 != 0 && share2 != 0 ){
            if (share1 > 0) {
                totalMoney = totalMoney + share1 * stockOne[i];
                share1 = 0;
            }
            if (share2 < 0) {
                totalMoney = totalMoney + share2 * stockTwo[i];
                share2 = 0;
            }
            if (share1 < 0) {
                totalMoney = totalMoney + share1 * stockOne[i];
                share1 = 0;
            }
            if (share2 > 0) {
                totalMoney = totalMoney + share2 * stockTwo[i];
                share2 = 0;
            }
            
            
        }
        else if ( i == spread.size() - 1 && share1 != 0 && share2 != 0){
            if (share1 > 0) {
                totalMoney = totalMoney + share1 * stockOne[i];
                share1 = 0;
            }
            if (share2 > 0) {
                totalMoney = totalMoney + share2 * stockTwo[i];
                share2 = 0;
            }
            if (share1 < 0) {
                totalMoney = totalMoney + share1 * stockOne[i];
                share1 = 0;
            }
            if (share2 < 0) {
                totalMoney = totalMoney + share2 * stockTwo[i];
                share2 = 0;
            }
        }
        
    }
    float rateOfReturn = (totalMoney - 1000000)/1000000;
    cout << "The total money at the end of the year is " << totalMoney << endl;
    cout << "The rate of return over the year is " << rateOfReturn << endl;
    return 0;
}
