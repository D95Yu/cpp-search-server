#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "document.h"

std::ostream& operator<<(std::ostream& output, const Document& document) {
    output << "{ " 
         << "document_id = " << document.id << ", "
         << "relevance = " << document.relevance << ", "
         << "rating = " << document.rating
         << " }";
    return output;   
}

template <typename Iterator>
class IteratorRange {
    public: 
    
    IteratorRange() {}

    IteratorRange(Iterator it_begin, Iterator it_end, size_t size) 
        : page_begin_(it_begin), page_end_(it_end), page_size_(size)
    {}

        Iterator begin() const {
            return page_begin_;
        }
        Iterator end() const {
            return page_end_;
        }
        size_t size() const {
            return page_size_;
        }

    private: 
        Iterator page_begin_;
        Iterator page_end_;
        size_t page_size_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range) {
    for (auto i = range.begin(); i != range.end(); ++i) {
        out << *i;
    }
    return out;
}

template <typename Iterator>
class Paginator {
    public: 
        Paginator() {}

        Paginator(Iterator it_begin, Iterator it_end, size_t page_size) {
            size_t cont_size = distance(it_begin, it_end);
            
            while (cont_size >= page_size) {
                Iterator page_begin = it_begin;
                Iterator page_end = page_begin + page_size;
                iterator_pages_.push_back({page_begin, page_end, page_size});
                advance(it_begin, page_size);
                cont_size = distance(it_begin, it_end);
            }
            if (distance(it_begin, it_end) > 0) {
                size_t dist = distance(it_begin, it_end);
                iterator_pages_.push_back({it_begin, it_end, dist});
            }
        }   

        auto begin() const {
            return iterator_pages_.begin();
        }

        auto end() const {
            return iterator_pages_.end();
        }

        auto size() const {
            return iterator_pages_.size();
        }

    private:
        std::vector<IteratorRange<Iterator>> iterator_pages_;
}; 

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}