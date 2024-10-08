// Datastructures.cc
//
// Student name: Natasha Dmello

#include "datastructures.hh"

#include <random>
#include <cmath>


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
    // Initialization

    idsofAffiliations.reserve(100000);
    idsofPublications.reserve(100000);

    getAffiliationNamesAndID.reserve(100000);

}

Datastructures::~Datastructures()
{
        for(auto a : affiliations_map_)
        {
            delete a.second;
        }
        affiliations_map_.clear();

        for(auto a : publications_map_)
        {
            delete a.second;
        }
        publications_map_.clear();

        for (auto connection : connections)
        {
                delete connection;
        }
        connections.clear();
}

unsigned int Datastructures::get_affiliation_count()
{
    // Returns the count of added affiliations.
        return affiliations_map_.size();
}

void Datastructures::clear_all()
{
    // Clears affiliation and publication data structures.

    for(auto a : affiliations_map_)
    {
        delete a.second;
    }
    for(auto a : publications_map_)
    {
        delete a.second;
    }

    // Clear connections and associated memory
    for (auto connection : connections)
    {
        delete connection;
    }
    connections.clear();

    affiliations_map_.clear();
    publications_map_.clear();
    idsofAffiliations.clear();
    idsofPublications.clear();
    getAffiliationNamesAndID.clear();
    getAffiliationCoordAndID.clear();
   // connections.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    // Returns a vector containing known affiliation IDs.
    if(!idsofAffiliations.empty())
    {
        return idsofAffiliations;
    }
    return {};
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    // Returns true if the entry is new and added to the data structures;
    // false if the affiliation ID already exists.

    if(affiliations_map_.find(id) == affiliations_map_.end())
    {
        Affiliation* new_affiliation = new Affiliation{id, name, xy, {}};
        new_affiliation->associated_publications.reserve(100000);
        affiliations_map_.insert({id, new_affiliation});
        idsofAffiliations.push_back(id);
        getAffiliationNamesAndID.push_back({name, id});
        getAffiliationCoordAndID.push_back({xy, id});
        return true;
    }
    return false;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    // Retrieves the name associated with the affiliation ID if it exists;
    // else, returns NO_NAME.

    auto iter = affiliations_map_.find(id);
    if(iter != affiliations_map_.end())
    {
        return iter->second->name;
    }
    return NO_NAME;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    // Retrieves the coord associated with the affiliation ID if it exists,
    // else, returns NO_COORD.

    auto iter = affiliations_map_.find(id);
    if(iter != affiliations_map_.end())
    {
        return iter->second->location;
    }
    return NO_COORD;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    // Generates a vector containing affiliation IDs sorted in
    // ascending order based on their names.

    if (!idsofAffiliations.empty())
    {
        std::vector<AffiliationID> IdsSortedByName;
        IdsSortedByName.reserve(getAffiliationNamesAndID.size());

        // Check if the data is already sorted
        bool sorted = true;
        for (size_t i = 1; i < getAffiliationNamesAndID.size(); ++i)
        {
            if (getAffiliationNamesAndID[i - 1].first > getAffiliationNamesAndID[i].first)
            {
                sorted = false;
                break;
            }
        }

        // Sort only if the data is not already sorted
        if (!sorted)
        {
            std::sort(getAffiliationNamesAndID.begin(), getAffiliationNamesAndID.end(),
                      [](const auto& entry1, const auto& entry2)
                      {
                          return entry1.first < entry2.first;
                      });
        }

        // Copy the sorted IDs to the result vector
        for (const auto& pair : getAffiliationNamesAndID)
        {
            IdsSortedByName.push_back(pair.second);
        }

        return IdsSortedByName;
    }
    return {};
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    // Generates a vector of affiliation IDs sorted in
    // ascending order based on coordinates.

    if (!idsofAffiliations.empty())
    {
        std::vector<AffiliationID> IdsSortedByCoord;
        IdsSortedByCoord.reserve(getAffiliationCoordAndID.size());

        // Check if the data is already sorted
        bool sorted = true;
        for (size_t i = 1; i < getAffiliationCoordAndID.size(); ++i)
        {
            Coord coord1 = getAffiliationCoordAndID[i - 1].first;
            Coord coord2 = getAffiliationCoordAndID[i].first;
            int distance_coord1 = sqrt(coord1.x * coord1.x + coord1.y * coord1.y);
            int distance_coord2 = sqrt(coord2.x * coord2.x + coord2.y * coord2.y);

            if (distance_coord1 > distance_coord2 || (distance_coord1 == distance_coord2 && coord1.y > coord2.y))
            {
                sorted = false;
                break;
            }
        }

        // Sort only if the data is not already sorted
        if (!sorted)
        {
            std::sort(getAffiliationCoordAndID.begin(), getAffiliationCoordAndID.end(), [](auto entry1, auto entry2)
            {
                Coord coord1 = entry1.first;
                Coord coord2 = entry2.first;
                int distance_coord1 = sqrt(coord1.x * coord1.x + coord1.y * coord1.y);
                int distance_coord2 = sqrt(coord2.x * coord2.x + coord2.y * coord2.y);
                if (distance_coord1 < distance_coord2)
                {
                    return true;
                }
                else if (distance_coord1 > distance_coord2)
                {
                    return false;
                }
                else
                {
                    return coord1.y < coord2.y;
                }
            });
        }

        // Copy the sorted IDs to the result vector
        for (auto record : getAffiliationCoordAndID)
        {
            IdsSortedByCoord.push_back(record.second);
        }

        return IdsSortedByCoord;
    }
    return {};
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    // Finds an affiliation with the given coordinate and returns its ID;
    // returns NO_AFFILIATION if none exists.

    auto it = std::find_if(affiliations_map_.begin(), affiliations_map_.end(),
                           [xy](const auto& record)
                           {
                               return record.second->location == xy;
                           });

    if (it != affiliations_map_.end())
    {
        return it->first;
    }

    return NO_AFFILIATION;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    // Modifies the location of the affiliation with the provided ID and returns true.
    // Returns false if no affiliation with the given ID is found.

    auto iter = affiliations_map_.find(id);
    if(iter != affiliations_map_.end())
    {
        for(unsigned int i = 0; i < getAffiliationCoordAndID.size(); ++i)
        {
            if(getAffiliationCoordAndID[i].first == iter->second->location)
            {
                getAffiliationCoordAndID[i].first = newcoord;
                break;
            }
        }
        iter->second->location = newcoord;
        return true;
    }
    return false;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    if (publications_map_.find(id) != publications_map_.end()) {
        return false; // Publication ID already exists
    }

    // Create a new publication
    Publication* newpublication = new Publication{id, name, year, affiliations, {}};
    newpublication->references.reserve(100000);
    publications_map_.insert({id, newpublication});
    idsofPublications.push_back(id);


    // Add connections between affiliations in the same publication
    for (size_t i = 0; i < affiliations.size(); ++i) {
        for (size_t j = i + 1; j < affiliations.size(); ++j) {
            // Check if the connection already exists
            auto iter = std::find_if(connections.begin(), connections.end(),
             [aff1 = affiliations[i], aff2 = affiliations[j]](const Connection* conn) {
                                return (conn->aff1 == aff1 && conn->aff2 == aff2) || (conn->aff1 == aff2 && conn->aff2 == aff1);
                            });

            if (iter != connections.end()) {

                // Increment the weight if the connection exists
                (*iter)->weight += 1;

            } else {

                Connection* conn = new Connection{affiliations[i], affiliations[j], 1};

                connections.insert(conn);
            }
        }
    }

    // Update associated publications for each affiliation
    for (auto aff : affiliations) {
        auto iter = affiliations_map_.find(aff);
        if (iter != affiliations_map_.end()) {
            iter->second->associated_publications.push_back(id);
        }
    }

    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    // Returns a vector of known publication IDs.
    if(!idsofPublications.empty())
    {
        return idsofPublications;
    }
    return {};
}

