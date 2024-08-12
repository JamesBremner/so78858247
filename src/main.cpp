#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <wex.h>
#include "cStarterGUI.h"

#include <cxy.h>
#include "KMeans.h"

class cRegion
{
public:
    cCluster myCluster;        /// KMeans cluster
    std::vector<cxy> myPoints; /// points in region
    cxy myCenter;              /// center of region
    double myRadius;
    double maxRadius2;
    int myTime;

    cRegion(cCluster &c)
        : myCluster(c), maxRadius2(1)
    {
        // find radius of cluster
        radius();

        myTime = myCluster.points().size();
    }
    std::string text();

    static void sort();

private:
    void radius();
};

std::vector<cxy> theLocations;
std::vector<cRegion> theRegions;

void cRegion::radius()
{
    cxy clusterCenter(myCluster.center().d[0], myCluster.center().d[1]);
    double r2 = 0;
    for (auto &p : myCluster.points())
    {
        cxy cxyp(p->d[0], p->d[1]);
        double d2 = clusterCenter.dist2(cxyp);
        if (d2 > maxRadius2)
        {
            r2 = maxRadius2;
            continue;
        }
        if (d2 > r2)
            r2 = d2;
        myPoints.emplace_back(p->d[0], p->d[1]);
    }
    myRadius = sqrt(r2);

    double sx, sy;
    sx = sy = 0;
    for (auto &p : myPoints)
    {
        sx += p.x;
        sy += p.y;
    }
    myCenter.x = sx / myPoints.size();
    myCenter.y = sy / myPoints.size();
}

void cRegion::sort()
{
    std::sort(
        theRegions.begin(), theRegions.end(),
        [](const cRegion &a, const cRegion &b) -> bool
        {
            return (a.myTime > b.myTime);
        });
}

std::string cRegion::text()
{
    std::stringstream ss;
    ss << std::setprecision(2)
       << "region at " << myCenter.x << "," << myCenter.y
       << " radius " << myRadius
       << " occupied for " << myTime << " hours\n";
    return ss.str();
}

void readLocations(const std::string &fname)
{
    theLocations.clear();
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error("Cannot open locations file");
    std::string line;
    while (getline(ifs, line))
    {
        // std::cout << line << "\n";
        int p = line.find(" ");
        if (p == -1)
            throw std::runtime_error("Bad format");
        theLocations.emplace_back(
            atof(line.substr(0, p).c_str()),
            atof(line.substr(p + 1).c_str()));
    }
}

void cluster()
{
    // construct KMeans with locations
    KMeans K;
    for (auto &ip : theLocations)
    {
        K.Add({ip.x, ip.y});
    }

    // Init KMeans
    K.Init(2);

    // Run KMeans
    K.Iter(10);

    // construct regions from clusters
    theRegions.clear();
    for (auto &c : K.clusters())
    {
        theRegions.emplace_back(c);
    }
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        show();
        run();
    }

private:
    wex::label &lb;
};

main()
{
    // cGUI theGUI;

    readLocations("../dat/data1.txt");

    cluster();

    cRegion::sort();

    std::cout << "Regions found, in decreasing occupation time\n";
    for (auto &r : theRegions)
    {
        std::cout << r.text();
    }

    return 0;
}
