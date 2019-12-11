# Nearest County Finder

Given a dataset of US Counties and their coordinates, find nearest county using a linked list and kd-tree.

**Class:** BU ENG EC504 - Advanced Data Structures

**Team:** David Abadi

## Build & Run

To run the linked list solution, download and extract the zip file on your local machine, then run the following commands:

```
cd ./sol_1/
module load gcc
make
./state_finder NationalFile_StateProvinceDecimalLatLong.txt
Enter k (1-10): 6
Enter latitude and longitude: 40.16482 -130.4679
```

To run the kd-tree solution:

```
cd ./sol_2/
module load gcc
make
./state_finder NationalFile_StateProvinceDecimalLatLong.txt
Enter k (1-10): 6
Enter latitude and longitude: 40.16482 -130.4679
```

\*\*Disclaimer: Need at least 1.5 MB of memory in order to run.
