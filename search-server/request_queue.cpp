#include <vector>
#include <string>

#include "request_queue.h"
#include "search_server.h"
#include "document.h"

using namespace std;

RequestQueue::RequestQueue(const SearchServer& search_server) 
    : search_server_(search_server)
{
}
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    
vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
    finded_result = search_server_.FindTopDocuments(raw_query, status);
    AddRequest(finded_result.size());
    return finded_result;
}
vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
    finded_result = search_server_.FindTopDocuments(raw_query);
    AddRequest(finded_result.size());
    return finded_result;
}
int RequestQueue::GetNoResultRequests() const {
    return no_result_requests_;
}

void RequestQueue::AddRequest(int result_num) {
    ++current_min_;
    while(!requests_.empty() && min_in_day_ <= current_min_ - requests_.front().timestamp) {
        if (requests_.front().results == 0) {
            --no_result_requests_;
        }
        requests_.pop_front();
    }
    requests_.push_back({current_min_, result_num});
    if (result_num == 0) {
        ++no_result_requests_;
    }
}
   

