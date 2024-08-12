Here is 24 hour location data for a person who lives at 0,0 and works from 9 to 5 at an office located at 10,0, but otherwise doesn't seem to have a life.

```
0 0 0
0 0 1
0 0 2
0 0 3
0 0 4
0 0 5
0 0 6
0 0 7
5 0 8
10 0 9
10 0 10
10 0 11
10 0 12
10 0 13
10 0 14
10 0 15
10 0 16
10 0 17
6 0 18
0 0 19
0 0 20
0 0 21
0 0 22
0 0 23
```
This application does a type of clustering of the regions where the most time has been spent.  Output for the above input:

```
Regions found, in decreasing occupation time
region at 0,0 radius 1 occupied for 14 hours
region at 10,0 radius 1 occupied for 10 hours
```

