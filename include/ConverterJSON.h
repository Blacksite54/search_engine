#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
using namespace std;

class ConverterJSON
{
    ConverterJSON() = default; // частный конструктор для реализации
    static ConverterJSON* instance;
    const string ANSWERS_FILE_PATH = "answers.json";
    string applicationName;
    string applicationVersion;
    int maxResponses  {5};
    vector<string> resourcesPaths;
    vector<string> textDocuments;
    vector<string> requests;

public:
    /**
    * Получаем объект экземпляра класса Converter JSON
    * @return экземпляр синглтона
    */
    static ConverterJSON* getInstance();
    /**
    * Получаем объект экземпляра класса Converter JSON
    * @return экземпляр синглтона
    */
    vector<string> getTextDocuments();
    /**
    * Получите максимальное количество ответов на запрос
    * @return максимальное количество ответов
    */
    int getResponsesLimit() const;
    /**
     * Получать запросы от requests.json
     * @return вектор запросов
     */
    vector<string> getRequests();
    /**
     * Помещать результаты запросов в answers.json file
     * @param[in] answers - вектор ответов, которые будут помещены в answers.json file
     */
    void putAnswers(vector<vector<pair<int, float>>> answers);
    /**
    * Прочитайте конфигурационный файл, указанный в CONFIG_FILE_PATH
    */
    void readConfigFile(string path = "config.json");
    /**
     * Файл запроса на чтение, указанный в REQUEST_FILE_PATH
     */
    void readRequestFile(string path = "requests.json");
    /**
     * Получите максимальное количество ответов, которые могут быть возвращены поисковым сервером
     * @return максимальное количество ответов
     */
    int getMaxResponses() const;
};
