#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include <iostream>
#include <vector>

class dataConverter {
public:
    dataConverter();
    void setJsonData(std::string  data);
    std::string getJsonData();
private:
    std::vector<double> sums;
    std::vector<int> muls;
    int k;
    bool is_set;
};


#endif // DATACONVERTER_H
