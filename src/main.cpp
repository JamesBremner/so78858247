#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

#include <cxy.h>
#include "KMeans.h"

class cRegion
{
public:
    cCluster myCluster;
    double myRadius;

    cRegion(cCluster &c)
        : myCluster(c)
    {
        // find radius of cluster
        cxy clusterCenter(myCluster.center().d[0], myCluster.center().d[1]);
        double r2 = 0;
        for (auto &p : myCluster.points())
        {
            cxy cxyp(p->d[0], p->d[1]);
            double d2 = clusterCenter.dist2(cxyp);
            if (d2 > r2)
                r2 = d2;
        }
        myRadius = sqrt( r2 );
    }
    std::string text()
    {
        std::stringstream ss;
        ss << "region at " << myCluster.center().d[0] << "," << myCluster.center().d[0]
           << " radius " << myRadius
           << " occupied for " << myCluster.points().size() << " hours\n";
        return ss.str();
    }
};

std::vector<cxy> theLocations;
std::vector<cRegion> theRegions;

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
    KMeans K;
    K.clearData();
    for (auto &ip : theLocations)
    {
        K.Add({ip.x, ip.y});
    }
    // Init KMeans
    K.Init(2);

    // Run KMeans
    K.Iter(10);

    // loop over clusters
    theRegions.clear();
    for (auto &c : K.clusters())
    {
        theRegions.emplace_back(c);
    }

    // loop over regions
    for (auto &r : theRegions)
    {
        std::cout << r.text();
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
    return 0;
}