Name Datastructures::get_publication_name(PublicationID id)
{
    // Retrieves the name of the publication with the provided ID;
    // returns NO_NAME if the publication does not exist.

    auto iter = publications_map_.find(id);
    if(iter != publications_map_.end())
    {
        return iter->second->name;
    }
    return NO_NAME;
}

Year Datastructures::get_publication_year(PublicationID id)
{
    // Returns the year of the publication with the given ID, or NO_YEAR if
    // such publication does not exist.

    auto iter = publications_map_.find(id);
    if(iter != publications_map_.end())
    {
        return iter->second->year;
    }
    return NO_YEAR;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    // Returns a vector of affiliation IDs associated with
    // the given publication ID in arbitrary order,
    // or a vector with a single item NO_AFFILIATION if the publication does not exist.

    auto iter = publications_map_.find(id);
    if(iter != publications_map_.end())
    {
        return iter->second->associated_affiliations;
    }

    return {NO_AFFILIATION};
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    // Establishes a reference from the second publication to the first.
    // Returns true if publications with the given IDs exist; otherwise, returns false.

    auto iter1 = publications_map_.find(id);
    auto iter2 = publications_map_.find(parentid);
    if(iter1 != publications_map_.end() and iter2 != publications_map_.end())
    {
        iter1->second->referenced_by = iter2->second;
        iter2->second->references.push_back(iter1->second->id);
        return true;
    }
    return false;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    // Returns a vector of publication IDs directly referenced by the publication
    // with the given ID. If the publication with the given ID doesn't exist,
    // returns a vector with a single item NO_PUBLICATION.

    auto iter = publications_map_.find(id);
    if(iter != publications_map_.end())
    {
        return iter->second->references;
    }
    return {NO_PUBLICATION};
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto iter_pub = publications_map_.find(publicationid);
    auto iter_aff = affiliations_map_.find(affiliationid);

    if (iter_pub != publications_map_.end() && iter_aff != affiliations_map_.end())
    {
        iter_pub->second->associated_affiliations.push_back(affiliationid);
        iter_aff->second->associated_publications.push_back(publicationid);

        // Add connections only for the newly added affiliation
        for (size_t i = 0; i < iter_pub->second->associated_affiliations.size() - 1; ++i)
        {
            // Create a temporary Connection object for find
            Connection tempConnection{iter_pub->second->associated_affiliations[i],
                                      affiliationid, 1};

            // Check if the connection already exists
            auto iter_conn = std::find_if(connections.begin(), connections.end(),
                [aff1 = tempConnection.aff1, aff2 = tempConnection.aff2](const Connection* conn) {
                    return (conn->aff1 == aff1 && conn->aff2 == aff2) ||
                           (conn->aff1 == aff2 && conn->aff2 == aff1);
                });

            if (iter_conn != connections.end())
            {
                // Increment the weight if the connection exists
                (*iter_conn)->weight += 1;
            }
            else
            {
                // Otherwise, add a new connection with a weight of 1
                Connection* conn = new Connection{tempConnection.aff1,
                                                  tempConnection.aff2, 1};
                connections.insert(conn);
            }
        }
        return true;
    }
    return false;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    // Returns vector of publication IDs produced by the affiliation with the given ID.
    // If no such affiliation exists, returns vector with a single element NO_PUBLICATION.

    auto iter = affiliations_map_.find(id);
    if(iter != affiliations_map_.end())
    {
        return iter->second->associated_publications;
    }
    return {NO_PUBLICATION};
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    // Returns IDs of directly referencing publications.
    // If there are none, returns NO_PUBLICATION.


    auto iter = publications_map_.find(id);
    if(iter != publications_map_.end())
    {
        if(iter->second->referenced_by != nullptr)
        {
            return iter->second->referenced_by->id;
        }
    }
    return NO_PUBLICATION;
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    // Returns publications released by the provided affiliation from the specified year onwards.
    // Publications are sorted in ascending order by year and then by ID in case of ties.
    // If the affiliation doesn't exist, returns {NO_YEAR, NO_PUBLICATION}.

    std::vector<std::pair<Year, PublicationID>> publications_after;
    auto iter1 = affiliations_map_.find(affiliationid);
    if(iter1 != affiliations_map_.end())
    {
        if(!iter1->second->associated_publications.empty())
        {
            for(auto publicationid : iter1->second->associated_publications)
            {
                auto iter2 = publications_map_.find(publicationid);
                if(iter2->second->year >= year)
                {
                    publications_after.push_back({iter2->second->year, publicationid});
                }
            }
            std::sort(publications_after.begin(), publications_after.end(), [](auto entry1, auto entry2)
            {
                if (entry1.first != entry2.first)
                {
                    return entry1.first < entry2.first;
                }
                return entry1.second < entry2.second;
            });
            return publications_after;
        }
    }
    return {{NO_YEAR, NO_PUBLICATION}};
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    // Returns a vector of publication IDs referencing the specified publication,
    // whether through direct or indirect references. If the publication with the
    // given ID doesn't exist, returns a vector with a single item NO_PUBLICATION.


    std::vector<PublicationID> chainOfReferences;
    auto iter = publications_map_.find(id);
    bool referenced = true;
    if(iter != publications_map_.end())
    {
        while(referenced)
        {
            if(iter->second->referenced_by != nullptr)
            {
                chainOfReferences.push_back(iter->second->referenced_by->id);
                iter = publications_map_.find(iter->second->referenced_by->id);
            }
            else
            {
                referenced = false;
            }
        }
        return chainOfReferences;
    }
    return {NO_PUBLICATION};
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id) {
    // Check if publication with the given ID exists
        if (publications_map_.find(id) == publications_map_.end()) {
            return {NO_PUBLICATION};
        }

        // Get the current publication
        Publication* currentPublication = publications_map_[id];

        // Store the result of indirect references
        std::vector<PublicationID> result;

       // Add direct references
       result.insert(result.end(), currentPublication->references.begin(), currentPublication->references.end());

       // Recursively add indirect references
       for (PublicationID referenceID : currentPublication->references) {
           std::vector<PublicationID> indirectReferences = get_all_references(referenceID);
           result.insert(result.end(), indirectReferences.begin(), indirectReferences.end());
       }

       return result;
   }

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy) {

   // Create vector to store the closest affiliations
       std::vector<AffiliationID> result;

       // Calculate squared distance for each affiliation and store in a vector
       std::vector<std::pair<int, AffiliationID>> distances;
       for (const auto& entry : getAffiliationCoordAndID)
       {
           Coord affiliationCoord = entry.first;
           int distance_squared = (xy.x - affiliationCoord.x) * (xy.x - affiliationCoord.x) +
                                  (xy.y - affiliationCoord.y) * (xy.y - affiliationCoord.y);
           distances.emplace_back(distance_squared, entry.second);
       }

       // Sort the distances vector based on squared distance
       std::sort(distances.begin(), distances.end());

       // Take the first three affiliations (if available)
       for (size_t i = 0; i < std::min(distances.size(), static_cast<size_t>(3)); ++i)
       {
           result.push_back(distances[i].second);
       }

       return result;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto affiliationIt = affiliations_map_.find(id);
    if (affiliationIt != affiliations_map_.end()) {
        // Remove the affiliation from associated publications
        for (PublicationID pubID : affiliationIt->second->associated_publications) {
            auto publicationIt = publications_map_.find(pubID);
            if (publicationIt != publications_map_.end()) {
                auto& affVec = publicationIt->second->associated_affiliations;
                affVec.erase(std::remove(affVec.begin(), affVec.end(), id), affVec.end());
            }
        }

        // Clear associated vectors
        affiliationIt->second->associated_publications.clear();

        // Erase the affiliation from various vectors
        idsofAffiliations.erase(std::remove(idsofAffiliations.begin(), idsofAffiliations.end(), id), idsofAffiliations.end());
        getAffiliationNamesAndID.erase(std::remove_if(getAffiliationNamesAndID.begin(), getAffiliationNamesAndID.end(),
                                                      [id](const auto& entry) { return entry.second == id; }),
                                       getAffiliationNamesAndID.end());
        getAffiliationCoordAndID.erase(std::remove_if(getAffiliationCoordAndID.begin(), getAffiliationCoordAndID.end(),
                                                      [id](const auto& entry) { return entry.second == id; }),
                                       getAffiliationCoordAndID.end());

        // Delete the affiliation object and remove from map
        delete affiliationIt->second;
        affiliations_map_.erase(affiliationIt);

        return true;
    }

    return false; // Affiliation not found
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    // Get chains of references for both IDs
    std::vector<PublicationID> chain1 = get_referenced_by_chain(id1);
    std::vector<PublicationID> chain2 = get_referenced_by_chain(id2);

    if (chain1.empty() || chain2.empty())
           return NO_PUBLICATION;

       // Reverse the chains to start from root
       std::reverse(chain1.begin(), chain1.end());
       std::reverse(chain2.begin(), chain2.end());

       size_t i = 0;
       while (i < chain1.size() && i < chain2.size() && chain1[i] == chain2[i])
       {
           ++i;
       }

       if (i == 0)
           return NO_PUBLICATION;
       else
           return chain1[i - 1];
   }

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto publicationIt = publications_map_.find(publicationid);

    // Check if publication with given ID exists
    if (publicationIt != publications_map_.end()) {
        // Remove publication from associated affiliations
        for (AffiliationID affiliation_id : publicationIt->second->associated_affiliations) {
            auto affiliationIt = affiliations_map_.find(affiliation_id);
            if (affiliationIt != affiliations_map_.end()) {
                auto& publications = affiliationIt->second->associated_publications;
                publications.erase(std::remove(publications.begin(), publications.end(), publicationid), publications.end());
            }
        }

        // Nullify referenced_by pointer in any publication that references this one
        for (auto& pair : publications_map_) {
            if (pair.second->referenced_by == publicationIt->second) {
                pair.second->referenced_by = nullptr;
            }
        }
        // Clear associated vectors
        publicationIt->second->associated_affiliations.clear();


        // Erase publication from various vectors
        idsofPublications.erase(std::remove(idsofPublications.begin(), idsofPublications.end(), publicationid), idsofPublications.end());

        // Delete publication object and remove from the map
        delete publicationIt->second;
        publications_map_.erase(publicationIt);

        return true;
    }


    return false; // Publication with given ID doesn't exist
}

