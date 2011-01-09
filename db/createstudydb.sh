#! /usr/bin/env bash

echo making empty system study database now if not already present....
rm -f ../../db/emptystudy.db
sqlite3 ../../db/emptystudy.db < ../../db/studydbschema.sql



      
