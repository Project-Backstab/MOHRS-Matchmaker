#include <mohrs/region.h>

std::map<MoHRS::Regions, std::string> MoHRS::RegionNames = {
    { MoHRS::Regions::Europe,        "Europe"        },
    { MoHRS::Regions::North_America, "North America" },
    { MoHRS::Regions::South_America, "South America" },
    { MoHRS::Regions::Africa,        "Africa"        },
    { MoHRS::Regions::Asia,          "Asia"          },
    { MoHRS::Regions::Oceania,       "Oceania"       },
    { MoHRS::Regions::Moon,          "Moon"          },
    { MoHRS::Regions::Mars,          "Mars"          },
};

