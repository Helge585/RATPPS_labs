#include "dataconverter.h"
#include <iostream>
#include <vector>
#include <algorithm>

dataConverter::dataConverter()
{
    is_set = false;
}

void dataConverter::setJsonData(std::string  data)
{
    unsigned left_ind, right_ind, len;
    left_ind = data.find("\"K\":") + 4;
    right_ind = data.find(",");
    len = right_ind - left_ind;
    k = std::stoi(data.substr(left_ind, len));

    left_ind = data.find("\"Sums\":[") + 8;
    right_ind = data.find("]");
    len = right_ind - left_ind;
    std::string str_sums = data.substr(left_ind, len);
    left_ind = 0;
    while (true) {
        right_ind = str_sums.find(",", left_ind);
        if (right_ind == std::string::npos) {
            sums.push_back(std::stod(str_sums.substr(left_ind, len - left_ind)));
            break;
        }
        sums.push_back(std::stod(str_sums.substr(left_ind, right_ind - left_ind)));
        left_ind = right_ind + 1;
    }

    left_ind = data.find("\"Muls\":[") + 8;
    right_ind = data.find("]", left_ind);
    len = right_ind - left_ind;
    std::string str_muls = data.substr(left_ind, len);
    left_ind = 0;
    while (true) {
        right_ind = str_muls.find(",", left_ind);
        if (right_ind == std::string::npos) {
            muls.push_back(std::stoi(str_muls.substr(left_ind, len - left_ind)));
            break;
        }
        muls.push_back(std::stoi(str_muls.substr(left_ind, right_ind - left_ind)));
        left_ind = right_ind + 1;
    }
    is_set = true;
}

std::string dataConverter::getJsonData()
{
    if (!is_set) {
        return "{}";
    }
    std::string result = "{";
    double sum_result = 0;
    for (unsigned i = 0; i < sums.size(); ++i) {
        sum_result += sums[i];
    }
    sum_result *= k;
    std::string str_sum_result = std::to_string(sum_result);
    str_sum_result = str_sum_result.substr(0, str_sum_result.find(".") + 3);
    result +=  "\"SumResult\":" + str_sum_result + ",";
    int mul_result = 1;
    for (unsigned i = 0; i < muls.size(); ++i) {
        mul_result *= muls[i];
    }
    result += "\"MulResult\":" + std::to_string(mul_result) + ",";
    sums.insert(sums.begin(), muls.begin(), muls.end());
    sort(sums.begin(), sums.end());
    result += "\"SortedInputs\":[";
    for (unsigned i = 0; ; ++i) {
        std::string str_sums_i = std::to_string(sums[i]);
        str_sums_i = str_sums_i.substr(0, str_sums_i.find(".") + 3);
        result += str_sums_i;
        if (i == sums.size() - 1) {
            result += "]";
            break;
        } else {
            result += ",";
        }
    }
    result += "}";
    return result;
}
