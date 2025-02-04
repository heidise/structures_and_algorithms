// Datastructures.cc
//
// Student name: Heidi Seppi
// Student email: heidi.seppi@tuni.fi
// Student number: H252889

#include "datastructures.hh"

#include <random>

#include <cmath>
#include <map>
#include <QDebug>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
    changedNames_ = true;
    changedCoordinates_ = true;

}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::get_affiliation_count()
{
    // O(1).
    return affiliationsMap_.size();
}

void Datastructures::clear_all()
{
    affiliationsMap_.clear();
    publicationsMap_.clear();
    changedNames_ = true;
    changedCoordinates_ = true;
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> aff_vector;
    aff_vector.reserve(affiliationsMap_.size()); //.size is constant. Reserve linear.
    auto iter_end = affiliationsMap_.end();
    for (auto iter = affiliationsMap_.begin(); iter != iter_end; iter++) { // O(n)
        aff_vector.push_back(iter->first); // Amortized constant.
    }
    return aff_vector;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{    
    bool succeeded = affiliationsMap_.insert({id, Affiliation(name, xy)}).second; // worst case O(log(n)).
    if (succeeded) {
        changedCoordinates_ = true;
        changedNames_ = true;
    }
    return succeeded;

}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto iter = affiliationsMap_.find(id); // Logarithmic
    if (iter == affiliationsMap_.end()) {
        return NO_NAME;
    }
    else {
        return iter->second.name;
    }
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto iter = affiliationsMap_.find(id);
    if (iter == affiliationsMap_.end()) {
        return NO_COORD;
    }
    else {
        return iter->second.coordinates;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    // If no changes have been made, then can return already once saved sorted vector.
    if (!changedNames_) {
        return sortedNameVector_;
    }

    std::vector<AffiliationID> aff_vector;
    std::vector<std::pair<AffiliationID, Name>> pair_vector;
    int size = affiliationsMap_.size(); // Constant.
    aff_vector.reserve(size); // Linear.
    pair_vector.reserve(size); // Linear. Reserves vector space for affiliations.

    auto iter_end = affiliationsMap_.end();
    for (auto iter = affiliationsMap_.begin(); iter != iter_end; iter++) { // O(n)
        pair_vector.push_back({iter->first, iter->second.name}); // Amortized constant.
    }

    std::sort(pair_vector.begin(), pair_vector.end(), [](auto p1, auto p2) { // O(N*logN)
        return p1.second < p2.second;
    });

    auto pair_end = pair_vector.end();
    for (auto iter = pair_vector.begin(); iter != pair_end; iter++) { // O(n)
        aff_vector.push_back(iter->first); // Amortized constant.
    }
    sortedNameVector_ = aff_vector; // Saving to if needed later.
    changedNames_ = false;
    return aff_vector;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    // If no changes have been made, then can return once already sorted vector.
    if (!changedCoordinates_) {
        return sortedCoordVector_;
    }

    std::vector<AffiliationID> aff_vector;
    std::vector<std::pair<AffiliationID, Coord>> pair_vector;
    auto size = affiliationsMap_.size();
    aff_vector.reserve(size); // O(n)
    pair_vector.reserve(size); // O(n), reserve is linear. Reserves vector space for affiliations.

    auto iter_end = affiliationsMap_.end();
    for (auto iter = affiliationsMap_.begin(); iter != iter_end; iter++) { // O(n)
        pair_vector.push_back({iter->first, iter->second.coordinates}); // Amortized constant.
    }

    std::sort(pair_vector.begin(), pair_vector.end(), [](auto p1, auto p2) { // O(N*logN)
        double distance1 = sqrt(pow(p1.second.x,2) + pow(p1.second.y,2));
        double distance2 = sqrt(pow(p2.second.x,2) + pow(p2.second.y,2));
        if (distance1 < distance2) {
            return true;
        }
        else if (distance1 == distance2 && p1.second.y < p2.second.y){
            return true;
        }
        else {
            return false;
        }
    });

    auto pair_end = pair_vector.end();
    for (auto iter = pair_vector.begin(); iter != pair_end; iter++) { // O(n)
        aff_vector.push_back(iter->first); // Amortized constant.
    }
    sortedCoordVector_ = aff_vector;
    changedCoordinates_ = false;
    return aff_vector;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto iter = std::find_if(affiliationsMap_.begin(), affiliationsMap_.end(), [&xy](auto p) // O(n)
    {return p.second.coordinates == xy;}); // lineaarinen N

    return (iter != affiliationsMap_.end()) ? iter->first : NO_AFFILIATION;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto iter = affiliationsMap_.find(id); // O(log(n))
    if (iter != affiliationsMap_.end()) {
        iter->second.coordinates = newcoord;
        changedCoordinates_ = true;
        return true;
    }
    else {
        return false;
    }
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    return publicationsMap_.insert({id, Node(id, name, year, affiliations)}).second; // Logarithmic but passing a vector so O(n).
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> pub_vector;
    pub_vector.reserve(publicationsMap_.size()); // Linear
    auto iter_end = publicationsMap_.end();
    for (auto iter = publicationsMap_.begin(); iter != iter_end; iter++) { // O(n)
        pub_vector.push_back(iter->first); // Amortized constant.
    }
    return pub_vector;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto iter = publicationsMap_.find(id); // Logarithmic.
    if (iter != publicationsMap_.end()) {
        return iter->second.name;
    }
    else {
        return NO_NAME;
    }
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto iter = publicationsMap_.find(id); // Logarithmic.
    if (iter != publicationsMap_.end()) {
        return iter->second.year;
    }
    else {
        return NO_YEAR;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto iter = publicationsMap_.find(id); // Logarithmic.
    if (iter != publicationsMap_.end()) {
        return iter->second.affiliations; // O(n)
    }
    else {
        return no_affiliations_vector_;
    }
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto iter1 = publicationsMap_.find(id); // logarithmic.
    auto iter2 = publicationsMap_.find(parentid); // logarithmic.
    auto iter_end = publicationsMap_.end();

    if (iter1 != iter_end && iter2 != iter_end) {
        iter2->second.referencing.push_back(std::make_shared<Node>(iter1->second)); // Adding to referencing list.
        iter1->second.parent = &(iter2->second); // Adding parent to the publication which has been referenced by.
        return true;
    }
    else {
        return false;
    }
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto iter = publicationsMap_.find(id); // logarithmic.
    if (iter != publicationsMap_.end()) {
        std::vector<PublicationID> pub_vector;
        pub_vector.reserve(iter->second.referencing.size()); // O(n)
        std::for_each(iter->second.referencing.begin(), iter->second.referencing.end(), [&pub_vector](auto item) {
            pub_vector.push_back((*item).id);
        });
        return pub_vector;
    }
    else {
        return no_publications_vector_;
    }
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto iter_pub = publicationsMap_.find(publicationid);
    auto iter_aff = affiliationsMap_.find(affiliationid);

    if (iter_pub != publicationsMap_.end() && iter_aff != affiliationsMap_.end()) {
        iter_pub->second.affiliations.push_back(affiliationid); // Adding affiliation to publication's list.
        iter_aff->second.publications.push_back(publicationid); // Adding publication to affiliation's list.
        return true;
    }
    else {
        return false;
    }
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    auto iter = affiliationsMap_.find(id);
    if (iter != affiliationsMap_.end()) {
        return iter->second.publications;
    }
    else {
        return no_publications_vector_;
    }
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    auto iter = publicationsMap_.find(id);
    if (iter != publicationsMap_.end()) {
        return (iter->second.parent != nullptr) ? iter->second.parent->id : NO_PUBLICATION; // when no parent.
    }
    else {
        return NO_PUBLICATION;
    }
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    // equal_range possible to use?

    auto iter = affiliationsMap_.find(affiliationid); // O(log(n))
    if (iter != affiliationsMap_.end()) {
        std::vector<std::pair<Year, PublicationID>> vec;
        vec.reserve(iter->second.publications.size());
        // Reserving enough memory even though not everything will be selected to a vector.

        // Looping through publicationID vector.
        auto iter_end = iter->second.publications.end();
        for (auto iter_pub = iter->second.publications.begin(); iter_pub != iter_end; iter_pub++) { // O(n)
            Year pub_year = publicationsMap_.find(*iter_pub)->second.year; // O(log(n))
            if (pub_year >= year) {
                vec.push_back(std::pair<Year, PublicationID>(pub_year,*iter_pub));
            }
        }

        // Sorting the vector lastly.
        std::sort(vec.begin(), vec.end(), [](auto p1, auto p2) { // O(n*log(n))
            if (p1.first < p2.first) {
                return true;
            }
            else if (p1.first == p2.first) {
                return p1.second < p2.second;
            }
            else {
                return false;
            }
        });

        return vec;
    }
    else {
        return no_year_no_pub_vector;
    }
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    auto iter = publicationsMap_.find(id); // O(log(n))
    if (iter == publicationsMap_.end()) {
        return no_publications_vector_;
    }

    auto parent = iter->second.parent;
    if (parent != nullptr) {
        std::vector<PublicationID> pub_vec;
        return iterate_parents(pub_vec, parent); // O(n)
    }
    else {
        // Publication is not cited anywhere.
       return std::vector<PublicationID>();
    }
}

// Not working!
std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    auto iter = publicationsMap_.find(id);
    if (iter != publicationsMap_.end()) {
        if (iter->second.referencing.empty()) {
            return std::vector<PublicationID>();
        }

        std::vector<PublicationID> all_references;
        //return iterate_references(all_references, iter->second.referencing);
        return no_publications_vector_;
    }
    else {
        return no_publications_vector_;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    if (affiliationsMap_.empty()) {
        return std::vector<AffiliationID>();
    }

    std::vector<std::pair<AffiliationID, Coord>> pair_vector;
    auto iter_end = affiliationsMap_.end();
    for (auto iter = affiliationsMap_.begin(); iter != iter_end; iter++) { // O(n)
        pair_vector.push_back({iter->first,iter->second.coordinates}); // Average O(1)
    }

    std::vector<AffiliationID> aff_vector;
    if (pair_vector.size() < 3) {
        for (auto iter2 = pair_vector.begin(); iter2 != pair_vector.end(); iter2++) { // O(n)
            aff_vector.push_back((*iter2).first);
        }
        return aff_vector;
    }
    else {
        std::sort(pair_vector.begin(), pair_vector.end(), [&xy](auto p1, auto p2) { // O(n*log(n))
            double dis1 = sqrt(pow((p1.second.x-xy.x),2)+pow((p1.second.y-xy.y),2));
            double dis2 = sqrt(pow((p2.second.x-xy.x),2)+pow((p2.second.y-xy.y),2));
            if (dis1 == dis2) {
                return p1.second.y < p2.second.y;
            }
            else {
                return dis1 < dis2;
            }
        });
    }

    aff_vector.push_back(pair_vector[0].first);
    aff_vector.push_back(pair_vector[1].first);
    aff_vector.push_back(pair_vector[2].first);
    return aff_vector;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto iter = affiliationsMap_.find(id); // O(log(n)
    if (iter == affiliationsMap_.end()) {
        return false;
    }

    affiliationsMap_.erase(iter); // Amortized constant

    for (auto i = publicationsMap_.begin(); i != publicationsMap_.end(); i++) { // O(n)
        auto vec = i->second.affiliations;
        auto aff_iter = std::remove_if(vec.begin(), vec.end(), [&id](auto item) { // O(n)
            return item == id;
        });
        if (aff_iter != vec.end()) {
            vec.pop_back();
            i->second.affiliations = vec;
        }
    }
    changedNames_ = true;
    changedCoordinates_ = true;
    return true;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    auto iter1 = publicationsMap_.find(id1); // O(log(n))
    auto iter2 = publicationsMap_.find(id2);
    auto iter_end = publicationsMap_.end();
    if (iter1 == iter_end && iter2 == iter_end) {
        return NO_PUBLICATION;
    }

    std::vector<PublicationID> parents1_vec;
    std::vector<PublicationID> parents2_vec;
    parents1_vec = get_referenced_by_chain(id1); // O(n)
    parents2_vec = get_referenced_by_chain(id2);

    if (parents1_vec.empty() || parents2_vec.empty()) {
        return NO_PUBLICATION;
    }

    // Finding common parent.
    for (auto it = parents1_vec.begin(); it != parents1_vec.end(); it++) { // O(n)
        auto find_iter = std::find_if(parents2_vec.begin(), parents2_vec.end(), [&it](auto id) { // O(n)
            return id == *it;
        });

        if (find_iter != parents2_vec.end()) {
            return *find_iter;
        }
    }

    return NO_PUBLICATION;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto iter = publicationsMap_.find(publicationid);
    if (iter == publicationsMap_.end()) {
        return false;
    }

    for (auto it = publicationsMap_.begin(); it != publicationsMap_.end(); it++) {
        if ((it->second.parent != nullptr) && (it->second.parent->id == publicationid)) {
            it->second.parent = nullptr;
        }

        auto vec_ref = it->second.referencing;
        auto pub_iter = std::remove_if(vec_ref.begin(), vec_ref.end(), [&publicationid](auto item) {
            return item->id == publicationid;
        });

        if (pub_iter != vec_ref.end()) {
            vec_ref.pop_back();
            it->second.referencing = vec_ref;
        }
    }

    publicationsMap_.erase(iter);

    for (auto i = affiliationsMap_.begin(); i != affiliationsMap_.end(); i++) {
        auto vec = i->second.publications;
        auto pub_iter = std::remove_if(vec.begin(), vec.end(), [&publicationid](auto item) {
            return item == publicationid;
        });

        if (pub_iter != vec.end()) {
            vec.pop_back();
            i->second.publications = vec;
        }
    }

    changedNames_ = true;
    changedCoordinates_ = true;
    return true;
}

// Private function for iterating through tree structure parents. Recursive function.
// Recursive until no more parents and there can be n-1 parents maximum.
std::vector<PublicationID> Datastructures::iterate_parents(std::vector<PublicationID>& vec, Node* parent) {
    if (parent != nullptr) {
        vec.push_back(parent->id);
        return iterate_parents(vec, parent->parent);
    }
    else {
        return vec;
    }
}

//std::vector<PublicationID> Datastructures::iterate_references(std::vector<PublicationID>& vec, std::vector<Node*>& referencing) {
//    auto iter_end = referencing.end();
//    for (auto iter = referencing.begin(); iter != iter_end; iter++) {
//        vec.push_back((*iter)->id);

//        if (!((*iter)->referencing.empty())) {
//            return iterate_references(vec, (*iter)->referencing);
//        }
//    }
//    return vec;
//}
