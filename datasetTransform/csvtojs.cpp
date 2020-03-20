#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>


void ParseCSV(std::vector<std::vector<std::string>> &dataSet, std::istream &source);
void ParseCSVRow(std::vector<std::string> &rowData, std::istream &source);
std::string ParseCSVField(std::istream &source);
std::string ParseCSVEscapedField(std::istream &source);
bool ParseCSVEscapedFieldQuote(std::ostream &destination, std::istream &source);
void EscapeChar(std::ostream &destination, char val);
std::string QuoteString(std::string input);
std::string CSVHeadingFieldToJSName(std::string input);


int main() {
  const std::string BASE_DIRECTORY = "/mnt/sda1/Documents/school/2020-1_Winter/cs260/unit4/cs260CreativeProject3/";
  const std::string DATA_SOURCE_DIRECTORY = "csse_covid_19_data/csse_covid_19_daily_reports/";
  const std::string DATA_DESTINATION_DIRECTORY = "datasetTransform/";
  const std::string DATA_SOURCE_FILENAMES_LOCATION = BASE_DIRECTORY + DATA_DESTINATION_DIRECTORY + "datafiles.txt";
  const std::string DATA_DESTINATION_LOCATION = BASE_DIRECTORY + DATA_DESTINATION_DIRECTORY + "covidData.js";

  const std::string GET_FILES_COMMAND = "(cd " + BASE_DIRECTORY + DATA_SOURCE_DIRECTORY + " && ls *.csv) >" + DATA_SOURCE_FILENAMES_LOCATION;
  std::system(GET_FILES_COMMAND.c_str());

  std::ifstream sourceFilesIFStream;
  sourceFilesIFStream.open(DATA_SOURCE_FILENAMES_LOCATION);
  if (!sourceFilesIFStream.fail()) {
    std::vector<std::string> sourceFiles{std::vector<std::string>()};
    while (!sourceFilesIFStream.eof()) {
      std::string nextFile;
      sourceFilesIFStream >> nextFile;
      sourceFiles.push_back(nextFile);
    }
    sourceFilesIFStream.close();

    try {
      std::vector<std::map<std::string,std::string>> data = std::vector<std::map<std::string,std::string>>();
      for (std::size_t i = 0; i < sourceFiles.size(); ++i) {
        std::ifstream csvFile;
        csvFile.open(BASE_DIRECTORY + DATA_SOURCE_DIRECTORY + sourceFiles.at(i));
        if (!csvFile.fail()) {
          std::string date = sourceFiles.at(i).substr(0, sourceFiles.at(i).size() - 4);
          std::cout << date << std::endl;

          csvFile.unsetf(std::ios_base::skipws);
          std::vector<std::vector<std::string>> csvData = std::vector<std::vector<std::string>>();
          ParseCSV(csvData, csvFile);

          for (std::size_t j = 1; j < csvData.size(); ++j) {
            if ((csvData.at(0).size() != 6) && (csvData.at(0).size() != 8)) {
              std::ostringstream eout;
              eout << "Data Error: number of headding fields is not 6 or 8; j = " << j << "; head.size() = " << csvData.at(0).size() << '.' << std::endl;
              throw std::runtime_error(eout.str());
            }

            if (csvData.at(j).size() == csvData.at(0).size()) {
              data.push_back(std::map<std::string,std::string>());

              std::pair<std::map<std::string,std::string>::iterator, bool> result = data.at(data.size()-1).insert(std::map<std::string,std::string>::value_type("Date Reported", QuoteString(date)));
              for (std::size_t k = 0; k < csvData.at(0).size(); ++k) {
                std::string dataValue;
                if (k < 3) {
                  dataValue = QuoteString(csvData.at(j).at(k));
                }
                else {
                  dataValue = csvData.at(j).at(k);
                  if (dataValue == "") {
                    dataValue = "0";
                  }
                }

                std::pair<std::map<std::string,std::string>::iterator, bool> result = data.at(data.size()-1).insert(std::map<std::string,std::string>::value_type(csvData.at(0).at(k), dataValue));
                if (!result.second) {
                  std::ostringstream eout;
                  eout << "Data Error: duplicate headding field; j = " << j << '.';
                  throw std::runtime_error(eout.str());
                }
              }
            }
            else {
              std::ostringstream eout;
              eout << "Data Error: number of row fields does not match number of headding fields; j = " << j << "; head.size() = " << csvData.at(0).size() << "; row.size() = " << csvData.at(j).size() << '.' << std::endl;
              throw std::runtime_error(eout.str());
            }
          }

          csvFile.close();
        }
        else {
          std::ostringstream eout;
          eout << "File Error: failed to open file \"" << sourceFiles.at(i) << '\"' << std::endl;
          throw std::runtime_error(eout.str());
        }
      }

      std::ofstream jsFile;
      jsFile.open(DATA_DESTINATION_LOCATION);
      if (!jsFile.fail()) {
        jsFile << "let covidData = [" << std::endl;
        for (std::size_t i = 0; i < data.size(); ++i) {
          std::cout << "\r" << std::setprecision(4) << (static_cast<double>(i * 100) / static_cast<double>(data.size() - 1)) << "%  ";
          jsFile << "    {" << std::endl;
          for (std::map<std::string, std::string>::iterator j = data.at(i).begin(); j != data.at(i).end(); ++j) {
            jsFile << "        " << CSVHeadingFieldToJSName(j->first) << ": " << j->second;
            if (j != --data.at(i).end()) {
              jsFile << ',';
            }
            jsFile << std::endl;
          }
          jsFile << "    }";
          if (i < (data.size() - 1)) {
            jsFile << ',';
          }
          jsFile << std::endl;
        }
        std::cout << std::endl;
        jsFile << "]" << std::endl;
        jsFile << std::endl;
        jsFile << "export default covidData";
        jsFile.close();
      }
      else {
        std::ostringstream eout;
        eout << "File Error: failed to open output file";
        throw std::runtime_error(eout.str());
      }
    }
    catch (std::runtime_error er) {
      std::cout << er.what() << std::endl;
    }
    catch (std::logic_error er) {
      std::cout << er.what() << std::endl;
    }
  }

  return 0;
}

