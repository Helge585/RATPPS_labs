#include <iostream>
#include <vector>
#include <algorithm>

class dataConverter {
public:
	dataConverter(std::string & data, bool is_xml);
	std::string getOutputData();
	void print();
private:
	void setXmlData(std::string & data);
	void setJsonData(std::string & data);
	std::string getXmlData();
	std::string  getJsonData();
private:
	std::vector<double> sums;
	std::vector<int> muls;
	int k;
	bool is_xml;
};

int main(int argc, char const *argv[])
{
	std::string data_type, data, buf;
	bool is_xml;

	std::cin >> data_type;
	if (data_type == "Xml") {
		is_xml = true;
	} else if (data_type == "Json") {
		is_xml = false;
	} else {
		std::cout << "Wrong type" << std::endl;
		return 0;
	}
	while (std::getline(std::cin, buf)) {
		data += buf;
	}

	dataConverter converter(data, is_xml);
	std::cout << converter.getOutputData() << std::endl;
	return 0;
}

dataConverter::dataConverter(std::string & data, bool is_xml)
{
	this->is_xml = is_xml;
	if (is_xml) {
		setXmlData(data);
	} else {
		setJsonData(data);
	}
}

std::string dataConverter::getOutputData() 
{
	if (is_xml) {
		return getXmlData();
	}
	return getJsonData();
}

void dataConverter::setJsonData(std::string & data)
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
}

std::string dataConverter::getJsonData()
{
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

void dataConverter::setXmlData(std::string & data)
{
	unsigned left_ind = data.find("<K>") + 3;
	unsigned right_ind = data.find("</K>");
	unsigned len = right_ind - left_ind;
	k =  std::stoi(data.substr(left_ind, len));
	int start_pos = 0;
	while (true) {
		left_ind = data.find("<decimal>", start_pos);
		if (left_ind == std::string::npos) {
			break;
		}
		left_ind += 9;
		right_ind = data.find("</decimal>", start_pos);
		len = right_ind - left_ind;
		sums.push_back(std::stod(data.substr(left_ind, len)));
		start_pos = right_ind + 9;
	}
	start_pos = 0;
	while (true) {
		left_ind = data.find("<int>", start_pos);
		if (left_ind == std::string::npos) {
			break;
		}
		left_ind += 5;
		right_ind = data.find("</int>", start_pos);
		len = right_ind - left_ind;
		muls.push_back(std::stoi(data.substr(left_ind, len)));
		start_pos = right_ind + 5;
	}	
}

void dataConverter::print()
{
	std::cout << k << std::endl;
	for (unsigned i = 0; i < sums.size(); ++i) {
		std::cout << sums[i] << " ";
	}
	std::cout << std::endl;
	for (unsigned i = 0; i < muls.size(); ++i) {
		std::cout << muls[i] << " ";
	}
	std::cout << std::endl;
}

std::string dataConverter::getXmlData()
{
	std::string result = "<Output>";
	double sum_result = 0;
	for (unsigned i = 0; i < sums.size(); ++i) {
		sum_result += sums[i];
	}
	sum_result *= k;
	std::string str_sum_result = std::to_string(sum_result);
	str_sum_result = str_sum_result.substr(0, str_sum_result.find(".") + 3);
	result +=  "<SumResult>" + str_sum_result + "</SumResult>";
	int mul_result = 1;
	for (unsigned i = 0; i < muls.size(); ++i) {
		mul_result *= muls[i];
	}
	result += "<MulResult>" + std::to_string(mul_result) + "</MulResult>";
	sums.insert(sums.begin(), muls.begin(), muls.end());
	sort(sums.begin(), sums.end());
	result += "<SortedInputs>";
	for (unsigned i = 0; i < sums.size(); ++i) {
		std::string str_sums_i = std::to_string(sums[i]);
		str_sums_i = str_sums_i.substr(0, str_sums_i.find(".") + 3);
		result += "<decimal>" + str_sums_i + "</decimal";
	}
	result += "</SortedInputs";
	return result;
}