std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id) {
    std::vector<Connection> result;
    result.reserve(connections.size());  // Reserve space based on the total number of connections

    for (const auto& conn : connections) {
        if (conn->aff1 == id || conn->aff2 == id) {
            // Only add the connection if it involves the specified affiliation
            result.push_back((conn->aff1 == id) ? *conn : Connection{conn->aff2, conn->aff1, conn->weight});
        }
    }

    return result;
}


std::vector<Connection> Datastructures::get_all_connections() {
    // Create a vector to store unique connections
    std::vector<Connection> uniqueConnections;

    // Copy unique connections to the vector
    for (auto const& connectionPtr : connections) {
        Connection connection = *connectionPtr;
        if (connection.aff1 > connection.aff2) {
            std::swap(connection.aff1, connection.aff2);
        }
        if (std::find(uniqueConnections.begin(), uniqueConnections.end(), connection) == uniqueConnections.end()) {
            uniqueConnections.push_back(connection);
        }
    }

    return uniqueConnections;
}

Path Datastructures::get_any_path(AffiliationID source, AffiliationID target) {
    std::unordered_set<AffiliationID> visited;
    Path path;

    if (affiliations_map_.find(source) == affiliations_map_.end() ||
        affiliations_map_.find(target) == affiliations_map_.end()) {
        // Invalid source or target affiliation
        return path;
    }

    // Helper function for DFS
    std::function<bool(AffiliationID)> dfs = [&](AffiliationID current) -> bool {
        visited.insert(current);

        if (current == target) {
            // Found a path to the target
            return true;
        }

        std::vector<Connection> connectedAffiliations = get_connected_affiliations(current);

        for (const Connection& conn : connectedAffiliations) {
            if (visited.find(conn.aff2) == visited.end()) {
                path.push_back(conn);

                if (dfs(conn.aff2)) {
                    return true;
                }

                path.pop_back();  // Backtrack if the current path doesn't lead to the target
            }
        }

        return false;
    };

    // Start DFS from the source
    dfs(source);

    return path;
}

