#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

struct Entry 
{
    size_t doc_id, count;

    // Required for tests:
    bool operator ==(const Entry& other) const 
    {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex 
{
    public:

    InvertedIndex() = default;

/** Выполните индексацию отдельного файла
    Обновите или заполните базу документов, в которой будет выполняться поиск
    @param [in] texts_input - содержимое документов
 */

void updateDocumentBase(const vector<string>& input_docs);

/** Получить количество записей "word" в базе готовых документов
    // @param [in] word - слово, для которого должна быть определена частота ввода.
   // @возвращаемый вектор слов с рассчитанной частотой ввода
*/

vector<Entry> getWordCount(const string& word);

/**Получить количество слов в определенном документе
   @param [in] word - слово для подсчета в документе
   @param [in] doc_id - идентификатор документа для поиска
    @return count слов в определенном документе
 */
size_t getWordCountInDoc(const string& word, const size_t doc_id) const; // ADDITIONAL METHOD

private:
void indexTheFile(const string& fileContent, size_t docId);
/** Выполните индексацию отдельного файла
 @param [in] fileContent - std::строка с содержимым файла
 @param [in] docId - идентификатор файла
 */

map<int, string> document_list; // карта документов (уникальный идентификатор и имя файла) для поиска в
static mutex mutexIndexMap; // доступ к индексной карте
map<string, vector<Entry>> frequencyDictionary; // fчастотный словарь для всех файлов
bool indexingIsOngoing;
};