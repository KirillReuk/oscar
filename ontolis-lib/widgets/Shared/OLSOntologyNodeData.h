#ifndef OLSONTOLOGYNODEDATA_H
#define OLSONTOLOGYNODEDATA_H

#include "OLSOntologyData.h"

struct OLSOntologyNodeData : public OLSOntologyData {
    QList<long> relations;
};

#endif // OLSONTOLOGYNODEDATA_H
