// Datastructures.hh
//
// Student name: Natasha Dmello

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <set>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate:  Size typically maintains a count of elements, and getting that count is a quick operation.
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Deletion of affiliations and publications in loops is linear, so O(m+n) = O(n)
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Copying the vector takes time proportional to the number of elements in the vector
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Map collisions or vector resizes can lead to O(n) complexity.
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Potential map collisions, where all keys hash to the same index, cause a linear search.
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate:  Potential hash collisions in the unordered map lookup.
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Due to the use of std::sort to alphabetically sort the affiliations based on their names.
    std::vector<AffiliationID> get_affiliations_alphabetically();


    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Due to sorting affiliations based on coordinates using std::sort.
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate:  It performs a linear search in affiliations_map based on coordinates.
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate:  Linear searches for updating the coordinate based on the affiliation ID.
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate:  Have to traverse all ‘n’ elements in the data structure.
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Returning a copy of the vector is O(n).
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(1)
    // Short rationale for estimate: 'find' operation in an unordered map is constant.
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Involves a constant-time lookup to retrieve the year of the publication.
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: There is a copy of a vector when returning vector
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Potential hash collisions
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: n is the number of elements in the references vector.
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Potential hash collisions
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Potential hash collisions
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Potential hash collisions
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Due to sorting operation of qualifying publications based on year and ID.
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: n is the length of chain of references
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Due to potential repeated processing of publications in a chain of references
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Due to sorting operation on std::sort on distance vector for n number of affiliations
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Removing aff. from associated pub. requires scanning all aff. of each pub.
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: It iterates through reversed reference chains of pub ids.
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Removing publications from associated aff. requires scanning all pub. of each aff.
    bool remove_publication(PublicationID publicationid);








private:

    //A struct that is used for storing the necessary affiliation informations
    struct Affiliation{
        AffiliationID id;
        Name name;
        Coord location;
        std::vector<PublicationID> associated_publications;
    };

    // Struct for storing publication information
    struct Publication{
        PublicationID id;
        Name name;
        Year year;
        std::vector<AffiliationID> associated_affiliations;
        std::vector<PublicationID> references;
        Publication* referenced_by = nullptr;
    };

    //Unordered map to store affiliation and publication information with its ID
    std::unordered_map<AffiliationID, Affiliation*> affiliations_map_ = {};
    std::unordered_map<PublicationID, Publication*> publications_map_ = {};

    //Vector to store IDs of affiliation and publication
    std::vector<AffiliationID> idsofAffiliations;
    std::vector<PublicationID> idsofPublications;

    //Vector to store Affiliation Name and ID as a pair
    std::vector<std::pair<Name, AffiliationID>> getAffiliationNamesAndID;

    // Vector to store Affiliation location and ID as a pair
    std::vector<std::pair<Coord, AffiliationID>> getAffiliationCoordAndID;



};

#endif // DATASTRUCTURES_HH
