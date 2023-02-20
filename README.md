![# Search-in-files](search-in-files.png)<br>
![GitHub author](https://img.shields.io/badge/made_by-Blacksite54-009919)
![GitHub repo size](https://img.shields.io/github/repo-size/Blacksite54/Search_engine)
![GitHub author](https://img.shields.io/badge/made_on-C++-red)
<br><i> Search for words in files</i> | <i> Поиск слов в файлах.</i>

## Description | Описание
![GitHub author](https://img.shields.io/badge/-ENG-blue)<br>
The search engine should be a console application
(an executable file that runs on any server or computer)
that performs a search and has the ability to configure through JSON format
files. The solutions applied in it can later be integrated into a search
engine running on the web.
The principles of the search engine should be as follows:
1. In the configuration file, before launching the application, the names
of the files that the engine will search for are set.
2. The search engine must independently crawl all files and
index them so that you can then find the most
relevant documents for any search query.
3. The user sets a request via the requests.json file. A query is
a set of words for which you need to find documents.
4. The query is transformed into a list of words.
5. The index searches for those documents on which all these words occur.
6. Search results are ranked, sorted and given to the user,
the maximum number of possible documents in the response is set in
the configuration file.
7. At the end, the program generates the answers file.json, which records
the search results.<br><br>
![GitHub author](https://img.shields.io/badge/-RUS-red)<br>
Поисковый движок должен представлять из себя консольное приложение
(исполняемый файл, запускаемый на любом сервере или компьютере),
осуществляющее поиск и имеющее возможность настройки через файлы формата
JSON. Применённые в нём решения можно впоследствии встроить в поисковый
движок работающий на веб.
Принципы работы поискового движка должны быть следующими:
1. В конфигурационном файле перед запуском приложения задаются названия
файлов, по которым движок будет осуществлять поиск.
2. Поисковый движок должен самостоятельно обходить все файлы и
индексировать их так, чтобы потом по любому поисковому запросу находить наиболее
релевантные документы.
3. Пользователь задаёт запрос через JSON-файл requests.json. Запрос — это
набор слов, по которым нужно найти документы.
4. Запрос трансформируется в список слов.
5. В индексе ищутся те документы, на которых встречаются все эти слова.
6. Результаты поиска ранжируются, сортируются и отдаются пользователю,
максимальное количество возможных документов в ответе задаётся в
конфигурационном файле.
7. В конце программа формирует файл answers.json, в который записывает
результаты поиска.
