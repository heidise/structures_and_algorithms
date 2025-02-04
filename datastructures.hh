// Datastructures.hh
//
// Student name: Heidi Seppi
// Student email: heidi.seppi@tuni.fi
// Student number: H252889

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <memory>

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
    // Short rationale for estimate: Map's size() is constant.
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Map's clear() is linear and
    // this is called for two different maps.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Using vector.reserve() (linear) to reserve enough memory
    // so reallocating memory doesn't happen while adding elements to vector in a loop.
    // Looping through all the map's items is linear and push_back is amortized constant.
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Insertion to map is in the worst case logarithmic.
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.find() is logarithmic.
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.find() is logarithmic.
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Vector's reserve() (linear) reserves enough memory for each vector so
    // that reallocating doesn't happen while adding items to a vector in a loop. Loops are linear.
    // Sorting is O(n*log(n)) and that's why perfomance is that.
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Vector's reserve() (linear) reserves enough memory for each vector so
    // that reallocating doesn't happen while adding items to a vector in a loop. Loops are linear and
    // push_back is amortized constant. Sorting is O(n*log(n)) and that's why perfomance is that.
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Using find_if is at worst linear.
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.find() is logarithmic.
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Insertion to map is logarithmic but passing a vector to a struct
    // causes performance to be O(n).
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Using vector.reserve() (linear) to reserve enough memory
    // so reallocating memory doesn't happen while adding elements to vector in a loop.
    // Looping through all the map items is linear and push_back is amortized constant.
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.find() is logarithmic.
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.find() is logarithmic.
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: map.find() is logarithmic but passing a vector of size n
    // causes the performance to be O(n).
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: find() from maps is in the worst case logarithmic.
    // There are no loops when adding elements to end of a vector (which is amortized constant operation).
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() is O(log(n)) and reserving (reserver()) enough memory for a vector is linear operation.
    // For_each is linear as well and adding elements to the end of the vector is amortized constant. So performance is O(n).
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find() from map is O(log(n)). Adding elements to the end
    // of the vector can now cause memory reallocating. No for loops used here.
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: map.find() is logarithmic but passing a vector of size n
    // causes the performance to be O(n).
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: map.find() is logarithmic.
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: For loop is linear and inside it is used map.find() which is O(log(n))
    // so that's why perfomance can be at worst O(n*(log(n)). Sorting is as well O(n*(log(n)).
    // Also, using reserve() to reserve enough memory for a vector.
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: map.find() is logarithmic. This function is using
    // recursive function iterate_parents to check all the parent nodes. So at worst
    // case it can loop through all possible parents (n-1). So at worst this is O(n).
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: O(n*log(n)) because of the std::sort.
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: map.find() is logarithmic. Looping through a vector
    // and using remove_if which is O(n) inside this loop.
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Looping through a vector
    // and using find_if which is O(n) inside this loop.
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: map.find() is logarithmic. Looping through a vector
    // and using remove_if which is O(n) inside this loop.
    bool remove_publication(PublicationID publicationid);


private:

    // Struct for to hold affiliation information.
    struct Affiliation
    {
        Name name;
        Coord coordinates;
        std::vector<PublicationID> publications; // Holding information of all the publications.

        // Constructor.
        Affiliation(Name new_name, Coord new_coordinates) {
            name = new_name;
            coordinates = new_coordinates;
        }
    };

    // Map for affiliations.
    std::map<AffiliationID, Affiliation> affiliationsMap_;

    //std::map<CoordHash, AffiliationID> sortedCoordinatesMap_;
    //std::map<Name, AffiliationID> sortedNameMap_;
    std::vector<AffiliationID> sortedNameVector_; // Works as "temporary memory".
    std::vector<AffiliationID> sortedCoordVector_; // Works as "temporary memory".
    bool changedNames_; // False if new information has been added.
    bool changedCoordinates_; // False if new information has been added.

    // Publications and referencenses in a tree structure.
    struct Node
    {
        PublicationID id;
        Name name;
        Year year;
        std::vector<AffiliationID> affiliations;
        std::vector<std::shared_ptr<Node>> referencing;
        Node* parent;

        // Node constructor.
        Node(PublicationID new_id, Name new_name, Year new_year, std::vector<AffiliationID> new_affilations) {
            id = new_id;
            name = new_name;
            year = new_year;
            affiliations =  new_affilations;
            parent = nullptr; // Value is nullptr until is referenced by.
        }
    };

    // Map for publication information.
    std::map<PublicationID, Node> publicationsMap_;

    // Has been made constant so that these can only be created once.
    std::vector<AffiliationID> const no_affiliations_vector_ = {NO_AFFILIATION};
    std::vector<PublicationID> const no_publications_vector_ = {NO_PUBLICATION};
    std::vector<std::pair<Year, PublicationID>> const no_year_no_pub_vector = {std::pair<Year, PublicationID>(NO_YEAR, NO_PUBLICATION)};

    // Private function to be able to iterate through tree structure parents recursively.
    // Estimate of performance: O(n)
    // Short rationale for estimate: Recursive function. At worst there can be n-1 parents and
    // has to go through all of those.
    std::vector<PublicationID> iterate_parents(std::vector<PublicationID>& vec, Node* parent);

    //std::vector<PublicationID> iterate_references(std::vector<PublicationID>& vec, std::vector<std::shared_ptr<Node>>& referencing);
};

#endif // DATASTRUCTURES_HH
