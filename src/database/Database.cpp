/****************************** Database **************************************
This file is part of the Ewings Esp8266 Stack.

This is free software. you can redistribute it and/or modify it but without any
warranty.

Author          : Suraj I.
created Date    : 1st June 2019
******************************************************************************/

#include "Database.h"

int DatabaseTableAbstractLayer::_total_instances = 0;
DatabaseTableAbstractLayer* DatabaseTableAbstractLayer::_instances[MAX_TABLES] = {nullptr};

/**
 * initialize database with size as input parameter.
 *
 * @param uint16_t|DATABASE_MAX_SIZE  _size
 */
void Database::init_database(  uint16_t _size ){

  beginConfigs(_size);
  this->_database_tables.reserve(MAX_TABLES);

  for (size_t i = 0; i < DatabaseTableAbstractLayer::_total_instances; i++) {
    DatabaseTableAbstractLayer::_instances[i]->boot();
  }
}

/**
 * clear all tables in database
 *
 * @param   uint16_t  _address
 * @return  bool
 */
void Database::clear_all(){

	for ( uint8_t i = 0; i < this->_database_tables.size(); i++) {

		if( this->_database_tables[i]._instance ) this->_database_tables[i]._instance->clear();
	}
}

/**
 * register table in database tables by their address
 *
 * @param   struct_tables _table
 * @return  bool
 */
bool Database::register_table( struct_tables& _table ) {

  struct_tables _last = this->get_last_table();
  if(
    ( _last._table_address + _last._table_size + 2 ) < _table._table_address &&
    ( _table._table_address + _table._table_size + 2 ) < DATABASE_MAX_SIZE
  ){
    this->_database_tables.push_back(_table);
    return true;
  }
  return false;
}

/**
 * get last registered table from database tables
 *
 * @return   struct_tables
 */
struct_tables Database::get_last_table(){

  struct_tables _last;
  uint8_t _last_add = 0;
  memset( &_last, 0, sizeof(struct_tables));
  for ( uint8_t i = 1; i < this->_database_tables.size(); i++) {

    if( this->_database_tables[_last_add]._table_address < this->_database_tables[i]._table_address ){
      _last_add = i;
    }
  }
  if( _last_add != 0 ) return this->_database_tables[_last_add]; return _last;
}

Database __database;