Path Datastructures::get_path_with_least_affiliations(AffiliationID source, AffiliationID target)
{
    using PQElement = std::pair<int, AffiliationID>;  // {distance, affiliation}
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
    std::unordered_map<AffiliationID, int> distances;
    std::unordered_set<AffiliationID> visited;
    std::unordered_map<AffiliationID, Connection> previous;

    pq.push({0, source});
    distances[source] = 0;

    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();

        if (current == target) {
            // Reconstruct the path
            Path path;
            while (current != source) {
                path.insert(path.begin(), previous[current]);  // Use insert at the beginning
                current = previous[current].aff1;
            }
            return path;
        }

        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);

        for (const auto& conn : get_connected_affiliations(current)) {
            AffiliationID neighbor = (conn.aff1 == current) ? conn.aff2 : conn.aff1;
            int new_dist = dist + 1;  // Assuming equal weight for all connections

            if (distances.find(neighbor) == distances.end() || new_dist < distances[neighbor]) {
                distances[neighbor] = new_dist;
                previous[neighbor] = conn;
                pq.push({new_dist, neighbor});
            }
        }
    }

    // No path found
    return Path();
}

Path Datastructures::get_path_of_least_friction(AffiliationID source, AffiliationID target) {
    if (affiliations_map_.find(source) == affiliations_map_.end() ||
        affiliations_map_.find(target) == affiliations_map_.end()) {
        // Invalid source or target affiliation
        return {};
    }

    Path path;
    double maxMinWeight = -std::numeric_limits<double>::infinity();
    std::vector<Connection>::size_type minConnections = std::numeric_limits<std::vector<Connection>::size_type>::max();

    std::unordered_set<AffiliationID> visited;
    std::queue<std::tuple<AffiliationID, Path, double, std::vector<Connection>::size_type>> bfsQueue;

    bfsQueue.push({source, {}, -std::numeric_limits<double>::infinity(), 0});
    visited.insert(source);

    while (!bfsQueue.empty()) {
        auto [current, currentPath, currentWeight, currentConnections] = bfsQueue.front();
        bfsQueue.pop();

        std::vector<Connection> connectedAffiliations = get_connected_affiliations(current);

        for (const Connection& conn : connectedAffiliations) {
            if (visited.find(conn.aff2) == visited.end()) {
                double newWeight = (current == source) ? static_cast<double>(conn.weight) : std::min(currentWeight, static_cast<double>(conn.weight));
                Path newPath = currentPath;
                newPath.push_back(conn);

                if (conn.aff2 == target) {
                    if (newWeight > maxMinWeight ||
                        (newWeight == maxMinWeight && currentConnections + 1 < minConnections)) {
                        maxMinWeight = newWeight;
                        path = newPath;
                        minConnections = currentConnections + 1;
                    }
                } else {
                    bfsQueue.push({conn.aff2, newPath, newWeight, currentConnections + 1});
                    visited.insert(conn.aff2);
                }
            }
        }
    }

    return path;
}

