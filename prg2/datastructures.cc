// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <stack>
#include <queue>

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
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::stop_count()
{

    return stop_Id.size();
}

void Datastructures::clear_all()
{

    stop_Id.clear();
    stops_alpha.clear();
    stops_coord.clear();
    stops_unord.clear();
    region_id.clear();
    regions.clear();

    max = nullptr;
    min = nullptr;
    min_max_updated = false;

    //tokan harkkatyön tietorakenteet
    route_Id.clear();
    edges.clear();
    routes_unord.clear();
}

std::vector<StopID> Datastructures::all_stops()
{

    return stop_Id;
}

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{


    auto iter = stops_unord.find(id);

    //jos id:llä löytyy jo pysäkki, palautetaan false
    if (iter != stops_unord.end()){
        return false;
    }

    stop_Id.push_back(id);
    Stop *info = new Stop{name,id,NO_REGION,xy};

    stops_unord[id] = info;
    stops_alpha.push_back(info);
    stops_coord.push_back(info);

    if (max == nullptr or
            calculate_dis(info->coord,max->coord) == false){
                      max = info;
}

    if (min == nullptr or
            calculate_dis(info->coord, min->coord) == true){
        min = info;
}
    min_max_updated = true;
    return true;
}

Name Datastructures::get_stop_name(StopID id)
{


    auto iter = stops_unord.find(id);

    if (iter != stops_unord.end()){
        return iter->second->name;
    }

    return NO_NAME;


}

Coord Datastructures::get_stop_coord(StopID id)
{


    auto iter = stops_unord.find(id);

    if (iter != stops_unord.end()){
        return iter->second->coord;
    }

    return NO_COORD;
}

std::vector<StopID> Datastructures::stops_alphabetically()
{


    std::vector<StopID> ids;

    //lambda jota käytetään nimien vertailuun
    auto cmp = [](Stop* const & a, Stop* const & b){
        return a->name < b->name;
    };

    if (std::is_sorted(stops_alpha.begin(), stops_alpha.end(), cmp) == false){
        std::sort(stops_alpha.begin(), stops_alpha.end(), cmp);
    }

    //lisätään järjestyksessä olevien pysäkkien id:t erilliseen vektoriin joka palautetaan
    for (auto value:stops_alpha){
       ids.push_back(value->id);
    }

    return ids;
}

std::vector<StopID> Datastructures::stops_coord_order()
{

    std::vector<StopID> ids;

    //lambda, jota käytetään pysäkkien koordinaattien vertailuun sort-funktiota käytettäessä
    auto cmp = [](Stop* const & a, Stop* const & b){
        double a_value = std::sqrt(std::pow(a->coord.x,2) + std::pow(a->coord.y,2));
        double b_value = std::sqrt(std::pow(b->coord.x,2) + std::pow(b->coord.y,2));

        if (a_value == b_value){
            return a->coord.y < b->coord.y;
        }

        else{
            return a_value < b_value;
        }

    };

    if (std::is_sorted(stops_coord.begin(), stops_coord.end(), cmp) == false){
        std::sort(stops_coord.begin(), stops_coord.end(), cmp);
    }

    for (auto value:stops_coord){
       ids.push_back(value->id);
    }

    return ids;

}

StopID Datastructures::min_coord()
{


    if (stops_coord.size() == 0){
        return NO_STOP;
    }

    //jos max ja minimi muuttujat eivät ole muuttuneet palautetaan suoraan muuttujan arvo
    if (min_max_updated == true){
        return min->id;
    }

    //muussa tapauksessa lasketaan maksimiarvo uusiksi
    else {

        for (auto value : stops_coord){

            if (min == nullptr){
                min = value;
            }

            else if (calculate_dis(value->coord,min->coord) == true){
                min = value;
            }
        }

        return min->id;
    }

}

StopID Datastructures::max_coord()
{

    if (stops_coord.size() == 0){
        return NO_STOP;
    }
    //jos max ja minimi muuttujat eivät ole muuttuneet palautetaan suoraan muuttujan arvo
    if (min_max_updated == true){
        return max->id;
    }

    //lasketaan minimiarvo uusiksi tarvittaessa
    else {

        for (auto value : stops_coord){
            if (max == nullptr){
                max = value;
            }
            else if (calculate_dis(value->coord,max->coord) == false){
                max=value;
            }
        }

        return max->id;
    }


}

