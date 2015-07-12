#include "DataControllerHelpers.hpp"

namespace ontolis {

namespace datacontroller {

namespace helpers {

void arrangeNodes(OLSOntologyDataController *dataController, ArrangeMethods arrangeMethod)
{
    std::list<std::pair<long, long>> relationsForLayout;
    int relationCount = dataController->relationCount();
    for (int i = 0; i < relationCount; i++) {
        OLSOntologyRelationData *relation = dataController->getRelationByIndex(i);
        relationsForLayout.push_back(std::pair<long, long>(relation->sourceNodeId, relation->destinationNodeId));
    }
    std::map<long, std::pair<double, double>> (*arrangeFunc)(std::list<std::pair<long, long>>);
    arrangeFunc = nullptr;
    if (arrangeMethod == ArrangeMethods::FruchtermanReingold) {
        qDebug() << "graph::helpers::fr_layouter";
        arrangeFunc = graph::helpers::fr_layouter;
    } else if (arrangeMethod == ArrangeMethods::GursoyAtun) {
        qDebug() << "graph::helpers::ga_layouter";
        arrangeFunc = graph::helpers::ga_layouter;
    } else {
        qDebug() << "Unsupported arrange method";
        throw;
    }
    std::map<long, std::pair<double, double>> positions = arrangeFunc(relationsForLayout);
    for (auto& kv : positions) {
        long nodeId = kv.first;
        double x = kv.second.first;
        double y = kv.second.second;
        dataController->setNodePosition(nodeId, QPointF(x, y));
    }
}

} // namespace helpers

} // namespace datacontroller

} // namespace ontolis
