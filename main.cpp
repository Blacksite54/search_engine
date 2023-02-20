#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

void writeAnswers(const vector<vector<RelativeIndex>>& allRequestsResults)
{
    if (allRequestsResults.empty())
    {
        cout << "No matches are found.\n";
        return;
    }
    vector<vector<pair<int, float>>> allRequestsResultsReadyForJSON;
    for (auto& requestResult : allRequestsResults)
    {
        vector<pair<int, float>> requestResultReadyForJSON;
        for (auto& pageRelevance : requestResult)
        {
            pair<int, float> relevancePair;
            relevancePair.first = (int) pageRelevance.doc_id;
            relevancePair.second = pageRelevance.rank;
            requestResultReadyForJSON.push_back(relevancePair);
        }
        allRequestsResultsReadyForJSON.push_back(requestResultReadyForJSON);
    }
    ConverterJSON::getInstance()->putAnswers(allRequestsResultsReadyForJSON);
}

int main()
{
    //Initialization:
    ConverterJSON::getInstance()->readConfigFile();
    ConverterJSON::getInstance()->readRequestFile();
    vector<string> documents = ConverterJSON::getInstance()->getTextDocuments();
    auto* invertedIndex = new InvertedIndex();
    invertedIndex->updateDocumentBase(documents);

    //Search:
    cout << "Searching...\n";
    SearchServer searchServer(*invertedIndex);
    searchServer.setMaxResponses(ConverterJSON::getInstance()->getMaxResponses());
    auto allRequestsResults = searchServer.search(ConverterJSON::getInstance()->getRequests());
    writeAnswers(allRequestsResults);
    cout << "End of search.\n";
    //Pause the console - uncomment if pause is necessary:
    cin.get();
    return 0;
}