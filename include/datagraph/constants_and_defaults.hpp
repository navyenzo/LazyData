#ifndef INCLUDE_DATA_GRAPH_CONSTANTS_AND_DEFAULTS_HPP_
#define INCLUDE_DATA_GRAPH_CONSTANTS_AND_DEFAULTS_HPP_



//-------------------------------------------------------------------
#include <string>
#include <vector>
#include <functional>
#include <complex>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Simple incremental ID to assign a unique ID to each component of
// the node graph
//-------------------------------------------------------------------
class IncrementalID
{
public:

    IncrementalID()
    {
    }

    static void reset_current_id(int64_t current_id_value)
    {
        incremental_id = current_id_value;
    }

    static int64_t get_id()
    {
        return ++incremental_id;
    }

    static int64_t peek_id()
    {
        return incremental_id;
    }



private:

    inline static int64_t incremental_id = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_DATA_GRAPH_CONSTANTS_AND_DEFAULTS_HPP_