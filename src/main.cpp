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

std::vector<cxy> theLocations;

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
    bool fpageAdded = false;
    for (auto &c : K.clusters())
    {
        std::cout << "region at " << c.center().d[0] << "," << c.center().d[0]
                  << " occupied for " << c.points().size() << " hours\n";
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
