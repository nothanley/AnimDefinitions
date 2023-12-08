#include "AnimDefHandler.h"
#include "Encoder/hash.h"

void ADefHandler::populateEventHashes(){
    for (const auto& string : this->m_aEventTitles){
        this->m_aEventHashes.push_back( Hash::fnv1a64(string.c_str()) );
    }

    std::vector<std::pair<std::string, uint64_t>> combinedVector;
    for (size_t i = 0; i < m_aEventHashes.size(); i++)
    {
        combinedVector.push_back( std::make_pair( m_aEventTitles.at(i), m_aEventHashes.at(i) ) );
    }

    // Sort the vector of pairs based on the uint values
    std::sort(combinedVector.begin(), combinedVector.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });


    m_aEventTitles.clear();
    m_aEventHashes.clear();
    for (const auto& pair : combinedVector) {
        m_aEventTitles.push_back(pair.first);
        m_aEventHashes.push_back(pair.second);
    }
}
