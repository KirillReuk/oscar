#ifndef ONTOLIS_DATACONTROLLER_HELPERS_H
#define ONTOLIS_DATACONTROLLER_HELPERS_H

#include "widgets/Shared/OLSOntologyDataController.h"
#include "GraphHelpers.hpp"

namespace ontolis {

namespace datacontroller {

namespace helpers {

enum class ArrangeMethods { GursoyAtun, FruchtermanReingold };

void arrangeNodes(OLSOntologyDataController *dataController, ArrangeMethods arrangeMethod);

} // namespace helpers

} // namespace datacontroller

} // namespace ontolis

#endif // ONTOLIS_DATACONTROLLER_HELPERS_H