PathWithDist Datastructures::get_shortest_path(AffiliationID source, AffiliationID target) {
    PathWithDist pathWithDist;

    if (affiliations_map_.find(source) == affiliations_map_.end() ||
        affiliations_map_.find(target) == affiliations_map_.end()) {
        // Invalid source or target affiliation
        return pathWithDist;
    }

    std::priority_queue<std::pair<double, AffiliationID>, std::vector<std::pair<double, AffiliationID>>, std::greater<std::pair<double, AffiliationID>>> pq;
    std::unordered_map<AffiliationID, double> distance;
    std::unordered_map<AffiliationID, Connection> previous;

    pq.push({0.0, source});
    distance[source] = 0.0;

    while (!pq.empty()) {
        AffiliationID current = pq.top().second;
        double currentDistance = pq.top().first;
        pq.pop();

        std::vector<Connection> connectedAffiliations = get_connected_affiliations(current);

        for (const Connection& conn : connectedAffiliations) {
            AffiliationID neighbor = conn.aff2;
            double x1 = affiliations_map_[current]->location.x;
            double y1 = affiliations_map_[current]->location.y;
            double x2 = affiliations_map_[neighbor]->location.x;
            double y2 = affiliations_map_[neighbor]->location.y;
            double newDistance = currentDistance + std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));


            if (distance.find(neighbor) == distance.end() || newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                previous[neighbor] = conn;
                pq.push({newDistance, neighbor});
            }
        }
    }

    // Reconstruct the path from source to target
    AffiliationID node = target;
    while (previous.find(node) != previous.end()) {
        AffiliationID previousNode = previous[node].aff1;

        // Calculate the distance between the current node and the previous node
        double x1 = affiliations_map_[previousNode]->location.x;
        double y1 = affiliations_map_[previousNode]->location.y;
        double x2 = affiliations_map_[node]->location.x;
        double y2 = affiliations_map_[node]->location.y;
        double segmentDistance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));

        pathWithDist.emplace_back(previous[node], segmentDistance);
        node = previousNode;
    }

    std::reverse(pathWithDist.begin(), pathWithDist.end());

    return pathWithDist;
}
