# valheim-tools
Just some valheim tools just for personal use.

## building

Requires cmake and a non prehistoric c++ compiler.

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## playerdb_reader
Prints information about a given player database

## portal_finder
Prints information about all portals in the world and reports if any portals are unconnected

## mapdatasync
Takes a list of character (fcl) files, and synchronizes the exploration state of the map between them. 
Generates new files which can then be replaced on each players computer.
Does not change anything else. 