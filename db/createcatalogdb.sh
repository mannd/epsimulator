#! /usr/bin/env bash

echo making empty system catalog database now if not already present....
rm -f ../../db/emptycatalog.db
sqlite3 ../../db/emptycatalog.db < ../../db/catalogdbschema.sql



      
