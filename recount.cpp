#include "recount.h"

/**
 * @brief recount::recount empty constructor
 */
recount::recount()
{
    this->code = -1;
}

//TO_DO: all methods of the class

recount::recount(int code,string municipal,int zone,int section,char name,int census, int abstentions, int v_cast,int v_invalid, int v_valid, int v_white, float p_participation,float p_abstention){
    this->code=code;
    this->municipal=municipal;
    this->zone=zone;
    this->section=section;
    this->name=name;
    this->census=census;
    this->abstentions=abstentions;
    this->v_cast=v_cast;
    this->v_invalid=v_invalid;
    this->v_valid=v_valid;
    this->v_white=v_white;
    this->p_participation=p_participation;
    this->p_abstention=p_abstention;

}

void recount::print(){

}

int recount::get_v_cast(){

   return this->v_cast;
}

int recount::get_abstentions(){

   return this->abstentions;
}

int recount::get_census(){

   return this->census;
}

int recount::get_blanco(){

   return this->v_white;
}

int recount::get_valid(){
    return this->v_valid;
}

int recount::get_invalid(){
    return this->v_invalid;
}

char recount::get_name(){
    return this->name;
}

int recount::get_section(){
    return this->section;
}

int recount::get_zone(){
    return this->zone;
}

string recount::get_municipal(){
    return this->municipal;
}

int recount::get_code(){
    return this->code;
}


float recount::get_p_participation(){
    return this->p_participation;
}

float recount::get_p_abstention(){
    return this->p_abstention;
}

void recount::set_p_participation(float p_participation){
this->p_participation=p_participation;
}

void recount::set_p_abstention(float p_abstention){
    this->p_abstention=p_abstention;

}














