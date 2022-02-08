// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>
#include <algorithm>

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

}

Datastructures::~Datastructures()
{

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
    stops.clear();
    region_id.clear();
    regions.clear();

    max = nullptr;
    min = nullptr;
    min_max_updated = false;

}

std::vector<StopID> Datastructures::all_stops()
{

    return stop_Id;
}

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{


    auto iter = stops.find(id);

    //jos id:llä löytyy jo pysäkki, palautetaan false
    if (iter != stops.end()){
        return false;
    }

    stop_Id.push_back(id);
    Stop *info = new Stop{name,id,NO_REGION,xy};

    stops[id] = info;
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


    auto iter = stops.find(id);

    if (iter != stops.end()){
        return iter->second->name;
    }

    return NO_NAME;


}

Coord Datastructures::get_stop_coord(StopID id)
{


    auto iter = stops.find(id);

    if (iter != stops.end()){
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



    auto iter = stops.find(id);

    if (iter != stops.end()){
        iter->second->name = newname;

        return true;
    }

    return false;



}

bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{



    auto iter = stops.find(id);

    if (iter != stops.end()){
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
    auto stop_iter = stops.find(id);

    //tarkistetaan että region ja pysäkki on olemassa ja että pysäkillä ei ole vielä regionia
    if ((reg_iter != regions.end() and stop_iter == stops.end())
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

    auto iter = stops.find(id);

    if (iter == stops.end()){
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


    auto stop_iter = stops.find(id);


    if (stop_iter == stops.end()){
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




    auto stop_info = stops.find(id);

    //käydään läpi tietorakenteet, joissa pysäkki on ja poistetaan se niistä
    if (stop_info != stops.end()){
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
        stops.erase(stop_info);

        return true;
    }

    return false;
}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{


    auto stop1 = stops.find(id1);

    auto stop2 = stops.find(id2);

    RegionID same_id = NO_REGION;

    //jos jompaa kumpaa pysäkkiä ei löydy
    if (stop1 == stops.end() or stop2 == stops.end()){
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