void ParseCSV(std::vector<std::vector<std::string>> &dataSet, std::istream &source) {
  while (!source.fail() && !source.eof()) {
    char tempChar = source.peek();
    switch (tempChar) {
      case '\n':
        source >> tempChar;
        break;
      case '\r':
        source >> tempChar;
        break;
      default:
        dataSet.push_back(std::vector<std::string>());
        ParseCSVRow(dataSet.at(dataSet.size() - 1), source);
        if (dataSet.at(dataSet.size() - 1).size() < 1) {
          dataSet.pop_back();
        }
        break;
    }
  }

  return;
}

void ParseCSVRow(std::vector<std::string> &rowData, std::istream &source) {
  bool isContinue{true};
  while (isContinue && !source.fail() && !source.eof()) {
    char tempChar = source.peek();
    std::string fieldData{""};
    switch (tempChar) {
      case '\"':
        source >> tempChar;
        fieldData = ParseCSVEscapedField(source);
        break;
      default:
        fieldData = ParseCSVField(source);
        break;
    }
    rowData.push_back(fieldData);

    if (!source.fail() && !source.eof()) {
      tempChar = source.peek();
      switch (tempChar) {
        case '\n':
          isContinue = false;
          break;
        case '\r':
          isContinue = false;
          break;
        case ',':
          source >> tempChar;
          break;
        default:
          std::ostringstream eout;
          eout << "Error: end of field reached without reaching deliminator; nextChar = " << tempChar << '.';
          throw std::logic_error(eout.str());
          break;
      }
    }
  }

  return;
}

std::string ParseCSVField(std::istream &source) {
  bool isContinue{true};
  std::ostringstream sout;
  while (isContinue && !source.fail() && !source.eof()) {
    char tempChar = source.peek();
    switch(tempChar) {
      case ',':
      case '\n':
      case '\r':
        isContinue = false;
        break;
      default:
        source >> tempChar;
        EscapeChar(sout, tempChar);
        break;
    }
  }

  return sout.str();
}

std::string ParseCSVEscapedField(std::istream &source) {
  bool isContinue{true};
  std::ostringstream sout;
  while (isContinue && !source.fail() && !source.eof()) {
    char tempChar = source.peek();
    switch (tempChar) {
      case '\"':
        source >> tempChar;
        isContinue = ParseCSVEscapedFieldQuote(sout, source);
        break;
      default:
        source >> tempChar;
        EscapeChar(sout, tempChar);
        break;
    }
  }

  return sout.str();
}

bool ParseCSVEscapedFieldQuote(std::ostream &destination, std::istream &source) {
  if (!source.fail() && !source.eof()) {
    char tempChar = source.peek();
    switch (tempChar) {
      case '\"':
        source >> tempChar;
        EscapeChar(destination, tempChar);
        return true;
      case '\n':
      case '\r':
      case ',':
        return false;
      default:
        std::ostringstream eout;
        eout << "CSV Syntax Error: Unexpected character after escaped field; character = " << tempChar;
        throw std::runtime_error(eout.str());
    }
  }
}

void EscapeChar(std::ostream &destination, char val) {
  switch (val) {
    case '\t':
      destination << '\\' << 't';
    case '\n':
      destination << '\\' << 'n';
    case '\r':
      destination << '\\' << 'r';
    case '\"':
    case '\\':
      destination << '\\';
    default:
      if (val >= ' ') {
        destination << val;
      }
      break;
  }

  return;
}

std::string QuoteString(std::string input) {
  std::ostringstream sout;
  sout << '\"' << input << '\"';
  return sout.str();
}

std::string CSVHeadingFieldToJSName(std::string input) {
  if (input == "Confirmed") {
    return "confirmed";
  }
  else if (input == "Country/Region") {
    return "countryOrRegion";
  }
  else if (input == "Date Reported") {
    return "dateReported";
  }
  else if (input == "Deaths") {
    return "deaths";
  }
  else if (input == "Last Update") {
    return "lastUpdate";
  }
  else if (input == "Province/State") {
    return "provinceOrState";
  }
  else if (input == "Recovered") {
    return "recovered";
  }
  else if (input == "Latitude") {
    return "latitude";
  }
  else if (input == "Longitude") {
    return "longitude";
  }
  else {
    std::ostringstream eout;
    eout << "CSV to JS Error: Unexpected headding field; field = " << input;
    throw std::runtime_error(eout.str());
  }
}
