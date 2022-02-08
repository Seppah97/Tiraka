// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <limits>
#include <stack>

// Types for IDs
using StopID = long int;
using RegionID = std::string;
using RouteID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
RouteID const NO_ROUTE = "!!NO_ROUTE!!";
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

// Type for time of day in minutes from midnight (i.e., 60*hours + minutes)
using Time = int;

// Return value for cases where color was not found
Time const NO_TIME = std::numeric_limits<Time>::min();

// Type for a duration of time (in minutes)
using Duration = int;

// Return value for cases where Duration is unknown
Duration const NO_DURATION = NO_VALUE;

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;



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

    // Phase 2 operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: Vektorin palauttaminen on lineearista
    std::vector<RouteID> all_routes();

    // Estimate of performance: O(n)
    // Short rationale for estimate: joudutaan käymään läpi vektoria, mikä on lineearista
    bool add_route(RouteID id, std::vector<StopID> stops);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Vektorin läpikäyminen lineaarista
    std::vector<std::pair<RouteID, StopID>> routes_from(StopID stopid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Vektorin palauttaminen lineaarista.
    std::vector<StopID> route_stops(RouteID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: tietorakenteiden läpikäynti lineaarista
    void clear_routes();

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: BFS algoritmin suoritusaika on solmut + kaaret
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_any(StopID fromstop, StopID tostop);

//    // Non-compulsory operations

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: BFS algoritmin suoritusaika on solmut + kaaret
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_least_stops(StopID fromstop, StopID tostop);

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: DFS algoritmin suoritusaika on solmut + kaaret
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_with_cycle(StopID fromstop);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_shortest_distance(StopID fromstop, StopID tostop);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_trip(RouteID routeid, const std::vector<Time> &stop_times);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Time, Duration> > route_times_from(RouteID routeid, StopID stopid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<StopID, RouteID, Time>> journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime);

    // Estimate of performance:
    // Short rationale for estimate:
    void add_walking_connections(); // Note! This method is completely optional, and not part of any testing

private:
    // Add stuff needed for your class implementation here
    struct Route;
    struct Stop{
        Name name;
        StopID id;
        RegionID rId;
        Coord coord;
        RouteID rouId = NO_ROUTE;
        std::string colour = "white";
        std::vector<Stop*> next_stops = {};
        Distance matka = 0;
        Distance cycle_matka = 0;
        Stop* last_visited = nullptr;
    };

    //käytetään tarkastamaan, onko tietorakenteeseen tullut muutoksia, jolloin joudutaan
    //määrittämään min ja max uudestaan
    bool min_max_updated = false;
    Stop* max = nullptr;
    Stop* min = nullptr;

    std::vector<StopID> stop_Id;
    std::unordered_map<StopID, Stop*> stops_unord;
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

    //tokan harkkatyön tietorakenteet yms tästä alaspäin

    struct Route{
        RouteID id;
        std::vector<StopID> stops;
    };

    struct Edge{
        StopID alku;
        StopID loppu;
        RouteID rouId;

    };

    std::vector<RouteID> route_Id;

    std::vector<Edge*> edges;

    std::unordered_map<RouteID, Route*> routes_unord;


    bool bfs_suoritettu = false;

    int stop_dis(Coord a, Coord b);

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: DFS tehokkuus on tällä tavalla toteutettuna solmujen ja kaarien summa
    void dfs(std::vector<std::tuple<StopID, RouteID, Distance>> &reitti,Stop* alku);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Lineaarista, koska käydään jokainen stackin alkio läpi
    void dfs_lisays(std::vector<std::tuple<StopID, RouteID, Distance>> &reitti, std::stack<Stop*> pino);

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: BFS tehokkuus on tällä tavalla toteutettuna solmujen ja kaarien summa
    void bfs(Stop* alku, Stop* loppu);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Vektorin läpikäynti lineaarista
    std::vector<std::tuple<StopID, RouteID, Distance>> bfs_matka(Stop* alku, Stop* loppu);



};

#endif // DATASTRUCTURES_HH
