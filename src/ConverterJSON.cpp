#include "ConverterJSON.h"

const int LINE_LENGTH = 46;
const int HEADER_SPACER = 15;

//static members initialization -------------------------
ConverterJSON* ConverterJSON::instance = nullptr;
ConverterJSON* ConverterJSON::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ConverterJSON();
    }
    return instance;
}
//---------------------------------------------------------

vector<string> ConverterJSON::getTextDocuments()
{
    textDocuments.clear();
    for (const auto& doc : resourcesPaths)
    {
        //создаем файл на чтение docReadingStream
        ifstream docReadingStream(doc);
        if (docReadingStream.is_open())
        {
            //создаем string буффер, в который будем вкладывать все из docReadingStream
            string buffer;
            while (!docReadingStream.eof())
            {
                string b;
                docReadingStream >> b;
                buffer += b;
                buffer += " ";
            }
            textDocuments.push_back(buffer);
            docReadingStream.close();
        }
        else
        {
            std::cerr << "File content reading:\t- file not found error " + doc << "\n";
        }
    }
    cout << "Input docs read success: " << resourcesPaths.size() << " files\n";
    return textDocuments;
}

int ConverterJSON::getResponsesLimit() const
{
    return maxResponses;
}

vector<string> ConverterJSON::getRequests()
{
    return requests;
}

void ConverterJSON::putAnswers(vector<vector<pair<int, float>>> answers)
{
    nlohmann::json answersDict;
    if (!answers.empty())
    {
        cout << "Answers pushing... ";
        //создаем и открываем на чтение answersFile. ios_base::trunc - Задает удаление содержимого существующего файла при создании его управляющего объекта.
        ofstream answersFile(ANSWERS_FILE_PATH, ios_base::trunc);
        if (answersFile.is_open())
        {
            int requestCount {0};
            nlohmann::json answerDictionary;
            for (auto request : answers)
            {
                answersDict["answers"]["request" + to_string(requestCount)]["result"] = !request.empty();
                if (request.size() == 1)
                {
                    answersDict["answers"]["request" + to_string(requestCount)]["docid"] = request[0].first;
                    answersDict["answers"]["request" + to_string(requestCount)]["rank"] = request[0].second;
                }
                else
                {
                    auto relevance_array = nlohmann::json::array();
                    int relevanceID{0};
                    for (auto relevance : request)
                    {
                        ++relevanceID;
                        if (relevanceID > maxResponses) break;

                        auto relevance_member = nlohmann::json::object();
                        relevance_member["docid"] = relevance.first;
                        relevance_member["rank"] = relevance.second;
                        relevance_array.push_back(relevance_member);
                    }
                    answersDict["answers"]["request" + to_string(requestCount)]["relevance"] = relevance_array;
                }
                ++requestCount;
            }
            answersFile << answersDict;
            answersFile.close();
            cout << "done\n";
        }
        else
        {
            cout << "\t error - file not found: " + ANSWERS_FILE_PATH << "\n";
        }
    }
    else
    {
        cout << "No answers to push.\n";
    }
}

void ConverterJSON::readConfigFile(string path)
{
    ifstream configFile(path);
    if (configFile.is_open())
    {
        //в переменные applicationName, applicationVersion, maxResponses записываем данные из файла config.JSON
        nlohmann::json configDictionary;
        configFile >> configDictionary;
        applicationName = configDictionary["config"]["name"];
        applicationVersion = configDictionary["config"]["version"];
        maxResponses = configDictionary["config"]["max_responses"];
        resourcesPaths.clear();
        for (auto f : configDictionary["files"])
        {
            resourcesPaths.push_back(f);
        }

        for (int i = 0; i < HEADER_SPACER; ++i) {cout << "=";}
        cout << "[Initialization]";
        for (int i = 0; i < HEADER_SPACER; ++i) {cout << "=";}
        //выводим на консоль данные, которые были записаны чуть выше
        cout << "\n" << applicationName << "\n";
        cout << "Version: " << applicationVersion << "\n";
        cout << "Max responses per request: " << maxResponses << "\n";
        cout << "Files library: " << resourcesPaths.size() << "\n";
        for (int i = 0; i < LINE_LENGTH; ++i) {cout << "-";}
        cout << "\n";
        configFile.close();
    }
    else
    {
        cerr << "\t - file not found error: " + path << "\n";
    }
}

void ConverterJSON::readRequestFile(string path)
{
    cout << "Requests reading: ";
    //чтение из файла
    ifstream configFile(path);
    if (configFile.is_open())
    {
        nlohmann::json requestsDictionary;
        configFile >> requestsDictionary;
        requests.clear();
        for (auto f : requestsDictionary["requests"])
        {
            requests.push_back(f);
        }
        configFile.close();
        //дабы правильно вывести в консоль is or are
        string requestOrRequests = requests.size() == 1 ? " request is " : " requests are ";
        cout << requests.size() << requestOrRequests << "found\n";
    }
    else
    {
        cerr << "\t - file not found error: " + path << "\n";
    }
}

int ConverterJSON::getMaxResponses() const {
    return maxResponses;
}
