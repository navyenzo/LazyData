#ifndef INCLUDE_UNIQUE_ID_HPP_
#define INCLUDE_UNIQUE_ID_HPP_



//-------------------------------------------------------------------
#include <string>
#include <functional> // For std::hash
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp 
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class UniqueID
 * @brief Provides functionality for generating unique identifiers.
 *
 * This class offers methods to generate universally unique identifiers (UUIDs)
 * and hash values based on these UUIDs. The generated UUIDs are in string format
 * and are suitable for identifying objects in distributed systems or databases.
 * Additionally, the class provides a method to generate a hash value from the UUID,
 * which can be useful for scenarios requiring integer identifiers.
 */
//-------------------------------------------------------------------
class UniqueID 
{
public:
    /**
     * @brief Generates a universally unique identifier (UUID) as a string.
     * 
     * @return std::string A unique identifier in string format.
     */
    static std::string generate_uuid_string()
    {
        static boost::uuids::random_generator generator;
        boost::uuids::uuid new_uuid = generator();
        return boost::uuids::to_string(new_uuid);
    }

    /**
     * @brief Generates a hash value from a UUID string.
     * 
     * @return uintptr_t A hash value corresponding to the UUID.
     */
    static int generate_uuid_hash()
    {
        std::string uuid_string = generate_uuid_string();
        return std::hash<std::string>{}(uuid_string);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_UNIQUE_ID_HPP_