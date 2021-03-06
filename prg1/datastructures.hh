// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <limits>

// Types for IDs
using StopID = long int;
using RegionID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
StopID const NO_STOP = -1;
RegionID const NO_REGION = "!!NO_REGION!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

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


// This is the class you are supposed to implement



class Datastructures
{
public:
    Datastructures();
    ~Datastructures();



    // Estimate of performance: O(n)
    // Short rationale for estimate: size() vie tietorakenteen koon verran aikaa
    int stop_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: clear funktion dokumentaation perusteella
    void clear_all();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Muuttujan palauttaminen vakioaikaista
    std::vector<StopID> all_stops();

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate: unordered mappiin lisäys on keskimäärin vakioaikaista
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate: unordered mapista etsiminen on keskimäärin vakioaikaista
    Name get_stop_name(StopID id);

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate: unordered mapista etsiminen on keskimäärin vakioaikaista
    Coord get_stop_coord(StopID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: std::sort dokumentaatio
    std::vector<StopID> stops_alphabetically();

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: std::sort dokumentaatio
    std::vector<StopID> stops_coord_order();

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: std::sort dokumentaatio.  Keskimäärin vakioaikainen kuitenkin, jos ei tehdä muokkauksia tietorakenteeseen
    StopID min_coord();

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate:std::sort dokumentaatio. Keskimäärin vakioaikainen kuitenkin, jos ei tehdä muokkauksia tietorakenteeseen
    StopID max_coord();

    // Estimate of performance: O(n)
    // Short rationale for estimate: joudutaan käymään stops_alphan kaikki alkiot läpi
    std::vector<StopID> find_stops(Name const& name);

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate: unordered mapista etsiminen on keskimäärin vakioaikaista
    bool change_stop_name(StopID id, Name const& newname);

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate: unordered mapista etsiminen on keskimäärin vakioaikaista
    bool change_stop_coord(StopID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate:unordered mapista etsiminen on keskimäärin vakioaikaista
    bool add_region(RegionID id, Name const& name);

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate:unordered mapista etsiminen on keskimäärin vakioaikaista
    Name get_region_name(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: muuttujan palautus vakioaikaista
    std::vector<RegionID> all_regions();

    // Estimate of performance:  ~theta(1)
    // Short rationale for estimate: unordered mapista etsiminen on keskimäärin vakioaikaista
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Estimate of performance: ~theta(1)
    // Short rationale for estimate: unordered mapista etsiminen on keskimäärin vakioaikaista
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: pyöritetään while looppia kunnes ollaan käyty kaikki mahdolliset regionit läpi
    std::vector<RegionID> stop_regions(StopID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Jos kaikki pysäkit ovat alueen sisällä, on niiden kaikkien läpikäynti lineearista.
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: Std::sortin nopeus nlog(n)
    std::vector<StopID> stops_closest_to(StopID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Jos poistettava alkio on viimeisenä tietorakenteessa, joudutaan käymään koko rakenne läpi
    //jolloin nopeus lineaarista.
    bool remove_stop(StopID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Jos joudutaan käymään kaikki yläregionit läpi, niin lineaarista.
    RegionID stops_common_region(StopID id1, StopID id2);

private:
    // Add stuff needed for your class implementation here
    struct Stop{
        Name name;
        StopID id;
        RegionID rId;
        Coord coord;
    };

    //käytetään tarkastamaan, onko tietorakenteeseen tullut muutoksia, jolloin joudutaan
    //määrittämään min ja max uudestaan
    bool min_max_updated = false;
    Stop* max = nullptr;
    Stop* min = nullptr;

    std::vector<StopID> stop_Id;
    std::unordered_map<StopID, Stop*> stops;
    std::vector<Stop*> stops_alpha;
    std::vector<Stop*> stops_coord;

    struct Region{
        std::string name;
        RegionID rId;
        std::vector<Stop*> stops = {};
        RegionID parentRegion = NO_REGION;
        std::vector<Region*> subregions = {};
    };

    std::vector<RegionID> region_id;
    std::unordered_map<RegionID, Region*> regions;

    bool calculate_dis(Coord a, Coord b);

    double coord_dis(Coord a, Coord b);

};

#endif // DATASTRUCTURES_HH
