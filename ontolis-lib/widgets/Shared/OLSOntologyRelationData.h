#ifndef OLSONTOLOGYRELATIONDATA_H
#define OLSONTOLOGYRELATIONDATA_H

#include "OLSOntologyData.h"

struct OLSOntologyRelationData : public OLSOntologyData {
    long sourceNodeId;
    long destinationNodeId;
};

#endif // OLSONTOLOGYRELATIONDATA_H
