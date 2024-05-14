#pragma once 

#include <vector>
#include <string>
#include <deque>

#include "search_server.h"
#include "document.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        finded_result = search_server_.FindTopDocuments(raw_query, document_predicate);
        AddRequest(finded_result.size());
        return finded_result;
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        // определите, что должно быть в структуре
        int timestamp;
        int results;        
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    int current_min_ = 0;
    const SearchServer& search_server_;
    int no_result_requests_ = 0;
    std::vector<Document> finded_result;

    void AddRequest(int result_num);
}; 