std::vector<StopID> Datastructures::find_stops(Name const& name)
{


    std::vector<StopID> ids = {};

    //käydään läpi alkiot ja lisätään saman nimiset alkiot erilliseen vektoriin joka palautetaan
    for (auto value:stops_alpha){
        if (value->name == name){
            ids.push_back(value->id);
        }
    }
    return ids;
}

bool Datastructures::change_stop_name(StopID id, const Name& newname)
{



    auto iter = stops_unord.find(id);

    if (iter != stops_unord.end()){
        iter->second->name = newname;

        return true;
    }

    return false;



}

bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{



    auto iter = stops_unord.find(id);

    if (iter != stops_unord.end()){
        iter->second->coord = newcoord;
        min_max_updated = false;
        return true;
    }

    return false;

}

bool Datastructures::add_region(RegionID id, const Name &name)
{

    auto iter = regions.find(id);

    //jos region on jo olemassa, palautetaan false
    if (iter != regions.end()){
        return false;
    }

    Region *info = new Region{name,id};
    region_id.push_back(id);
    regions[id] = info;
    return true;
}

Name Datastructures::get_region_name(RegionID id)
{

    auto iter = regions.find(id);

    if (iter != regions.end()){
        return iter->second->name;
    }

    return NO_NAME;
}

std::vector<RegionID> Datastructures::all_regions()
{


    return region_id;
}

