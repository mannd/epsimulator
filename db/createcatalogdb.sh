#!/usr/bin/env bash

echo making empty system catalog database now if not already present....
rm -f ../../db/catalog.db
sqlite3 ../../db/catalog.db < ../../db/catalogdbschema.sql



      
