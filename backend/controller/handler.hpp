#if !defined(HANDLER_HPP)
#define HANDLER_HPP

#include "lib/httplib.h"

void codeSements(const httplib::Request &request, httplib::Response &response) {
    const char PARAM_KEY_SORTBY[] = "sortBy";
    const char PARAM_KEY_TAG[] = "tag";
    bool selectTag = request.has_param(PARAM_KEY_TAG);
    bool doSort = request.has_param(PARAM_KEY_SORTBY);
    if (request.has_param(PARAM_KEY_TAG)) {
        
    } else {
        if (request.has_param(PARAM_KEY_SORTBY)) {
        }
    }
    auto paramSortBy = request.get_param_value("sortBy");
    auto paramTag = request.get_param_value("tag");
}

#endif // HANDLER_HPP