bool Datastructures::add_stop_to_region(StopID id, RegionID parentid)
{


    auto reg_iter = regions.find(parentid);
    auto stop_iter = stops_unord.find(id);

    //tarkistetaan että region ja pysäkki on olemassa ja että pysäkillä ei ole vielä regionia
    if ((reg_iter != regions.end() and stop_iter == stops_unord.end())
            or stop_iter->second->rId != NO_REGION){
        return false;
    }

    reg_iter->second->stops.push_back(stop_iter->second);
    stop_iter->second->rId = parentid;

    return true;
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{

    auto region_iter = regions.find(id);
    auto parent_iter = regions.find(parentid);

    //tarkistetaan että regionit ovat olemassa
    if (region_iter == regions.end() or parent_iter == regions.end()
            or region_iter->second->parentRegion != NO_REGION){
        return false;
    }

    region_iter->second->parentRegion = parentid;
    parent_iter->second->subregions.push_back(region_iter->second);

    return true;
}

std::vector<RegionID> Datastructures::stop_regions(StopID id)
{


    std::vector<RegionID> regs;

    auto iter = stops_unord.find(id);

    if (iter == stops_unord.end()){
        regs.push_back(NO_REGION);
        return regs;
    }

    if (iter->second->rId == NO_REGION){
        return regs;
    }

    //Etsitään itraattori regioniin johon pysäkki kuuluu
    auto regiter = regions.find(iter->second->rId);
    //lisätään regionin id palautettavaan vektoriin
    regs.push_back(regiter->second->rId);

    //Selvitetään, onko regionilla ylempiä regioneja
    RegionID parent_id = regiter->second->parentRegion;
    bool value = true;
    while (value == true){
        //jos regionilla on yläregion, suoritetaan tämä
        if (parent_id != NO_REGION){
            regs.push_back(parent_id);
            auto iter = regions.find(parent_id);
            parent_id = iter->second->parentRegion;
        }

        //jos yläregionia ei ole, poistutaan loopista ja palautetaan vectori regs sen jälkeen
        else{
            value = false;
        }
    }

    return regs;
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // You don't have to use this method for anything, if you don't need it
}

std::pair<Coord,Coord> Datastructures::region_bounding_box(RegionID id)
{


    //oikean yläkulman ja vasemman alakulman koordinaatit
    Coord oikea = NO_COORD;
    Coord vasen =  NO_COORD;

    std::pair<Coord, Coord> rajat = std::make_pair(vasen, oikea);
    auto region = regions.find(id);

    if (region != regions.end()){
        //Käydään läpi annetun regionin alueella olevien pysäkkien koordinaatit
        for (auto stop : region->second->stops){
            if (oikea == NO_COORD){
                oikea = stop->coord;
            }

            if (vasen == NO_COORD){
                vasen = stop->coord;
            }

            if (oikea.x < stop->coord.x){
                oikea.x = stop->coord.x;
            }

            if (oikea.y < stop->coord.y){
                oikea.y = stop->coord.y;
            }

            if (vasen.x > stop->coord.x){
                vasen.x = stop->coord.x;
            }

            if (vasen.y > stop->coord.y){
                vasen.y = stop->coord.y;
            }


        }

        //käydään läpi annetun regionin ala-alueiden pysäkkien koordinaatit rekursiivisesti
        for (auto alue : region->second->subregions){
            std::pair<Coord, Coord> verrattava = region_bounding_box(alue->rId);

            if (verrattava.first == NO_COORD or verrattava.second == NO_COORD){
                break;
            }

            if (vasen == NO_COORD){
                vasen = verrattava.first;
            }

            if (oikea == NO_COORD){
                oikea = verrattava.second;
            }

            if (verrattava.first.x < vasen.x){
                vasen.x = verrattava.first.x;
            }
            if (verrattava.first.y < vasen.y){
                vasen.y = verrattava.first.y;
            }

            if (verrattava.second.y > oikea.y){
                oikea.y = verrattava.second.y;
            }

            if (verrattava.second.x > oikea.x){
                oikea.x = verrattava.second.x;
        }

        }



}
    //kumpikaan koordinaateista ei saa olla NO_COORD
    if (oikea == NO_COORD or vasen == NO_COORD){
        return {NO_COORD,NO_COORD};
    }

    else{
        //päivitetään oikeat koordinaatit palautettavaan rakenteeseen
        rajat.first = vasen;
        rajat.second = oikea;
        return rajat;
    }
}

std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{


    auto stop_iter = stops_unord.find(id);


    if (stop_iter == stops_unord.end()){
        return {NO_STOP};
    }

    std::vector<StopID> ids;
    //vektori johon tallennetaan pair, joka sisältää pysäkin pointterin sekä etäisyyden vertailtavaan pysäkkiin
    std::vector<std::pair<Stop*, double>> distance;

    auto cmp = [](std::pair<Stop*,double> a,std::pair<Stop*,double> b){

        if (a.second == b.second){
            return a.first->coord.y < b.first->coord.y;
        }

        else{
            return a.second < b.second;
        }
    };

    //lisätään alkiot ja niiden etäisyydet vertailtaavaan alkioon erilliseen vektoriin.
    for (auto alkio : stops_coord){
       double dis = coord_dis(stop_iter->second->coord,alkio->coord);
       distance.push_back(std::make_pair(alkio,dis));
    }

    std::sort(distance.begin(),distance.end(), cmp);

    //lisätään palautettavaan vektoriin 5 alkiota jotka ovat lähinnä ensimmäistä alkiota
    for (unsigned i = 1; i < distance.size();++i){
        if (i == 6){
            break;
        }

        else{
            ids.push_back(distance[i].first->id);

        }
    }

    return ids;


}

bool Datastructures::remove_stop(StopID id)
{




    auto stop_info = stops_unord.find(id);

    //käydään läpi tietorakenteet, joissa pysäkki on ja poistetaan se niistä
    if (stop_info != stops_unord.end()){
        RegionID rID = stop_info->second->rId;


        for (unsigned i=0; i < stop_Id.size();++i){
            if (stop_Id[i] == id){
                stop_Id.erase(stop_Id.begin()+i);
                break;
            }
        }

        for (unsigned i=0; i < stops_alpha.size();++i){
            if (stops_alpha[i]->id == id){
                stops_alpha.erase(stops_alpha.begin()+i);
                break;
            }
        }

        for (unsigned i=0; i < stops_coord.size();++i){
            if (stops_coord[i]->id == id){
                stops_coord.erase(stops_coord.begin()+i);
                break;
            }
        }



        auto region = regions.find(rID);

        if (region != regions.end()){
            for (unsigned i = 0; i < region->second->stops.size(); ++i){
                if (region->second->stops[i]->id == id){
                    region->second->stops.erase(region->second->stops.begin()+i);
                    break;
                }
            }
        }

        if (max != nullptr){
            if (max->id == id){
                max = nullptr;
                min_max_updated = false;
            }

        }

        if(min != nullptr){
            if (min->id == id){
                min = nullptr;
                min_max_updated = false;
            }

        }

        delete stop_info->second;
        stops_unord.erase(stop_info);

        return true;
    }

    return false;
}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{


    auto stop1 = stops_unord.find(id1);

    auto stop2 = stops_unord.find(id2);

    RegionID same_id = NO_REGION;

    //jos jompaa kumpaa pysäkkiä ei löydy
    if (stop1 == stops_unord.end() or stop2 == stops_unord.end()){
        return same_id;
    }


    if (stop1->second->rId == stop2->second->rId){
        same_id = stop1->second->rId;
        return same_id;
    }



    bool value = true;

    //lasketaan stop1 alueen yläalueiden määrä
    int regions1 = 0;
    auto region_iter1 = regions.find(stop1->second->rId);

    while (value == true){
        if (region_iter1->second->parentRegion != NO_REGION){
            ++regions1;
            region_iter1 = regions.find(region_iter1->second->parentRegion);
        }

        else{
            value = false;
        }


    }


    value = true;

    //lasketaan toisen pysäkin alueen yläalueiden määrä
    int regions2 = 0;
    auto region_iter2 = regions.find(stop2->second->rId);
    while (value == true){
        if (region_iter2->second->parentRegion != NO_REGION){
            ++regions2;
            region_iter2 = regions.find(region_iter2->second->parentRegion);
        }

        else{
            value = false;
        }


    }

    int upper_regions1 = regions1;
    int upper_regions2 = regions2;


    //käydään alueita läpi niin kauan, että löydetään yhteinen alue
    //jos aluetta ei löydy, palautuu NO_REGION
    if (upper_regions2 >= upper_regions1){
        region_iter2 = regions.find(stop2->second->rId);
        region_iter1 = regions.find(stop1->second->rId);

        while (true){
            --upper_regions2;
            if (region_iter2->second->rId == region_iter1->second->rId){
                same_id = region_iter2->second->rId;
                return same_id;
            }

            else if (region_iter2->second->parentRegion != NO_REGION){
                region_iter2 = regions.find(region_iter2->second->parentRegion);
            }

            else if (region_iter2->second->parentRegion == NO_REGION and
                     region_iter1->second->parentRegion != NO_REGION){
                region_iter1 = regions.find(region_iter1->second->parentRegion);
                region_iter2 = regions.find(stop2->second->rId);
                upper_regions2 = regions2;
            }

            else{
                return same_id;
            }

        }

    }

    else if (upper_regions2 < upper_regions1){
        auto region_iter1 = regions.find(stop1->second->rId);
        auto region_iter2 = regions.find(stop2->second->rId);

        while (true){

            if (region_iter1->second->rId == region_iter2->second->rId){
                same_id = region_iter2->second->rId;
                return same_id;
            }

            else if (region_iter1->second->parentRegion != NO_REGION){
                region_iter1 = regions.find(region_iter1->second->parentRegion);
            }

            else if (region_iter1->second->parentRegion == NO_REGION and
                     region_iter2->second->parentRegion != NO_REGION){
                region_iter2 = regions.find(region_iter2->second->parentRegion);
                region_iter1 = regions.find(stop1->second->rId);
                upper_regions1 = regions1;
            }
            else{
                return same_id;
            }

        }

    }


    return same_id;
}

bool Datastructures::calculate_dis(Coord a, Coord b)
{
    //tämän funktion avulla lasketaan kumpi annetuista koordinaateista on lähempänä origoa
    double a_value = std::sqrt(std::pow(a.x,2) + std::pow(a.y,2));
    double b_value = std::sqrt(std::pow(b.x,2) + std::pow(b.y,2));

    if (a_value == b_value){
        return a.y < b.y;
    }

    else{
        return a_value < b_value;
    }
}

double Datastructures::coord_dis(Coord a, Coord b)
{
    //käytetään kahden koordinaatin välisen etäisyyden laskemiseen
    double distance = std::sqrt(std::pow(a.x-b.x,2) + std::pow(a.y-b.y,2));
    return distance;
}


int Datastructures::stop_dis(Coord a, Coord b)
{
    //laskee pysäkkien välisen etäisyyden
    int distance = std::floor(std::sqrt(std::pow(a.x - b.x,2) + std::pow(a.y - b.y,2)));
    return distance;
}
void Datastructures::dfs_lisays(std::vector<std::tuple<StopID, RouteID, Distance>> &reitti,
                                std::stack<Stop*> pino)
{
    //lisätään reitti vektoriin

    //viimeksi käsitelty node
    auto last_kasi = pino.top();
    //pinon alkuperäinen koko
    unsigned int koko = pino.size();
    while(!pino.empty()){
        auto kasiteltava = pino.top();

        if(pino.size() == koko){
            //jos koko on edelleen alkuperäinen
            reitti.push_back(std::make_tuple(kasiteltava->id,kasiteltava->rouId,kasiteltava->cycle_matka));
            pino.pop();
            last_kasi = kasiteltava;
        }


        else{
            //suoritetaan, jos pinosta jo poistettu solmuja
            pino.pop();
            auto routes = routes_from(kasiteltava->id);

            for (auto rid : routes){
                if (rid.second == last_kasi->id){
                    kasiteltava->rouId = rid.first;
                    break;
                }
            }
            reitti.push_back(std::make_tuple(kasiteltava->id,kasiteltava->rouId,kasiteltava->matka));
            last_kasi = kasiteltava;
        }

    }

}

void Datastructures::bfs(Datastructures::Stop *alku, Stop *loppu)
{
    //alustetaan bfs
    for (auto stop : stops_alpha){
        stop->matka = 0;
        stop->last_visited = nullptr;
        stop->colour = "white";
        stop->rouId = NO_ROUTE;
    }

    std::queue<Stop*> jono;

    alku->colour = "grey";

    jono.push(alku);

    while(!jono.empty()){
        auto kasiteltava = jono.front();
        jono.pop();



        for (auto naapuri : kasiteltava->next_stops){
            if (naapuri->colour == "white"){
                naapuri->colour = "grey";
                naapuri->matka += kasiteltava->matka + stop_dis(naapuri->coord, kasiteltava->coord);
                naapuri->last_visited = kasiteltava;

                jono.push(naapuri);
                //jos päädytään loppusolmuun, lopetetaan funktion ajaminen
                if (naapuri == loppu){
                    naapuri->colour = "black";
                    return;
                }
            }
        }

        kasiteltava->colour = "black";
    }

}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::bfs_matka(Datastructures::Stop *alku, Datastructures::Stop *loppu)
{

    std::vector<std::tuple<StopID, RouteID, Distance>> reitti;


    auto kasiteltava = loppu;

    while (kasiteltava != nullptr){
        if (kasiteltava == alku){
            reitti.push_back(std::make_tuple(alku->id, alku->rouId,alku->matka));
            return reitti;
        }

        else if(kasiteltava->last_visited == nullptr){
            return {};
        }

        else{
            reitti.push_back(std::make_tuple(kasiteltava->id,kasiteltava->rouId, kasiteltava->matka));
            //Selvitetään reitti, jota pitkin ollaan tultu käsiteltävään solmuun
            auto reitti_id = routes_from(kasiteltava->last_visited->id);
                for (auto rid : reitti_id){
                    if (rid.second == kasiteltava->id){
                        //lisätään viimeksi vierailtuun solmuun reitti mitä käytetty
                        kasiteltava->last_visited->rouId = rid.first;
                        break;
                    }
                }
            kasiteltava = kasiteltava->last_visited;
        }
    }

    return {};



}

void Datastructures::dfs(std::vector<std::tuple<StopID, RouteID, Distance>> &reitti, Datastructures::Stop *alku)
{
    for (auto stop : stops_alpha){
        stop->matka = 0;
        stop->cycle_matka = 0;
        stop->last_visited = nullptr;
        stop->colour = "white";
        stop->rouId = NO_ROUTE;
    }

    std::stack<Stop*> pino;
    //sisältää kaikki reitit
    std::stack<std::stack<Stop*>> route;

    pino.push(alku);
    //pusketaan reitti stackkiin, jossa kaikki reitit
    route.push(pino);

    while(!route.empty()){
        auto route_kasiteltava = route.top();
        auto kasiteltava = route_kasiteltava.top();
        route.pop();

        if(kasiteltava->colour == "white"){
            kasiteltava->colour = "grey";
            pino.push(kasiteltava);


            for(auto naapuri : kasiteltava->next_stops){
                if (naapuri->colour=="white"){
                    naapuri->matka += kasiteltava->matka + stop_dis(naapuri->coord, kasiteltava->coord);
                    std::stack<Stop*> uusi_reitti = route_kasiteltava;
                    //pusketaan uusi reitti stackkiin jota käsitellään
                    uusi_reitti.push(naapuri);
                    route.push(uusi_reitti);
                }
                else if(naapuri->colour == "grey"){
                    naapuri->cycle_matka += kasiteltava->matka + stop_dis(naapuri->coord, kasiteltava->coord);
                    std::stack<Stop*> uusi_reitti = route_kasiteltava;

                    uusi_reitti.push(naapuri);
                    //lisätään oikea reitti funktion haluamaan vektoriin
                    dfs_lisays(reitti,uusi_reitti);
                    return;
                }


            }
        }

        else{
            kasiteltava->colour = "black";
        }
    }


}

std::vector<RouteID> Datastructures::all_routes()
{

    return route_Id;
}

bool Datastructures::add_route(RouteID id, std::vector<StopID> stops)
{   
    auto route = routes_unord.find(id);

    if (route != routes_unord.end() or stops.size() == 1 or stops.size() == 0){
        return false;
    }


    Route *info = new Route{id, stops};

    routes_unord[id] = info;

    route_Id.push_back(id);
    for (unsigned int it = 0; it < stops.size();++it){

        auto stop_info = stops_unord.find(stops[it]);

        if (stop_info == stops_unord.end()){
            return false;
        }


        else if(it + 1 == stops.size()){
            break;
        }


        Edge *edg_info = new Edge{stops[it], stops[it +1 ], id};
        edges.push_back(edg_info);



        stop_info->second->next_stops.push_back(stops_unord.find(stops[it+1])->second);

    }




    return true;
}

std::vector<std::pair<RouteID, StopID>> Datastructures::routes_from(StopID stopid)
{
    // Replace this comment and the line below with your implementation
    std::vector<std::pair<RouteID, StopID>> reitit = {};

    auto it = stops_unord.find(stopid);

    if (it == stops_unord.end()){
        return {{NO_ROUTE, NO_STOP}};
    }

    //Käydään kaaret läpi ja lisätään routeid/stopid pari tietorakenteeseen,
    //kun löydetään kaari jonka alkuid vastaa annettua stopid:ta
    for (auto kaari : edges){
        if (kaari->alku == stopid){
            reitit.push_back(std::make_pair(kaari->rouId,kaari->loppu));
        }
    }

    return reitit;
}

std::vector<StopID> Datastructures::route_stops(RouteID id)
{
    // Replace this comment and the line below with your implementation

    auto it = routes_unord.find(id);

    if (it == routes_unord.end()){
        return {NO_STOP};
    }

    return it->second->stops;
}

void Datastructures::clear_routes()
{
    // Replace this comment and the line below with your implementation
    for(auto edge:edges){
        delete edge;
    }

    for (auto route : routes_unord){
        delete route.second;
    }
    route_Id.clear();
    edges.clear();
    routes_unord.clear();



}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_any(StopID fromstop, StopID tostop)
{
    // Replace this comment and the line below with your implementation
    std::vector<std::tuple<StopID, RouteID, Distance>> journey = {};

    auto first_stop = stops_unord.find(fromstop);
    auto last_stop = stops_unord.find(tostop);

    if (first_stop == stops_unord.end() or last_stop == stops_unord.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }

    //ajetaan bfs
    bfs(first_stop->second, last_stop->second);
    //selvitetään bfs kulkema matka annettujen stoppien mukaan
    journey = bfs_matka(first_stop->second,last_stop->second);

    std::reverse(journey.begin(),journey.end());

    return journey;
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_least_stops(StopID fromstop, StopID tostop)
{
    std::vector<std::tuple<StopID, RouteID, Distance>> journey = {};

    auto first_stop = stops_unord.find(fromstop);
    auto last_stop = stops_unord.find(tostop);

    if (first_stop == stops_unord.end() or last_stop == stops_unord.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }

    //ajetaan bfs
    bfs(first_stop->second, last_stop->second);


    journey = bfs_matka(first_stop->second,last_stop->second);

    std::reverse(journey.begin(),journey.end());
    return journey;
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_with_cycle(StopID fromstop)
{
    // Replace this comment and the line below with your implementation
    std::vector<std::tuple<StopID, RouteID, Distance>> journey = {};

    auto first_stop = stops_unord.find(fromstop);

    if (first_stop == stops_unord.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }

    //ajetaan dfs
    dfs(journey,first_stop->second);
    std::reverse(journey.begin(),journey.end());
    return journey;
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_shortest_distance(StopID fromstop, StopID tostop)
{
    // Replace this comment and the line below with your implementation
    return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
}

bool Datastructures::add_trip(RouteID routeid, std::vector<Time> const& stop_times)
{
    // Replace this comment and the line below with your implementation
    return false;
}

std::vector<std::pair<Time, Duration>> Datastructures::route_times_from(RouteID routeid, StopID stopid)
{
    // Replace this comment and the line below with your implementation
    return {{NO_TIME, NO_DURATION}};
}

std::vector<std::tuple<StopID, RouteID, Time> > Datastructures::journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime)
{
    // Replace this comment and the line below with your implementation
    return {{NO_STOP, NO_ROUTE, NO_TIME}};
}

void Datastructures::add_walking_connections()
{
    // Replace this comment and the line below with your implementation
}
