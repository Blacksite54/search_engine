#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include "InvertedIndex.h"
using namespace std;

struct RelativeIndex
{
    size_t doc_id;
    float rank;
    size_t absoluteIndex = 0; // Added for convenience.

    bool operator ==(const RelativeIndex& other) const 
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
    bool operator > (const RelativeIndex& other) const
    {
        return (rank > other.rank || (rank == other.rank && doc_id < other.doc_id));
    }
};

class SearchServer 
{
    public:
    
    /**
    *ссылка на экземпляр инвертированного индекса для
    * дайте поисковому серверу знать частоту слов в файлах.
    */
    SearchServer(InvertedIndex& idx) : _index(idx){ };

    /**
    * Обработайте поисковый запрос
    * @param [in] queries_input - поисковые запросы из requests.json
    * @return отсортированный список релевантных ответов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

    /**
    * Установите максимальное количество ответов с поискового сервера
    * @param [in] newMaxResponses - новое значение для максимального количества ответов
    */
    void setMaxResponses(const int& newMaxResponses);

    private:

    InvertedIndex _index;
    int maxResponses {5};

    /**
     * Получение уникальных слов из строки запроса
     * @param [in] request - отдельная строка из request.json
     * @return набор уникальных слов
     */
    set<string> getUniqueWords(const string& request);

    /**
     * Получаем вектор записей для набора слов
     * @param [in] words - набор слов
     * @return вектор записей
     */
    vector<pair<string, size_t>> getWordsEntries(const set<string>& words);

    /**
     * Отсортируйте вектор записей в направлении возрастания количества записей
     * @param [in/out] wordsEntries - вектор записей
     */
    void sortWordsAscendingToEntries(vector<pair<string, size_t>>& wordsEntries);

    /**
     * Получите вектор документов, в которых можно найти все слова из запроса
     * @param [in] words - вектор пар "word - entry count"
     * @return вектор идентификаторов документов, в котором можно найти все слова
     */
    vector<size_t> getAllDocumentsWithWords(const vector<pair<string, size_t>>& words);

    /**
     * Вычислите абсолютную релевантность документа для определенных слов
     * @param [in] docId - id документа
     * @param [in] uniqueWords - вектор слов запроса
     * @return абсолютная актуальность
     */
    size_t getAbsoluteRelevanceForDocument (size_t docId, set<string>& uniqueWords);
};