#include "SearchServer.h"

set<string> SearchServer::getUniqueWords(const string& request)
{
    set<string> result;
    istringstream ist(request);
    for (string word; ist >> word; )
    {
        //Преобразование символов в нижний регистр:
        transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c){ return tolower(c); });

        result.emplace(word);
    }
    return result;
}

vector<pair<string, size_t>> SearchServer::getWordsEntries(const set<string>& words)
{
    vector<pair<string, size_t>> result;
    for (const auto& word : words)
    {
        auto wordEntries = _index.getWordCount(word);
        size_t wordEntriesSum = 0;
        for (auto wordEntry : wordEntries)
        {
            wordEntriesSum += wordEntry.count;
        }
        pair<string, size_t> wordAndEntry;
        wordAndEntry.first = word;
        wordAndEntry.second = wordEntriesSum;
        result.push_back(wordAndEntry);
    }
    return result;
}

vector<size_t> SearchServer::getAllDocumentsWithWords(const vector<pair<string, size_t>> &words)
{
    vector<size_t> docIds {};
    // Получение записей и идентификаторов документов docIds:
    for (const auto& wordAndEntry : words)
    {
        auto entries = _index.getWordCount(wordAndEntry.first);
        for (auto entry : entries)
        {
            docIds.push_back(entry.doc_id);
        }
    }

    // Получение уникальных идентификаторов из docIds:
    set<size_t> uniqueDocIds (docIds.begin(), docIds.end());
    docIds.clear();
    docIds.assign(uniqueDocIds.begin(), uniqueDocIds.end());
    sort(docIds.begin(), docIds.end(), less<size_t>());
    return docIds;
}

void SearchServer::sortWordsAscendingToEntries(vector<pair<string, size_t>>& wordsEntries)
{
    sort(wordsEntries.begin(), wordsEntries.end(), [](auto &left, auto &right)
    {
        return left.second < right.second;
    });
}

size_t SearchServer::getAbsoluteRelevanceForDocument(size_t docId, set<std::string> &uniqueWords) {
   size_t absoluteRelevance {0};
   for (const auto& word : uniqueWords)
   {
       size_t wordCountInDoc = _index.getWordCountInDoc(word, docId);
       absoluteRelevance += wordCountInDoc;
   }
    return absoluteRelevance;
}

vector<vector<RelativeIndex>> SearchServer::search(const vector<string>& queries_input)
{
    vector<vector<RelativeIndex>> result{};
    if (queries_input.empty())
    {
        cout << "Requests are empty.\n";
        return result;
    }

    for (const auto& query : queries_input)
    {
        // Получение уникальных слов из запроса
        set<string> uniqueWords = getUniqueWords(query);
        if (uniqueWords.empty())
        {
            cout << "\t-bad request.\n";
            continue;
        }

        // Получите количество записей для каждого слова
        auto wordsEntries = getWordsEntries(uniqueWords);

        // Сортировка уникальных слов в соответствии с количеством записей в направлении возрастания
        sortWordsAscendingToEntries(wordsEntries);

        // Получить документ список документов
        auto documentIds = getAllDocumentsWithWords(wordsEntries);
        string docOrDocs = documentIds.size() == 1 ? " document " : " documents ";
        string wordOrWords = uniqueWords.size() == 1 ? " word: " : " words: ";

        // Получите абсолютную релевантность и максимальную релевантность:
        vector<RelativeIndex>* relativeIndexes = new vector<RelativeIndex>();
        size_t maxAbsoluteRelevance {0};
        for (const auto& docId : documentIds)
        {
            size_t absoluteRelevance = getAbsoluteRelevanceForDocument(docId, uniqueWords);
            auto* relativeIndex = new RelativeIndex();
            relativeIndex->doc_id = docId;
            relativeIndex->absoluteIndex = absoluteRelevance;
            relativeIndexes->push_back(*relativeIndex);
            delete relativeIndex;
            if (absoluteRelevance > maxAbsoluteRelevance) maxAbsoluteRelevance = absoluteRelevance;
        }

        // Получите относительную релевантность для каждого документа:
        for (auto& relativeIndex : *relativeIndexes)
        {
            if (maxAbsoluteRelevance != 0)
            {
                float rank = (float) relativeIndex.absoluteIndex / (float) maxAbsoluteRelevance;
                int rounded = (int) std::round(rank * 100);
                rank = (float) rounded / 100;
                relativeIndex.rank = rank;
            }
            else relativeIndex.rank = 0;
        }

        // Отсортируйте документы по релевантности (по убыванию):
        sort(relativeIndexes->begin(), relativeIndexes->end(), [&relativeIndexes](RelativeIndex &left, RelativeIndex &right)
        {
           return (left.rank > right.rank || (left.rank == right.rank && left.doc_id < right.doc_id));
        });

        //Вырежьте результат в соответствии с максимальным количеством ответов из перевернутого индекса:
        if (relativeIndexes->size() > maxResponses)
        {
            relativeIndexes->erase(relativeIndexes->begin() + maxResponses, relativeIndexes->end());
        }

        // Подтолкните этот вектор к результату:
        result.push_back(*relativeIndexes);
        delete relativeIndexes;
    }
    return result;
}

void SearchServer::setMaxResponses(const int &newMaxResponses)
{
    this->maxResponses = newMaxResponses;
}