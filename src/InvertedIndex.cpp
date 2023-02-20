#include "InvertedIndex.h"

const int HEADER_SPACER = 15;

//static members initialization -----------------------------
mutex InvertedIndex::mutexIndexMap;
//-------------------------------------------------------------

void InvertedIndex::updateDocumentBase(const vector<string>& input_docs)
{
    if (input_docs.empty())
    {
        cerr << "\t- Indexing: no content in docs content base\n";
        return;
    }

    indexingIsOngoing = true;

    frequencyDictionary.clear();
    size_t docId = 0;
    //input_docs - vector <string> с всеми словами их файлов, рассортированных по файлам
    for (const auto& content : input_docs)
    {
        // Start indexing thread.
        thread index([this, &content, docId](){indexTheFile(content, docId);});
        ++docId;
        index.join();
    }
    indexingIsOngoing = false;
}

vector<Entry> InvertedIndex::getWordCount(const string& word)
{
    if (indexingIsOngoing)
    {
        cout << "Index is ongoing, please repeat the request later.\n";
        return {};
    }
    auto it = frequencyDictionary.find(word);
    if (it != frequencyDictionary.end())
    {
        return it->second;
    }
    else
    {
        return {};
    }
}

void InvertedIndex::indexTheFile(const string& fileContent, size_t docId)
{
    // Разделите документ на слова.
    map<string, Entry> fileFreqDictionary;
    Entry entry{};
    entry.doc_id = docId;
    entry.count = 1;
    istringstream ist(fileContent);
    for (string word; ist >> word; )
    {
        //Преобразование символов в нижний регистр:
        //transform связывает указанную функцию с одним диапазоном и сохраняет вывод в другом диапазоне
        //tolower - Преобразует параметр в его эквивалент в нижнем регистре
        transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c){ return tolower(c); });

        pair<string, Entry> file_word_frequency {word, entry};
        if (!fileFreqDictionary.emplace(file_word_frequency).second)
        {
            fileFreqDictionary.find(word)->second.count += 1;
        }
    }
    // Place the word in map.
    mutexIndexMap.lock();
    for (const auto& wordIterator : fileFreqDictionary)
    {
        pair<string, vector<Entry>> wordFrequency;
        wordFrequency.first = wordIterator.first;
        vector<Entry> entryVector {wordIterator.second};
        wordFrequency.second = entryVector;
        if (!frequencyDictionary.emplace(wordFrequency).second)
        {
            frequencyDictionary.find(wordFrequency.first)->second.push_back(wordIterator.second);
        }
    }
    mutexIndexMap.unlock();
}

size_t InvertedIndex::getWordCountInDoc(const string& word, const size_t doc_id) const
{
    if (indexingIsOngoing) {
        cout << "Index is ongoing, please repeat the request later.\n";
        return 0;
    }
    size_t wordCountInDoc = 0;
    auto it = frequencyDictionary.find(word);
    if (it != frequencyDictionary.end()) {
        auto entryVector = it->second;
        for (auto entry : entryVector) {
            if (entry.doc_id == doc_id) wordCountInDoc += entry.count;
        }
    } else {
        cout << "Word \"" << word << "\" not found.\n";
    }
    return wordCountInDoc;
}