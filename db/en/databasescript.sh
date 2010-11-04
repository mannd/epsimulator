#! /usr/bin/env bash

echo making database now....
rm -f ../../db/en/epsimulator.db
sqlite3 ../../db/en/epsimulator.db < ../../db/en/databaseschema



      
